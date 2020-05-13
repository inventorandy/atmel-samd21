/**
 * \file
 *
 * \brief Abstract display related functionality implementation.
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
#include <display_mono.h>

/**
 * \brief Construct / initialize instance of abstract display
 */
struct display_mono *display_mono_construct(struct display_mono *const me, struct display_ctrl_mono *const dc,
                                            const gfx_coord_t width, const gfx_coord_t height,
                                            struct display_mono_interface *const interface)
{
	me->dc        = dc;
	me->width     = width;
	me->height    = height;
	me->interface = interface;

	return me;
}

/**
 * \brief Put a byte to the display controller RAM
 */
void display_mono_put_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                           const uint8_t data)
{
	me->dc->interface->set_page_address(me->dc, page);
	me->dc->interface->set_column_address(me->dc, column);
	me->dc->interface->write_data(me->dc, data);
}

/**
 * \brief Get a byte from the display controller RAM
 */
uint8_t display_mono_get_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column)
{
	me->dc->interface->set_page_address(me->dc, page);
	me->dc->interface->set_column_address(me->dc, column);

	return me->dc->interface->read_data(me->dc);
}

/**
 * \brief Read/Modify/Write a byte on the display controller
 */
void display_mono_mask_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                            const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color)
{
	uint8_t temp = me->interface->get_byte(me, page, column);

	switch (color) {
	case GFX_PIXEL_SET:
		temp |= (uint8_t)pixel_mask;
		break;

	case GFX_PIXEL_CLR:
		temp &= ~(uint8_t)pixel_mask;
		break;

	case GFX_PIXEL_XOR:
		temp ^= (uint8_t)pixel_mask;
		break;

	default:
		break;
	}

	me->interface->put_byte(me, page, column, temp);
}

/**
 * \brief Get the pixel value at x,y
 */
uint8_t display_mono_get_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                               const gfx_coord_t width, const gfx_coord_t height, const uint8_t pixels)
{
	uint8_t page;
	uint8_t pixel_mask;

	if ((x > width - 1) || (y > height - 1)) {
		return 0;
	}

	page       = y / pixels;
	pixel_mask = (1 << (y - (page * 8)));

	return me->interface->get_byte(me, page, x) & pixel_mask;
}

/**
 * \brief Draw pixel to screen
 */
void display_mono_draw_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                             const enum gfx_mono_color color, const gfx_coord_t width, const gfx_coord_t height,
                             const uint8_t pixels)
{
	uint8_t page;
	uint8_t pixel_mask;
	uint8_t pixel_value;

	/* Discard pixels drawn outside the screen */
	if ((x > width - 1) || (y > height - 1)) {
		return;
	}

	page       = y / pixels;
	pixel_mask = (1 << (y - (page * 8)));

	/*
	 * Read the page containing the pixel in interest, then perform the
	 * requested action on this pixel before writing the page back to the
	 * display.
	 */
	pixel_value = me->interface->get_byte(me, page, x);

	switch (color) {
	case GFX_PIXEL_SET:
		pixel_value |= pixel_mask;
		break;

	case GFX_PIXEL_CLR:
		pixel_value &= ~pixel_mask;
		break;

	case GFX_PIXEL_XOR:
		pixel_value ^= pixel_mask;
		break;

	default:
		break;
	}

	me->interface->put_byte(me, page, x, pixel_value);
}

/**
 * \brief Read a page from the display controller
 */
void display_mono_get_page(const struct display_mono *const me, enum gfx_mono_color *data, const gfx_coord_t page,
                           const gfx_coord_t page_offset, gfx_coord_t width)
{
	me->dc->interface->set_page_address(me->dc, page);
	me->dc->interface->set_column_address(me->dc, page_offset);

	do {
		*data++ = (enum gfx_mono_color)me->dc->interface->read_data(me->dc);
	} while (--width);
}

/**
 * \brief Put a page from RAM to the display controller.
 */
void display_mono_put_page(const struct display_mono *const me, const enum gfx_mono_color *data, const gfx_coord_t page,
                           const gfx_coord_t page_offset, gfx_coord_t width)
{
	me->dc->interface->set_page_address(me->dc, page);
	me->dc->interface->set_column_address(me->dc, page_offset);

	do {
		me->dc->interface->write_data(me->dc, *data++);
	} while (--width);
}

/**
 * \brief Put framebuffer to the display controller
 */
void display_mono_put_framebuffer(const struct display_mono *const me, const uint8_t *const framebuffer,
                                  const gfx_coord_t width, const uint8_t pages)
{
	uint8_t page = 0;

	for (; page < pages; page++) {
		me->dc->interface->set_page_address(me->dc, page);
		me->dc->interface->set_column_address(me->dc, 0);
		me->interface->put_page(me, (const enum gfx_mono_color *const)framebuffer + (page * width), page, 0, width);
	}
}

/**
 * \brief Initialize abstract display controller and abstract display.
 */
void display_mono_init(const struct display_mono *const me, const gfx_coord_t width, const uint8_t pages)
{
	uint8_t page = 0;

	if (me->dc) {
		me->dc->interface->set_start_line_address(me->dc, 0);
	}
	for (; page < pages; page++) {
		uint8_t column = 0;

		for (; column < width; column++) {
			me->interface->put_byte(me, page, column, 0x00);
		}
	}
}
