/* accelerometer-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);

// Constants
#define ACCELEROMETER_V2_DATA_RATE_0_781HZ 0
#define ACCELEROMETER_V2_DATA_RATE_1_563HZ 1
#define ACCELEROMETER_V2_DATA_RATE_3_125HZ 2
#define ACCELEROMETER_V2_DATA_RATE_6_2512HZ 3
#define ACCELEROMETER_V2_DATA_RATE_12_5HZ 4
#define ACCELEROMETER_V2_DATA_RATE_25HZ 5
#define ACCELEROMETER_V2_DATA_RATE_50HZ 6
#define ACCELEROMETER_V2_DATA_RATE_100HZ 7
#define ACCELEROMETER_V2_DATA_RATE_200HZ 8
#define ACCELEROMETER_V2_DATA_RATE_400HZ 9
#define ACCELEROMETER_V2_DATA_RATE_800HZ 10
#define ACCELEROMETER_V2_DATA_RATE_1600HZ 11
#define ACCELEROMETER_V2_DATA_RATE_3200HZ 12
#define ACCELEROMETER_V2_DATA_RATE_6400HZ 13
#define ACCELEROMETER_V2_DATA_RATE_12800HZ 14
#define ACCELEROMETER_V2_DATA_RATE_25600HZ 15

#define ACCELEROMETER_V2_FULL_SCALE_2G 0
#define ACCELEROMETER_V2_FULL_SCALE_4G 1
#define ACCELEROMETER_V2_FULL_SCALE_8G 2

#define ACCELEROMETER_V2_INFO_LED_CONFIG_OFF 0
#define ACCELEROMETER_V2_INFO_LED_CONFIG_ON 1
#define ACCELEROMETER_V2_INFO_LED_CONFIG_SHOW_HEARTBEAT 2

#define ACCELEROMETER_V2_RESOLUTION_8BIT 0
#define ACCELEROMETER_V2_RESOLUTION_16BIT 1

#define ACCELEROMETER_V2_IIR_BYPASS_APPLIED 0
#define ACCELEROMETER_V2_IIR_BYPASS_BYPASSED 1

#define ACCELEROMETER_V2_LOW_PASS_FILTER_NINTH 0
#define ACCELEROMETER_V2_LOW_PASS_FILTER_HALF 1

#define ACCELEROMETER_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define ACCELEROMETER_V2_BOOTLOADER_MODE_FIRMWARE 1
#define ACCELEROMETER_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define ACCELEROMETER_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define ACCELEROMETER_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define ACCELEROMETER_V2_BOOTLOADER_STATUS_OK 0
#define ACCELEROMETER_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define ACCELEROMETER_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define ACCELEROMETER_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define ACCELEROMETER_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define ACCELEROMETER_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define ACCELEROMETER_V2_STATUS_LED_CONFIG_OFF 0
#define ACCELEROMETER_V2_STATUS_LED_CONFIG_ON 1
#define ACCELEROMETER_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define ACCELEROMETER_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_ACCELERATION 1
#define FID_SET_CONFIGURATION 2
#define FID_GET_CONFIGURATION 3
#define FID_SET_ACCELERATION_CALLBACK_CONFIGURATION 4
#define FID_GET_ACCELERATION_CALLBACK_CONFIGURATION 5
#define FID_SET_INFO_LED_CONFIG 6
#define FID_GET_INFO_LED_CONFIG 7
#define FID_SET_CONTINUOUS_ACCELERATION_CONFIGURATION 9
#define FID_GET_CONTINUOUS_ACCELERATION_CONFIGURATION 10
#define FID_SET_FILTER_CONFIGURATION 13
#define FID_GET_FILTER_CONFIGURATION 14

#define FID_CALLBACK_ACCELERATION 8
#define FID_CALLBACK_CONTINUOUS_ACCELERATION_16_BIT 11
#define FID_CALLBACK_CONTINUOUS_ACCELERATION_8_BIT 12

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAcceleration;

typedef struct {
	TFPMessageHeader header;
	int32_t x;
	int32_t y;
	int32_t z;
} __attribute__((__packed__)) GetAcceleration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t data_rate;
	uint8_t full_scale;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t data_rate;
	uint8_t full_scale;
} __attribute__((__packed__)) GetConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetAccelerationCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAccelerationCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetAccelerationCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t config;
} __attribute__((__packed__)) SetInfoLEDConfig;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetInfoLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t config;
} __attribute__((__packed__)) GetInfoLEDConfig_Response;

typedef struct {
	TFPMessageHeader header;
	int32_t x;
	int32_t y;
	int32_t z;
} __attribute__((__packed__)) Acceleration_Callback;

typedef struct {
	TFPMessageHeader header;
	bool enable_x;
	bool enable_y;
	bool enable_z;
	uint8_t resolution;
} __attribute__((__packed__)) SetContinuousAccelerationConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetContinuousAccelerationConfiguration;

typedef struct {
	TFPMessageHeader header;
	bool enable_x;
	bool enable_y;
	bool enable_z;
	uint8_t resolution;
} __attribute__((__packed__)) GetContinuousAccelerationConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	int16_t acceleration[30];
} __attribute__((__packed__)) ContinuousAcceleration16Bit_Callback;

typedef struct {
	TFPMessageHeader header;
	int8_t acceleration[60];
} __attribute__((__packed__)) ContinuousAcceleration8Bit_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t iir_bypass;
	uint8_t low_pass_filter;
} __attribute__((__packed__)) SetFilterConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetFilterConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t iir_bypass;
	uint8_t low_pass_filter;
} __attribute__((__packed__)) GetFilterConfiguration_Response;

// Function prototypes
BootloaderHandleMessageResponse get_acceleration(const GetAcceleration *data, GetAcceleration_Response *response);
BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data);
BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response);
BootloaderHandleMessageResponse set_acceleration_callback_configuration(const SetAccelerationCallbackConfiguration *data);
BootloaderHandleMessageResponse get_acceleration_callback_configuration(const GetAccelerationCallbackConfiguration *data, GetAccelerationCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_info_led_config(const SetInfoLEDConfig *data);
BootloaderHandleMessageResponse get_info_led_config(const GetInfoLEDConfig *data, GetInfoLEDConfig_Response *response);
BootloaderHandleMessageResponse set_continuous_acceleration_configuration(const SetContinuousAccelerationConfiguration *data);
BootloaderHandleMessageResponse get_continuous_acceleration_configuration(const GetContinuousAccelerationConfiguration *data, GetContinuousAccelerationConfiguration_Response *response);
BootloaderHandleMessageResponse set_filter_configuration(const SetFilterConfiguration *data);
BootloaderHandleMessageResponse get_filter_configuration(const GetFilterConfiguration *data, GetFilterConfiguration_Response *response);

// Callbacks
bool handle_acceleration_callback(void);
bool handle_continuous_acceleration_16_bit_callback(void);
bool handle_continuous_acceleration_8_bit_callback(void);


#endif
