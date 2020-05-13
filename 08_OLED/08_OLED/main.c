#include <atmel_start.h>
#include <hal_gpio.h>
#include "gfx_mono_main.h"
#include "gfx_mono_font_basic_6x7.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Display Message
	const uint8_t msg[] = "Hello, YouTube!";
	
	// Enable SPI and OLED
	spi_m_sync_enable(&DISPLAY_SPI);
	gfx_mono_init();
	
	// Draw a Rectangle
	gfx_mono_draw_rect(&MONOCHROME_GRAPHICS_desc, 0, 0, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_HEIGHT, GFX_PIXEL_SET);
	
	// Draw a Message
	gfx_mono_text_draw_string(&MONOCHROME_TEXT_desc, msg, 20, 12, &basic_6x7);

	/* Replace with your application code */
	while (1) {
	}
}
