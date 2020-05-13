/**
 * \file
 *
 * \brief Abstract display controller related functionality implementation.
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

#include <display_ctrl_mono.h>
#include <hal_gpio.h>
#include <hal_delay.h>

/**
 * \brief Construct / initialize instance of abstract display controller
 */
struct display_ctrl_mono *display_ctrl_mono_construct(struct display_ctrl_mono *const           me,
                                                      struct io_descriptor *const               io,
                                                      struct display_ctrl_mono_interface *const interface)
{
	me->io        = io;
	me->interface = interface;

	return me;
}

/**
 * \brief Perform the hard reset of display controller
 */
void display_ctrl_mono_hard_reset(const uint8_t pin, const uint32_t delay)
{
	gpio_set_pin_level(pin, false);
	delay_us(delay);
	gpio_set_pin_level(pin, true);
	delay_us(delay);
}
