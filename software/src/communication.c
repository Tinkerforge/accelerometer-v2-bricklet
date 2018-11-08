/* accelerometer-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "kx122.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_ACCELERATION: return get_acceleration(message, response);
		case FID_SET_CONFIGURATION: return set_configuration(message);
		case FID_GET_CONFIGURATION: return get_configuration(message, response);
		case FID_SET_ACCELERATION_CALLBACK_CONFIGURATION: return set_acceleration_callback_configuration(message);
		case FID_GET_ACCELERATION_CALLBACK_CONFIGURATION: return get_acceleration_callback_configuration(message, response);
		case FID_SET_CONTINUOUS_ACCELERATION_CONFIGURATION: return set_continuous_acceleration_configuration(message);
		case FID_GET_CONTINUOUS_ACCELERATION_CONFIGURATION: return get_continuous_acceleration_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse get_acceleration(const GetAcceleration *data, GetAcceleration_Response *response) {
	response->header.length = sizeof(GetAcceleration_Response);
	response->x             = kx122.acceleration[KX122_X];
	response->y             = kx122.acceleration[KX122_Y];
	response->z             = kx122.acceleration[KX122_Z];

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data) {
	if((data->data_rate  > ACCELEROMETER_V2_DATA_RATE_25600HZ) ||
	   (data->full_scale > ACCELEROMETER_V2_FULL_SCALE_8G)) {
		   return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	kx122.config_new_data_rate  = data->data_rate;
	kx122.config_new_full_scale = data->full_scale;
	kx122.config_new            = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response) {
	response->header.length = sizeof(GetConfiguration_Response);
	response->data_rate     = kx122.config_current_data_rate;
	response->full_scale    = kx122.config_current_full_scale;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_acceleration_callback_configuration(const SetAccelerationCallbackConfiguration *data) {
	kx122.acceleration_period              = data->period;
	kx122.acceleration_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_acceleration_callback_configuration(const GetAccelerationCallbackConfiguration *data, GetAccelerationCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetAccelerationCallbackConfiguration_Response);
	response->period              = kx122.acceleration_period;
	response->value_has_to_change = kx122.acceleration_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_continuous_acceleration_configuration(const SetContinuousAccelerationConfiguration *data) {
	if(data->resolution > ACCELEROMETER_V2_RESOLUTION_16BIT) {
		   return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	kx122.config_cont_new_enable[0]  = data->enable_x;
	kx122.config_cont_new_enable[1]  = data->enable_y;
	kx122.config_cont_new_enable[2]  = data->enable_z;
	kx122.config_cont_new_resolution = data->resolution;
	kx122.config_cont_new            = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_continuous_acceleration_configuration(const GetContinuousAccelerationConfiguration *data, GetContinuousAccelerationConfiguration_Response *response) {
	response->header.length = sizeof(GetContinuousAccelerationConfiguration_Response);
	response->resolution    = kx122.config_cont_current_resolution;
	response->enable_x      = kx122.config_cont_current_enable[0];
	response->enable_y      = kx122.config_cont_current_enable[1];
	response->enable_z      = kx122.config_cont_current_enable[2];

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}


bool handle_acceleration_callback(void) {
	static bool is_buffered = false;
	static Acceleration_Callback cb;
	static int16_t last_value[3] = {0, 0, 0};
	static uint32_t last_time = 0;

	if(!is_buffered) {
		if((kx122.acceleration_period == 0) || 
		   (!system_timer_is_time_elapsed_ms(last_time, kx122.acceleration_period))) {
			return false;
		}

		if(kx122.acceleration_value_has_to_change               && 
		   (last_value[KX122_X] == kx122.acceleration[KX122_X]) && 
		   (last_value[KX122_Y] == kx122.acceleration[KX122_Y]) && 
		   (last_value[KX122_Z] == kx122.acceleration[KX122_Z])) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Acceleration_Callback), FID_CALLBACK_ACCELERATION);
		cb.x = kx122.acceleration[KX122_X];
		cb.y = kx122.acceleration[KX122_Y];
		cb.z = kx122.acceleration[KX122_Z];

		last_time           = system_timer_get_ms();
		last_value[KX122_X] = kx122.acceleration[KX122_X];
		last_value[KX122_Y] = kx122.acceleration[KX122_Y];
		last_value[KX122_Z] = kx122.acceleration[KX122_Z];
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Acceleration_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_continuous_acceleration_16_bit_callback(void) {
	static bool is_buffered = false;
	static ContinuousAcceleration16Bit_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(ContinuousAcceleration16Bit_Callback), FID_CALLBACK_CONTINUOUS_ACCELERATION_16_BIT);
		// TODO: Implement ContinuousAcceleration16Bit callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(ContinuousAcceleration16Bit_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_continuous_acceleration_8_bit_callback(void) {
	static bool is_buffered = false;
	static ContinuousAcceleration8Bit_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(ContinuousAcceleration8Bit_Callback), FID_CALLBACK_CONTINUOUS_ACCELERATION_8_BIT);
		// TODO: Implement ContinuousAcceleration8Bit callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(ContinuousAcceleration8Bit_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
