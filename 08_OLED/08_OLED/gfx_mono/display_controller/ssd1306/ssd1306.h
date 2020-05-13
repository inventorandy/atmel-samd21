/**
 * \file
 *
 * \brief SSD1306 display controller related functionality declaration.
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

#ifndef _SSD1306_H_INCLUDED
#define _SSD1306_H_INCLUDED

#include <display_ctrl_mono.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup SSD1306 display driver
 *
 * \section ssd1306_display_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/**
 * \brief SSD1306 display controller commands
 */
#define SSD1306_CMD_SET_LOW_COL(column) (0x00 | (column))
#define SSD1306_CMD_SET_HIGH_COL(column) (0x10 | (column))
#define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE 0x20
#define SSD1306_CMD_SET_COLUMN_ADDRESS 0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS 0x22
#define SSD1306_CMD_SET_START_LINE(line) (0x40 | (line))
#define SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0 0x81
#define SSD1306_CMD_SET_CHARGE_PUMP_SETTING 0x8D
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0 0xA0
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0 0xA1
#define SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON 0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON 0xA5
#define SSD1306_CMD_SET_NORMAL_DISPLAY 0xA6
#define SSD1306_CMD_SET_INVERSE_DISPLAY 0xA7
#define SSD1306_CMD_SET_MULTIPLEX_RATIO 0xA8
#define SSD1306_CMD_SET_DISPLAY_ON 0xAF
#define SSD1306_CMD_SET_DISPLAY_OFF 0xAE
#define SSD1306_CMD_SET_PAGE_START_ADDRESS(page) (0xB0 | (page & 0x07))
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP 0xC0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN 0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO 0xD5
#define SSD1306_CMD_SET_PRE_CHARGE_PERIOD 0xD9
#define SSD1306_CMD_SET_COM_PINS 0xDA
#define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL 0xDB
#define SSD1306_CMD_NOP 0xE3
#define SSD1306_CMD_SCROLL_H_RIGHT 0x26
#define SSD1306_CMD_SCROLL_H_LEFT 0x27
#define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_RIGHT 0x29
#define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_LEFT 0x2A
#define SSD1306_CMD_DEACTIVATE_SCROLL 0x2E
#define SSD1306_CMD_ACTIVATE_SCROLL 0x2F
#define SSD1306_CMD_SET_VERTICAL_SCROLL_AREA 0xA3

/**
 * \brief Write a command to the display controller
 *
 * This function pulls pin A0 low before writing to the controller.
 *
 * \param[in] me The pointer to display instance
 * \param[in] command The command to write
 */
void ssd1306_write_command(const struct display_ctrl_mono *const me, const uint8_t command);

/**
 * \brief Write data to the display controller
 *
 * This function sets the pin A0 before writing to the controller.
 *
 * \param[in] me The pointer to display instance
 * \param[in] data The data to write
 */
void ssd1306_write_data(const struct display_ctrl_mono *const me, const uint8_t data);

/**
 * \brief Read data from the display controller
 *
 * \note The controller does not support read in serial mode.
 *
 * \param[in] me The pointer to display instance
 *
 * \retval 8 bit data read from the controller
 */
uint8_t ssd1306_read_data(const struct display_ctrl_mono *const me);

/**
 * \brief Perform a hard reset of the display controller
 *
 * This functions will reset the display controller by setting the reset pin
 * low.
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_hard_reset(const struct display_ctrl_mono *const me);

/**
 * \brief Enable the display sleep mode
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_enable_sleep(const struct display_ctrl_mono *const me);

/**
 * \brief Disable the display sleep mode
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_disable_sleep(const struct display_ctrl_mono *const me);

/**
 * \brief Set current page in display RAM
 *
 * This command is usually followed by the configuration of the column address
 * because this scheme will provide access to all locations in the display
 * RAM.
 *
 * \param[in] me The pointer to display instance
 * \param[in] address The page address to set
 */
void ssd1306_set_page_address(const struct display_ctrl_mono *const me, const uint8_t address);

/**
 * \brief Set current column in display RAM
 *
 * \param[in] me The pointer to display instance
 * \param[in] address The column address to set
 */
void ssd1306_set_column_address(const struct display_ctrl_mono *const me, uint8_t address);

/**
 * \brief Set the display start draw line address
 *
 * This function will set which line should be the start draw line.
 *
 * \param[in] me The pointer to display instance
 * \param[in] address The line address to set
 */
void ssd1306_set_start_line_address(const struct display_ctrl_mono *const me, const uint8_t address);

/**
 * \brief Turn the display display on
 *
 * This function will turn on the display.
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_on(const struct display_ctrl_mono *const me);

/**
 * \brief Turn the display display off
 *
 * This function will turn off the display.
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_off(const struct display_ctrl_mono *const me);

/**
 * \brief Set the display contrast level
 *
 * \warning This will set the voltage for the display, settings this value too
 * high may result in damage to the display. Hence the limit for these settings
 * must be defined in the \ref st74565r_config.h file.
 *
 * Contrast values outside the max and min values will be clipped to the defined
 * \ref ST7565R_DISPLAY_CONTRAST_MAX and \ref ST7565R_DISPLAY_CONTRAST_MIN.
 *
 * \param[in] me The pointer to display instance
 * \param[in] contrast A number between 0 and 63 where the max values is given
 *                     by the display.
 *
 * \retval contrast the contrast value written to the display controller
 */
uint8_t ssd1306_set_contrast(const struct display_ctrl_mono *const me, const uint8_t contrast);

/**
 * \brief Invert all pixels on the display
 *
 * This function will invert all pixels on the display.
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_display_invert_enable(const struct display_ctrl_mono *const me);

/**
 * \brief Disable invert of all pixels on the display
 *
 * This function will disable invert on all pixels on the display.
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_display_invert_disable(const struct display_ctrl_mono *const me);

/**
 * \brief Clear the display
 *
 * \param[in] me The pointer to display instance
 */
void ssd1306_clear(const struct display_ctrl_mono *const me);

/**
 * \brief Display text on the display
 *
 * \param[in] me The pointer to display instance
 * \param[in] string A string to write.
 */
void ssd1306_write_text(const struct display_ctrl_mono *const me, const uint8_t *string);

#ifdef __cplusplus
}
#endif

#endif /* _SSD1306_H_INCLUDED */
