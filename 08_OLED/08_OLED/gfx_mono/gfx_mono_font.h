/**
 * \file
 *
 * \brief Type definitions for fonts
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
#ifndef _GFX_MONO_FONT_H_INCLUDED
#define _GFX_MONO_FONT_H_INCLUDED

#include <utils.h>
#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono text
 *
 * @{
 */

/**
 * \brief Valid storage locations for font data
 *
 * \todo Add support for fonts in regular ram
 */
enum font_data_type {
	/** Font data stored in program/flash memory. */
	FONT_LOC_PROGMEM,
};

/** Storage structure for font meta data. */
struct font {
	/** Type of storage used for binary font data. See \ref font_data_type. */
	enum font_data_type type;
	union {
		uint8_t PROGMEM_PTR_T progmem;
	} data;
	/** Width of one font character, in pixels. */
	uint8_t width;
	/** Height of one font character, in pixels. */
	uint8_t height;
	/** ASCII value of first character in font set. */
	uint8_t first_char;
	/** ASCII value of last character in the set. */
	uint8_t last_char;
	/** Line spacing. */
	uint8_t line_spacing;
};

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_FONT_H_INCLUDED */
