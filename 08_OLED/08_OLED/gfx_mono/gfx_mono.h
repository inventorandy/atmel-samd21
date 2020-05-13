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
#ifndef _GFX_MONO_H_INCLUDED
#define _GFX_MONO_H_INCLUDED

#include <utils.h>
#include <display_mono.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono
 *
 * \section gfx_mono_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/** Bitmask for drawing circle octant 0. */
#define GFX_OCTANT0 (1 << 0)
/** Bitmask for drawing circle octant 1. */
#define GFX_OCTANT1 (1 << 1)
/** Bitmask for drawing circle octant 2. */
#define GFX_OCTANT2 (1 << 2)
/** Bitmask for drawing circle octant 3. */
#define GFX_OCTANT3 (1 << 3)
/** Bitmask for drawing circle octant 4. */
#define GFX_OCTANT4 (1 << 4)
/** Bitmask for drawing circle octant 5. */
#define GFX_OCTANT5 (1 << 5)
/** Bitmask for drawing circle octant 6. */
#define GFX_OCTANT6 (1 << 6)
/** Bitmask for drawing circle octant 7. */
#define GFX_OCTANT7 (1 << 7)

/** Bitmask for drawing circle quadrant 0. */
#define GFX_QUADRANT0 (GFX_OCTANT0 | GFX_OCTANT1)
/** Bitmask for drawing circle quadrant 1. */
#define GFX_QUADRANT1 (GFX_OCTANT2 | GFX_OCTANT3)
/** Bitmask for drawing circle quadrant 2. */
#define GFX_QUADRANT2 (GFX_OCTANT4 | GFX_OCTANT5)
/** Bitmask for drawing circle quadrant 3. */
#define GFX_QUADRANT3 (GFX_OCTANT6 | GFX_OCTANT7)

/** Bitmask for drawing left half of circle. */
#define GFX_LEFTHALF (GFX_QUADRANT3 | GFX_QUADRANT0)
/** Bitmask for drawing top half of circle. */
#define GFX_TOPHALF (GFX_QUADRANT0 | GFX_QUADRANT1)
/** Bitmask for drawing right half of circle. */
#define GFX_RIGHTHALF (GFX_QUADRANT1 | GFX_QUADRANT2)
/** Bitmask for drawing bottom half of circle. */
#define GFX_BOTTOMHALF (GFX_QUADRANT2 | GFX_QUADRANT3)

/** Bitmask for drawing whole circle. */
#define GFX_WHOLE 0xFF

/**
 * \brief GFX mono bitmap type
 */
struct gfx_mono_bitmap {
	/** Width of bitmap */
	gfx_coord_t width;
	/** Height of bitmap */
	gfx_coord_t height;
	/** Bitmap type */
	enum gfx_mono_bitmap_type type;
	union {
		/** Pointer to pixels for bitmap stored in RAM */
		enum gfx_mono_color *pixmap;
		/** Pointer to pixels for bitmap stored in progmem */
		enum gfx_mono_color PROGMEM_T *progmem;
	} data;
};

/**
 * \brief GFX mono type
 */
struct gfx_mono {
	/** The pointer to display used by GFX */
	struct display_mono *display;
};

/**
 * \brief Construct GFX mono
 *
 * All further calls to GFX mono API will use display provided via this
 * function.
 *
 * \param[in out] me  The pointer to GFX mono instance.
 * \param[in] display The pointer to display structure.
 *
 * \return The pointer to initialized gfx mono structure
 */
struct gfx_mono *gfx_mono_construct(struct gfx_mono *const me, struct display_mono *const display);

/**
 * \brief Draw a horizontal line, one pixel wide (generic implementation)
 *
 * \note This function does a very simple bounds checking that does not
 * check if the line is placed outside the screen. If you supply an
 * x- or y-coordinate outside the display the behaviour is undefined,
 * and you risk overwriting portions of internal SRAM.
 *
 * \param[in] me     The pointer to GFX mono instance.
 * \param[in] x      X coordinate of leftmost pixel.
 * \param[in] y      Y coordinate of the line.
 * \param[in] length Length of the line in pixels.
 * \param[in] color  Pixel operation of the line.
 */
void gfx_mono_draw_horizontal_line(const struct gfx_mono *const me, gfx_coord_t x, gfx_coord_t y, gfx_coord_t length,
                                   enum gfx_mono_color color);

/**
 * \brief Draw a vertical line, one pixel wide (generic implementation)
 *
 * \note This function does a very simple bounds checking that does not
 * check if the line is placed outside the screen. If you supply an
 * x- or y-coordinate outside the display the behaviour is undefined,
 * and you risk overwriting portions of internal SRAM.
 *
 * \param[in] me     The pointer to GFX mono instance.
 * \param[in] x      X coordinate of the line.
 * \param[in] y      Y coordinate of the topmost pixel.
 * \param[in] length Length of the line in pixels.
 * \param[in] color  Pixel operation of the line.
 */
