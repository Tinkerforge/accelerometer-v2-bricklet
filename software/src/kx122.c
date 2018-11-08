/* accelerometer-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * kx122.c: Driver for KX122 accelerometer
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

#include "kx122.h"

#include "configs/config_kx122.h"

#include "bricklib2/os/coop_task.h"
#include "bricklib2/logging/logging.h"

#include "xmc_spi.h"
#include "xmc_ccu4.h"

#include "communication.h"

#define KX122_REG_READ 0x80

#define kx122_rx_irq_handler IRQ_Hdlr_11
#define kx122_tx_irq_handler IRQ_Hdlr_12

#define kx122_int2_irq_handler IRQ_Hdlr_21

const uint8_t kx122_data_rate_to_osa[] = {
	0b1000,
	0b1001,
	0b1010,
	0b1011,
	0b0000,
	0b0001,
	0b0010,
	0b0011,
	0b0100,
	0b0101,
	0b0110,
	0b0111,
	0b1100,
	0b1101,
	0b1110,
	0b1111,
};

KX122 kx122;
CoopTask kx122_task;

volatile bool kx122_int2 = false;
void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) kx122_int2_irq_handler(void) {
	kx122_int2 = true;
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) kx122_rx_irq_handler(void) {
	while(!XMC_USIC_CH_RXFIFO_IsEmpty(KX122_USIC)) {
		kx122.data_spi[kx122.data_read_index] = KX122_USIC->OUTR;
		kx122.data_read_index++;
	}
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) kx122_tx_irq_handler(void) {
	while(!XMC_USIC_CH_TXFIFO_IsFull(KX122_USIC)) {
		KX122_USIC->IN[0] = kx122.data_spi[kx122.data_write_index];
		kx122.data_write_index++;
		if(kx122.data_write_index >= kx122.data_length) {
			XMC_USIC_CH_TXFIFO_DisableEvent(KX122_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
			return;
		}
	}
}

void kx122_task_spi_transceive(void) {
	XMC_SPI_CH_EnableSlaveSelect(KX122_USIC, XMC_SPI_CH_SLAVE_SELECT_0);
	XMC_USIC_CH_TXFIFO_EnableEvent(KX122_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
	XMC_USIC_CH_TriggerServiceRequest(KX122_USIC, KX122_SERVICE_REQUEST_TX);

	while(kx122.data_read_index < kx122.data_length) {
		// Turn rx event off before we manually look at rx fifo to be sure that there can't be any race condition
		XMC_USIC_CH_RXFIFO_DisableEvent(KX122_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
		kx122_rx_irq_handler();
		XMC_USIC_CH_RXFIFO_EnableEvent(KX122_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
		coop_task_yield();
	}

	XMC_SPI_CH_DisableSlaveSelect(KX122_USIC);
}

int8_t kx122_task_spi_read(uint8_t reg, uint8_t *data, uint16_t length) {
	kx122.data_read_index  = 0;
	kx122.data_write_index = 0;
	kx122.data_spi[0] = reg | KX122_REG_READ;
	for(uint16_t i = 0; i < length; i++) {
		kx122.data_spi[i+1] = reg + 1 + i;
	}

	kx122.data_length = length + 1;

	kx122_task_spi_transceive();

	memcpy(data, &kx122.data_spi[1], length);

	return 0;
}

int8_t kx122_task_spi_write(uint8_t reg, uint8_t *data, uint16_t length) {
	kx122.data_read_index  = 0;
	kx122.data_write_index = 0;
	kx122.data_spi[0] = reg;
	memcpy(&kx122.data_spi[1], data, length);

	kx122.data_length = length + 1;

	kx122_task_spi_transceive();

	return 0;
}

uint32_t kx122_get_standby_delay_in_ms(const uint8_t data_rate) {
	// Standby delay should be 2/ODR seconds
	switch(data_rate) {
		case ACCELEROMETER_V2_DATA_RATE_0_781HZ:  return 2560;
		case ACCELEROMETER_V2_DATA_RATE_1_563HZ:  return 1280;
		case ACCELEROMETER_V2_DATA_RATE_3_125HZ:  return 640;
		case ACCELEROMETER_V2_DATA_RATE_6_2512HZ: return 320;
		case ACCELEROMETER_V2_DATA_RATE_12_5HZ:   return 160;
		case ACCELEROMETER_V2_DATA_RATE_25HZ:     return 2000/25;
		case ACCELEROMETER_V2_DATA_RATE_50HZ:     return 2000/50;
		case ACCELEROMETER_V2_DATA_RATE_100HZ:    return 2000/100;
		case ACCELEROMETER_V2_DATA_RATE_200HZ:    return 2000/200;
		case ACCELEROMETER_V2_DATA_RATE_400HZ:    return 2000/400;
		default:                                  return 3;
	}
}

void kx122_update_config_task(void) {
	if(kx122.config_new || kx122.config_cont_new) {
		kx122.config_new = false;
		kx122.config_cont_new =  false;

		bool use_stream = kx122.config_cont_new_enable[0] || kx122.config_cont_new_enable[1] || kx122.config_cont_new_enable[2];

		kx122.config_cont_current_enable[0]  = kx122.config_cont_new_enable[0];
		kx122.config_cont_current_enable[1]  = kx122.config_cont_new_enable[1];
		kx122.config_cont_current_enable[2]  = kx122.config_cont_new_enable[2];
		kx122.config_cont_current_resolution = kx122.config_cont_new_resolution;

		uint8_t old_data_rate  = kx122.config_current_data_rate;
		uint8_t old_full_scale = kx122.config_current_full_scale;

		kx122.config_current_data_rate  = kx122.config_new_data_rate;
		kx122.config_current_full_scale = kx122.config_new_full_scale;

		// Standby mode
		uint8_t data = 0b00100000 | (old_full_scale << 3); 
		kx122_task_spi_write(KX122_REG_CNTL1, &data, 1);

		// Wait for kx122 to enter standby
		coop_task_sleep_ms(kx122_get_standby_delay_in_ms(old_data_rate));

		// Set new data rate
		data = kx122_data_rate_to_osa[kx122.config_current_data_rate];
		kx122_task_spi_write(KX122_REG_ODCNTL, &data, 1);

		// Clear buffer
		data = 0;
		kx122_task_spi_write(KX122_REG_BUF_CLEAR, &data, 1);
		
		if(use_stream) {
			// Set watermark interrupt threshold to 66
			data = 66;
			kx122_task_spi_write(KX122_REG_BUF_CNTL1, &data, 1);
			data = 0b11100001;
			kx122_task_spi_write(KX122_REG_BUF_CNTL2, &data, 1);
			data = 0b00100000;
			kx122_task_spi_write(KX122_REG_INS2, &data, 1);

			data = 0b00111011; // enable and clear latched interrupt source for interrupt 2
			kx122_task_spi_write(KX122_REG_INC5, &data, 1);
			data = 0b00100000; // watermark interrupt on interrupt 2
			kx122_task_spi_write(KX122_REG_INC6, &data, 1);

			kx122.cont_enabled = true;
		} else {
			// Set interrupt config
			data = 0b00111011; // enable and clear latched interrupt source for interrupt 2
			kx122_task_spi_write(KX122_REG_INC5, &data, 1);
			data = 0b00010000; // data ready on interrupt 2
			kx122_task_spi_write(KX122_REG_INC6, &data, 1);

			kx122.cont_enabled = false;
		}

		// Operating mode, data ready enable and new full scale
		data = 0b10100000 | (kx122.config_current_full_scale << 3); 
		kx122_task_spi_write(KX122_REG_CNTL1, &data, 1);

		// Wait for kx122 to leave standby
		coop_task_sleep_ms(kx122_get_standby_delay_in_ms(kx122.config_current_data_rate));

		switch(kx122.config_current_full_scale) {
			case ACCELEROMETER_V2_FULL_SCALE_2G: kx122.acceleration_mult = 625;  break; // 625  = 20000/32
			case ACCELEROMETER_V2_FULL_SCALE_4G: kx122.acceleration_mult = 1250; break; // 1250 = 40000/32
			case ACCELEROMETER_V2_FULL_SCALE_8G: kx122.acceleration_mult = 2500; break; // 2500 = 80000/32
		}
	}
}

void kx122_read_buffer_task(void) {
	kx122_task_spi_read(KX122_REG_BUF_READ, (uint8_t *)kx122.cont_acceleration, 120);

	kx122.acceleration[0] = kx122.cont_acceleration[27] * kx122.acceleration_mult / 1024;
	kx122.acceleration[1] = kx122.cont_acceleration[28] * kx122.acceleration_mult / 1024;
	kx122.acceleration[2] = kx122.cont_acceleration[29] * kx122.acceleration_mult / 1024;
}

void kx122_tick_task(void) {
	int16_t acceleration[3] = {0, 0, 0};

	while(true) {
		kx122_update_config_task();

		if(kx122_int2) {
			kx122_int2 = false;
			if(kx122.cont_enabled) {
				kx122_read_buffer_task();
			} else {
				kx122_task_spi_read(0x06, (uint8_t*)&acceleration, 6);
				kx122.acceleration[KX122_X] = acceleration[KX122_X] * kx122.acceleration_mult / 1024; // 1024 = 32768/32
				kx122.acceleration[KX122_Y] = acceleration[KX122_Y] * kx122.acceleration_mult / 1024;
				kx122.acceleration[KX122_Z] = acceleration[KX122_Z] * kx122.acceleration_mult / 1024;
			}
		} else {
			coop_task_yield();
		}
	}
}

void kx122_init_spi(void) {
 	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = KX122_SPI_BAUDRATE,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
		.mode             = KX122_MOSI_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// MISO pin configuration
	const XMC_GPIO_CONFIG_t miso_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
		.mode             = KX122_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// SELECT pin configuration
	const XMC_GPIO_CONFIG_t select_pin_config = {
		.mode             = KX122_SELECT_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure MISO pin
	XMC_GPIO_Init(KX122_MISO_PIN, &miso_pin_config);

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(KX122_USIC, &channel_config);
	KX122_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0
	KX122_USIC->PCR_SSCMode &= ~USIC_CH_PCR_SSCMode_TIWEN_Msk; // Disable time between bytes

	XMC_SPI_CH_SetBitOrderMsbFirst(KX122_USIC);

	XMC_SPI_CH_SetWordLength(KX122_USIC, 8);
	XMC_SPI_CH_SetFrameLength(KX122_USIC, 64);

	XMC_SPI_CH_SetTransmitMode(KX122_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(KX122_USIC,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(KX122_USIC, 2);


	// Set input source path
	XMC_SPI_CH_SetInputSource(KX122_USIC, KX122_MISO_INPUT, KX122_MISO_SOURCE);

	// SPI Mode: CPOL=1 and CPHA=1
	KX122_USIC_CHANNEL->DX1CR |= USIC_CH_DX1CR_DPOL_Msk;

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(KX122_USIC, 48, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Configure receive FIFO
	XMC_USIC_CH_RXFIFO_Configure(KX122_USIC, 32, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Set service request for tx FIFO transmit interrupt
	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(KX122_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, KX122_SERVICE_REQUEST_TX);  // IRQ KX122_IRQ_TX

	// Set service request for rx FIFO receive interrupt
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(KX122_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD, KX122_SERVICE_REQUEST_RX);  // IRQ KX122_IRQ_RX
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(KX122_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE, KX122_SERVICE_REQUEST_RX); // IRQ KX122_IRQ_RX

	//Set priority and enable NVIC node for transmit interrupt
	NVIC_SetPriority((IRQn_Type)KX122_IRQ_TX, KX122_IRQ_TX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)KX122_IRQ_TX);

	// Set priority and enable NVIC node for receive interrupt
	NVIC_SetPriority((IRQn_Type)KX122_IRQ_RX, KX122_IRQ_RX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)KX122_IRQ_RX);

	// Start SPI
	XMC_SPI_CH_Start(KX122_USIC);

	// Configure SCLK pin
	XMC_GPIO_Init(KX122_SCLK_PIN, &sclk_pin_config);

	// Configure slave select pin
	XMC_GPIO_Init(KX122_SELECT_PIN, &select_pin_config);

	// Configure MOSI pin
	XMC_GPIO_Init(KX122_MOSI_PIN, &mosi_pin_config);

	XMC_USIC_CH_RXFIFO_Flush(KX122_USIC);
	XMC_USIC_CH_RXFIFO_EnableEvent(KX122_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
}

// We need an interrupt on P0_0. Since this pin is not interrupt capable
// we use the CCU4 instead to generate an interrupt on rising edge.
void kx122_init_int2(void) {
	XMC_CCU4_SLICE_CAPTURE_CONFIG_t capture_config = {
		.fifo_enable = true,
		.timer_clear_mode = XMC_CCU4_SLICE_TIMER_CLEAR_MODE_ALWAYS,
		.same_event = false,
		.ignore_full_flag = true,
		.prescaler_mode = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
		.prescaler_initval = 0,
		.float_limit = 0,
		.timer_concatenation = false,
	};

	XMC_CCU4_SLICE_EVENT_CONFIG_t capture_event0_config = {
		.mapped_input = XMC_CCU4_SLICE_INPUT_C,
		.edge = XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE,
		.level = XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH,
		.duration = XMC_CCU4_SLICE_EVENT_FILTER_DISABLED,
	};

    XMC_CCU4_Init(KX122_TIMER_CCU4, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);

    // Start the prescaler
    XMC_CCU4_StartPrescaler(KX122_TIMER_CCU4);

	XMC_CCU4_SLICE_CaptureInit(KX122_TIMER_CCU4_SLICE, &capture_config);

	// Input selection with edge trigger
	XMC_CCU4_SLICE_ConfigureEvent(KX122_TIMER_CCU4_SLICE, XMC_CCU4_SLICE_EVENT_0, &capture_event0_config);

	// Enable capture slice (clearing a slice from idle mode)
	XMC_CCU4_EnableClock(KX122_TIMER_CCU4, 0);

	// Enable the event generation for event_id
	XMC_CCU4_SLICE_EnableEvent(KX122_TIMER_CCU4_SLICE, XMC_CCU4_SLICE_IRQ_ID_EVENT0);

	// Configure the SR line for the event
	XMC_CCU4_SLICE_SetInterruptNode(KX122_TIMER_CCU4_SLICE, XMC_CCU4_SLICE_IRQ_ID_EVENT0, XMC_CCU4_SLICE_SR_ID_0);

	// Interrupt configuration for Capture
	NVIC_SetPriority(KX122_IRQ_INT2, KX122_IRQ_INT2_PRIORITY);
	NVIC_EnableIRQ(KX122_IRQ_INT2);
}

void kx122_init(void) {
	memset(&kx122, 0, sizeof(KX122));

	const XMC_GPIO_CONFIG_t pin_config_input = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	const XMC_GPIO_CONFIG_t pin_config_output = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_LOW
	};

	XMC_GPIO_Init(KX122_INT1_PIN,    &pin_config_input);
	XMC_GPIO_Init(KX122_INT2_PIN,    &pin_config_input);
	XMC_GPIO_Init(KX122_TRIGGER_PIN, &pin_config_output);

	kx122_init_int2();
	kx122_init_spi();

	kx122.config_new            = true;
	kx122.config_new_data_rate  = ACCELEROMETER_V2_DATA_RATE_100HZ;
	kx122.config_new_full_scale = ACCELEROMETER_V2_FULL_SCALE_2G;

	coop_task_init(&kx122_task, kx122_tick_task);
}

void kx122_tick(void) {
	coop_task_tick(&kx122_task);
}