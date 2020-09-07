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

#include <hpl_adc_base.h>

struct adc_sync_descriptor BATTERY_ADC;

struct usart_sync_descriptor TARGET_IO;

void BATTERY_ADC_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PB00, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PB00, PINMUX_PB00B_ADC_AIN8);

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA04, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA04, PINMUX_PA04B_ADC_VREFP);
}

void BATTERY_ADC_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, ADC);
	_gclk_enable_channel(ADC_GCLK_ID, CONF_GCLK_ADC_SRC);
}

void BATTERY_ADC_init(void)
{
	BATTERY_ADC_CLOCK_init();
	BATTERY_ADC_PORT_init();
	adc_sync_init(&BATTERY_ADC, ADC, (void *)NULL);
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM3_PAD0);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM3_PAD1);
}

void TARGET_IO_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	usart_sync_init(&TARGET_IO, SERCOM3, (void *)NULL);
	TARGET_IO_PORT_init();
}

void system_init(void)
{
	init_mcu();

	BATTERY_ADC_init();

	TARGET_IO_init();
}
