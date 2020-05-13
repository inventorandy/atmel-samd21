/**
 * \file
 *
 * \brief Font and text drawing routines
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

#include <gfx_mono.h>
#include <gfx_mono_text.h>
#include <utils_assert.h>

#define CONFIG_FONT_PIXELS_PER_BYTE 8

static void gfx_mono_text_draw_char_progmem(const struct gfx_mono_text *const me, const uint8_t ch, const gfx_coord_t x,
                                            const gfx_coord_t y, const struct font *font);

/**
 * \brief Construct GFX mono text
 */
struct gfx_mono_text *gfx_mono_text_construct(struct gfx_mono_text *const me, struct gfx_mono *const gfx)
{
	me->gfx = gfx;

	return me;
}

/**
 * \brief Draws a character to the display
 */
void gfx_mono_text_draw_char(const struct gfx_mono_text *const me, const uint8_t c, const gfx_coord_t x,
                             const gfx_coord_t y, const struct font *font)
{
	gfx_mono_draw_filled_rect(me->gfx, x, y, font->width, font->height, GFX_PIXEL_CLR);

	switch (font->type) {
	case FONT_LOC_PROGMEM:
		gfx_mono_text_draw_char_progmem(me, c, x, y, font);
		break;

	default:
		/* Unsupported mode, call assert */
		ASSERT(false);
		break;
	}
}

/**
 * \brief Draws a string to the display
 */
void gfx_mono_text_draw_string(const struct gfx_mono_text *const me, const uint8_t *str, gfx_coord_t x, gfx_coord_t y,
                               const struct font *font)
{
	/* Save X in order to know where to return to on CR. */
	const gfx_coord_t start_of_string_position_x = x;

	/* Sanity check on parameters, assert if str or font is NULL. */
	ASSERT(str != NULL);
	ASSERT(font != NULL);

	/* Draw characters until trailing null byte */
	do {
		/* Handle '\n' as newline, draw normal characters. */
		if (*str == '\n') {
			x = start_of_string_position_x;
			y += font->height + 1;
		} else if (*str == '\r') {
			/* Skip '\r' characters. */
		} else {
			gfx_mono_text_draw_char(me, *str, x, y, font);
			x += font->width;
		}
	} while (*(++str));
}

/**
 * \brief Draws a string located in program memory to the display
 */
void gfx_mono_text_draw_progmem_string(const struct gfx_mono_text *const me, uint8_t PROGMEM_PTR_T str, gfx_coord_t x,
                                       gfx_coord_t y, const struct font *font)
{
	uint8_t temp_char;
	/* Save X in order to know where to return to on CR. */
	const gfx_coord_t start_of_string_position_x = x;

	/* Sanity check on parameters, assert if str or font is NULL. */
	ASSERT(str != NULL);
	ASSERT(font != NULL);

	/* Draw characters until trailing null byte */
	temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)str);

	while (temp_char) {
		/* Handle '\n' as newline, draw normal characters. */
		if (temp_char == '\n') {
			x = start_of_string_position_x;
			y += font->height + 1;
		} else if (temp_char == '\r') {
			/* Skip '\r' characters. */
		} else {
			gfx_mono_text_draw_char(me, temp_char, x, y, font);
			x += font->width;
		}

		temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(++str));
	}
}

/**
 * \brief Computes the bounding box of a string
 */
void gfx_mono_text_get_string_bounding_box(const struct gfx_mono_text *const me, const uint8_t *str,
                                           const struct font *font, gfx_coord_t *width, gfx_coord_t *height)
{
	gfx_coord_t font_width  = font->width;
	gfx_coord_t font_height = font->height;

	gfx_coord_t max_width  = 1;
	gfx_coord_t max_height = font_height;
	gfx_coord_t x          = 0;

	/* Sanity check on parameters, assert if str or font is NULL. */
	ASSERT(str != NULL);
	ASSERT(font != NULL);

	/* Handle each character until trailing null byte */
	do {
		/* Handle '\n' as newline, draw normal characters. */
		if (*str == '\n') {
			x = 0;
			max_height += font_height;
		} else if (*str == '\r') {
			/* Skip '\r' characters. */
		} else {
			x += font_width;
			if (x > max_width) {
				max_width = x;
			}
		}
	} while (*(++str));

	/* Return values through references */
	*width  = max_width;
	*height = max_height;
}

