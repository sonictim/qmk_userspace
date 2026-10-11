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
#ifdef POINTING_DEVICE_ENABLE
#    include "bk_pointing_device.h"
#    include "smart_scroll.h"
#endif

/* The pointer layer is 1 (must match AUTO_MOUSE_DEFAULT_LAYER in config.h);
 * the others sit above it so they override it when both are active. */
enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_POINTER,
    LAYER_BASE2,   // Pro Tools: home-row mods and combos (TG on MISSING's G).
    LAYER_MISSING, // Numpad, Ctrl+numpad, arrows, volume.
    LAYER_KEYPAD,  // Brackets and comparison macros.
    LAYER_SYSTEM,  // Boot, EEPROM clear, lighting.
};

_Static_assert(LAYER_POINTER == AUTO_MOUSE_DEFAULT_LAYER, "Set AUTO_MOUSE_DEFAULT_LAYER in config.h to match LAYER_POINTER");

/* Short names so every key fits the same column width in the layouts. */
#define SFT_ESC LSFT_T(KC_ESC)
#define GUI_SPC LGUI_T(KC_SPC)
#define HYP_SPC HYPR_T(KC_SPC)
#define KEYPAD LT(LAYER_KEYPAD, KC_SPC)
#define MISSING LT(LAYER_MISSING, KC_SLSH)
#define SYS MO(LAYER_SYSTEM)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define K_DRAG LT(0, KC_K) // Tap K, hold drag-scroll (see process_record_user).
enum custom_keycodes {
    SMTSCRL = SAFE_RANGE, // Smart scroll (hold).
    M_PASS,               // SECRET_PASSWORD, then Enter.
    M_PASS2,              // SECRET_PASSWORD2, then Enter.
    M_USER,               // SECRET_USERNAME, then Tab.
    M_EMAIL,              // SECRET_EMAIL, then Tab.
    M_NEQ,                // !=
    M_GTE,                // >=
    M_LTE,                // <=
    AUTOPTR,              // Auto pointer layer on/off (off at boot).
    K_CTRL,               // K_DRAG while Ctrl is held: plain K (pre_process_record_user).
};

/* Combos: keys pressed together (within COMBO_TERM).  The keys must match the
 * layout exactly, so the A-F ones only fire on BASE2's home-row mods, never on
 * base's plain letters.  When one combo is part of a bigger one (S+D and D+F
 * inside S+D+F), QMK waits and picks the bigger one.
 *   S + D     -> Shift+S       (tap only, see get_combo_must_tap)
 *   D + F     -> Shift+Cmd+S   (tap only)
 *   S + D + F -> Shift+Opt+S   (tap only)
 *   J + K     -> left click    (held while the keys are held)
 *   K + L     -> right click
 * With Ctrl held, K is K_CTRL instead of K_DRAG, so the clicks have a K_CTRL
 * copy too (Ctrl+click). */
enum combos {
    COMBO_SFT_S,
    COMBO_SFT_CMD_S,
    COMBO_SFT_OPT_S,
    COMBO_LCLICK,
    COMBO_RCLICK,
    COMBO_LCLICK_CTRL,
    COMBO_RCLICK_CTRL,
};
const uint16_t PROGMEM sft_s_combo[]     = {LSFT_T(KC_S), LALT_T(KC_D), COMBO_END};
const uint16_t PROGMEM sft_cmd_s_combo[] = {LALT_T(KC_D), LGUI_T(KC_F), COMBO_END};
const uint16_t PROGMEM sft_opt_s_combo[] = {LSFT_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), COMBO_END};
const uint16_t PROGMEM lclick_combo[]    = {KC_J, K_DRAG, COMBO_END};
const uint16_t PROGMEM rclick_combo[]    = {KC_L, K_DRAG, COMBO_END};
const uint16_t PROGMEM lclick_ctrl_combo[] = {KC_J, K_CTRL, COMBO_END};
const uint16_t PROGMEM rclick_ctrl_combo[] = {KC_L, K_CTRL, COMBO_END};
combo_t                key_combos[]      = {
    [COMBO_SFT_S] = COMBO(sft_s_combo, LSFT(KC_S)), [COMBO_SFT_CMD_S] = COMBO(sft_cmd_s_combo, LSG(KC_S)), [COMBO_SFT_OPT_S] = COMBO(sft_opt_s_combo, LSA(KC_S)), [COMBO_LCLICK] = COMBO(lclick_combo, MS_BTN1), [COMBO_RCLICK] = COMBO(rclick_combo, MS_BTN2),
    [COMBO_LCLICK_CTRL] = COMBO(lclick_ctrl_combo, MS_BTN1), [COMBO_RCLICK_CTRL] = COMBO(rclick_ctrl_combo, MS_BTN2),
};

