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

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* The whole layout lives in keymap.c; Argos/VIA are not used, so nothing in
 * EEPROM overrides it. */

/* Pointer layer index.  Must match LAYER_POINTER in keymap.c.  The
 * bk_pointing_device module uses it for auto-mouse, the DPI indicator and
 * auto-precision. */
#ifdef AUTO_MOUSE_DEFAULT_LAYER
#    undef AUTO_MOUSE_DEFAULT_LAYER
#endif
#define AUTO_MOUSE_DEFAULT_LAYER 3

/* Workaround: bk_pointing_device builds an Argos info packet using these
 * names from argos.h, even when Argos is not enabled.  Values match argos.h.
 * Remove if the Argos module is added back. */
#define pointing_device_type_unknown 0
#define pointing_device_type_trackpad_procyon 1
#define pointing_device_type_trackball 2
#define pointing_device_type_trackpad_cirque 3

/* Per-layer RGB colors need the layer state on both halves. */
#define SPLIT_LAYER_STATE_ENABLE

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
 * is released.  Core default is 650.
 */
#ifdef AUTO_MOUSE_TIME
#    undef AUTO_MOUSE_TIME
#endif
#define AUTO_MOUSE_TIME 450

/* Movement required to activa:te the layer.  Lower = more sensitive.
 * Core default: 10. */
#ifdef AUTO_MOUSE_THRESHOLD
#    undef AUTO_MOUSE_THRESHOLD
#endif
#define AUTO_MOUSE_THRESHOLD 5

/* Cooldown (ms) after a non-mouse keypress before the trackball may re-trigger
 * the layer.  Raise this if the layer fires while you type.  Core default: 25. */
#ifdef AUTO_MOUSE_DEBOUNCE
#    undef AUTO_MOUSE_DEBOUNCE
#endif
#define AUTO_MOUSE_DEBOUNCE 25

/* Tap-hold timing, in ms (was set in Argos). */
#define TAPPING_TERM 200

/* Tap-hold: decide mod-taps faster.
 * PERMISSIVE_HOLD: another key tapped (down+up) while a mod-tap is held -> hold.
 * HOLD_ON_OTHER_KEY_PRESS_PER_KEY: see get_hold_on_other_key_press() in keymap.c. */
#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
