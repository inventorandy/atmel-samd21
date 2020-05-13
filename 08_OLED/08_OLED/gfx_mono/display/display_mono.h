/**
 * \file
 *
 * \brief Abstract display related functionality declaration.
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

#ifndef _DISPLAY_MONO_H_INCLUDED
#define _DISPLAY_MONO_H_INCLUDED

#include <display_ctrl_mono.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \addtogroup Abstract display driver
 *
 * \section abstract_display_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/**
 * \brief Coordinate type
 */
typedef uint8_t gfx_coord_t;

/**
 * \brief Pixel operations
 */
enum gfx_mono_color {
	/** Pixel is cleared */
	GFX_PIXEL_CLR = 0,
	/** Pixel is set on screen (OR) */
	GFX_PIXEL_SET = 1,
	/** Pixel is XORed */
	GFX_PIXEL_XOR = 2,
};

/**
 * \brief Bitmap types
 */
enum gfx_mono_bitmap_type {
	/** Bitmap stored in SRAM */
	GFX_MONO_BITMAP_RAM,
	/** Bitmap stored in progmem */
	GFX_MONO_BITMAP_PROGMEM
};

struct display_mono;

/**
 * \brief Interface of abstract display
 */
struct display_mono_interface {
	/** The pointer to the function to get byte from a display */
	uint8_t (*get_byte)(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column);
	/** The pointer to the function to draw byte to a display */
	void (*put_byte)(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
	                 const uint8_t data);
	/** The pointer to the function to draw page to a display */
	void (*put_page)(const struct display_mono *const me, const enum gfx_mono_color *const data, const gfx_coord_t page,
	                 const gfx_coord_t page_offset, const gfx_coord_t width);
	/** The pointer to the function to draw pixel to a display */
	void (*draw_pixel)(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
	                   const enum gfx_mono_color color);
	/** The pointer to the function to mask byte from a display */
	void (*mask_byte)(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
	                  const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color);
};

/**
 * \brief Abstract display structure
 */
struct display_mono {
	/** The pointer to a display controller */
	struct display_ctrl_mono *dc;
	/** The width and height of a display */
	gfx_coord_t width, height;
	/** The pointer to the interface of abstract display */
	struct display_mono_interface *interface;
};

/**
 * \brief Construct / initialize instance of abstract display
 *
 * This function initializes the given abstract display.
 *
 * \param[in, out] me The pointer to abstract display structure
 * \param[in] dc The pointer to io display controller
 * \param[in] width The display's width
 * \param[in] height The display's height
 * \param[in] interface The pointer to virtual table
 *
 * \return The pointer to initialized abstract display structure.
 */
struct display_mono *display_mono_construct(struct display_mono *const me, struct display_ctrl_mono *const dc,
                                            const gfx_coord_t width, const gfx_coord_t height,
                                            struct display_mono_interface *const interface);

/**
 * \brief Construct / initialize instance of UG2832HSWEG04 display
 *
 * This function initializes the given UG2832HSWEG04 display.
 *
 * \param[in out] me      The pointer to UG2832HSWEG04 display controller
 * \param[in] framebuffer The pointer to a RAM buffer
 * \param[in] io          The pointer to io descriptor
 * \param[in] cs          The cs pin
 * \param[in] res         The res pin
 * \param[in] dc          The dc pin
 *
 * \return The pointer to initialized abstract display controller.
 */
struct display_mono *ug2832hsweg04_construct(struct display_mono *const me, uint8_t *const framebuffer,
                                             struct io_descriptor *const io, const uint8_t cs, const uint8_t res,
                                             const uint8_t dc);

/**
 * \brief Construct / initialize instance of null display
 *
 * This function initializes the given null display.
 *
 * \param[in out] me      The pointer to null display controller
 * \param[in] framebuffer The pointer to a RAM buffer
 * \param[in] width       The width of display
 * \param[in] height      The height of display
 *
 * \return The pointer to initialized abstract display controller.
 */
struct display_mono *display_null_construct(struct display_mono *const display, uint8_t *const framebuffer,
                                            const gfx_coord_t width, const gfx_coord_t height);

