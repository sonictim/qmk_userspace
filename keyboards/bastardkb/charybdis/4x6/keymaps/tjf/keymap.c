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
    LAYER_PROTOOLS, // Pro Tools mode: empty; turns on instant mods and combos (TG on MISSING's G).
    LAYER_MISSING,  // Numpad, Ctrl+numpad, arrows, volume.
    LAYER_KEYPAD,   // Brackets and comparison macros.
    LAYER_SYSTEM,   // Boot, EEPROM clear, lighting.
};

_Static_assert(LAYER_POINTER == AUTO_MOUSE_DEFAULT_LAYER, "Set AUTO_MOUSE_DEFAULT_LAYER in config.h to match LAYER_POINTER");

/* Short names so every key fits the same column width in the layouts. */
#define KEYPAD LT(LAYER_KEYPAD, KC_SPC)
#define MISSING LT(LAYER_MISSING, KC_SLSH)
#define PT_Z LT(LAYER_POINTER, KC_Z)
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
};

/* Combos: keys pressed together (within COMBO_TERM).  The S/D/F ones only fire
 * with PROTOOLS on (combo_should_trigger).  When one combo is part of a bigger one
 * (S+D and D+F inside S+D+F), QMK waits and picks the bigger one.
 *   S + D     -> Shift+S       (tap only, see get_combo_must_tap)
 *   D + F     -> Shift+Cmd+S   (tap only)
 *   S + D + F -> Shift+Opt+S   (tap only)
 *   J + K     -> left click    (held while the keys are held)
 *   K + L     -> drag-scroll   (held while the keys are held)
 *   L + ;     -> right click
 * No click combo is inside another, so each fires as soon as both keys are
 * down. */
enum combos {
    COMBO_SFT_S,
    COMBO_SFT_CMD_S,
    COMBO_SFT_OPT_S,
    COMBO_LCLICK,
    COMBO_RCLICK,
    COMBO_DRGSCRL,
};
const uint16_t PROGMEM sft_s_combo[]     = {LSFT_T(KC_S), LALT_T(KC_D), COMBO_END};
const uint16_t PROGMEM sft_cmd_s_combo[] = {LALT_T(KC_D), LGUI_T(KC_F), COMBO_END};
const uint16_t PROGMEM sft_opt_s_combo[] = {LSFT_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), COMBO_END};
const uint16_t PROGMEM lclick_combo[]    = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM rclick_combo[]    = {KC_L, KC_SCLN, COMBO_END};
const uint16_t PROGMEM drgscrl_combo[]   = {KC_K, KC_L, COMBO_END};
// clang-format off
combo_t key_combos[] = {
    [COMBO_SFT_S]     = COMBO(sft_s_combo,     LSFT(KC_S)),
    [COMBO_SFT_CMD_S] = COMBO(sft_cmd_s_combo, LSG(KC_S)),
    [COMBO_SFT_OPT_S] = COMBO(sft_opt_s_combo, LSA(KC_S)),
    [COMBO_LCLICK]    = COMBO(lclick_combo,    MS_BTN1),
    [COMBO_RCLICK]    = COMBO(rclick_combo,    MS_BTN2),
    [COMBO_DRGSCRL]   = COMBO(drgscrl_combo,   SMTSCRL),
};
// clang-format on

