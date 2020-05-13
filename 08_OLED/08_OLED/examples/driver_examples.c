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
 * Example of using DISPLAY_SPI to write "Hello World" using the IO abstraction.
 */
static uint8_t example_DISPLAY_SPI[12] = "Hello World!";

void DISPLAY_SPI_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&DISPLAY_SPI, &io);

	spi_m_sync_enable(&DISPLAY_SPI);
	io_write(io, example_DISPLAY_SPI, 12);
}
