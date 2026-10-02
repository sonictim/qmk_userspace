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
    LAYER_BASE2,
    LAYER_POINTER2,
    LAYER_MISSING, // Numpad, Ctrl+numpad, arrows, volume.
    LAYER_KEYPAD,  // Brackets and comparison macros.
    LAYER_SYSTEM,  // Boot, EEPROM clear, lighting.
};

_Static_assert(LAYER_POINTER == AUTO_MOUSE_DEFAULT_LAYER, "Set AUTO_MOUSE_DEFAULT_LAYER in config.h to match LAYER_POINTER");

/* Short names so every key fits the same column width in the layouts. */
#define SFT_ESC LSFT_T(KC_ESC)
#define GUI_SPC LGUI_T(KC_SPC)
#define KEYPAD LT(LAYER_KEYPAD, KC_SPC)
#define MISSING LT(LAYER_MISSING, KC_SPC)
#define SYS MO(LAYER_SYSTEM)
#define BASE2_TO TO(LAYER_BASE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_Z2 LT(LAYER_POINTER2, KC_Z)
#define PT_SCLN LT(LAYER_POINTER, KC_SCLN)

enum custom_keycodes {
    SMTSCRL = SAFE_RANGE, // Smart scroll (hold).
    CTL_SFT,              // Hold Ctrl+Shift and leave the auto pointer layer.
    M_PASS,               // SECRET_PASSWORD, then Enter.
    M_PASS2,              // SECRET_PASSWORD2, then Enter.
    M_USER,               // SECRET_USERNAME, then Tab.
    M_EMAIL,              // SECRET_EMAIL, then Tab.
    M_NEQ,                // !=
    M_GTE,                // >=
    M_LTE,                // <=
};

/* A + S pressed together (within COMBO_TERM) -> CTL_SFT: Ctrl+Shift right away,
 * held until either key is released.  The keys must match the layout exactly,
 * so this only fires on the base layer's mod-taps, never on BASE2's plain A/S. */
const uint16_t PROGMEM ctl_sft_combo[] = {LCTL_T(KC_A), LSFT_T(KC_S), COMBO_END};
combo_t                key_combos[]    = {
    COMBO(ctl_sft_combo, CTL_SFT),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
             KC_GRV,      KC_1,              KC_2,          KC_3,         KC_4,           KC_5,          KC_6,              KC_7,      KC_8,         KC_9,              KC_0,    KC_MINS,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
             KC_TAB,          KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,            KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,       KC_BSLS,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            SFT_ESC,  LCTL_T(KC_A),  LSFT_T(KC_S),  LALT_T(KC_D),  LGUI_T(KC_F),          KC_G,            KC_H,          KC_J,          KC_K,          KC_L,       PT_SCLN,       KC_QUOT,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
           KC_LCTL,        PT_Z,          KC_X,          KC_C,          KC_V,          KC_B,            KC_N,          KC_M,       KC_COMM,        KC_DOT,       KC_SLSH,        KC_EQL,
  // ╰────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         KC_LALT,       GUI_SPC,        MISSING,         KC_ENT,         LM(LAYER_BASE, MOD_HYPR),
                                                                        KEYPAD,    OSM(MOD_HYPR),         KC_BSPC
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
            KC_ESC,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            KC_LSFT,       KC_A,            KC_S,        KC_D,           KC_F,       _______,         _______,       KC_J,       KC_K,    KC_L,       KC_SCLN,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       PT_Z2,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       KC_LGUI,       _______,         _______,    LM(LAYER_BASE2, MOD_HYPR),
                                                        OSL(LAYER_POINTER2),    OSL(LAYER_POINTER2),  _______
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),

  [LAYER_POINTER2] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            M_PASS2,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            M_EMAIL,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            M_USER,       _______,       _______,       _______,       _______,       _______,         _______,       MS_BTN1,       DRGSCRL,       MS_BTN2,       _______,       _______,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            M_PASS,       _______,       _______,       _______,       _______,       _______,         _______,       _______,       _______,       _______,       _______,       _______,
  // ╰─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────╯
                                                         _______,       _______,       _______,         _______,       _______,
                                                                        _______,       _______,         _______
  //                                              ╰────────────────────────────────────────────╯ ╰─────────────────────────────╯
  ),


  [LAYER_MISSING] = LAYOUT(
  // ╭─────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────╮
            KC_F11,         KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,           KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,       KC_F12,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,      _______,     _______,        _______,         _______,       _______,       KC_LBRC,       KC_RBRC,       KC_PGUP,       KC_VOLU,
  // ├─────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────┤
            _______,       _______,       _______,       _______,    _______,        _______,        KC_LEFT,       KC_DOWN,         KC_UP,       KC_RGHT,       KC_PGDN,       KC_VOLD,
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
            _______,       _______,       _______,       _______,       _______,       _______,         RM_NEXT,    TG(LAYER_BASE2),  _______,       _______,       _______,       _______,
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

/* Note: bk_pointing_device calls this before its own keycode handling, and
 * returning false stops both it and the rest of QMK from seeing the key. */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef POINTING_DEVICE_ENABLE
        case SMTSCRL:
            smart_scroll_set_active(record->event.pressed);
            return false;
#endif // POINTING_DEVICE_ENABLE
        case CTL_SFT:
            if (record->event.pressed) {
                register_mods(MOD_LCTL | MOD_LSFT);
            } else {
                unregister_mods(MOD_LCTL | MOD_LSFT);
            }
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
            /* QMK leaves the pointer layer on for modifiers (for Ctrl+click);
             * treat this like a normal key instead, so it drops the layer. */
            auto_mouse_reset_trigger(record->event.pressed);
#endif
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
    bkpd_set_auto_mouse_layer_enabled(true);
}
#endif // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
static void fill_layer_color(uint8_t led_min, uint8_t led_max, uint8_t r, uint8_t g, uint8_t b) {
    const uint8_t val = rgb_matrix_get_val();
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, r * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, g * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, b * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
}

/* Pressed-key lighting for BASE2 and POINTER2.  Each key press picks a new
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
 * scaled by the current RGB brightness.  BASE2 and POINTER2 go dark and light
 * only the keys being pressed (see above).  Base and pointer show the normal
 * RGB effect. */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    track_pressed_keys(led_min, led_max);
    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE2:
        case LAYER_POINTER2:
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

/* Shift mod-taps on non-letter keys (Esc/Shift, Enter/Shift, ...) turn into
 * Shift the moment another key goes down, and so do A and S (see below).
 * Other letter mod-taps (home-row mods) keep the default behavior so fast
 * rolls aren't read as modifiers.
 * Matches on keycode, so it follows keys wherever they are in the layout. */
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        /* Ctrl on A and Shift on S also turn on the moment another key goes
         * down, so Ctrl/Shift + click, drag or J/K happen instantly in Pro
         * Tools.  Base layer only: BASE2 (for typing) uses plain A and S. */
        case LCTL_T(KC_A):
        case LSFT_T(KC_S):
            return true;
    }
    if (IS_QK_MOD_TAP(keycode)) {
        uint8_t  mods = QK_MOD_TAP_GET_MODS(keycode);
        uint16_t tap  = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        if ((mods & MOD_LSFT) && !(tap >= KC_A && tap <= KC_Z)) {
            return true;
        }
    }
    return false;
}
