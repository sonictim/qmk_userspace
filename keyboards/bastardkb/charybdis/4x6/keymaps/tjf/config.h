/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 4
#endif // VIA_ENABLE

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

#ifdef AUTO_MOUSE_DEFAULT_LAYER
#    undef AUTO_MOUSE_DEFAULT_LAYER
#endif
#define AUTO_MOUSE_DEFAULT_LAYER 3

#ifdef LED_DPI_INDICATOR_INDEX
#    undef LED_DPI_INDICATOR_INDEX
#endif
#define LED_DPI_INDICATOR_INDEX 1 // accomodate invisible 2 first LEDs for easier DIY build debugging

#ifdef RGBLIGHT_LED_COUNT
#    undef RGBLIGHT_LED_COUNT
#endif
#define RGBLIGHT_LED_COUNT 56

/* Auto mouse layer (a.k.a. "auto pointer").
 *
 * This firmware uses QMK's core auto-mouse feature, which the
 * `bastardkb/bk_pointing_device` module turns on.  The old
 * `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_*` defines are dead and are gone.
 *
 * Idle time (ms) after the last trackball movement before the pointer layer
 * is released.  Core default is 650; Argos cannot set this, so it lives here.
 */
#ifdef AUTO_MOUSE_TIME
#    undef AUTO_MOUSE_TIME
#endif
#define AUTO_MOUSE_TIME 300

/* Movement required to activate the layer.  Lower = more sensitive.
 * Core default: 10. */
#ifdef AUTO_MOUSE_THRESHOLD
#    undef AUTO_MOUSE_THRESHOLD
#endif
#define AUTO_MOUSE_THRESHOLD 6

/* Cooldown (ms) after a non-mouse keypress before the trackball may re-trigger
 * the layer.  Raise this if the layer fires while you type.  Core default: 25. */
#ifdef AUTO_MOUSE_DEBOUNCE
#    undef AUTO_MOUSE_DEBOUNCE
#endif
#define AUTO_MOUSE_DEBOUNCE 25
