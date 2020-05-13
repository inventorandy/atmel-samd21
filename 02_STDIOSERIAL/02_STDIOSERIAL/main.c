#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// Input Variable
	uint8_t *name = malloc(20);
	
	// Print a welcome message
	printf("Hello! What's your name?\r\n");
	
	// Get user's name
	scanf("%s", name);
	
	// Print second message
	printf("Hello %s. This is a cool video.\r\n", name);
}
