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
void communication_init(void);

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

#define ACCELEROMETER_V2_RESOLUTION_8BIT 0
#define ACCELEROMETER_V2_RESOLUTION_16BIT 1

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

#define FID_CALLBACK_ACCELERATION 6

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAcceleration;

typedef struct {
	TFPMessageHeader header;
	int16_t x;
	int16_t y;
	int16_t z;
} __attribute__((__packed__)) GetAcceleration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t data_rate;
	uint8_t full_scale;
	uint8_t resolution;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t data_rate;
	uint8_t full_scale;
	uint8_t resolution;
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
	int16_t x;
	int16_t y;
	int16_t z;
} __attribute__((__packed__)) Acceleration_Callback;


// Function prototypes
BootloaderHandleMessageResponse get_acceleration(const GetAcceleration *data, GetAcceleration_Response *response);
BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data);
BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response);
BootloaderHandleMessageResponse set_acceleration_callback_configuration(const SetAccelerationCallbackConfiguration *data);
BootloaderHandleMessageResponse get_acceleration_callback_configuration(const GetAccelerationCallbackConfiguration *data, GetAccelerationCallbackConfiguration_Response *response);

// Callbacks
bool handle_acceleration_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 1
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_acceleration_callback, \


#endif
