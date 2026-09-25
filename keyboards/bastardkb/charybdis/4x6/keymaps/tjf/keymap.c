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
#include QMK_KEYBOARD_H
#include <stdlib.h>
#ifdef POINTING_DEVICE_ENABLE
#    include "bk_pointing_device.h"
#endif

/* Login-detail macros read their text from secrets.h, which git ignores (see
 * secrets.h.example).  Without it, those keys do nothing. */
#if __has_include("secrets.h")
#    include "secrets.h"
#endif
#ifndef SECRET_PASSWORD
#    define SECRET_PASSWORD ""
#endif
#ifndef SECRET_USERNAME
#    define SECRET_USERNAME ""
#endif
#ifndef SECRET_EMAIL
#    define SECRET_EMAIL ""
#endif

/* Layer numbers match the old Argos layout.  Layers 1 and 2 are empty
 * placeholders; the pointer layer stays at 3 (matches AUTO_MOUSE_DEFAULT_LAYER
 * in config.h) and the others sit above it so they override it. */
enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_UNUSED_1,
    LAYER_UNUSED_2,
    LAYER_POINTER,
    LAYER_HYPER,  // One-shot: every key sent with Hyper.
    LAYER_NUMNAV, // Numpad, Ctrl+numpad, arrows, volume.
    LAYER_SYM,    // Brackets and comparison macros.
    LAYER_SYSTEM, // Boot, EEPROM clear, lighting.
};

enum custom_keycodes {
    SMTSCRL = SAFE_RANGE, // Smart scroll (hold).
    SMTSCRL_TOG,          // Smart scroll (toggle).
    M_PASS,               // SECRET_PASSWORD, then Enter.
    M_USER,               // SECRET_USERNAME, then Tab.
    M_EMAIL,              // SECRET_EMAIL, then Tab.
    M_NEQ,                // !=
    M_GTE,                // >=
    M_LTE,                // <=
};

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // POINTING_DEVICE_ENABLE

/* Number row: tap for the digit, double-tap for the F-key. */
enum tap_dances {
    TD_1,
    TD_2,
    TD_3,
    TD_4,
    TD_5,
    TD_6,
    TD_7,
    TD_8,
    TD_9,
    TD_0,
    TD_MINS,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_1] = ACTION_TAP_DANCE_DOUBLE(KC_1, KC_F1),
    [TD_2] = ACTION_TAP_DANCE_DOUBLE(KC_2, KC_F2),
    [TD_3] = ACTION_TAP_DANCE_DOUBLE(KC_3, KC_F3),
    [TD_4] = ACTION_TAP_DANCE_DOUBLE(KC_4, KC_F4),
    [TD_5] = ACTION_TAP_DANCE_DOUBLE(KC_5, KC_F5),
    [TD_6] = ACTION_TAP_DANCE_DOUBLE(KC_6, KC_F6),
    [TD_7] = ACTION_TAP_DANCE_DOUBLE(KC_7, KC_F7),
    [TD_8] = ACTION_TAP_DANCE_DOUBLE(KC_8, KC_F8),
    [TD_9] = ACTION_TAP_DANCE_DOUBLE(KC_9, KC_F9),
    [TD_0] = ACTION_TAP_DANCE_DOUBLE(KC_0, KC_F10),
    [TD_MINS] = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_F12),
};

