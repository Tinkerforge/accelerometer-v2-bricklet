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

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "xmc_gpio.h"

#include "configs/config_kx122.h"
#include "kx122.h"

extern volatile uint8_t kx122_acceleration_index;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_ACCELERATION: return get_acceleration(message, response);
		case FID_SET_CONFIGURATION: return set_configuration(message);
		case FID_GET_CONFIGURATION: return get_configuration(message, response);
		case FID_SET_ACCELERATION_CALLBACK_CONFIGURATION: return set_acceleration_callback_configuration(message);
		case FID_GET_ACCELERATION_CALLBACK_CONFIGURATION: return get_acceleration_callback_configuration(message, response);
		case FID_SET_INFO_LED_CONFIG: return set_info_led_config(message);
		case FID_GET_INFO_LED_CONFIG: return get_info_led_config(message, response);
		case FID_SET_CONTINUOUS_ACCELERATION_CONFIGURATION: return set_continuous_acceleration_configuration(message);
		case FID_GET_CONTINUOUS_ACCELERATION_CONFIGURATION: return get_continuous_acceleration_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse get_acceleration(const GetAcceleration *data, GetAcceleration_Response *response) {
	response->header.length = sizeof(GetAcceleration_Response);

	const uint8_t index = kx122_acceleration_index;
	response->x         = kx122_adc_value_to_g(kx122.acceleration[index].x);
	response->y         = kx122_adc_value_to_g(kx122.acceleration[index].y);
	response->z         = kx122_adc_value_to_g(kx122.acceleration[index].z);

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

BootloaderHandleMessageResponse set_info_led_config(const SetInfoLEDConfig *data) {
	if(data->config == ACCELEROMETER_V2_INFO_LED_CONFIG_OFF) {
		kx122.info_led_flicker_state.config = LED_FLICKER_CONFIG_OFF;
		XMC_GPIO_SetOutputHigh(KX122_INFO_LED_PIN);
	}
	else if(data->config == ACCELEROMETER_V2_INFO_LED_CONFIG_ON) {
		kx122.info_led_flicker_state.config = LED_FLICKER_CONFIG_ON;
		XMC_GPIO_SetOutputLow(KX122_INFO_LED_PIN);
	}
	else if(data->config == ACCELEROMETER_V2_INFO_LED_CONFIG_SHOW_HEARTBEAT) {
		kx122.info_led_flicker_state.config = LED_FLICKER_CONFIG_HEARTBEAT;
		kx122.info_led_flicker_state.start = system_timer_get_ms();
	}
	else {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_info_led_config(const GetInfoLEDConfig *data, GetInfoLEDConfig_Response *response) {
	response->header.length = sizeof(GetInfoLEDConfig_Response);
	response->config        = kx122.info_led_flicker_state.config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_continuous_acceleration_configuration(const SetContinuousAccelerationConfiguration *data) {
	if(data->resolution > ACCELEROMETER_V2_RESOLUTION_16BIT) {
		   return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	kx122.config_cont_enable[0]    = data->enable_x;
	kx122.config_cont_enable[1]    = data->enable_y;
	kx122.config_cont_enable[2]    = data->enable_z;
	kx122.config_cont_resolution   = data->resolution;
	kx122.config_cont_enable_count = kx122.config_cont_enable[0] + kx122.config_cont_enable[1] + kx122.config_cont_enable[2];

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_continuous_acceleration_configuration(const GetContinuousAccelerationConfiguration *data, GetContinuousAccelerationConfiguration_Response *response) {
	response->header.length = sizeof(GetContinuousAccelerationConfiguration_Response);
	response->resolution    = kx122.config_cont_resolution;
	response->enable_x      = kx122.config_cont_enable[0];
	response->enable_y      = kx122.config_cont_enable[1];
	response->enable_z      = kx122.config_cont_enable[2];

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}


bool handle_acceleration_callback(void) {
	static bool is_buffered = false;
	static Acceleration_Callback cb;
	static KX122Acceleration last_value = {0, 0, 0};
	static uint32_t last_time = 0;

	if(!is_buffered) {
		if((kx122.acceleration_period == 0) || 
		   (!system_timer_is_time_elapsed_ms(last_time, kx122.acceleration_period))) {
			return false;
		}

		const uint8_t index = kx122_acceleration_index;
		if(kx122.acceleration_value_has_to_change        && 
		   (last_value.x == kx122.acceleration[index].x) && 
		   (last_value.y == kx122.acceleration[index].y) && 
		   (last_value.z == kx122.acceleration[index].z)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Acceleration_Callback), FID_CALLBACK_ACCELERATION);
		cb.x         = kx122_adc_value_to_g(kx122.acceleration[index].x);
		cb.y         = kx122_adc_value_to_g(kx122.acceleration[index].y);
		cb.z         = kx122_adc_value_to_g(kx122.acceleration[index].z);

		last_time    = system_timer_get_ms();
		last_value.x = kx122.acceleration[index].x;
		last_value.y = kx122.acceleration[index].y;
		last_value.z = kx122.acceleration[index].z;
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
		if((kx122.config_cont_resolution == ACCELEROMETER_V2_RESOLUTION_16BIT) &&
		   (kx122.config_cont_enable_count > 0)) {
			const uint8_t length =  kx122_acceleration_index - kx122.acceleration_read_index;
			if(length < 30/kx122.config_cont_enable_count) {
				return false;
			}

			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(ContinuousAcceleration16Bit_Callback), FID_CALLBACK_CONTINUOUS_ACCELERATION_16_BIT);
			uint8_t i = 0;
			while(i < 30) {
				if(kx122.config_cont_enable[0]) {
					cb.acceleration[i] = kx122.acceleration[kx122.acceleration_read_index].x;
					i++;
				}
				if(kx122.config_cont_enable[1]) {
					cb.acceleration[i] = kx122.acceleration[kx122.acceleration_read_index].y;
					i++;
				}
				if(kx122.config_cont_enable[2]) {
					cb.acceleration[i] = kx122.acceleration[kx122.acceleration_read_index].z;
					i++;
				}
				kx122.acceleration_read_index++;
			}
		} else {
			return false;
		}
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
		if((kx122.config_cont_resolution == ACCELEROMETER_V2_RESOLUTION_8BIT) &&
		   (kx122.config_cont_enable_count > 0)) {
			const uint8_t length = kx122_acceleration_index - kx122.acceleration_read_index;
			if(length < 60/kx122.config_cont_enable_count) {
				return false;
			}

			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(ContinuousAcceleration8Bit_Callback), FID_CALLBACK_CONTINUOUS_ACCELERATION_8_BIT);
			uint8_t i = 0;
			while(i < 60) {
				if(kx122.config_cont_enable[0]) {
					cb.acceleration[i] = ((int8_t*)&kx122.acceleration[kx122.acceleration_read_index].x)[1];
					i++;
				}
				if(kx122.config_cont_enable[1]) {
					cb.acceleration[i] = ((int8_t*)&kx122.acceleration[kx122.acceleration_read_index].y)[1];
					i++;
				}
				if(kx122.config_cont_enable[2]) {
					cb.acceleration[i] = ((int8_t*)&kx122.acceleration[kx122.acceleration_read_index].z)[1];
					i++;
				}

				kx122.acceleration_read_index++;
			}
		} else {
			return false;
		}
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
	static uint32_t counter = 0;

	// Only handle callbacks every second call to give bootloader code some time to receive/send other messages
	counter++;
	if(counter == 2) {
		counter = 0;

		if(kx122.config_cont_enable_count > 0) {
			if(kx122.config_cont_resolution == ACCELEROMETER_V2_RESOLUTION_16BIT) {
				handle_continuous_acceleration_16_bit_callback();
			} else {
				handle_continuous_acceleration_8_bit_callback();
			}
		} else {
			handle_acceleration_callback();
		}
	}
}