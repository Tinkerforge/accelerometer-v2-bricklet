/* accelerometer-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_kx122.h: Configuration for KX122 accelerometer
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

#ifndef CONFIG_KX122_H
#define CONFIG_KX122_H

#include "xmc_gpio.h"
#include "xmc_spi.h"

#define KX122_INT1_PIN             P0_5
#define KX122_INT2_PIN             P0_0
#define KX122_TRIGGER_PIN          P0_12

#define KX122_TIMER_CCU4           CCU40
#define KX122_TIMER_CCU4_SLICE     CCU40_CC40

#define KX122_IRQ_INT2             21
#define KX122_IRQ_INT2_PRIORITY    0


#define KX122_SPI_BAUDRATE         400000
#define KX122_USIC_CHANNEL         USIC0_CH1
#define KX122_USIC                 XMC_SPI0_CH1

#define KX122_SCLK_PIN             P0_8
#define KX122_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_8_AF_U0C1_SCLKOUT)

#define KX122_SELECT_PIN           P0_9
#define KX122_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_9_AF_U0C1_SELO0)

#define KX122_MOSI_PIN             P0_6
#define KX122_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_6_AF_U0C1_DOUT0)

#define KX122_MISO_PIN             P0_7
#define KX122_MISO_INPUT           XMC_USIC_CH_INPUT_DX0
#define KX122_MISO_SOURCE          0b011 // DX0D

#define KX122_SERVICE_REQUEST_RX   2
#define KX122_SERVICE_REQUEST_TX   3

#define KX122_IRQ_RX               11
#define KX122_IRQ_RX_PRIORITY      2
#define KX122_IRQ_TX               12
#define KX122_IRQ_TX_PRIORITY      3

#endif