void gfx_mono_draw_vertical_line(const struct gfx_mono *const me, gfx_coord_t x, gfx_coord_t y, gfx_coord_t length,
                                 enum gfx_mono_color color);

/**
 * \brief Draw a line between two arbitrary points (generic implementation).
 *
 * \param[in] me    The pointer to GFX mono instance.
 * \param[in] x1    Start X coordinate.
 * \param[in] y1    Start Y coordinate.
 * \param[in] x2    End X coordinate.
 * \param[in] y2    End Y coordinate.
 * \param[in] color Pixel operation of the line.
 */
void gfx_mono_draw_line(const struct gfx_mono *const me, gfx_coord_t x1, gfx_coord_t y1, gfx_coord_t x2, gfx_coord_t y2,
                        enum gfx_mono_color color);

/**
 * \brief Draw an outline of a rectangle (generic implementation).
 *
 * \param[in] me     The pointer to GFX mono instance.
 * \param[in] x      X coordinate of the left side.
 * \param[in] y      Y coordinate of the top side.
 * \param[in] width  Width of the rectangle.
 * \param[in] height Height of the rectangle.
 * \param[in] color  Pixel operation of the line.
 */
void gfx_mono_draw_rect(const struct gfx_mono *const me, gfx_coord_t x, gfx_coord_t y, gfx_coord_t width,
                        gfx_coord_t height, enum gfx_mono_color color);

/**
 * \brief Draw a filled rectangle (generic implementation).
 *
 * \param[in] me     The pointer to GFX mono instance.
 * \param[in] x      X coordinate of the left side.
 * \param[in] y      Y coordinate of the top side.
 * \param[in] width  Width of the rectangle.
 * \param[in] height Height of the rectangle.
 * \param[in] color  Pixel operation of the line
 */
void gfx_mono_draw_filled_rect(const struct gfx_mono *const me, gfx_coord_t x, gfx_coord_t y, gfx_coord_t width,
                               gfx_coord_t height, enum gfx_mono_color color);

/**
 * \brief Draw an outline of a circle or arc (generic implementation).
 *
 * The radius is the distance from the center to the circumference,
 * which means that the total width or height of a circle will be
 * (radius*2+1).
 *
 * The octant_mask parameter is a bitmask that decides which octants of
 * the circle to draw. Use the GFX_OCTANTn, GFX_QUADRANTn, GFX_xHALF and
 * GFX_WHOLE constants and OR them together if required. Radius equal to
 * zero gives a single pixel.
 *
 * \param[in] me          The pointer to GFX mono instance.
 * \param[in] x           X coordinate of center.
 * \param[in] y           Y coordinate of center.
 * \param[in] radius      Circle radius in pixels.
 * \param[in] color       Pixel operation.
 * \param[in] octant_mask Bitmask indicating which octants to draw.
 */
void gfx_mono_draw_circle(const struct gfx_mono *const me, gfx_coord_t x, gfx_coord_t y, gfx_coord_t radius,
                          enum gfx_mono_color color, uint8_t octant_mask);

/**
 * \brief Draw a filled circle or sector (generic implementation).
 *
 * The radius is the distance from the center to the circumference,
 * which means that the total width or height of a circle will be
 * (radius*2+1).
 *
 * The quadrant_mask parameter is a bitmask that decides which quadrants
 * of the circle to draw. Use the GFX_QUADRANTn, GFX_xHALF and
 * GFX_WHOLE constants and OR them together if required. Radius equal to
 * zero gives a single pixel.
 *
 * \note This function only supports quadrants while gfx_draw_circle()
 *       supports octants. This is to improve performance on drawing
 *       filled circles.
 * \param[in] me          The pointer to GFX mono instance.
 * \param[in] x           X coordinate of center.
 * \param[in] y           Y coordinate of center.
 * \param[in] radius      Circle radius in pixels.
 * \param[in] color       Pixel operation.
 * \param[in] quadrant_mask Bitmask indicating which quadrants to draw.
 */
void gfx_mono_draw_filled_circle(const struct gfx_mono *const me, gfx_coord_t x, gfx_coord_t y, gfx_coord_t radius,
                                 enum gfx_mono_color color, uint8_t quadrant_mask);

/**
 * \brief Put bitmap from FLASH or RAM to display
 *
 * This function will output bitmap data from FLASH or RAM.
 * The bitmap y-coordinate will be aligned with display pages, rounded down.
 * Ie: placing a bitmap at x=10, y=5 will put the bitmap at x = 10,y = 0 and
 * placing a bitmap at x = 10, y = 10 will put the bitmap at x = 10, y = 8
 *
 * \param[in] me     The pointer to GFX mono instance.
 * \param[in] bitmap The bitmap to draw.
 * \param[in] x      X coordinate of center.
 * \param[in] y      Y coordinate of center.
 */
void gfx_mono_put_bitmap(const struct gfx_mono *const me, struct gfx_mono_bitmap *bitmap, gfx_coord_t x, gfx_coord_t y);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_H_INCLUDED */
