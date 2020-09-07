#include <atmel_start.h>
#include <stdio.h>

// ADC Calculation Defs
#define ADC_REF 3300
#define ADC_COUNT 4095.0f
#define ADC_LSB_MV (ADC_REF / ADC_COUNT)

// Number of readings to take and average over
#define NUM_READINGS 512.0f

/**
 * Union for Readings
 *
 */
typedef union u_reading {
	uint16_t i;
	uint8_t c[2];
} reading;

// Float for the Reading
float battery_voltage = 0.0f;

// Battery Reading
reading battery_reading;

/**
 * Battery Reading Method
 */
static void read_battery_level()
{
	uint32_t sum = 0;
	for (int x = 0; x < NUM_READINGS; x++)
	{
		// Read from the ADC
		adc_sync_read_channel(&BATTERY_ADC, 0, battery_reading.c, 2);
		
		// Add to the Sum
		sum += battery_reading.i;
		
		// Short Delay
		delay_us(20);
	}
	
	// Calculate the Battery Voltage
	battery_voltage = ((sum / NUM_READINGS) * ADC_LSB_MV) / 1000.0f;
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Enable ADC
	adc_sync_enable_channel(&BATTERY_ADC, 0);
	
	// Enable UART
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	/* Replace with your application code */
	while (1) {
		// Read the Battery Voltage
		read_battery_level();
		
		// Print the Reading
		printf("Battery reading at %0.2f volts\r\n", battery_voltage);
		
		// Delay by 5 Seconds
		delay_ms(5000);
	}
}
