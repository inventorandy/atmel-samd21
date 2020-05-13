#include <atmel_start.h>
#include <stdio.h>
#include "ext_tsys01.h"

// IO Descriptor for Debug UART
struct io_descriptor *debug_io;

// Timer Task for Reading Temperature
static struct timer_task temp_timer;

// Temperature Float
static float temperature = 0.0f;

// UART String
static char *temp_output;
static char buf[5];

/**
 * Initialise UART on Debug Out
 *
 */
void init_uart(void)
{
	// Enable UART Drivers on the virtual com port
	usart_sync_get_io_descriptor(&DEBUGOUT, &debug_io);
	usart_sync_enable(&DEBUGOUT);
}

static void read_temp_cb(const struct timer_task *const timer_task)
{
	// Read the Temperature
	read_temperature(&temperature);
	gcvt(temperature, 5, buf);
	
	// Create the Reading String
	sprintf(&temp_output[0], "Temperature is %sC\r\n", buf);
	
	// Write to Console
	io_write(debug_io, temp_output, strlen(temp_output));
}

void init_timer()
{
	// Set up the timer
	temp_timer.interval = 1000;
	temp_timer.cb = read_temp_cb;
	temp_timer.mode = TIMER_TASK_REPEAT;
	
	// Add it to the Queue
	timer_add_task(&TIMER, &temp_timer);
	timer_start(&TIMER);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Initialise the TSYS01 and Read the EEPROM values
	init_tsys();
	read_eeprom();
	
	// Allocate some memory for the temperature string
	temp_output = malloc(30);
	
	// Initialise the UART and Timer
	init_uart();
	init_timer();

	/* Replace with your application code */
	while (1) {
	}
}
