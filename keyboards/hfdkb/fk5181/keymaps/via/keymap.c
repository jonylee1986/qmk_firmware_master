// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "fk5181/fk5181.h"
#include "keycodes.h"
#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_B] = LAYOUT_ansi(
        KC_MUTE, KC_ENT,                                                                                                                       BT_SW,
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_PAUS, KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT,MO(WIN_FN),KC_RCTL,KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [WIN_FN] = LAYOUT_ansi(
        _______, _______,                                                                                                                      _______,
        EE_CLR,  KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, KC_MPRV, KC_MNXT, BRGB_TOG, KC_CALC, KC_MAIL, KC_WHOM, KC_MSEL, KC_LOCK, _______, _______, KC_INS,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, BT_VOL,           _______,
        RGB_TEST, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_MOD,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_HUI,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_VAI, _______,
        _______, GU_TOGG, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_VAD, RGB_SPI
    ),

    [MAC_B] = LAYOUT_ansi(
        KC_MUTE, KC_ENT,                                                                                                                       BT_SW,
        KC_ESC,  KC_BRID, KC_BRIU, C(KC_UP),KC_LPAD, KC_F5,   KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, KC_PAUS, KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                             KC_RGUI, MO(MAC_FN),KC_RALT,KC_LEFT,KC_DOWN, KC_RGHT
    ),

    [MAC_FN] = LAYOUT_ansi(
        _______, _______,                                                                                                                      _______,
        EE_CLR,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   RGB_VAD, RGB_VAI, BRGB_TOG,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, BT_VOL,           _______,
        RGB_TEST, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_MOD,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_HUI,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_VAI, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_VAD, RGB_SPI
    )
};
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [WIN_B] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U)},
    [WIN_FN] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U)},
    [MAC_B] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U)},
    [MAC_FN] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U)}
};
#endif

// clang-format on

bool ALL_KEY_LOCK = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (ALL_KEY_LOCK) {
        switch (keycode) {
            case MO(WIN_FN):
            case EE_CLR:
            case KC_LOCK:
                break;
            default:
                return false;
        }
    }

    switch (keycode) {
        case KC_LOCK: {
            if (record->event.pressed) {
                ALL_KEY_LOCK = !ALL_KEY_LOCK;
            }
            return false;
        }
        default:
            return true;
    }
}

void housekeeping_task_user(void) {}
