/**
 * \file
 *
 * \brief Abstract display controller related functionality declaration.
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

#ifndef _DISPLAY_CTRL_MONO_H_INCLUDED
#define _DISPLAY_CTRL_MONO_H_INCLUDED

#include <compiler.h>
#include <hal_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup Abstract display controller driver
 *
 * \section abstract_display_ctrl_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

struct display_ctrl_mono;

/**
 * \brief Interface of abstract display controller
 */
struct display_ctrl_mono_interface {
	/** The pointer to the function to write data to a display controller */
	void (*write_data)(const struct display_ctrl_mono *const me, const uint8_t data);
	/** The pointer to the function to read data from a display controller */
	uint8_t (*read_data)(const struct display_ctrl_mono *const me);
	/** The pointer to the function to set page address for a display
	 * controller */
	void (*set_page_address)(const struct display_ctrl_mono *const me, const uint8_t address);
	/** The pointer to the function to set column address for a display
	 * controller */
	void (*set_column_address)(const struct display_ctrl_mono *const me, uint8_t address);
	/** The pointer to function to set start line address for a display
	 * controller */
	void (*set_start_line_address)(const struct display_ctrl_mono *const me, const uint8_t address);
};

/**
 * \brief Abstract display controller structure
 */
struct display_ctrl_mono {
	/** The pointer to an io descriptor to be used by a display controller */
	struct io_descriptor *io;
	/** The pointer to the interface of abstract display controller */
	struct display_ctrl_mono_interface *interface;
};

/**
 * \brief SSD1306 display controller structure
 */
struct ssd1306 {
	/** The parent class of ssd1306 display controller */
	struct display_ctrl_mono parent;
	/** Display controller pins */
	uint8_t pin_cs, pin_res, pin_dc;
};

/**
 * \brief ST7565R display controller structure
 */
struct st7565r {
	/** The parent class of st7565r display controller */
	struct display_ctrl_mono parent;
	/** Display controller pins */
	uint8_t pin_cs, pin_a0, pin_reset;
};

/**
 * \brief Construct / initialize instance of abstract display controller
 *
 * This function initializes the given abstract display controller.
 *
 * \param[in, out] me The pointer to abstract display controller structure
 * \param[in] io The pointer to io descriptor
 * \param[in] interface The pointer to virtual table
 *
 * \return The pointer to initialized abstract display controller structure.
 */
struct display_ctrl_mono *display_ctrl_mono_construct(struct display_ctrl_mono *const           me,
                                                      struct io_descriptor *const               io,
                                                      struct display_ctrl_mono_interface *const interface);

/**
 * \brief Construct / initialize instance of ST7565R display controller
 *
 * This function initializes the given ST7565R display controller.
 *
 * \param[in out] me The pointer to ST7565R display controller structure
 * \param[in] io The pointer to io descriptor
 * \param[in] cs The cs pin
 * \param[in] a0 The a0 pin
 * \param[in] reset The reset pin
 *
 * \return The pointer to initialized abstract display controller structure.
 */
struct display_ctrl_mono *st7565r_construct(struct display_ctrl_mono *const me, struct io_descriptor *const io,
                                            const uint8_t cs, const uint8_t a0, const uint8_t reset);

/**
 * \brief Construct / initialize instance of SSD1306 display controller
 *
 * This function initializes the given SSD1306 display controller.
 *
 * \param[in out] me The pointer to SSD1306 display controller structure
 * \param[in] io The pointer to io descriptor
 * \param[in] cs The cs pin
 * \param[in] res The res pin
 * \param[in] dc The dc pin
 *
 * \return The pointer to initialized abstract display controller structure.
 */
struct display_ctrl_mono *ssd1306_construct(struct display_ctrl_mono *const me, struct io_descriptor *const io,
                                            const uint8_t cs, const uint8_t res, const uint8_t dc);

/**
 * \brief Perform the hard reset of display controller
 *
 * \param[in] pin The pin to manipulate for hard reset
 * \param[in] delay The required delay between pin level changes
 */
void display_ctrl_mono_hard_reset(const uint8_t pin, const uint32_t delay);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_CTRL_MONO_H_INCLUDED */