/* I + O together -> F6. */
const uint16_t PROGMEM io_combo[] = {KC_I, KC_O, COMBO_END};
combo_t key_combos[] = {
    COMBO(io_combo, KC_F6),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
               KC_GRV,         TD(TD_1),         TD(TD_2),         TD(TD_3),         TD(TD_4),         TD(TD_5),            TD(TD_6),         TD(TD_7),         TD(TD_8),         TD(TD_9),         TD(TD_0),      TD(TD_MINS),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
               KC_TAB,             KC_Q,             KC_W,             KC_E,             KC_R,             KC_T,                KC_Y,             KC_U,             KC_I,             KC_O,             KC_P,          KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       LSFT_T(KC_ESC),     LCTL_T(KC_A),     LSFT_T(KC_S),     LALT_T(KC_D),     LGUI_T(KC_F),             KC_G,                KC_H,             KC_J,             KC_K,             KC_L,          KC_SCLN,          KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      LCTL_T(KC_LBRC),     LSFT_T(KC_Z),             KC_X,             KC_C,             KC_V,             KC_B,                KC_N,             KC_M,          KC_COMM,           KC_DOT,          KC_SLSH,           KC_EQL,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                                                    KC_LALT,   LGUI_T(KC_SPC), MO(LAYER_NUMNAV),              KC_ENT,           KC_ENT,
                                                                        LT(LAYER_SYM, KC_SPC), OSL(LAYER_HYPER),             KC_BSPC
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_UNUSED_1] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                _______, _______, _______,    _______, _______,
                                         _______, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_UNUSED_2] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                _______, _______, _______,    _______, _______,
                                         _______, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     KC_LSFT, _______, _______, _______, _______, _______,    _______, MS_BTN1, SMTSCRL, MS_BTN2, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     KC_LCTL, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                KC_LALT, _______, _______,    _______, _______,
                                         _______, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_HYPER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      HYPR(KC_GRV),    HYPR(KC_1),    HYPR(KC_2),    HYPR(KC_3),    HYPR(KC_4),    HYPR(KC_5),       HYPR(KC_6),    HYPR(KC_7),    HYPR(KC_8),    HYPR(KC_9),    HYPR(KC_0), HYPR(KC_MINS),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      HYPR(KC_TAB),    HYPR(KC_Q),    HYPR(KC_W),    HYPR(KC_E),    HYPR(KC_R),    HYPR(KC_T),       HYPR(KC_Y),    HYPR(KC_U),    HYPR(KC_I),    HYPR(KC_O),    HYPR(KC_P), HYPR(KC_BSLS),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      HYPR(KC_ESC),    HYPR(KC_A),    HYPR(KC_S),    HYPR(KC_D),    HYPR(KC_F),    HYPR(KC_G),       HYPR(KC_H),    HYPR(KC_J),    HYPR(KC_K),    HYPR(KC_L), HYPR(KC_SCLN), HYPR(KC_QUOT),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            M_PASS,    HYPR(KC_Z),    HYPR(KC_X),    HYPR(KC_C),    HYPR(KC_V),    HYPR(KC_B),       HYPR(KC_N),    HYPR(KC_M), HYPR(KC_COMM),  HYPR(KC_DOT), HYPR(KC_SLSH),  HYPR(KC_EQL),
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                                        _______,       _______,       _______,          _______,       KC_PENT,
                                                                       _______,       _______,          _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_NUMNAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
              KC_PSLS,            KC_P1,            KC_P2,            KC_P3,            KC_P4,            KC_P5,               KC_P6,            KC_P7,            KC_P8,            KC_P9,            KC_P0,          KC_PAST,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
              _______,          _______,      LCTL(KC_P7),      LCTL(KC_P8),      LCTL(KC_P9),          _______,             KC_PAST,          KC_PSLS,          KC_PPLS,          KC_PMNS,          KC_PGUP,          KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     MO(LAYER_SYSTEM),          _______,      LCTL(KC_P4),      LCTL(KC_P5),      LCTL(KC_P6),    LCTL(KC_PSLS),             KC_LEFT,          KC_DOWN,            KC_UP,          KC_RGHT,          KC_PGDN,          KC_VOLD,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
              _______,          _______,      LCTL(KC_P1),      LCTL(KC_P2),      LCTL(KC_P3),      LCTL(KC_P0),             _______,          _______,          _______,          _______,          _______,          KC_MUTE,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                                                    _______,          _______,          _______,             _______,          KC_PENT,
                                                                                      _______,          _______,              KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_SYM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
           _______,       _______,       _______,       _______,       _______,       _______,          _______,       _______,       _______,       _______,       _______,       _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
           M_EMAIL,       _______,       _______,       _______,       _______,       _______,          _______,        KC_EQL,       KC_PPLS,       KC_PMNS,       _______,       _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            M_USER,       _______,       KC_LBRC, LSFT(KC_LBRC),    LSFT(KC_9), LSFT(KC_QUOT),          KC_QUOT,    LSFT(KC_0), LSFT(KC_RBRC),       KC_RBRC,       KC_SCLN,       _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            M_PASS,       _______,       _______,       _______,       _______,       _______,          _______,         M_NEQ,         M_LTE,         M_GTE,       _______,       _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                                        _______,       _______,       _______,          _______,       _______,
                                                                       _______,       _______,          _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_SYSTEM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
     _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______,   RM_ON,  RM_OFF, RM_PREV,  EE_CLR,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    _______, _______, DRGSCRL, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     _______, _______, _______, _______, _______, _______,    RM_NEXT, _______, _______, _______, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                _______, _______, _______,    _______, _______,
                                         _______, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