/* Tap-only combos fire when the keys are released within COMBO_HOLD_TERM
 * (= TAPPING_TERM).  Held longer, the combo is dropped and the keys act on
 * their own, so they become the normal home-row mods. */
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch (combo_index) {
        case COMBO_SFT_S:
        case COMBO_SFT_CMD_S:
        case COMBO_SFT_OPT_S:
            return layer_state_is(LAYER_PROTOOLS);
    }
    return true;
}

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
  //╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
      KC_ESC,              KC_1,                KC_2,                KC_3,                KC_4,                KC_5,                  KC_6,                KC_7,                KC_8,                KC_9,                KC_0,                KC_MINS,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      KC_TAB,              KC_Q,                KC_W,                KC_E,                KC_R,                KC_T,                  KC_Y,                KC_U,                KC_I,                KC_O,                KC_P,                KC_BSLS,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      LSFT_T(KC_ESC),      LCTL_T(KC_A),        LSFT_T(KC_S),        LALT_T(KC_D),        LGUI_T(KC_F),        KC_G,                  KC_H,                KC_J,                KC_K,                KC_L,                KC_SCLN,             KC_QUOT,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      KC_LCTL,             PT_Z,                KC_X,                KC_C,                KC_V,                KC_B,                  KC_N,                KC_M,                KC_COMM,             KC_DOT,              KC_SLSH,             KC_EQL,
  //╰─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
                                                                     KC_LALT,             LGUI_T(KC_SPC),      HYPR_T(KC_SPC),        KC_ENT,              MISSING,
                                                                                          KEYPAD,              MISSING,               KC_BSPC
  //                                                               ╰──────────────────────────────────────────────────────────────╯ ╰─────────────────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  //╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             MS_BTN1,             SMTSCRL,             MS_BTN2,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //╰─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
                                                                     _______,             _______,             _______,               _______,             _______,
                                                                                          _______,             _______,               _______
  //                                                               ╰──────────────────────────────────────────────────────────────╯ ╰─────────────────────────────────────────╯
  ),

  [LAYER_PROTOOLS] = LAYOUT(
  //╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //╰─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
                                                                     _______,             _______,             _______,               _______,             _______,
                                                                                          _______,             _______,               _______
  //                                                               ╰──────────────────────────────────────────────────────────────╯ ╰─────────────────────────────────────────╯
  ),

  [LAYER_MISSING] = LAYOUT(
  //╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
      KC_GRV,              KC_F1,               KC_F2,               KC_F3,               KC_F4,               KC_F5,                 KC_F6,               KC_F7,               KC_F8,               KC_F9,               KC_F10,              KC_F12,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             KC_LBRC,             KC_RBRC,             KC_PGUP,             KC_VOLU,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             KC_LCTL,             LSFT(KC_S),          KC_LALT,             KC_LGUI,             TG(LAYER_PROTOOLS),    KC_LEFT,             KC_DOWN,             KC_UP,               KC_RGHT,             KC_PGDN,             KC_VOLD,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      MO(LAYER_SYSTEM),    _______,             _______,             _______,             _______,             _______,               M_NEQ,               KC_EQL,              M_LTE,               M_GTE,               _______,             KC_MUTE,
  //╰─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
                                                                     _______,             _______,             _______,               KC_PENT,             KC_PENT,
                                                                                          _______,             _______,               KC_DEL
  //                                                               ╰──────────────────────────────────────────────────────────────╯ ╰─────────────────────────────────────────╯
  ),

  [LAYER_KEYPAD] = LAYOUT(
  //╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
      M_PASS2,             KC_P1,               KC_P2,               KC_P3,               KC_P4,               KC_P5,                 KC_P6,               KC_P7,               KC_P8,               KC_P9,               KC_P0,               KC_PMNS,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      M_EMAIL,             _______,             _______,             _______,             _______,             _______,               _______,             KC_P7,               KC_P8,               KC_P9,               _______,             KC_PPLS,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      M_USER,              _______,             _______,             _______,             _______,             _______,               KC_PSLS,             KC_P4,               KC_P5,               KC_P6,               _______,             KC_PEQL,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      M_PASS,              _______,             _______,             _______,             _______,             _______,               KC_PDOT,             KC_P1,               KC_P2,               KC_P3,               KC_PSLS,             KC_PAST,
  //╰─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
                                                                     _______,             _______,             _______,               _______,             KC_PENT,
                                                                                          _______,             _______,               KC_P0
  //                                                               ╰──────────────────────────────────────────────────────────────╯ ╰─────────────────────────────────────────╯
  ),

  [LAYER_SYSTEM] = LAYOUT(
  //╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             QK_BOOT,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             RM_ON,               RM_OFF,              RM_PREV,             EE_CLR,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               _______,             _______,             _______,             _______,             _______,             _______,
  //├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
      _______,             _______,             _______,             _______,             _______,             _______,               RM_NEXT,             AUTOPTR,             _______,             _______,             _______,             _______,
  //╰─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
                                                                     _______,             _______,             _______,               _______,             _______,
                                                                                          _______,             _______,               _______
  //                                                               ╰──────────────────────────────────────────────────────────────╯ ╰─────────────────────────────────────────╯
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

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
static bool auto_pointer_on = false; // AUTOPTR flips this.  Off at boot.
#endif

/* Note: bk_pointing_device calls this before its own keycode handling, and
 * returning false stops both it and the rest of QMK from seeing the key. */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef POINTING_DEVICE_ENABLE
        case SMTSCRL:
            smart_scroll_set_active(record->event.pressed);
            return false;
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
        case DRGSCRL:
            /* Pause auto mouse while drag-scroll is held (K+L): drag-scroll
             * movement would turn the pointer layer on, and the layer change
             * cancels drag-scroll. */
            set_auto_mouse_enable(auto_pointer_on && !record->event.pressed);
            return true; // bk_pointing_device does the drag-scroll.
#    endif
#endif // POINTING_DEVICE_ENABLE
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
#    include "lib/lib8tion/lib8tion.h" // sqrt16() for the ripple.

static void fill_layer_color(uint8_t led_min, uint8_t led_max, uint8_t r, uint8_t g, uint8_t b) {
    const uint8_t val = rgb_matrix_get_val();
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, r * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, g * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS, b * val / RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
}

/* Pressed-key lighting for PROTOOLS.  Each key press picks a new color; the key
 * stays lit while held and fades out after release, and a ring of that color
 * ripples outward from it.  Each half spots presses by comparing its own key
 * matrix frame to frame, so this works on both sides of the split without
 * sending key events across. */
static bool     key_held[RGB_MATRIX_LED_COUNT];
static uint8_t  key_hue[RGB_MATRIX_LED_COUNT];
static uint16_t key_released_at[RGB_MATRIX_LED_COUNT];
static bool     key_fading[RGB_MATRIX_LED_COUNT];
static uint8_t  next_hue = 0;

typedef struct {
    led_point_t center; // Where the key is (g_led_config.point).
    uint8_t     hue;
    uint16_t    start;
    bool        active;
} ripple_t;
static ripple_t ripples[RIPPLE_COUNT];
static uint8_t  next_ripple = 0; // Oldest one is reused when all are busy.

static void start_ripple(uint8_t led, uint8_t hue) {
    ripples[next_ripple] = (ripple_t){g_led_config.point[led], hue, timer_read(), true};
    next_ripple          = (next_ripple + 1) % RIPPLE_COUNT;
}

/* Watches for presses and releases on every frame, on any layer, so the state
 * is current when PROTOOLS turns on. */
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
                start_ripple(led, key_hue[led]);
            } else if (!held && key_held[led]) {
                key_released_at[led] = timer_read();
                key_fading[led]      = true;
            }
            key_held[led] = held;
        }
    }
}

