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

/* All options for the tjf keymap.  The layout itself lives in keymap.c;
 * Argos/VIA are not used, so nothing in EEPROM overrides it. */

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* ---------------------------------------------------------------------------
 * Tap-hold
 * ------------------------------------------------------------------------- */

/* Time (ms) a key must be held to count as a hold. */
#define TAPPING_TERM 200

/* Decide mod-taps faster.
 * PERMISSIVE_HOLD: another key tapped (down+up) while a mod-tap is held -> hold.
 * HOLD_ON_OTHER_KEY_PRESS_PER_KEY: see get_hold_on_other_key_press() in keymap.c. */
#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

/* ---------------------------------------------------------------------------
 * Auto mouse (pointer layer turns on when the trackball moves)
 * ------------------------------------------------------------------------- */

/* Pointer layer index.  Must match LAYER_POINTER in keymap.c (checked at
 * build time).  bk_pointing_device also uses it for the DPI indicator. */
#ifdef AUTO_MOUSE_DEFAULT_LAYER
#    undef AUTO_MOUSE_DEFAULT_LAYER
#endif
#define AUTO_MOUSE_DEFAULT_LAYER 1

/* Idle time (ms) after the last movement before the layer turns off.
 * Core default: 650. */
#ifdef AUTO_MOUSE_TIME
#    undef AUTO_MOUSE_TIME
#endif
#define AUTO_MOUSE_TIME 450

/* Movement needed to turn the layer on.  Lower = more sensitive.
 * Core default: 10. */
#ifdef AUTO_MOUSE_THRESHOLD
#    undef AUTO_MOUSE_THRESHOLD
#endif
#define AUTO_MOUSE_THRESHOLD 5

/* Cooldown (ms) after a non-mouse keypress before the trackball may turn the
 * layer on again.  Raise this if the layer fires while you type.
 * Core default: 25. */
#ifdef AUTO_MOUSE_DEBOUNCE
#    undef AUTO_MOUSE_DEBOUNCE
#endif
#define AUTO_MOUSE_DEBOUNCE 25

/* ---------------------------------------------------------------------------
 * Pointer (applied at every boot)
 * ------------------------------------------------------------------------- */

/* Trackball DPI.  Steps of 200 from 400.  DPI_MOD still changes it until the
 * next power-up. */
#define POINTER_DPI 1200

/* Drop to sniping DPI whenever the pointer layer is on. */
#define POINTER_AUTO_PRECISION false

/* Invert DRGSCRL drag-scroll axes (smart scroll is separate). */
#define POINTER_DRAGSCROLL_INVERT_X false
#define POINTER_DRAGSCROLL_INVERT_Y true

/* ---------------------------------------------------------------------------
 * Smart scroll (SMTSCRL key, see smart_scroll.c)
 * ------------------------------------------------------------------------- */

/* Counts of motion before picking vertical or horizontal. */
#define SMART_SCROLL_LOCK_THRESHOLD 16

/* Higher = slower scrolling. */
#define SMART_SCROLL_DIVISOR 12.0f

/* Idle time (ms) before the axis unlocks and is picked again; 0 = never. */
#define SMART_SCROLL_RELOCK_MS 300

/* ---------------------------------------------------------------------------
 * Lighting
 * ------------------------------------------------------------------------- */

/* Per-layer colors as r, g, b (0-255), scaled by the RGB brightness.  Base and
 * pointer layers show the normal RGB effect. */
#define LAYER_COLOR_HYPER 0, 39, 64
#define LAYER_COLOR_NUMNAV 15, 0, 64
#define LAYER_COLOR_SYM 64, 0, 58
#define LAYER_COLOR_SYSTEM 64, 0, 4

/* Per-layer colors need the layer state on both halves. */
#define SPLIT_LAYER_STATE_ENABLE

#ifdef LED_DPI_INDICATOR_INDEX
#    undef LED_DPI_INDICATOR_INDEX
#endif
#define LED_DPI_INDICATOR_INDEX 1 // accomodate invisible 2 first LEDs for easier DIY build debugging

#ifdef RGBLIGHT_LED_COUNT
#    undef RGBLIGHT_LED_COUNT
#endif
#define RGBLIGHT_LED_COUNT 56

/* ---------------------------------------------------------------------------
 * Workarounds (not settings)
 * ------------------------------------------------------------------------- */

/* bk_pointing_device builds an Argos info packet using these names from
 * argos.h, even when Argos is not enabled.  Values match argos.h.  Remove if
 * the Argos module is added back. */
#define pointing_device_type_unknown 0
#define pointing_device_type_trackpad_procyon 1
#define pointing_device_type_trackball 2
#define pointing_device_type_trackpad_cirque 3
