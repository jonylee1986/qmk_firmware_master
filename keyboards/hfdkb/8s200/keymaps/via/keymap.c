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
#include "keymap_japanese.h"

// clang-format off

enum user_keycodes {
    SW_OS1 = QK_KB_0,
    WIN_LOCK,
    KC_MICT,
    KC_LAPA,
};

#define KC_PRS LCMD(LSFT(KC_4))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT_72_jis( /* Base */
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  JP_YEN,  KC_BSPC, KC_DEL,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,                   KC_PGUP,
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,           KC_PGDN,
		KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, JP_BSLS, KC_RSFT, KC_UP,   KC_END,
		KC_LCTL, KC_LGUI, KC_LALT, JP_MHEN,                   KC_SPC,                    JP_HENK, JP_KANA, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [1] = LAYOUT_72_jis( /* FN */
		KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,
		_______, KC_MPRV, KC_MPLY, KC_MNXT, EE_CLR,  _______, KC_CALC, _______, KC_UP,   _______, KC_PSCR, KC_SCRL, KC_PAUS,                   _______,
		RGB_TOG, RGB_HUI, KC_VOLU, KC_VOLD, KC_MUTE, _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_INS,  _______, _______, _______,          _______,
		_______,          KC_APP,  _______, _______, _______, _______, NK_TOGG, RGB_MOD, KC_NO,   SW_OS1,  _______, _______, _______, RGB_VAI, _______,
		_______, WIN_LOCK,_______, _______,                   _______,                   _______, MO(4),   _______, _______, RGB_SPD, RGB_VAD, RGB_SPI),

    [2] = LAYOUT_72_jis( /* Base */
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  JP_YEN,  KC_BSPC, KC_DEL,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,                   KC_PGUP,
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,           KC_PGDN,
		KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, JP_BSLS, KC_RSFT, KC_UP,   KC_END,
		KC_LCTL, KC_LOPT, KC_LCMD, KC_LNG2,                   KC_SPC,                    KC_LNG1, KC_RCMD, MO(3),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [3] = LAYOUT_72_jis( /* FN */
		KC_GRV,  KC_BRID, KC_BRIU, KC_MICT, KC_LAPA, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______,
		_______, KC_MPRV, KC_MPLY, KC_MNXT, EE_CLR,  _______, KC_CALC, _______, KC_UP,   _______, KC_PRS,  KC_SCRL, KC_PAUS,                   _______,
		_______, RGB_HUI, KC_VOLU, KC_VOLD, KC_MUTE, _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_INS,  _______, _______, _______,          _______,
		_______,          KC_APP,  _______, _______, _______, _______, NK_TOGG, RGB_MOD, SW_OS1,  KC_NO,   _______, _______, _______, RGB_VAI, _______,
		_______, _______, _______, _______,                   _______,                   _______, MO(4),   _______, _______, RGB_SPD, RGB_VAD, RGB_SPI),

    [4] = LAYOUT_72_jis( /* FN */
		_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
		RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
		_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______,                   _______,                   _______, _______, _______, _______, _______, _______, _______),

    [5] = LAYOUT_72_jis( /* FN */
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
		_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______,                   _______,                   _______, _______, _______, _______, _______, _______, _______),
};
// clang-format on

static uint32_t long_pressed_time    = 0;
static uint16_t long_pressed_keycode = 0;
static uint8_t  all_blink_cnt        = 0;
static uint32_t all_blink_time       = 0;
static RGB      all_blink_color      = {0};
static uint8_t  single_blink_cnt     = 0;
static uint8_t  single_blink_index   = 0;
static RGB      single_blink_color   = {0};
static uint32_t single_blink_time    = 0;
static uint8_t  EE_CLR_blink_cnt     = 0;
static RGB      EE_CLR_blink_color   = {0};
static uint32_t EE_CLR_blink_time    = 0;

// clang-format off
static uint8_t rgb_matrix_effects_table[] = {
    RGB_MATRIX_BREATHING,
    RGB_MATRIX_SOLID_COLOR,
    RGB_MATRIX_CYCLE_LEFT_RIGHT,
    RGB_MATRIX_RAINBOW_MOVING_CHEVRON,
    RGB_MATRIX_CYCLE_OUT_IN_DUAL,
    RGB_MATRIX_DUAL_BEACON,
    RGB_MATRIX_RAINBOW_BEACON,
    RGB_MATRIX_RAINDROPS,
    RGB_MATRIX_SOLID_REACTIVE,
    RGB_MATRIX_CUSTOM_EFFECT_OFF,
};
// clang-format on

