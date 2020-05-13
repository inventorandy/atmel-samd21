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

static void button_on_PA15_pressed(void)
{
}

/**
 * Example of using BUTTON
 */
void BUTTON_example(void)
{

	ext_irq_register(PIN_PA15, button_on_PA15_pressed);
}
