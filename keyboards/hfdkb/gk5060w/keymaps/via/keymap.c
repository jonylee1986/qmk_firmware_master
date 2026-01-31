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
    MAC_B,
    MAC_FN,
    MAC_EX,
    WIN_B,
    WIN_FN,
    WIN_EX,
};

#define KC_DITN KC_DICTATION
#define KC_DNDB KC_DO_NOT_DISTURB
#define KC_SPOT KC_SPOTLIGHT

#define KC_TASK G(KC_TAB)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [MAC_B] = LAYOUT_ansi_64( /* Base */
        QK_GESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP,   KC_RSFT,
        MO(MAC_FN), KC_LCTL, KC_LOPT, KC_LCMD,                   KC_SPC,                    KC_RCMD, KC_ROPT, KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_64( /* FN */
        KC_GRV,     KC_BRID, KC_BRIU, KC_MCTL, C(KC_SPC), KC_DITN, KC_DNDB, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,
        _______,    BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,             _______, _______, _______, _______, _______, SW_WIN,  SW_MAC, _______, _______, _______, KC_PGUP, _______,
        _______,    _______, MO(MAC_EX), _______,                MO(MAC_EX),                _______, MO(MAC_EX), KC_HOME, KC_PGDN, KC_END),

    [MAC_EX] = LAYOUT_ansi_64( /* FN */
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, EE_CLR,
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,             _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,    _______, _______, _______,                   _______,                   _______, _______, _______, _______,  _______),

    [WIN_B] = LAYOUT_ansi_64( /* Base */
        QK_GESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP,   KC_RSFT,
        MO(WIN_FN), KC_LCTL, KC_LWIN, KC_LALT,                   KC_SPC,                    KC_RALT, KC_RWIN, KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_64( /* FN */
        KC_GRV,     KC_BRID, KC_BRIU, KC_TASK, S(KC_LEFT_CTRL), _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,
        _______,    BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,             _______, _______, _______, _______, _______, SW_WIN,  SW_MAC,  _______, _______, _______, KC_PGUP, _______,
        _______,    _______, MO(WIN_EX), _______,                MO(WIN_EX),                _______, MO(WIN_EX), KC_HOME, KC_PGDN, KC_END),

    [WIN_EX] = LAYOUT_ansi_64( /* FN */
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, EE_CLR,
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,             _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,    _______, _______, _______,                   _______,                   _______, _______, _______, _______,  _______),
};

// clang-format on

uint32_t CTRL_pressed_time = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case C(KC_SPC): {
            if (!record->event.pressed) {
                unregister_code(KC_SPC);
                CTRL_pressed_time = timer_read32();
            } else {
                register_code(KC_LCTL);
                register_code(KC_SPC);
            }
            return false;
        } break;

        default:
            break;
    }

    return true;
}

void housekeeping_task_user(void) {
    if (CTRL_pressed_time && (timer_elapsed32(CTRL_pressed_time) >= 500)) {
        CTRL_pressed_time = 0;
        unregister_code(KC_LCTL);
    }
}
