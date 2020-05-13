/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#ifndef GFX_MONO_MAIN_H
#define GFX_MONO_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ug2832hsweg04.h>
#include <gfx_mono.h>
#include <gfx_mono_text.h>
#include <gfx_mono_font_basic_6x7.h>

/*! The graphics library */
extern struct gfx_mono MONOCHROME_GRAPHICS_desc;
/*! The graphics text library */
extern struct gfx_mono_text MONOCHROME_TEXT_desc;

/**
 * \brief Initialize GFX Mono library
 */
void gfx_mono_init(void);

#ifdef __cplusplus
}
#endif

#endif /* GFX_MONO_MAIN_H */
