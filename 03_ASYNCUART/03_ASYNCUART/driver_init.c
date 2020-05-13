/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

/*! The buffer size for USART */
#define SERIAL_BUFFER_SIZE 16

struct usart_async_descriptor SERIAL;

static uint8_t SERIAL_buffer[SERIAL_BUFFER_SIZE];

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void SERIAL_CLOCK_init()
{

	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void SERIAL_PORT_init()
{

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM3_PAD0);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM3_PAD1);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void SERIAL_init(void)
{
	SERIAL_CLOCK_init();
	usart_async_init(&SERIAL, SERCOM3, SERIAL_buffer, SERIAL_BUFFER_SIZE, (void *)NULL);
	SERIAL_PORT_init();
}

void system_init(void)
{
	init_mcu();

	SERIAL_init();
}