/* Smart drag-scroll.
 *
 * While SMTSCRL is held (or after SMTSCRL_TOG is tapped), trackball motion
 * becomes scrolling.  The first
 * SMART_SCROLL_LOCK_THRESHOLD counts of movement decide the axis (vertical or
 * horizontal); after that, only that axis scrolls until the key is released,
 * or until the ball sits still for SMART_SCROLL_RELOCK_MS, after which the
 * next movement picks the axis again.
 */
#    ifndef SMART_SCROLL_LOCK_THRESHOLD
#        define SMART_SCROLL_LOCK_THRESHOLD 16 // Counts of motion before picking an axis.
#    endif
#    ifndef SMART_SCROLL_DIVISOR
#        define SMART_SCROLL_DIVISOR 12.0f // Higher = slower scrolling.
#    endif
#    ifndef SMART_SCROLL_RELOCK_MS
#        define SMART_SCROLL_RELOCK_MS 300 // Idle time before the axis unlocks; 0 = never.
#    endif

typedef enum {
    SCROLL_AXIS_NONE,
    SCROLL_AXIS_V,
    SCROLL_AXIS_H,
} scroll_axis_t;

static bool          smart_scroll_active      = false;
static scroll_axis_t smart_scroll_axis        = SCROLL_AXIS_NONE;
static int16_t       smart_scroll_probe_x     = 0; // Motion gathered before the axis is chosen.
static int16_t       smart_scroll_probe_y     = 0;
static float         smart_scroll_acc         = 0; // Sub-tick remainder on the locked axis.
static uint32_t      smart_scroll_last_motion = 0;

static void smart_scroll_reset(void) {
    smart_scroll_axis    = SCROLL_AXIS_NONE;
    smart_scroll_probe_x = 0;
    smart_scroll_probe_y = 0;
    smart_scroll_acc     = 0;
}

/* Returns false when the key was a smart-scroll key and has been handled. */
static bool process_smart_scroll(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SMTSCRL:
            smart_scroll_active = record->event.pressed;
            smart_scroll_reset();
            return false;
        case SMTSCRL_TOG:
            if (record->event.pressed) {
                smart_scroll_active = !smart_scroll_active;
                smart_scroll_reset();
            }
            return false;
    }
    return true;
}

/* Keep the auto mouse layer active while SMTSCRL is held. */
bool is_mouse_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SMTSCRL:
        case SMTSCRL_TOG:
            return true;
    }
    return false;
}

report_mouse_t pointing_device_task_user(report_mouse_t report) {
    if (!smart_scroll_active) {
        return report;
    }

    int16_t dx = report.x;
    int16_t dy = report.y;
    report.x   = 0;
    report.y   = 0;

    if (dx == 0 && dy == 0) {
        return report;
    }
    // Re-pick the axis if the ball has been still long enough.
    if (SMART_SCROLL_RELOCK_MS > 0 && smart_scroll_axis != SCROLL_AXIS_NONE && timer_elapsed32(smart_scroll_last_motion) > SMART_SCROLL_RELOCK_MS) {
        smart_scroll_reset();
    }
    smart_scroll_last_motion = timer_read32();

    if (smart_scroll_axis == SCROLL_AXIS_NONE) {
        smart_scroll_probe_x += dx;
        smart_scroll_probe_y += dy;
        int16_t ax = abs(smart_scroll_probe_x);
        int16_t ay = abs(smart_scroll_probe_y);
        if (ax + ay < SMART_SCROLL_LOCK_THRESHOLD) {
            return report; // Not enough movement to decide yet.
        }
        smart_scroll_axis = (ay >= ax) ? SCROLL_AXIS_V : SCROLL_AXIS_H;
        // Carry the probe motion into the first scroll so nothing is lost.
        dx = smart_scroll_probe_x;
        dy = smart_scroll_probe_y;
    }

    smart_scroll_acc += (float)(smart_scroll_axis == SCROLL_AXIS_V ? -dy : dx) / SMART_SCROLL_DIVISOR;

    int16_t ticks = (int16_t)smart_scroll_acc; // Truncates toward zero; remainder carries over.
    if (ticks > 127) ticks = 127;
    if (ticks < -127) ticks = -127;
    smart_scroll_acc -= ticks;

    if (smart_scroll_axis == SCROLL_AXIS_V) {
        report.v = ticks;
    } else {
        report.h = ticks;
    }
    return report;
}
#endif // POINTING_DEVICE_ENABLE

