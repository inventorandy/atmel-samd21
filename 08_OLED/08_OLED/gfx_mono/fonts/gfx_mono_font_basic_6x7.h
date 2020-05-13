/**
 * \file
 *
 * \brief Basic 6x7 font declaration
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
#ifndef _GFX_MONO_FONT_BASIC_6X7_H_INCLUDED
#define _GFX_MONO_FONT_BASIC_6X7_H_INCLUDED

#include <compiler.h>
#include <utils.h>
#include <gfx_mono_font.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono fonts
 *
 * @{
 */

/** Width of each glyph, including spacer column. */
#define FONT_BASIC_6x7_WIDTH 6
/** Height of each glyph, excluding spacer line. */
#define FONT_BASIC_6x7_HEIGHT 7
/** Line height. */
#define FONT_BASIC_6x7_LINESPACING 8
/** First character defined. */
#define FONT_BASIC_6x7_FIRSTCHAR ((uint8_t)' ')
/** Last character defined. */
#define FONT_BASIC_6x7_LASTCHAR ((uint8_t)'}')
/** Font definition */
extern struct font basic_6x7;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_FONT_BASIC_6X7_H_INCLUDED */
