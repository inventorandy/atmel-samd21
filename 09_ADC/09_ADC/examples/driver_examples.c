/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using BATTERY_ADC to generate waveform.
 */
void BATTERY_ADC_example(void)
{
	uint8_t buffer[2];

	adc_sync_enable_channel(&BATTERY_ADC, 0);

	while (1) {
		adc_sync_read_channel(&BATTERY_ADC, 0, buffer, 2);
	}
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}
