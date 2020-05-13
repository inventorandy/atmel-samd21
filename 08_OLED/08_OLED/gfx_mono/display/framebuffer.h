/**
 * \file
 *
 * \brief Framebuffer related functionality declaration.
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
#ifndef _FRAMEBUFFER_H_INCLUDED
#define _FRAMEBUFFER_H_INCLUDED

#include <display_mono.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup Framebuffer display driver
 *
 * \section framebuffer_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/**
 * \brief Framebuffer structure
 */
struct framebuffer {
	/** The parent class of framebuffer display */
	struct display_mono parent;
	/** The pointer to RAM buffer */
	uint8_t *fbpointer;
};

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
                                           const gfx_coord_t width, const gfx_coord_t height);

/**
 * \brief Put a page from RAM to the buffer in RAM.
 *
 * \param[in] me          The pointer to display instance
 * \param[in] data        The pointer to data to be written
 * \param[in] page        The page address
 * \param[in] page_offset The offset into page (x coordinate)
 * \param[in] width       The width of display
 */
void framebuffer_put_page(const struct display_mono *const me, const enum gfx_mono_color *const data,
                          const gfx_coord_t page, const gfx_coord_t page_offset, gfx_coord_t width);

/**
 * \brief Read a page from the buffer in RAM
 *
 * \param[in] me          The pointer to display instance
 * \param[in] data        The pointer where to store the read data
 * \param[in] page        The page address
 * \param[in] page_offset The offset into page (x coordinate)
 * \param[in] width       The number of bytes to be read
 */
void framebuffer_get_page(const struct display_mono *const me, enum gfx_mono_color *data, const gfx_coord_t page,
                          const gfx_coord_t page_offset, gfx_coord_t width);

/**
 * \brief Draw pixel to the buiffer in RAM
 *
 * \param[in] me    The pointer to display instance
 * \param[in] x     X coordinate of the pixel
 * \param[in] y     Y coordinate of the pixel
 * \param[in] color Pixel operation
 */
void framebuffer_draw_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y,
                            const enum gfx_mono_color color);

/**
 * \brief Get the pixel value at x,y
 *
 * \param[in] me   The pointer to display instance
 * \param[in] x    X coordinate of pixel
 * \param[in] y    Y coordinate of pixel
 *
 * \return Non zero value if pixel is set.
 */
uint8_t framebuffer_get_pixel(const struct display_mono *const me, const gfx_coord_t x, const gfx_coord_t y);

/**
 * \brief Put a byte to the buffer in RAM
 *
 * \param[in] me     The pointer to display instance
 * \param[in] page   Page address
 * \param[in] column Page offset (x coordinate)
 * \param[in] data   Data to be written
 */
void framebuffer_put_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                          const uint8_t data);

/**
 * \brief Get a byte from the buffer in RAM
 *
 * \param[in] me     The pointer to display instance
 * \param[in] page   Page address
 * \param[in] column Page offset (x coordinate)
 *
 * \return data from framebuffer.
 */
uint8_t framebuffer_get_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column);

/**
 * \brief Read/Modify/Write a byte on the buffer in RAM
 *
 * \param[in] me         The pointer to display instance
 * \param[in] page       Page address
 * \param[in] column     Page offset (x coordinate)
 * \param[in] pixel_mask Mask for pixel operation
 * \param[in] color      Pixel operation
 */
void framebuffer_mask_byte(const struct display_mono *const me, const gfx_coord_t page, const gfx_coord_t column,
                           const enum gfx_mono_color pixel_mask, const enum gfx_mono_color color);

/**@}*/
#ifdef __cplusplus
}
#endif
#endif /* _FRAMEBUFFER_H_INCLUDED */
