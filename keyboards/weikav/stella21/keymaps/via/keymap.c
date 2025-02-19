// Copyright 2024 Wind (@yelishang)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_B] = LAYOUT_ansi(
        KC_ESC,  KC_TAB, KC_BSPC, MO(WIN_FN),
        KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_P4,   KC_P5,   KC_P6,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,
                 KC_P0,   KC_PDOT
    ),

    [WIN_FN] = LAYOUT_ansi(
        EE_CLR,  RGB_MOD, RGB_HUI, _______,
        KC_USB,  _______, _______, _______,
        KC_BT1,  KC_BT2,  KC_2G4,  _______,
        RGB_SPI, RGB_SPD, _______,
        RGB_VAI, RGB_VAD, _______, RGB_TOG,
                 _______, _______
    ),

    [MAC_B] = LAYOUT_ansi(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
                 _______, _______
    ),

    [MAC_FN] = LAYOUT_ansi(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
                 _______, _______
    )
};
// clang-format on
