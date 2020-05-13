#include <atmel_start.h>
#include <stdbool.h>
#include <stdio.h>
#include "ext_flash.h"

// Debug Out IO Descriptor
struct io_descriptor *debug_io;

// Flags for UART Receive
volatile debug_io_receiving, debug_io_complete = false;

// Maximum Size of UART Message for Memory
#define MAX_MESSAGE_SIZE		64

// Char Array for UART Received Bytes
static uint8_t debug_io_received_bytes[MAX_MESSAGE_SIZE];

// Byte Counter
uint8_t debug_io_bytes_received_counter = 0;

/**
 * UART Receive Callback function.
 *
 */
static void debug_rx_cb(const struct usart_async_descriptor *const io_descr)
{
	// Counters
	uint8_t ch, count;
	
	// Read a character
	count = io_read(debug_io, &ch, 1);
	
	// Now we're checking if we've received already
	if (debug_io_receiving == false)
	{
		// Set the Receiving Flag
		debug_io_receiving = true;
		
		// Reset the Byte Counter
		debug_io_bytes_received_counter = 0;
		
		// Start filling the buffer
		debug_io_received_bytes[debug_io_bytes_received_counter] = ch;
		
		// Increment the Counter
		debug_io_bytes_received_counter += count;
	}
	else
	{
		// Continue filling the buffer
		debug_io_received_bytes[debug_io_bytes_received_counter] = ch;
		
		// Increment the Counter
		debug_io_bytes_received_counter += count;
		
		// Check for end of string
		if (ch == '\n')
		{
			// Set the Completion Flag
			debug_io_complete = true;
		}
	}
}

/**
 * Method for initialising UART
 *
 */
static void init_com_port()
{
	// Register the Callback Function
	usart_async_register_callback(&DEBUGOUT, USART_ASYNC_RXC_CB, debug_rx_cb);
	
	// Set the IO Descriptor
	usart_async_get_io_descriptor(&DEBUGOUT, &debug_io);
	
	// Enable ASYNC UART
	usart_async_enable(&DEBUGOUT);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Initialise COM Port
	init_com_port();
	
	// Initialise Serial Flash
	EXTFLASH_init();

	/* Replace with your application code */
	while (1)
	{
		// Check if we're receiving
		if (debug_io_receiving == true)
		{
			// Check if receive complete
			if (debug_io_complete == true)
			{
				// Open Serial Flash
				if (EXTFLASH_open())
				{
					// Erase First Block
					if (EXTFLASH_erase(0, FLASH_ERASE_SECTOR_SIZE))
					{
						// Delay to allow erase
						delay_ms(240);
					}
					
					// Buffer for Write Data
					uint8_t wdata[MAX_MESSAGE_SIZE];
					memcpy(&wdata[0], &debug_io_received_bytes[0], MAX_MESSAGE_SIZE);
					
					// Attempt to Write Data
					if (EXTFLASH_write(0, MAX_MESSAGE_SIZE, wdata))
					{
						// Buffer for Read Data
						uint8_t rdata[MAX_MESSAGE_SIZE];
						memset(rdata, 0x00, MAX_MESSAGE_SIZE);
						
						// Attempt to Read Data
						if (EXTFLASH_read(0, MAX_MESSAGE_SIZE, &rdata[0]))
						{
							// Response String
							char *response = malloc(128);
							sprintf(response, "Stored in memory: %s", rdata);
							
							// Print to Console
							io_write(debug_io, response, strlen(response));
						}
					}
				}
				
				// Reset Flags
				debug_io_receiving = false;
				debug_io_complete = false;
			}
		}
	}
}