typedef union {
    uint32_t raw;
    struct {
        uint8_t rgb_mode;
    };
} keyboard_config_t;
keyboard_config_t keyboard_config;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SW_OS1: {
            if (record->event.pressed) {
                // Do something when SW_OS1 is pressed
                long_pressed_time    = timer_read32();
                long_pressed_keycode = SW_OS1;
            } else {
                long_pressed_time = 0;
            }
        }
            return false;

        case EE_CLR: {
            if (record->event.pressed) {
                long_pressed_time    = timer_read32();
                long_pressed_keycode = EE_CLR;
            } else {
                long_pressed_time = 0;
            }
        }
            return false;

        case RGB_SPD: {
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    rgb_matrix_config.speed = 27;
                } else {
                    rgb_matrix_config.speed -= RGB_MATRIX_SPD_STEP;
                }
            }
        }
            return false;

        case RGB_MOD: {
            if (record->event.pressed) {
                keyboard_config.rgb_mode++;
                if (keyboard_config.rgb_mode > (sizeof(rgb_matrix_effects_table) / sizeof(rgb_matrix_effects_table[0]) - 1)) {
                    keyboard_config.rgb_mode = 0;
                }
                eeconfig_update_user(keyboard_config.raw);
                rgb_matrix_mode(rgb_matrix_effects_table[keyboard_config.rgb_mode]);
            }
        }
            return false;

        case NK_TOGG: {
            if (record->event.pressed) {
                single_blink_time  = timer_read32();
                single_blink_cnt   = 2;
                single_blink_index = 73;
                single_blink_color = (RGB){100, 100, 100};
            }
        } break;

        case WIN_LOCK: {
            if (record->event.pressed) {
                keymap_config.no_gui = !keymap_config.no_gui;
            }
        } break;
        case KC_MICT: {
            if (record->event.pressed) {
                tap_code16(KC_MCTL);
            }
        } break;
        case KC_LAPA: {
            if (record->event.pressed) {
                tap_code16(KC_LPAD);
            }
        } break;

        default:
            break;
    }

    return true;
}

void housekeeping_task_user(void) {
    if ((timer_elapsed32(long_pressed_time) > 2000) && (long_pressed_time)) { // 检查是否有按键长按
        long_pressed_time = 0;
        switch (long_pressed_keycode) {
            case SW_OS1: {
                if (get_highest_layer(default_layer_state) == 0) { // WIN_BASE
                    set_single_persistent_default_layer(2);
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config); // Write default value to EEPROM now
                    single_blink_time  = timer_read32();
                    single_blink_cnt   = 2;
                    single_blink_index = 73;
                    single_blink_color = (RGB){100, 100, 100};
                } else if (get_highest_layer(default_layer_state) == 2) { // Mac_BASE
                    set_single_persistent_default_layer(0);
                    single_blink_time  = timer_read32();
                    single_blink_cnt   = 2;
                    single_blink_index = 74;
                    single_blink_color = (RGB){.r = 0, .g = 0, .b = 100};
                }
            } break;

            case EE_CLR: {
                EE_CLR_blink_time  = timer_read32();
                EE_CLR_blink_cnt   = 6;
                EE_CLR_blink_color = (RGB){.r = 100, .g = 100, .b = 100};
            } break;

            default:
                break;
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (all_blink_cnt) { // 全键闪烁
        rgb_matrix_set_color_all(0, 0, 0);
        if (timer_elapsed32(all_blink_time) > 300) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt & 0x1) {
            rgb_matrix_set_color_all(all_blink_color.r, all_blink_color.g, all_blink_color.b);
        }
    }

    if (single_blink_cnt) { // 单键闪烁
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

    if (EE_CLR_blink_cnt) { // 复位闪烁
        if (EE_CLR_blink_cnt % 2) {
            rgb_matrix_set_color(73, EE_CLR_blink_color.r, EE_CLR_blink_color.g, EE_CLR_blink_color.b);
            rgb_matrix_set_color(74, EE_CLR_blink_color.r, EE_CLR_blink_color.g, EE_CLR_blink_color.b);
        } else {
            rgb_matrix_set_color(73, 0, 0, 0);
            rgb_matrix_set_color(74, 0, 0, 0);
        }
        if (timer_elapsed32(EE_CLR_blink_time) > 300) {
            if (EE_CLR_blink_cnt > 0) {
                EE_CLR_blink_time = timer_read32();
                EE_CLR_blink_cnt--;
            }
            if (!EE_CLR_blink_cnt) {
                eeconfig_init();
                eeconfig_update_rgb_matrix_default();
                keyboard_config.rgb_mode = 0;
                eeconfig_update_user(keyboard_config.raw); // Write default value to EEPROM now
                rgb_matrix_mode(rgb_matrix_effects_table[keyboard_config.rgb_mode]);
                keymap_config.no_gui = false;
                keymap_config.nkro   = false;
                // eeconfig_update_keymap(&keymap_config);
            }
        }
    }

    return true;
}

void keyboard_post_init_user(void) {
    keyboard_config.raw = eeconfig_read_user();
    rgb_matrix_mode(rgb_matrix_effects_table[keyboard_config.rgb_mode]);
}

void eeconfig_init_user(void) {
    keyboard_config.raw      = 0;
    keyboard_config.rgb_mode = 0;              // Default to first effect
    eeconfig_update_user(keyboard_config.raw); // Write default value to EEPROM now
}
