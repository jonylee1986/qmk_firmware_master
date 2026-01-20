/* Copyright (C) 2023 jonylee@hfd
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

enum __layers {
    WIN_B,
    WIN_FN,
    WIN_EX,
    MAC_B,
    MAC_FN,
    MAC_EX,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_B] = LAYOUT_ansi_67( /* Base */
        KC_ESC,  KC_1,     KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS, KC_HOME,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,  KC_PGUP,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LWIN,  KC_LALT,                              KC_SPC,                             KC_RALT, MO(WIN_FN), KC_LEFT, KC_DOWN, KC_RGHT ),

    [WIN_FN] = LAYOUT_ansi_67( /* FN */
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        BT_USB,  RM_TOGG,  RM_PREV,  RM_NEXT,  RM_HUEU, RM_HUED, RM_SATU, RM_SATD, RM_VALU, RM_VALD, RM_SPDU, RM_SPDD,    _______, _______, _______,
        _______, _______,  SW_OS1,   _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,
        _______,           KC_MSEL,  KC_VOLD,  KC_VOLU, KC_MUTE, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        KEY_FUN, GU_TOGG,  _______,                              EE_CLR,                             _______, _______,    _______, _______, _______),

    [WIN_EX] = LAYOUT_ansi_67( /* FN */
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,
        _______,           _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______,  _______,                              _______,                            _______, _______,    _______, _______, _______),

    [MAC_B] = LAYOUT_ansi_67( /* Base */
        KC_ESC,  KC_1,     KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS, KC_HOME,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,  KC_PGUP,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LOPT,  KC_LCMD,                              KC_SPC,                             KC_RCMD, MO(MAC_FN), KC_LEFT, KC_DOWN, KC_RGHT ),

    [MAC_FN] = LAYOUT_ansi_67( /* FN */
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        BT_USB,  RM_TOGG,  RM_PREV,  RM_NEXT,  RM_HUEU, RM_HUED, RM_SATU, RM_SATD, RM_VALU, RM_VALD, RM_SPDU, RM_SPDD,    _______, _______, _______,
        _______, SW_OS1,   _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,
        _______,           KC_MSEL,  KC_VOLD,  KC_VOLU, KC_MUTE, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        KEY_FUN, _______,  _______,                              EE_CLR,                             _______, _______,    _______, _______, _______),

    [MAC_EX] = LAYOUT_ansi_67( /* FN */
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,
        _______,           _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______,  _______,                              _______,                            _______, _______,    _______, _______, _______),
};

// clang-format on
uint16_t FUN_table[][12] = {
    {KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL},
    {KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12},
};

static uint8_t  single_blink_cnt   = 0;
static uint8_t  single_blink_index = 0;
static RGB      single_blink_color = {0};
static uint32_t single_blink_time  = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KEY_FUN: {
            if (record->event.pressed) {
                if (get_highest_layer(default_layer_state) == WIN_B) {
                    if (dynamic_keymap_get_keycode(WIN_B, 0, 1) == KC_1) {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(WIN_B, 0, i + 1, FUN_table[1][i]);
                        }
                        single_blink_color = (RGB){100, 100, 100};
                    } else if (dynamic_keymap_get_keycode(WIN_B, 0, 1) == KC_F1) {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(WIN_B, 0, i + 1, FUN_table[0][i]);
                        }
                        single_blink_color = (RGB){0, 0, 100};
                    }
                    single_blink_cnt   = 6;
                    single_blink_index = 66;
                    if (timer_elapsed32(single_blink_time) >= 300) {
                        single_blink_time = timer_read32();
                    }
                } else if (get_highest_layer(default_layer_state) == MAC_B) {
                    if (dynamic_keymap_get_keycode(MAC_B, 0, 1) == KC_1) {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(MAC_B, 0, i + 1, FUN_table[1][i]);
                        }
                        single_blink_color = (RGB){100, 100, 100};
                    } else if (dynamic_keymap_get_keycode(MAC_B, 0, 1) == KC_F1) {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(MAC_B, 0, i + 1, FUN_table[0][i]);
                        }
                        if (timer_elapsed32(single_blink_time) >= 300) {
                            single_blink_time = timer_read32();
                        }
                        single_blink_color = (RGB){0, 0, 100};
                    }
                    single_blink_cnt   = 6;
                    single_blink_index = 66;
                    if (timer_elapsed32(single_blink_time) >= 300) {
                        single_blink_time = timer_read32();
                    }
                }
            }
            return false;
        } break;
    }

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (single_blink_cnt) {
        if (timer_elapsed32(single_blink_time) > 300) {
            single_blink_time = timer_read32();
            single_blink_cnt--;
        }
        if (single_blink_cnt % 2) {
            rgb_matrix_set_color(single_blink_index, single_blink_color.r, single_blink_color.g, single_blink_color.b);
        } else {
            rgb_matrix_set_color(single_blink_index, 0, 0, 0);
        }
    }

    return true;
}
