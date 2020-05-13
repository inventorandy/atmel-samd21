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

void TSYS_example(void)
{
	struct io_descriptor *TSYS_io;

	i2c_m_sync_get_io_descriptor(&TSYS, &TSYS_io);
	i2c_m_sync_enable(&TSYS);
	i2c_m_sync_set_slaveaddr(&TSYS, 0x12, I2C_M_SEVEN);
	io_write(TSYS_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using DEBUGOUT to write "Hello World" using the IO abstraction.
 */
void DEBUGOUT_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&DEBUGOUT, &io);
	usart_sync_enable(&DEBUGOUT);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

static struct timer_task TIMER_task1, TIMER_task2;
/**
 * Example of using TIMER.
 */
static void TIMER_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_example(void)
{
	TIMER_task1.interval = 100;
	TIMER_task1.cb       = TIMER_task1_cb;
	TIMER_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_task2.interval = 200;
	TIMER_task2.cb       = TIMER_task2_cb;
	TIMER_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER, &TIMER_task1);
	timer_add_task(&TIMER, &TIMER_task2);
	timer_start(&TIMER);
}
