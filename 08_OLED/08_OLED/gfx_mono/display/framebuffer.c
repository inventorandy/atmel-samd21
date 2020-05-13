/**
 * \file
 *
 * \brief Framebuffer related functionality implementation.
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

#include <framebuffer.h>

/**
 * \brief Framebuffer virtual functions table
 */
static struct display_mono_interface framebuffer_interface
    = {framebuffer_get_byte, framebuffer_put_byte, framebuffer_put_page, framebuffer_draw_pixel, framebuffer_mask_byte};

/**
 * \brief Construct / initialize instance of framebuffer
 *
 * This function initializes the given framebuffer.
 *
 * \param[in] me          The pointer to framebuffer
 * \param[in] framebuffer The pointer to a RAM buffer
 * \param[in] width       The width of display
 * \param[in] height      The height of display
 *
 * \return The pointer to initialized abstract display controller.
 */
struct display_mono *framebuffer_construct(struct display_mono *const me, uint8_t *const framebuffer,
                                           const gfx_coord_t width, const gfx_coord_t height)
{
	struct framebuffer *display = (struct framebuffer *)me;

	display_mono_construct(&display->parent, NULL, width, height, &framebuffer_interface);
	display->fbpointer = framebuffer;

	return me;
}

/**
 * \brief Put a page from RAM to the buffer in RAM.
 */
void framebuffer_put_page(const struct display_mono *const me, const enum gfx_mono_color *const data,
                          const gfx_coord_t page, const gfx_coord_t page_offset, gfx_coord_t width)
{
	const struct framebuffer * disp           = (const struct framebuffer *)me;
	const enum gfx_mono_color *data_pt        = data;
	gfx_coord_t *              framebuffer_pt = disp->fbpointer + ((page * disp->parent.width) + page_offset);

	do {
		*framebuffer_pt++ = *data_pt++;
	} while (--width > 0);
}

/**
 * \brief Read a page from the buffer in RAM
 */
void framebuffer_get_page(const struct display_mono *const me, enum gfx_mono_color *data, const gfx_coord_t page,
                          const gfx_coord_t page_offset, gfx_coord_t width)
{
	const struct framebuffer *disp           = (const struct framebuffer *)me;
	gfx_coord_t *             framebuffer_pt = disp->fbpointer + ((page * disp->parent.width) + page_offset);

	do {
		*data++ = (enum gfx_mono_color) * framebuffer_pt++;
	} while (--width > 0);
}

/**
 * \brief Draw pixel to the buiffer in RAM
 */
void framebuffer_draw_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                            const enum gfx_mono_color color)
{
	const struct framebuffer *disp = (const struct framebuffer *)me;

	display_mono_draw_pixel(&disp->parent, x, y, color, disp->parent.width, disp->parent.height, 8);
}

/**
 * \brief Get the pixel value at x,y
 */
uint8_t framebuffer_get_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y)
{
	const struct framebuffer *disp = (const struct framebuffer *)me;

	return display_mono_get_pixel(&disp->parent, x, y, disp->parent.width, disp->parent.height, 8);
}

/**
 * \brief Put a byte to the buffer in RAM
 */
void framebuffer_put_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                          const uint8_t data)
{
	const struct framebuffer *disp = (const struct framebuffer *)me;

	*(disp->fbpointer + (page * disp->parent.width) + column) = data;
}

/**
 * \brief Get a byte from the buffer in RAM
 */
uint8_t framebuffer_get_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column)
{
	const struct framebuffer *disp = (const struct framebuffer *)me;

	return *(disp->fbpointer + (page * disp->parent.width) + column);
}

/**
 * \brief Read/Modify/Write a byte on the buffer in RAM
 */
void framebuffer_mask_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                           const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color)
{
	const struct framebuffer *disp = (const struct framebuffer *)me;

	display_mono_mask_byte(&disp->parent, page, column, pixel_mask, color);
}