/**
 * \brief Construct / initialize instance of C12832A1Z display
 *
 * This function initializes the given C12832A1Z display.
 *
 * \param[in out] me      The pointer to C12832A1Z display controller
 * \param[in] framebuffer The pointer to a RAM buffer
 * \param[in] io          The pointer to io descriptor
 * \param[in] cs          The cs pin
 * \param[in] a0          The a0 pin
 * \param[in] reset       The reset pin
 *
 * \return The pointer to initialized abstract display controller.
 */
struct display_mono *c12832a1z_construct(struct display_mono *const display, uint8_t *const framebuffer,
                                         struct io_descriptor *const io, const uint8_t cs, const uint8_t a0,
                                         const uint8_t reset);

/**
 * \brief Put a byte to the display controller RAM
 *
 * \param[in] me     The pointer to display instance
 * \param[in] page   Page address
 * \param[in] column Page offset (x coordinate)
 * \param[in] data   Data to be written
 */
void display_mono_put_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                           const uint8_t data);

/**
 * \brief Get a byte from the display controller RAM
 *
 * \param[in] me     The pointer to display instance
 * \param[in] page   Page address
 * \param[in] column Page offset (x coordinate)
 *
 * \return data from display controller or framebuffer.
 */
uint8_t display_mono_get_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column);

/**
 * \brief Read/Modify/Write a byte on the display controller
 *
 * \param[in] me         The pointer to display instance
 * \param[in] page       Page address
 * \param[in] column     Page offset (x coordinate)
 * \param[in] pixel_mask Mask for pixel operation
 * \param[in] color      Pixel operation
 */
void display_mono_mask_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                            const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color);

/**
 * \brief Get the pixel value at x,y
 *
 * \param[in] me     The pointer to display instance
 * \param[in] x      X coordinate of pixel
 * \param[in] y      Y coordinate of pixel
 * \param[in] width  The width of display
 * \param[in] height The height of display
 * \param[in] pixels The amount of pixels per byte
 *
 * \return Non zero value if pixel is set.
 */
uint8_t display_mono_get_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                               const gfx_coord_t width, const gfx_coord_t height, const uint8_t pixels);

/**
 * \brief Draw pixel to screen
 *
 * \param[in] me     The pointer to display instance
 * \param[in] x      X coordinate of the pixel
 * \param[in] y      Y coordinate of the pixel
 * \param[in] color  Pixel operation
 * \param[in] width  The width of display
 * \param[in] height The height of display
 * \param[in] pixels The amount of pixels per byte
 */
void display_mono_draw_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                             const enum gfx_mono_color color, const gfx_coord_t width, const gfx_coord_t height,
                             const uint8_t pixels);

/**
 * \brief Read a page from the display controller
 *
 * \param[in] me          The pointer to display instance
 * \param[in] data        The pointer where to store the read data
 * \param[in] page        The page address
 * \param[in] page_offset The offset into page (x coordinate)
 * \param[in] width       The width of display
 */
void display_mono_get_page(const struct display_mono *const me, enum gfx_mono_color *data, const gfx_coord_t page,
                           const gfx_coord_t page_offset, gfx_coord_t width);

/**
 * \brief Put a page from RAM to the display controller.
 *
 * \param[in] me          The pointer to display instance
 * \param[in] data        The pointer to data to be written
 * \param[in] page        The page address
 * \param[in] page_offset The offset into page (x coordinate)
 * \param[in] width       The width of display
 */
void display_mono_put_page(const struct display_mono *const me, const enum gfx_mono_color *data, const gfx_coord_t page,
                           const gfx_coord_t page_offset, gfx_coord_t width);

/**
 * \brief Put framebuffer to the display controller
 *
 * This function will output the complete framebuffer from RAM to the
 * display controller.
 *
 * \note This is done automatically if using the graphic primitives. Only
 * needed if you are manipulating the framebuffer directly in your code.
 *
 * \param[in] me          The pointer to display instance
 * \param[in] framebuffer The pointer to framebuffer to put
 * \param[in] width       The width of display
 * \param[in] pages       The amount of display pages
 */
void display_mono_put_framebuffer(const struct display_mono *const me, const uint8_t *const framebuffer,
                                  const gfx_coord_t width, const uint8_t pages);

/**
 * \brief Initialize abstract display controller and abstarct display.
 *
 * \param[in] me    The pointer to display instance
 * \param[in] width The width of display
 * \param[in] pages The amount of display pages
 */
void display_mono_init(const struct display_mono *const me, const gfx_coord_t width, const uint8_t pages);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_MONO_H_INCLUDED */