/* Tap-only combos fire when the keys are released within COMBO_HOLD_TERM
 * (= TAPPING_TERM).  Held longer, the combo is dropped and the keys act on
 * their own, so they become the normal home-row mods. */
bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case COMBO_SFT_S:
        case COMBO_SFT_CMD_S:
        case COMBO_SFT_OPT_S:
            return true;
    }
    return false;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
             KC_ESC,      KC_1,              KC_2,          KC_3,         KC_4,           KC_5,          KC_6,              KC_7,      KC_8,         KC_9,              KC_0,    KC_MINS,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
             KC_TAB,          KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,            KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,       KC_BSLS,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            SFT_ESC,        KC_A,            KC_S,       KC_D,       KC_F,          KC_G,            KC_H,          KC_J,          K_DRAG,          KC_L,       KC_SCLN,       KC_QUOT,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
           KC_LCTL,        PT_Z,          KC_X,          KC_C,          KC_V,          KC_B,            KC_N,          KC_M,       KC_COMM,        KC_DOT,       KC_SLSH,        KC_EQL,
  // ╰────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         KC_LALT,       GUI_SPC,       HYP_SPC,         KC_ENT,         MISSING,
                                                                        KEYPAD,    MISSING,         KC_BSPC
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       MS_BTN1,       SMTSCRL,       MS_BTN2,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       _______,       _______,         _______,       _______,
                                                                        _______,       _______,         _______
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),

  [LAYER_BASE2] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       LCTL_T(KC_A),  LSFT_T(KC_S),  LALT_T(KC_D),  LGUI_T(KC_F),  _______,         _______,       _______,       _______,    _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       _______,       _______,         _______,    _______,
                                                                        _______,    _______,          _______
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),




  [LAYER_MISSING] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            KC_GRV,         KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,           KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,       KC_F12,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,      _______,     _______,        _______,         _______,       _______,       KC_LBRC,       KC_RBRC,       KC_PGUP,       KC_VOLU,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       KC_LCTL,       KC_LSFT,       KC_LALT,    KC_LGUI,     TG(LAYER_BASE2),        KC_LEFT,       KC_DOWN,         KC_UP,       KC_RGHT,       KC_PGDN,       KC_VOLD,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            SYS,       _______,      _______,        _______,        _______,        _______,           M_NEQ,        KC_EQL,         M_LTE,         M_GTE,       _______,       KC_MUTE,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       _______,       _______,       KC_PENT,       KC_PENT,
                                                                        _______,       _______,          KC_DEL
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),

  [LAYER_KEYPAD] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            M_PASS2,       KC_P1,       KC_P2,       KC_P3,       KC_P4,       KC_P5,         KC_P6,       KC_P7,       KC_P8,       KC_P9,       KC_P0,       KC_PMNS,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            M_EMAIL,       _______,       _______,       _______,       _______,       _______,         _______,        KC_P7,         KC_P8,         KC_P9,       _______,       KC_PPLS,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
             M_USER,       _______,       _______,       _______,        _______,       _______,         KC_PSLS,         KC_P4,       KC_P5,       KC_P6,       _______,       KC_PEQL,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
             M_PASS,       _______,       _______,       _______,       _______,       _______,         KC_PDOT,         KC_P1,         KC_P2,         KC_P3,       KC_PSLS,       KC_PAST,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       _______,       _______,         _______,       KC_PENT,
                                                                        _______,       _______,         KC_P0
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),

  [LAYER_SYSTEM] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       QK_BOOT,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,         RM_ON,        RM_OFF,       RM_PREV,        EE_CLR,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       DRGSCRL,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         RM_NEXT,       AUTOPTR,       _______,       _______,       _______,       _______,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       _______,       _______,         _______,       _______,
                                                                        _______,       _______,         _______
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),
};
// clang-format on

