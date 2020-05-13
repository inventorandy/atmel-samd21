/**
 * \file
 *
 * \brief Monochrome graphic library API header file
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
#ifndef _GFX_MONO_TEXT_H_INCLUDED
#define _GFX_MONO_TEXT_H_INCLUDED

#include <utils.h>
#include <compiler.h>
#include <gfx_mono.h>
#include <gfx_mono_font.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono text
 *
 * \section gfx_mono_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/**
 * \brief GFX mono type
 */
struct gfx_mono_text {
	/** The pointer to GFX used GFX text */
	struct gfx_mono *gfx;
};

/**
 * \brief Construct GFX mono
 *
 * All further calls to GFX mono API will use display provided via this
 * function.
 *
 * \param[in out] me The pointer to GFX mono text instance
 * \param[in] gfx    The pointer to GFX mono instance
 *
 * \return The pointer to initialized gfx mono text structure
 */
struct gfx_mono_text *gfx_mono_text_construct(struct gfx_mono_text *const me, struct gfx_mono *const gfx);

/**
 * \brief Draws a character to the display
 *
 * \param[in] me   The pointer to GFX mono text instance
 * \param[in] c    Character to be drawn
 * \param[in] x    X coordinate on screen.
 * \param[in] y    Y coordinate on screen.
 * \param[in] font Font to draw character in
 */
void gfx_mono_text_draw_char(const struct gfx_mono_text *const me, const uint8_t c, const gfx_coord_t x,
                             const gfx_coord_t y, const struct font *font);

/**
 * \brief Draws a string to the display
 *
 * This function will draw a string located in memory to the display.
 *
 * \param[in] me   The pointer to GFX mono text instance
 * \param[in] str  Pointer to string
 * \param[in] x    X coordinate on screen
 * \param[in] y    Y coordinate on screen
 * \param[in] font Font to draw string in
 */
void gfx_mono_text_draw_string(const struct gfx_mono_text *const me, const uint8_t *str, const gfx_coord_t x,
                               const gfx_coord_t y, const struct font *font);

/**
 * \brief Computes the bounding box of a string
 *
 * \note If string is empty the returned width will be 1 pixel and the height
 * equal to the font height.
 *
 * \param[in] me     The pointer to GFX mono text instance
 * \param[in] str    String to calculate bounding box for
 * \param[in] font   Font used
 * \param[in] width  Pointer to width result
 * \param[in] height Pointer to height result
 */
void gfx_mono_text_get_string_bounding_box(const struct gfx_mono_text *const me, const uint8_t *str,
                                           const struct font *font, gfx_coord_t *width, gfx_coord_t *height);

/**
 * \brief Draws a string located in program memory to the display
 *
 * This function will draw a string located in program memory to the display,
 * this differs from gfx_mono_text_draw_string() by using constant string data
 * from the program memory instead of string data in RAM.
 *
 * Using program memory for constant strings will reduce the applications need
 * for RAM, and thus lower the overall size footprint.
 *
 * \param[in] me   The pointer to GFX mono text instance
 * \param[in] str  Pointer to string located in program memory
 * \param[in] x    X coordinate on screen.
 * \param[in] y    Y coordinate on screen.
 * \param[in] font Font to draw string in
 */
void gfx_mono_text_draw_progmem_string(const struct gfx_mono_text *const me, uint8_t PROGMEM_PTR_T str, gfx_coord_t x,
                                       gfx_coord_t y, const struct font *font);

/**
 * \brief Computes the bounding box of a string located in program memory
 *
 * \note If string is empty the returned width will be 1 pixel and the height
 * equal to the font height.
 *
 * \param[in] me     The pointer to GFX mono text instance
 * \param[in] str    String in program memory to calculate bounding box for
 * \param[in] font   Font used
 * \param[in] width  Pointer to width result
 * \param[in] height Pointer to height result
 */
void gfx_mono_text_get_progmem_string_bounding_box(const struct gfx_mono_text *const me, uint8_t PROGMEM_PTR_T str,
                                                   const struct font *font, gfx_coord_t *width, gfx_coord_t *height);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_TEXT_H_INCLUDED */