/* All LEDs off except held keys (full brightness), released keys (fading out
 * over PRESSED_FADE_MS) and ripples.  Where several overlap, the brightest
 * wins. */
static void light_pressed_keys(uint8_t led_min, uint8_t led_max) {
    /* Each ripple's ring radius this frame; it is done once past the edge. */
    uint8_t radius[RIPPLE_COUNT];
    for (uint8_t i = 0; i < RIPPLE_COUNT; i++) {
        uint32_t r = (uint32_t)timer_elapsed(ripples[i].start) * RIPPLE_SPEED / 1000;
        if (r >= RIPPLE_MAX_RADIUS) {
            ripples[i].active = false;
        }
        radius[i] = r < RIPPLE_MAX_RADIUS ? r : RIPPLE_MAX_RADIUS;
    }

    for (uint8_t led = led_min; led < led_max; led++) {
        uint8_t val = 0;
        uint8_t hue = key_hue[led];
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
        for (uint8_t i = 0; i < RIPPLE_COUNT; i++) {
            if (!ripples[i].active) {
                continue;
            }
            int16_t dx   = g_led_config.point[led].x - ripples[i].center.x;
            int16_t dy   = g_led_config.point[led].y - ripples[i].center.y;
            int16_t dist = sqrt16(dx * dx + dy * dy);
            int16_t off  = abs(dist - radius[i]); // How far this LED is from the ring.
            if (off >= RIPPLE_WIDTH) {
                continue;
            }
            /* Brightest on the ring, dimmer to the sides, and the whole ring
             * fades as it grows. */
            uint8_t v = (uint32_t)PRESSED_BRIGHTNESS * (RIPPLE_WIDTH - off) / RIPPLE_WIDTH * (RIPPLE_MAX_RADIUS - radius[i]) / RIPPLE_MAX_RADIUS;
            if (v > val) {
                val = v;
                hue = ripples[i].hue;
            }
        }
        rgb_t rgb = hsv_to_rgb((hsv_t){hue, 255, val});
        rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
    }
}