/* ---------------------------------------------------------------------------
 * Hooks.  Values come from config.h.
 * ------------------------------------------------------------------------- */

/* Login-detail macros read their text from secrets.h, which git ignores (see
 * secrets.h.example).  Without it, those keys do nothing. */
#if __has_include("secrets.h")
#    include "secrets.h"
#endif
#ifndef SECRET_PASSWORD
#    define SECRET_PASSWORD ""
#endif
#ifndef SECRET_PASSWORD2
#    define SECRET_PASSWORD2 ""
#endif
#ifndef SECRET_USERNAME
#    define SECRET_USERNAME ""
#endif
#ifndef SECRET_EMAIL
#    define SECRET_EMAIL ""
#endif

/* Sends `text` followed by `after`, or nothing if `text` is empty. */
static void send_secret(const char *text, uint16_t after) {
    if (text[0] != '\0') {
        send_string(text);
        tap_code(after);
    }
}

#ifdef POINTING_DEVICE_ENABLE
/* K_DRAG early start: moving the ball while K is down starts drag-scroll
 * without waiting for TAPPING_TERM (see pointing_device_task_user).  Tracked
 * on raw key events, because combos and tap-hold hold K back from
 * process_record_user, and a J+K / K+L click combo eats K's events entirely. */
static bool     k_drag_down     = false; // K physically down.
static uint16_t k_drag_at       = 0;     // When K went down.
static bool     k_drag_early    = false; // Drag-scroll started by ball movement.
static bool     k_drag_skip     = false; // Drop K_DRAG's remaining events (no K typed).
static uint8_t  click_keys_down = 0;     // J or L down: K may be part of a click combo.
static bool     k_ctrl          = false; // This K press was turned into K_CTRL.
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
static bool auto_pointer_on = false; // AUTOPTR flips this.  Off at boot.
#    endif

/* Runs on the raw key event, before combos and tap-hold timing. */
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case K_DRAG:
            /* Ctrl held (Ctrl+K, Ctrl+Shift+K): swap in K_CTRL, a plain key,
             * so K isn't held back by tap-hold and never drag-scrolls.  It
             * still waits up to COMBO_TERM for the Ctrl+click combos.
             * Speculative mods count, so BASE2's Ctrl-A works while QMK is
             * still deciding tap or hold. */
            if (record->event.pressed ? (get_mods() | get_speculative_mods()) & MOD_MASK_CTRL : k_ctrl) {
                k_ctrl          = record->event.pressed;
                record->keycode = K_CTRL;
                break;
            }
            k_drag_down = record->event.pressed;
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
            /* Pause auto mouse while K is down: drag-scroll movement would
             * turn the pointer layer on, and the layer change cancels
             * drag-scroll. */
            set_auto_mouse_enable(auto_pointer_on && !record->event.pressed);
#    endif
            if (record->event.pressed) {
                k_drag_at   = timer_read();
                k_drag_skip = false;
            } else if (k_drag_early) {
                /* Turn it off here, not in process_record_user: if K was
                 * part of a combo, that never sees the release. */
                k_drag_early = false;
                k_drag_skip  = true;
                bkpd_mode_release(MODE_DRAGSCROLL);
            }
            break;
        case KC_J:
        case KC_L:
            if (record->event.pressed) {
                click_keys_down++;
            } else if (click_keys_down) {
                click_keys_down--;
            }
            break;
    }
    return true;
}
#endif // POINTING_DEVICE_ENABLE

/* Note: bk_pointing_device calls this before its own keycode handling, and
 * returning false stops both it and the rest of QMK from seeing the key. */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef POINTING_DEVICE_ENABLE
        case SMTSCRL:
            smart_scroll_set_active(record->event.pressed);
            return false;
        case K_DRAG:
            if (k_drag_early || k_drag_skip) {
                /* Started by ball movement: don't type the K, whether QMK
                 * calls it a tap or a hold.  pre_process_record_user turns
                 * drag-scroll off on release. */
                if (!record->event.pressed) {
                    k_drag_skip = false;
                }
                return false;
            }
            if (record->tap.count) {
                /* Tapped: QMK sends the K.  With Flow Tap that happens while K
                 * is still down, so stop the ball from starting drag-scroll. */
                if (record->event.pressed) {
                    k_drag_down = false;
                }
                return true;
            }
            /* Held past TAPPING_TERM: drag-scroll until released. */
            if (record->event.pressed) {
                bkpd_mode_set_active(MODE_DRAGSCROLL);
            } else {
                bkpd_mode_release(MODE_DRAGSCROLL);
            }
            return false;
