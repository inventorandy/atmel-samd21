/**
 * \file
 *
 * \brief SSD1306 display controller related functionality implementation.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <display_ctrl_mono.h>
#include <ssd1306.h>
#include <hal_gpio.h>
#include <ssd1306_font.h>

static void ssd1306_init(struct display_ctrl_mono *const me);

/**
 * \brief SSD1306 virtual functions table
 */
static struct display_ctrl_mono_interface ssd1306_interface = {
    ssd1306_write_data,
    ssd1306_read_data,
    ssd1306_set_page_address,
    ssd1306_set_column_address,
    ssd1306_set_start_line_address,
};

/**
 * \brief Construct / initialize instance of SSD1306 display controller
 */
struct display_ctrl_mono *ssd1306_construct(struct display_ctrl_mono *const me, struct io_descriptor *const io,
                                            const uint8_t cs, const uint8_t res, const uint8_t dc)
{
	struct ssd1306 *ssd = (struct ssd1306 *)me;

	display_ctrl_mono_construct(me, io, &ssd1306_interface);

	ssd->pin_cs  = cs;
	ssd->pin_res = res;
	ssd->pin_dc  = dc;

	ssd1306_init(me);

	return me;
}

/**
 * \brief Write a command to the display controller
 */
void ssd1306_write_command(const struct display_ctrl_mono *const me, const uint8_t command)
{
	struct io_descriptor *      io   = me->io;
	const struct ssd1306 *const ctrl = (const struct ssd1306 *const)me;

	gpio_set_pin_level(ctrl->pin_dc, false);
	gpio_set_pin_level(ctrl->pin_cs, false);
	io->write(io, &command, 1);
	gpio_set_pin_level(ctrl->pin_cs, true);
}

/**
 * \brief Write data to the display controller
 */
void ssd1306_write_data(const struct display_ctrl_mono *const me, const uint8_t data)
{
	struct io_descriptor *      io   = me->io;
	const struct ssd1306 *const ctrl = (const struct ssd1306 *const)me;

	gpio_set_pin_level(ctrl->pin_dc, true);
	gpio_set_pin_level(ctrl->pin_cs, false);
	io->write(io, &data, 1);
	gpio_set_pin_level(ctrl->pin_cs, true);
}

/**
 * \brief Read data from the display controller
 */
uint8_t ssd1306_read_data(const struct display_ctrl_mono *const me)
{
	(void)me;
	return 0;
}

/**
 * \brief Perform a hard reset of the display controller
 */
void ssd1306_hard_reset(const struct display_ctrl_mono *const me)
{
	display_ctrl_mono_hard_reset(((const struct ssd1306 *)me)->pin_res, 10);
}

/**
 * \brief Enable the display sleep mode
 */
void ssd1306_enable_sleep(const struct display_ctrl_mono *const me)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_DISPLAY_OFF);
}

/**
 * \brief Disable the display sleep mode
 */
void ssd1306_disable_sleep(const struct display_ctrl_mono *const me)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_DISPLAY_ON);
}

/**
 * \brief Set current page in display RAM
 */
void ssd1306_set_page_address(const struct display_ctrl_mono *const me, const uint8_t address)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_PAGE_START_ADDRESS(address & 0xF));
}

/**
 * \brief Set current column in display RAM
 */
void ssd1306_set_column_address(const struct display_ctrl_mono *const me, uint8_t address)
{
	address &= 0x7F;
	ssd1306_write_command(me, SSD1306_CMD_SET_HIGH_COL(address >> 4));
	ssd1306_write_command(me, SSD1306_CMD_SET_LOW_COL(address & 0x0F));
}

/**
 * \brief Set the display start draw line address
 */
void ssd1306_set_start_line_address(const struct display_ctrl_mono *const me, const uint8_t address)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_START_LINE(address & 0x3F));
}

/**
 * \brief Turn the display display on
 */
void ssd1306_on(const struct display_ctrl_mono *const me)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_DISPLAY_ON);
}

/**
 * \brief Turn the display display off
 */
void ssd1306_off(const struct display_ctrl_mono *const me)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_DISPLAY_OFF);
}

/**
 * \brief Initialize the display controller
 *
 * Call this function to initialize the hardware interface and the display
 * controller. When initialization is done the display is turned on and ready
 * to receive data.
 *
 * \param[in] me The pointer to display instance
 */
static void ssd1306_init(struct display_ctrl_mono *const me)
{
	ssd1306_hard_reset(me);
	ssd1306_write_command(me, SSD1306_CMD_SET_MULTIPLEX_RATIO);
	ssd1306_write_command(me, 0x1F);
	ssd1306_write_command(me, SSD1306_CMD_SET_DISPLAY_OFFSET);
	ssd1306_write_command(me, 0x00);
	ssd1306_write_command(me, SSD1306_CMD_SET_START_LINE(0x00));
	ssd1306_write_command(me, SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0);
	ssd1306_write_command(me, SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN);
	ssd1306_write_command(me, SSD1306_CMD_SET_COM_PINS);
	ssd1306_write_command(me, 0x02);
	ssd1306_set_contrast(me, 0x8F);
	ssd1306_write_command(me, SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON);
	ssd1306_write_command(me, SSD1306_CMD_SET_NORMAL_DISPLAY);
	ssd1306_write_command(me, SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
	ssd1306_write_command(me, 0x80);
	ssd1306_write_command(me, SSD1306_CMD_SET_CHARGE_PUMP_SETTING);
	ssd1306_write_command(me, 0x14);
	ssd1306_write_command(me, SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL);
	ssd1306_write_command(me, 0x40);
	ssd1306_write_command(me, SSD1306_CMD_SET_PRE_CHARGE_PERIOD);
	ssd1306_write_command(me, 0xF1);
	ssd1306_on(me);
}

/**
 * \brief Set the display contrast level
 */
uint8_t ssd1306_set_contrast(const struct display_ctrl_mono *const me, const uint8_t contrast)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);
	ssd1306_write_command(me, contrast);

	return contrast;
}

/**
 * \brief Invert all pixels on the display
 */
void ssd1306_display_invert_enable(const struct display_ctrl_mono *const me)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_INVERSE_DISPLAY);
}

/**
 * \brief Disable invert of all pixels on the display
 */
void ssd1306_display_invert_disable(const struct display_ctrl_mono *const me)
{
	ssd1306_write_command(me, SSD1306_CMD_SET_NORMAL_DISPLAY);
}

/**
 * \brief Clear the display
 */
void ssd1306_clear(const struct display_ctrl_mono *const me)
{
	for (uint8_t page = 0; page < 4; ++page) {
		ssd1306_set_page_address(me, page);
		ssd1306_set_column_address(me, 0);
		for (uint8_t col = 0; col < 128; ++col) {
			ssd1306_write_data(me, 0x00);
		}
	}
}

/**
 * \brief Display text on the display
 *
 * \param[in] string A string to write.
 */
void ssd1306_write_text(const struct display_ctrl_mono *const me, const uint8_t *string)
{
	uint8_t *char_ptr;
	uint8_t  i;

	while (*string != 0) {
		if (*string < 0x7F) {
			char_ptr = font_table[*string - 32];
			for (i = 1; i <= char_ptr[0]; i++) {
				ssd1306_write_data(me, char_ptr[i]);
			}
			ssd1306_write_data(me, 0x00);
		}
		string++;
	}
}