/* Each layer lights every LED in one color (LAYER_COLOR_* in config.h),
 * scaled by the current RGB brightness.  PROTOOLS goes dark and lights only the
 * keys being pressed (see above).  Base and pointer show the normal RGB
 * effect. */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    track_pressed_keys(led_min, led_max);
    switch (get_highest_layer(layer_state)) {
        case LAYER_PROTOOLS:
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

/* Mod-taps (Shift/Esc, the Cmd and Hyper spaces, the home-row A/S/D/F, and any new ones):
 * Speculative Hold (config.h).  The modifier is on from the moment the key
 * goes down, and is cancelled if the key turns out to be a tap.  QMK's
 * default only covers Shift and Ctrl; this covers every modifier. */
bool get_speculative_hold(uint16_t keycode, keyrecord_t *record) {
    return true;
}

/* Layer-taps (KEYPAD, MISSING, PT_Z, and any new ones): Flow Tap
 * (FLOW_TAP_TERM in config.h).  Pressed soon after a typing key (letter,
 * space, punctuation; QMK's is_flow_tap_key), it is a tap right away.  Off
 * while Ctrl, Cmd or Alt is held.  Never for mod-taps: Flow Tap would block
 * their modifier right after typing (Cmd+F), and they have Speculative Hold. */
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    switch (keycode) {
        /* Home-row mods: Flow Tap while typing on base, never in Pro Tools mode. */
        case LCTL_T(KC_A):
        case LSFT_T(KC_S):
        case LALT_T(KC_D):
        case LGUI_T(KC_F):
            if (layer_state_is(LAYER_PROTOOLS)) {
                return 0;
            }
            break;
        default:
            if (IS_QK_MOD_TAP(keycode)) {
                return 0;
            }
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
        /* Home-row mods: instant in Pro Tools mode (PROTOOLS), so mod + key or
         * click goes out at once.  Off on base so typing rolls stay letters. */
        case LCTL_T(KC_A):
        case LSFT_T(KC_S):
        case LALT_T(KC_D):
        case LGUI_T(KC_F):
            return layer_state_is(LAYER_PROTOOLS);
        /* Hyper includes Shift, so the rule below would catch it.  Space is
         * typed constantly: only Hyper after TAPPING_TERM (or PERMISSIVE_HOLD),
         * so fast rolls stay spaces. */
        case HYPR_T(KC_SPC):
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