#endif // POINTING_DEVICE_ENABLE
        case K_CTRL:
            if (record->event.pressed) {
                register_code(KC_K);
            } else {
                unregister_code(KC_K);
            }
            return false;
    }
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case M_PASS:
            send_secret(SECRET_PASSWORD, KC_ENT);
            return false;
        case M_PASS2:
            send_secret(SECRET_PASSWORD2, KC_ENT);
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
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
        case AUTOPTR:
            auto_pointer_on = !auto_pointer_on;
            set_auto_mouse_enable(auto_pointer_on);
            /* A disabled auto mouse never turns its layer off. */
            layer_off(LAYER_POINTER);
            return false;
#endif
    }
    return true;
}

#ifdef POINTING_DEVICE_ENABLE
/* Keep the auto mouse layer active while smart scroll is held. */
bool is_mouse_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SMTSCRL:
            return true;
    }
    return false;
}

report_mouse_t pointing_device_task_user(report_mouse_t report) {
    /* Ball moved while K is down: start drag-scroll without waiting for
     * TAPPING_TERM.  Not while J or L is down, or before COMBO_TERM has
     * passed, so the J+K / K+L click combos still click-drag. */
    if (k_drag_down && !k_drag_early && !click_keys_down && timer_elapsed(k_drag_at) > COMBO_TERM && (report.x || report.y)) {
        k_drag_early = true;
        bkpd_mode_set_active(MODE_DRAGSCROLL);
    }
    return smart_scroll_task(report);
}

/* Pointer settings, applied at every boot.  bk_pointing_device's post-init runs
 * before this hook, so these win.  Without Argos the module only saves one byte
 * of its config (the on/off flags), so each mode's DPI and invert would come up
 * as 0 after a reboot: bkpd_modes_init() resets them, then config.h sets them. */
void keyboard_post_init_user(void) {
    bkpd_modes_init();
    bkpd_mode_change_dpi(MODE_NORMAL, POINTER_DPI);
    bkpd_mode_change_dpi(MODE_SNIPING, POINTER_SNIPING_DPI);
    bkpd_mode_change_dpi(MODE_DRAGSCROLL, POINTER_DRAGSCROLL_DPI);
    bkpd_mode_set_invert(MODE_DRAGSCROLL, 0, POINTER_DRAGSCROLL_INVERT_X);
    bkpd_mode_set_invert(MODE_DRAGSCROLL, 1, POINTER_DRAGSCROLL_INVERT_Y);
    bkpd_mode_apply_dpi(bkpd_mode_get_active_id());

    bkpd_set_auto_precision_on_mouse_layer_enabled(POINTER_AUTO_PRECISION);
    bkpd_set_auto_mouse_layer_enabled(false); // AUTOPTR turns it on.
}
#endif // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
static void fill_layer_color(uint8_t led_min, uint8_t led_max, uint8_t r, uint8_t g, uint8_t b) {
    const uint8_t val = rgb_matrix_get_val();
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, r * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, g * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, b * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
}

/* Pressed-key lighting for BASE2.  Each key press picks a new
 * color; the key stays lit while held and fades out after release.  Each half
 * spots presses by comparing its own key matrix frame to frame, so this works
 * on both sides of the split without sending key events across. */
static bool     key_held[RGB_MATRIX_LED_COUNT];
static uint8_t  key_hue[RGB_MATRIX_LED_COUNT];
static uint16_t key_released_at[RGB_MATRIX_LED_COUNT];
static bool     key_fading[RGB_MATRIX_LED_COUNT];
static uint8_t  next_hue = 0;

/* Watches for presses and releases on every frame, on any layer, so the state
 * is current when BASE2 turns on. */
static void track_pressed_keys(uint8_t led_min, uint8_t led_max) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED || led < led_min || led >= led_max) {
                continue;
            }
            bool held = matrix_is_on(row, col);
            if (held && !key_held[led]) {
                key_hue[led] = next_hue;
                next_hue += PRESSED_HUE_STEP;
            } else if (!held && key_held[led]) {
                key_released_at[led] = timer_read();
                key_fading[led]      = true;
            }
            key_held[led] = held;
        }
    }
}