/* Sends `text` followed by `after`, or nothing if `text` is empty (no
 * secrets.h). */
static void send_secret(const char *text, uint16_t after) {
    if (text[0] != '\0') {
        send_string(text);
        tap_code(after);
    }
}

/* Note: bk_pointing_device calls this before its own keycode handling, and
 * returning false stops both it and the rest of QMK from seeing the key. */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef POINTING_DEVICE_ENABLE
    if (!process_smart_scroll(keycode, record)) {
        return false;
    }
#endif // POINTING_DEVICE_ENABLE
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case M_PASS:
            send_secret(SECRET_PASSWORD, KC_ENT);
            return false;
        case M_USER:
            send_secret(SECRET_USERNAME, KC_TAB);
            return false;
        case M_EMAIL:
            send_secret(SECRET_EMAIL, KC_TAB);
            return false;
        case M_NEQ:
            SEND_STRING("!=");
            return false;
        case M_GTE:
            SEND_STRING(">=");
            return false;
        case M_LTE:
            SEND_STRING("<=");
            return false;
    }
    return true;
}

#ifdef POINTING_DEVICE_ENABLE
/* Trackball DPI at boot (was set in Argos).  Steps of 200 from 400.  DPI_MOD
 * still changes it, but it resets to this on the next power-up. */
#    ifndef POINTER_DPI
#        define POINTER_DPI 1200
#    endif

/**
 * \brief Apply the pointer settings that used to live in Argos.
 *
 * `bk_pointing_device` restores these from EEPROM.  Module post-init runs
 * before this hook, so setting them here wins; EEPROM is only written when a
 * value actually changes.
 */
void keyboard_post_init_user(void) {
    bkpd_set_pointer_default_dpi(POINTER_DPI);
    bkpd_set_auto_precision_on_mouse_layer_enabled(false);
    bkpd_set_dragscroll_axis_invert_x(false);
    bkpd_set_dragscroll_axis_invert_y(true); // DRGSCRL only; smart scroll is separate.
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    set_auto_mouse_layer(LAYER_POINTER);
    set_auto_mouse_enable(true);
#    endif // POINTING_DEVICE_AUTO_MOUSE_ENABLE
}
#endif // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
/* Per-layer colors (from the Argos config).  Each layer lights every LED in
 * one color, scaled by the current RGB brightness.  Base and pointer layers
 * show the normal RGB effect. */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t r, g, b;
    switch (get_highest_layer(layer_state)) {
        case LAYER_UNUSED_1:
            r = 64, g = 54, b = 0;
            break;
        case LAYER_UNUSED_2:
            r = 19, g = 64, b = 0;
            break;
        case LAYER_HYPER:
            r = 0, g = 39, b = 64;
            break;
        case LAYER_NUMNAV:
            r = 15, g = 0, b = 64;
            break;
        case LAYER_SYM:
            r = 64, g = 0, b = 58;
            break;
        case LAYER_SYSTEM:
            r = 64, g = 0, b = 4;
            break;
        default:
            return false;
    }
    const uint8_t val = rgb_matrix_get_val();
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, r * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, g * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, b * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
    return false;
}
#endif // RGB_MATRIX_ENABLE

/* Shift mod-taps on non-letter keys (Esc/Shift, Enter/Shift, ...) turn into
 * Shift the moment another key goes down.  Letter mod-taps (home-row mods)
 * keep the default behavior so fast typing rolls aren't read as capitals.
 * Matches on keycode, so it follows keys wherever they are in the layout. */
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_MOD_TAP(keycode)) {
        uint8_t  mods = QK_MOD_TAP_GET_MODS(keycode);
        uint16_t tap  = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        if ((mods & MOD_LSFT) && !(tap >= KC_A && tap <= KC_Z)) {
            return true;
        }
    }
    return false;
}
