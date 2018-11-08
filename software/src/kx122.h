/* accelerometer-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * kx122.h: Driver for KX122 accelerometer
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

#ifndef KX122_H
#define KX122_H

#include <stdint.h>
#include <stdbool.h>

#define KX122_CONT_ACCELERATION_BUFFER_SIZE (1000*3)

#define KX122_DATA_SPI_BUFFER_SIZE 128

#define KX122_X 0
#define KX122_Y 1
#define KX122_Z 2

typedef struct {
	uint8_t data_spi[KX122_DATA_SPI_BUFFER_SIZE];
	uint8_t data_read_index;
	uint8_t data_write_index;
	uint16_t data_length;

	int32_t acceleration[3];

	uint32_t acceleration_period;
	bool acceleration_value_has_to_change;

	uint32_t acceleration_mult;

	uint8_t config_current_data_rate;
	uint8_t config_current_full_scale;

	uint8_t config_new_data_rate;
	uint8_t config_new_full_scale;
	bool config_new;

	bool config_cont_current_enable[3];
	uint8_t config_cont_current_resolution;

	bool config_cont_new_enable[3];
	uint8_t config_cont_new_resolution;
	bool config_cont_new;

	bool cont_enabled;
	int16_t cont_acceleration[KX122_CONT_ACCELERATION_BUFFER_SIZE];
} KX122;

extern KX122 kx122;

void kx122_init(void);
void kx122_tick(void);

#define KX122_REG_XHP_L         0x00
#define KX122_REG_XHP_H         0x01
#define KX122_REG_YHP_L         0x02
#define KX122_REG_YHP_H         0x03
#define KX122_REG_ZHP_L         0x04
#define KX122_REG_ZHP_H         0x05
#define KX122_REG_XOUT_L        0x06
#define KX122_REG_XOUT_H        0x07
#define KX122_REG_YOUT_L        0x08
#define KX122_REG_YOUT_H        0x09
#define KX122_REG_ZOUT_L        0x0A
#define KX122_REG_ZOUT_H        0x0B
#define KX122_REG_COTR          0x0C
#define KX122_REG_WHO_AM_I      0x0F
#define KX122_REG_TSCP          0x10
#define KX122_REG_TSPP          0x11
#define KX122_REG_INS1          0x12
#define KX122_REG_INS2          0x13
#define KX122_REG_INS3          0x14
#define KX122_REG_STATUS        0x15
#define KX122_REG_INT_REL       0x17
#define KX122_REG_CNTL1         0x18
#define KX122_REG_CNTL2         0x19
#define KX122_REG_CNTL3         0x1A
#define KX122_REG_ODCNTL        0x1B
#define KX122_REG_INC1          0x1C
#define KX122_REG_INC2          0x1D
#define KX122_REG_INC3          0x1E
#define KX122_REG_INC4          0x1F
#define KX122_REG_INC5          0x20
#define KX122_REG_INC6          0x21
#define KX122_REG_TILT_TIMER    0x22
#define KX122_REG_WUFC          0x23
#define KX122_REG_TDTRC         0x24
#define KX122_REG_TDTC          0x25
#define KX122_REG_TTH           0x26
#define KX122_REG_TTL           0x27
#define KX122_REG_FTD           0x28
#define KX122_REG_STD           0x29
#define KX122_REG_TLT           0x2A
#define KX122_REG_TWS           0x2B
#define KX122_REG_FFTH          0x2C
#define KX122_REG_FFC           0x2D
#define KX122_REG_FFCNTL        0x2E
#define KX122_REG_ATH           0x30
#define KX122_REG_TILT_ANGLE_LL 0x32
#define KX122_REG_TILT_ANGLE_HL 0x33
#define KX122_REG_HYST_SET      0x34
#define KX122_REG_LP_CNTL       0x35
#define KX122_REG_BUF_CNTL1     0x3A
#define KX122_REG_BUF_CNTL2     0x3B
#define KX122_REG_BUF_STATUS1   0x3C
#define KX122_REG_BUF_STATUS2   0x3D
#define KX122_REG_BUF_CLEAR     0x3E
#define KX122_REG_BUF_READ      0x3F
#define KX122_REG_SELF_TEST     0x60

#endif