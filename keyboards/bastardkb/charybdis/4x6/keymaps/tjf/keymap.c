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

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
};

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifdef POINTING_DEVICE_ENABLE
/* Smart drag-scroll borrows Argos' "Custom mode 1" keycodes so it can be
 * assigned from Argos.  Handling them here stops bk_pointing_device from
 * running its own custom mode.  See bk_pointing_modes.c for the table. */
#    define SMTSCRL 0x7E14     // Custom mode 1 (hold)
#    define SMTSCRL_TOG 0x7E15 // Custom mode 1 (toggle)
#else
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_LGUI, KC_SPC,   LOWER,      RAISE,  KC_ENT,
                                           KC_LALT, KC_BSPC,     KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_LBRC,   KC_P7,   KC_P8,   KC_P9, KC_RBRC, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_TOGG, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PPLS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_PEQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      RM_PREV, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PAST,   KC_P1,   KC_P2,   KC_P3, KC_PSLS, KC_PDOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, XXXXXXX, _______,    XXXXXXX, _______,
                                           XXXXXXX, XXXXXXX,      KC_P0
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, XXXXXXX,    _______, XXXXXXX,
                                           _______, _______,    XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,  EE_CLR,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, MS_BTN1, SMTSCRL, MS_BTN2, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  MS_BTN2, MS_BTN1, MS_BTN3,    MS_BTN3, MS_BTN1,
                                           XXXXXXX, MS_BTN2,    MS_BTN2
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
#        define SMART_SCROLL_LOCK_THRESHOLD 8 // Counts of motion before picking an axis.
#    endif
#    ifndef SMART_SCROLL_DIVISOR
#        define SMART_SCROLL_DIVISOR 8.0f // Higher = slower scrolling.
#    endif
#    ifndef SMART_SCROLL_RELOCK_MS
#        define SMART_SCROLL_RELOCK_MS 500 // Idle time before the axis unlocks; 0 = never.
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

/* Note: bk_pointing_device calls this before its own keycode handling, and
 * returning false stops both it and the rest of QMK from seeing the key. */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
/**
 * \brief Force the auto mouse layer on at boot.
 *
 * `bk_pointing_device` restores this flag from EEPROM (the Argos toggle) and
 * defaults it to off.  Module post-init runs before this hook, so setting it
 * here wins.  Argos can still turn it off until the next power cycle.
 */
void keyboard_post_init_user(void) {
    set_auto_mouse_layer(LAYER_POINTER);
    set_auto_mouse_enable(true);
}
#endif // POINTING_DEVICE_AUTO_MOUSE_ENABLE
