#include <atmel_start.h>

static void button_pressed(void)
{
	// Toggle LED Pin
	gpio_toggle_pin_level(STATUSLED);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Register the Interrupt
	ext_irq_register(PA15, button_pressed);

	/* Replace with your application code */
	while (1) {
	}
}
