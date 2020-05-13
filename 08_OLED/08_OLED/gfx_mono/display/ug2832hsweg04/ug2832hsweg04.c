/**
 * \file
 *
 * \brief UG2832HSWEG04 display related functionality implementation.
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

#include <ug2832hsweg04.h>
#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
#include <framebuffer.h>
#endif

/**
 * \brief UG2832HSWEG04 virtual functions table
 */
static struct display_mono_interface ug2832hsweg04_interface = {ug2832hsweg04_get_byte,
                                                                ug2832hsweg04_put_byte,
                                                                ug2832hsweg04_put_page,
                                                                ug2832hsweg04_draw_pixel,
                                                                ug2832hsweg04_mask_byte};

/**
 * \brief Construct / initialize instance of UG2832HSWEG04 display
 */
struct display_mono *ug2832hsweg04_construct(struct display_mono *const me, uint8_t *const framebuffer,
                                             struct io_descriptor *const io, const uint8_t cs, const uint8_t res,
                                             const uint8_t dc)
{
	struct ug2832hsweg04 *display = (struct ug2832hsweg04 *)me;

	display_mono_construct(&display->parent,
	                       &display->dc.parent,
	                       UG2832HSWEG04_LCD_WIDTH,
	                       UG2832HSWEG04_LCD_HEIGHT,
	                       &ug2832hsweg04_interface);
	ssd1306_construct(&display->dc.parent, io, cs, res, dc);
#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
	framebuffer_construct(&display->framebuffer.parent, framebuffer, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_HEIGHT);
#endif

	display_mono_init(me, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_PAGES);

	return me;
}

/**
 * \brief Put a page from RAM to the display controller.
 */
void ug2832hsweg04_put_page(const struct display_mono *const me, const enum gfx_mono_color *data,
                            const gfx_coord_t page, const gfx_coord_t page_offset, const gfx_coord_t width)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
	framebuffer_put_page(&disp->framebuffer.parent, data, page, page_offset, width);
#endif
	display_mono_put_page(&disp->parent, data, page, page_offset, width);
}

/**
 * \brief Read a page from the display controller
 */
void ug2832hsweg04_get_page(const struct display_mono *const me, enum gfx_mono_color *data, const gfx_coord_t page,
                            const gfx_coord_t page_offset, const gfx_coord_t width)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
	framebuffer_get_page(&disp->framebuffer.parent, data, page, page_offset, width);
#else
	display_mono_get_page(&disp->parent, data, page, page_offset, width);
#endif
}

/**
 * \brief Draw pixel to screen
 */
void ug2832hsweg04_draw_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                              const enum gfx_mono_color color)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

	display_mono_draw_pixel(&disp->parent, x, y, color, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_HEIGHT, 8);
}

/**
 * \brief Get the pixel value at x,y
 */
uint8_t ug2832hsweg04_get_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

	return display_mono_get_pixel(&disp->parent, x, y, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_HEIGHT, 8);
}

/**
 * \brief Put a byte to the display controller RAM
 */
void ug2832hsweg04_put_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                            const uint8_t data)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
	framebuffer_put_byte(&disp->framebuffer.parent, page, column, data);
#endif
	display_mono_put_byte(&disp->parent, page, column, data);
}

/**
 * \brief Get a byte from the display controller RAM
 */
uint8_t ug2832hsweg04_get_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
	return framebuffer_get_byte(&disp->framebuffer.parent, page, column);
#else
	return display_mono_get_byte(&disp->parent, page, column);
#endif
}

/**
 * \brief Read/Modify/Write a byte on the display controller
 */
void ug2832hsweg04_mask_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                             const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color)
{
	struct ug2832hsweg04 *disp = (struct ug2832hsweg04 *)me;

	display_mono_mask_byte(&disp->parent, page, column, pixel_mask, color);
}
