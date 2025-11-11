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
#include "common/bt_task.h"

// clang-format off

enum layers {
    WIN_B,
    WIN_F,
    WIN_F2,
    MAC_B,
    MAC_F,
    MAC_F2,
};

#define BT_1 BT_HOST1
#define BT_2 BT_HOST2
#define BT_3 BT_HOST3
#define WL_2_4 BT_2_4G
#define WL_TEST WL_2_4G

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

key_combination_t key_comb_list[4] = {
    {3, {KC_LCTL, KC_LSFT, KC_ESC}},
    {2, {KC_LWIN, KC_K}},
    {2, {KC_LCMD, KC_SPC}},
    {3, {KC_LCMD, KC_LSFT, KC_4}},
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_83_ansi( /* Base */
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_INS,  KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
        KC_LCTL, KC_LWIN, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_F] = LAYOUT_83_ansi( /* FN */
        EE_CLR,  KC_BRID, KC_BRIU, KC_CALC, KC_MYCM, KC_TASK, KC_PRJT, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,          _______, _______,
        _______, BT_1,    BT_2,    BT_3,    WL_2_4,  _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,           RM_HUEU,
        RM_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, RM_NEXT,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, WL_TEST, _______, _______, KC_INS,           LCD_ENTER,        _______,
        _______,          _______, _______, _______, _______, _______, _______, SW_OS1, LCD_LEFT,LCD_RIGHT,_______, _______,          RM_VALU, _______,
        _______, GU_TOGG, _______,                            _______,                            MO(2),   _______, _______, RM_SPDD, RM_VALD, RM_SPDU),

    [WIN_F2] = LAYOUT_83_ansi( /* FN */
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_INS,  KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          SLED_HUI,
        SLED_TOG,KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, SLED_MOD,         KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          SLED_VAI,KC_PGDN,
        KC_LCTL, KC_LWIN, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, SLED_SPD,SLED_VAD,SLED_SPI),

    [MAC_B] = LAYOUT_83_ansi( /* Base */
        KC_ESC,  KC_BRID, KC_BRIU, KC_MCTL, KC_SEAR, KC_SIRI, KC_SNAP, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,          KC_INS,  KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
        KC_LCTL, KC_LOPT, KC_LCMD,                            KC_SPC,                             KC_RCMD, MO(4),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_F] = LAYOUT_83_ansi( /* FN */
        EE_CLR,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           _______, _______,
        _______, BT_1,    BT_2,    BT_3,    WL_2_4,  _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,           RM_HUEU,
        RM_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_NEXT,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, WL_TEST, _______, _______, _______,          LCD_ENTER,        _______,
        _______,          _______, _______, _______, _______, _______, _______, SW_OS1, LCD_LEFT,LCD_RIGHT,_______, _______,          RM_VALU, _______,
        _______, _______, _______,                            _______,                            MO(5),   _______, _______, RM_SPDD, RM_VALD, RM_SPDU),

    [MAC_F2] = LAYOUT_83_ansi( /* FN */
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_INS,  KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          SLED_HUI,
        SLED_TOG,KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, SLED_MOD,         KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          SLED_VAI,KC_PGDN,
        KC_LCTL, KC_LOPT, KC_LCMD,                            KC_SPC,                             KC_RCMD, MO(4),   KC_RCTL, SLED_SPD,SLED_VAD,SLED_SPI),

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_B]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_F]  = {ENCODER_CCW_CW(_______, _______)},
    [WIN_F2] = {ENCODER_CCW_CW(_______, _______)},
    [MAC_B]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_F]  = {ENCODER_CCW_CW(_______, _______)},
    [MAC_F2] = {ENCODER_CCW_CW(_______, _______)},
};
#endif // ENCODER_MAP_ENABLE

#if defined(DIP_SWITCH_MAP_ENABLE)
const uint16_t PROGMEM dip_switch_map[NUM_DIP_SWITCHES][NUM_DIP_STATES] = {
    DIP_SWITCH_OFF_ON(KC_MUTE, _______)
};
#endif

bool     is_siri_active = false;
uint32_t siri_timer     = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
         case KC_SIRI:
            if (record->event.pressed) {
                if (!is_siri_active) {
                    is_siri_active = true;
                    register_code(KC_LCMD);
                    register_code(KC_SPACE);
                }
                siri_timer = timer_read32();
            }
            return false; // Skip all further processing of this key

        case KC_TASK:
        case KC_PRJT:
        case KC_SEAR:
        case KC_SNAP:
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    register_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    unregister_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            }
            return false; // Skip all further processing of this key

        default:
            break;
    }

    return true;
}

void housekeeping_task_user(void) {
    if (is_siri_active) {
        if (timer_elapsed32(siri_timer) >= 500) {
            unregister_code(KC_LCMD);
            unregister_code(KC_SPACE);
            is_siri_active = false;
        }
    }
}
