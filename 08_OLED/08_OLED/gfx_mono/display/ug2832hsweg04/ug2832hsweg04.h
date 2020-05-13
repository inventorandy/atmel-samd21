/**
 * \file
 *
 * \brief UG2832HSWEG04 display related functionality declaration.
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

#ifndef _UG2832HSWEG05_H_INCLUDED
#define _UG2832HSWEG05_H_INCLUDED

#include <ug2832hsweg04_config.h>
#include <display_mono.h>
#include <ssd1306.h>
#include <framebuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup UG2832HSWEG04 display driver
 *
 * \section ug2832hsweg04_display_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#define UG2832HSWEG04_LCD_WIDTH 128
#define UG2832HSWEG04_LCD_HEIGHT 32
#define UG2832HSWEG04_LCD_PIXELS_PER_BYTE 8
#define UG2832HSWEG04_LCD_PAGES (UG2832HSWEG04_LCD_HEIGHT / UG2832HSWEG04_LCD_PIXELS_PER_BYTE)
#define UG2832HSWEG04_LCD_FRAMEBUFFER_SIZE                                                                             \
	((UG2832HSWEG04_LCD_WIDTH * UG2832HSWEG04_LCD_HEIGHT) / UG2832HSWEG04_LCD_PIXELS_PER_BYTE)

/**
 * \brief ug2832hsweg04 display structure
 */
struct ug2832hsweg04 {
	/** The parent class of ug2832hsweg04 display */
	struct display_mono parent;
	/** The ssd1306 display controller */
	struct ssd1306 dc;
#if CONF_UG2832HSWEG04_FRAMEBUFFER == 1
	/** The display RAM buffer */
	struct framebuffer framebuffer;
#endif
};

/**
 * \brief Put framebuffer to the display controller
 *
 * This function will output the complete framebuffer from RAM to the
 * display controller.
 *
 * \note This is done automatically if using the graphic primitives. Only
 * needed if you are manipulating the framebuffer directly in your code.
 *
 * \param[in] me The pointer to display instance
 */
void ug2832hsweg04_put_framebuffer(const struct display_mono *const me);

/**
 * \brief Put a page from RAM to the display controller.
 *
 * If the controller is accessed by the SPI interface, we can not read
 * back data from the LCD controller RAM. Because of this all data that is
 * written to the display controller in this mode is also written to a
 * framebuffer in MCU RAM.
 *
 * \param[in] me          The pointer to display instance
 * \param[in] data        The pointer to data to be written
 * \param[in] page        The page address
 * \param[in] page_offset The offset into page (x coordinate)
 * \param[in] width       The number of bytes to be written.
 *
 * The following example will write 32 bytes from data_buf to the page 0,
 * column 10. This will place data_buf in the rectangle x1=10,y1=0,x2=42,y2=8
 * (10 pixels from the upper left corner of the screen):
 * \code
    ug2832hsweg04_put_page(&display, data_buf, 0, 10, 32);
\endcode
 */
void ug2832hsweg04_put_page(const struct display_mono *const me, const enum gfx_mono_color *const data,
                            const gfx_coord_t page, const gfx_coord_t page_offset, const gfx_coord_t width);

/**
 * \brief Read a page from the display controller
 *
 * If the display controller is accessed by the SPI interface we cannot read
 * data directly from the controller. In that case we will read the data from
 * the local framebuffer instead.
 *
 * \param[in] me          The pointer to display instance
 * \param[in] data        The pointer where to store the read data
 * \param[in] page        The page address
 * \param[in] page_offset The offset into page (x coordinate)
 * \param[in] width       The number of bytes to be read
 *
 * The following example will read back the first 128 bytes (first page) from
 * the display memory:
 * \code
    ug2832hsweg04_get_page(&display, read_buffer, 0, 0, 128);
\endcode
 */
void ug2832hsweg04_get_page(const struct display_mono *const me, enum gfx_mono_color *data, const gfx_coord_t page,
                            const gfx_coord_t page_offset, const gfx_coord_t width);

/**
 * \brief Draw pixel to screen
 *
 * \param[in] me    The pointer to display instance
 * \param[in] x     X coordinate of the pixel
 * \param[in] y     Y coordinate of the pixel
 * \param[in] color Pixel operation
 *
 * The following will set the pixel at x=10,y=10:
 * \code
    ug2832hsweg04_draw_pixel(&display, 10, 10, GFX_PIXEL_SET);
\endcode
 * The following example will clear the pixel at x=10,y=10:
 * \code
    ug2832hsweg04_draw_pixel(&display, 10, 10, GFX_PIXEL_CLR);
\endcode
 * And the following will toggle the pixel at x=10,y=10:
 * \code
    ug2832hsweg04_draw_pixel(&display, 10, 10, GFX_PIXEL_XOR);
\endcode
 */
void ug2832hsweg04_draw_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                              const enum gfx_mono_color color);

/**
 * \brief Get the pixel value at x,y
 *
 * \param[in] me   The pointer to display instance
 * \param[in] x    X coordinate of pixel
 * \param[in] y    Y coordinate of pixel
 *
 * \return Non zero value if pixel is set.
 *
 * The following example will read the pixel value from x=10,y=10:
 * \code
    pixelval = ug2832hsweg04_get_pixel(&display, 10,10);
\endcode
 */
uint8_t ug2832hsweg04_get_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y);

/**
 * \brief Put a byte to the display controller RAM
 *
 * If the display controller is accessed by the SPI interface we will also put
 * the data to the local framebuffer.
 *
 * \param[in] me     The pointer to display instance
 * \param[in] page   Page address
 * \param[in] column Page offset (x coordinate)
 * \param[in] data   Data to be written
 *
 * This example will put the value 0xFF to the first byte in the display memory
 * setting a 8 pixel high column of pixels in the upper left corner of the
 * display.
 * \code
    ug2832hsweg04_put_byte(&display, 0, 0, 0xFF, false);
\endcode
 */
void ug2832hsweg04_put_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                            const uint8_t data);

/**
 * \brief Get a byte from the display controller RAM
 *
 * If the display controller is accessed by the SPI interface we cannot read the
 * data. In this case return the data from the local framebuffer instead.
 *
 * \param[in] me     The pointer to display instance
 * \param[in] page   Page address
 * \param[in] column Page offset (x coordinate)
 *
 * \return data from display controller or framebuffer.
 *
 * The following code will read the first byte from the display memory or the
 * local framebuffer if direct read is not possible. The data represents the
 * pixels from x = 0 and y = 0 to y = 7.
 * \code
    data = ug2832hsweg04_get_byte(&display, 0, 0);
\endcode
 */
uint8_t ug2832hsweg04_get_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column);

/**
 * \brief Read/Modify/Write a byte on the display controller
 *
 * This function will read the byte from the display controller (or the
 * framebuffer if we cannot read directly from the controller) and
 * do a mask operation on the byte according to the pixel operation selected
 * by the color argument and the pixel mask provided.
 *
 * \param[in] me         The pointer to display instance
 * \param[in] page       Page address
 * \param[in] column     Page offset (x coordinate)
 * \param[in] pixel_mask Mask for pixel operation
 * \param[in] color      Pixel operation
 *
 * A small example that will XOR the first byte of display memory with 0xAA
 * \code
    ug2832hsweg04_mask_byte(&display, 0, 0, 0xAA, GFX_PIXEL_XOR);
\endcode
 */
void ug2832hsweg04_mask_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                             const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _UG2832HSWEG05_H_INCLUDED */
