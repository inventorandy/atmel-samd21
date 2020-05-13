#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1)
	{
		// Toggle LED Pin
		gpio_toggle_pin_level(BLINKLED);
		
		// Delay in Loop
		delay_ms(1024);
	}
}