/**
 * \brief Computes the bounding box of a string located in program memory
 */
void gfx_mono_text_get_progmem_string_bounding_box(const struct gfx_mono_text *const me, uint8_t PROGMEM_PTR_T str,
                                                   const struct font *font, gfx_coord_t *width, gfx_coord_t *height)
{
	gfx_coord_t font_width  = font->width;
	gfx_coord_t font_height = font->height;

	uint8_t     temp_char;
	gfx_coord_t max_width  = 1;
	gfx_coord_t max_height = font_height;
	gfx_coord_t x          = 0;

	/* Sanity check on parameters, assert if str or font is NULL. */
	ASSERT(str != NULL);
	ASSERT(font != NULL);

	/* Handle each character until trailing null byte */
	temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)str);

	while (temp_char) {
		/* Handle '\n' as newline, draw normal characters. */
		if (temp_char == '\n') {
			x = 0;
			max_height += font_height;
		} else if (*str == '\r') {
			/* Skip '\r' characters. */
		} else {
			x += font_width;
			if (x > max_width) {
				max_width = x;
			}
		}

		temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(++str));
	}

	/* Return values through references */
	*width  = max_width;
	*height = max_height;
}

/**
 * \internal
 * \brief Helper function that draws a character from a font in progmem
 *        to the display
 *
 * This function will first calculate the start offset in the font character
 * data before iterating over the specific character data.
 *
 * Only pixels in the character that should be enabled are done so, the caller
 * is required to prepare the drawing area before printing a character to it.
 * This is done by the gfx_mono_draw_string() and
 * gfx_mono_draw_progmem_string() functions.
 *
 * \param[in] ch       Character to be drawn
 * \param[in] x        X coordinate on screen.
 * \param[in] y        Y coordinate on screen.
 * \param[in] font     Font to draw character in
 */
static void gfx_mono_text_draw_char_progmem(const struct gfx_mono_text *const me, const uint8_t ch, const gfx_coord_t x,
                                            const gfx_coord_t y, const struct font *font)
{
	uint8_t PROGMEM_PTR_T glyph_data;
	uint16_t              glyph_data_offset;
	uint8_t               char_row_size;
	uint8_t               rows_left;
	uint8_t               i;
	gfx_coord_t           inc_x = x;
	gfx_coord_t           inc_y = y;

	/* Sanity check on parameters, assert if font is NULL. */
	ASSERT(font != NULL);

	char_row_size = font->width / CONFIG_FONT_PIXELS_PER_BYTE;
	if (font->width % CONFIG_FONT_PIXELS_PER_BYTE) {
		char_row_size++;
	}

	glyph_data_offset = char_row_size * font->height * ((uint8_t)ch - font->first_char);
	glyph_data        = font->data.progmem + glyph_data_offset;
	rows_left         = font->height;

	do {
		uint8_t glyph_byte   = 0;
		uint8_t pixelsToDraw = font->width;

		for (i = 0; i < pixelsToDraw; i++) {
			if (i % CONFIG_FONT_PIXELS_PER_BYTE == 0) {
				glyph_byte = PROGMEM_READ_BYTE(glyph_data);
				glyph_data++;
			}

			if ((glyph_byte & 0x80)) {
				me->gfx->display->interface->draw_pixel(me->gfx->display, inc_x, inc_y, GFX_PIXEL_SET);
			}

			inc_x += 1;
			glyph_byte <<= 1;
		}

		inc_y += 1;
		inc_x = x;
		rows_left--;
	} while (rows_left > 0);
}
