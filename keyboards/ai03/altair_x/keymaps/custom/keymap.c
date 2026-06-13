/* Copyright 2024 ai03 Design Studio */
/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

#define LAYER_DEFAULT 0
#define LAYER_LOWER 1
#define LAYER_RAISE 2
#define LAYER_MOUSE 3

enum custom_keycodes {
    CK_LOWER = SAFE_RANGE,
    CK_RAISE,
    CK_MOUSE,
    CK_SFESC = RSFT_T(KC_ESC)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [LAYER_DEFAULT] = LAYOUT(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_VOLU, CK_MOUSE, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_VOLD,  KC_MINS, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MPLY, XXXXXXX,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, CK_SFESC,
                                   KC_LALT, KC_LGUI, CK_LOWER, KC_ENT,  KC_SPC, CK_RAISE,  KC_RALT,  KC_RGUI
    ),

    [LAYER_LOWER] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______, _______, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        _______, _______, KC_PGDN, KC_UP,   KC_PGUP, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT, KC_MINS, _______,
        _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT,_______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______, _______,
                                   _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [LAYER_RAISE] = LAYOUT(
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
        _______, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD, KC_MINS, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,
                                   _______, _______, _______, KC_SPC,  _______, _______, _______, _______
    ),

    [LAYER_MOUSE] = LAYOUT(
        XXXXXXX, MS_BTN2, MS_WHLD, MS_UP,   MS_WHLU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, MS_BTN1, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   KC_LALT, KC_LGUI, CK_LOWER, KC_ENT,  KC_SPC, CK_RAISE,  KC_RALT,  KC_RGUI
    )
};

static bool lower_pressed = false;
static bool raise_pressed = false;

void type_code(uint8_t keycode) {
    register_code(keycode);
    unregister_code(keycode);
}

void set_eisu(void){
  type_code(KC_LNG2);
};

void set_kana(void){
  type_code(KC_LNG1);
};

void mod_tap_action(bool *flag, uint8_t layer, keyrecord_t *record, void (*cb)(void)) {
    if (record->event.pressed) {
        *flag = true;
        layer_on(layer);
        update_tri_layer(LAYER_LOWER, LAYER_RAISE, LAYER_MOUSE);

        if (layer_state_is(LAYER_MOUSE)) {
            lower_pressed = false;
            raise_pressed = false;
        }
    } else {
        layer_off(layer);
        update_tri_layer(LAYER_LOWER, LAYER_RAISE, LAYER_MOUSE);

        if (*flag) {
            cb();
        }
        *flag = false;
    }
}

void mouse_key_action(bool *flag, keyrecord_t *record) {
    if (record->event.pressed) {
        *flag = true;
        layer_on(LAYER_MOUSE);
    } else {
        layer_off(LAYER_MOUSE);
        *flag = false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CK_LOWER:
            mod_tap_action(&lower_pressed, LAYER_LOWER, record, set_eisu);
            return false;
            break;

        case CK_RAISE:
            mod_tap_action(&raise_pressed, LAYER_RAISE, record, set_kana);
            return false;
            break;

        case CK_MOUSE:
            mouse_key_action(&lower_pressed, record);
            return false;
            break;

        default:
            if (record->event.pressed) {
                lower_pressed = false;
                raise_pressed = false;
            }
    }

    return true;
}