/* All LEDs off except held keys (full brightness) and released keys (fading
 * out over PRESSED_FADE_MS). */
static void light_pressed_keys(uint8_t led_min, uint8_t led_max) {
    for (uint8_t led = led_min; led < led_max; led++) {
        uint8_t val = 0;
        if (key_held[led]) {
            val = PRESSED_BRIGHTNESS;
        } else if (key_fading[led]) {
            uint16_t elapsed = timer_elapsed(key_released_at[led]);
            if (elapsed < PRESSED_FADE_MS) {
                val = (uint32_t)PRESSED_BRIGHTNESS * (PRESSED_FADE_MS - elapsed) / PRESSED_FADE_MS;
            } else {
                key_fading[led] = false;
            }
        }
        rgb_t rgb = hsv_to_rgb((hsv_t){key_hue[led], 255, val});
        rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
    }
}

/* Each layer lights every LED in one color (LAYER_COLOR_* in config.h),
 * scaled by the current RGB brightness.  BASE2 goes dark and lights only the
 * keys being pressed (see above).  Base and pointer show the normal RGB
 * effect. */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    track_pressed_keys(led_min, led_max);
    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE2:
            light_pressed_keys(led_min, led_max);
            break;
        case LAYER_MISSING:
            fill_layer_color(led_min, led_max, LAYER_COLOR_NUMNAV);
            break;
        case LAYER_KEYPAD:
            fill_layer_color(led_min, led_max, LAYER_COLOR_SYM);
            break;
        case LAYER_SYSTEM:
            fill_layer_color(led_min, led_max, LAYER_COLOR_SYSTEM);
            break;
    }
    return false;
}
#endif // RGB_MATRIX_ENABLE

/* Mod-taps (SFT_ESC, GUI_SPC, HYP_SPC, BASE2's A/S/D/F, and any new ones):
 * Speculative Hold (config.h).  The modifier is on from the moment the key
 * goes down, and is cancelled if the key turns out to be a tap.  QMK's
 * default only covers Shift and Ctrl; this covers every modifier. */
bool get_speculative_hold(uint16_t keycode, keyrecord_t *record) {
    return true;
}

/* Layer-taps (KEYPAD, MISSING, PT_Z, K_DRAG, and any new ones): Flow Tap
 * (FLOW_TAP_TERM in config.h).  Pressed soon after a typing key (letter,
 * space, punctuation; QMK's is_flow_tap_key), it is a tap right away.  Off
 * while Ctrl, Cmd or Alt is held.  Never for mod-taps: Flow Tap would block
 * their modifier right after typing (Cmd+F), and they have Speculative Hold. */
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    if (IS_QK_MOD_TAP(keycode)) {
        return 0;
    }
    if (is_flow_tap_key(prev_keycode)) {
        return FLOW_TAP_TERM;
    }
    return 0;
}

/* Mod-taps that become their modifier the moment another key goes down, so
 * that key (or a J+K click) is sent on its press instead of waiting for the
 * tap/hold decision.  Speculative Hold already sends the modifier itself on
 * keydown; this is about the key that comes next.  The cost is that a fast
 * roll reads as modifier+key, so not on keys used for typing words.
 * Matches on keycode, so it follows keys wherever they are in the layout. */
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        /* BASE2's home-row mods (Pro Tools): Ctrl, Shift, Option, Cmd +
         * click, drag or key happen instantly.  Base uses plain letters. */
        case LCTL_T(KC_A):
        case LSFT_T(KC_S):
        case LALT_T(KC_D):
        case LGUI_T(KC_F):
            return true;
        /* Hyper includes Shift, so the rule below would catch it.  Space is
         * typed constantly: only Hyper after TAPPING_TERM (or PERMISSIVE_HOLD),
         * so fast rolls stay spaces. */
        case HYP_SPC:
            return false;
    }
    /* Shift mod-taps on non-letter keys (SFT_ESC). */
    if (IS_QK_MOD_TAP(keycode)) {
        uint8_t  mods = QK_MOD_TAP_GET_MODS(keycode);
        uint16_t tap  = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        if ((mods & MOD_LSFT) && !(tap >= KC_A && tap <= KC_Z)) {
            return true;
        }
    }
    return false;
}
