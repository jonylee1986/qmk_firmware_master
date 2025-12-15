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

#ifdef BT_MODE_ENABLE
#    include "common/bt_task.h"
#endif

enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_B] = LAYOUT_ansi_81( /* Base */
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,                    KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,           KC_PGUP,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LWIN,  KC_LALT,                              KC_SPC,                             KC_RALT, MO(WIN_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_81( /* FN */
        _______, KC_MYCM,  KC_MAIL,  KC_WSCH,  KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,    KC_VOLU,                   _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G, _______, _______, _______, _______, _______, _______, _______,    _______, BT_VOL,           _______,
        _______, _______,  KC_W2UP,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    RM_HUEU, RM_NEXT,          _______,
        _______, SW_OS1,   _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             RM_TOGG,          _______,
        _______,           _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, RM_VALU, _______,
        _______, GU_TOGG,  _______,                              EE_CLR,                             _______, _______,    _______, RM_SPDD, RM_VALD, RM_SPDU),

    [MAC_B] = LAYOUT_ansi_81( /* Base */
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,                    KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,           KC_PGUP,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LOPT,  KC_LCMD,                              KC_SPC,                             KC_RCMD, MO(WIN_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_81( /* FN */
        _______, KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD, KC_NO,   KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,    KC_VOLU,                   _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G, _______, _______, _______, _______, _______, _______, _______,    _______, BT_VOL,           _______,
        _______, _______,  KC_W2UP,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    RM_HUEU, RM_NEXT,          _______,
        _______, _______,  SW_OS1,   _______,  _______, _______, _______, _______, _______, _______, _______, _______,             RM_TOGG,          _______,
        _______,           _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, RM_VALU, _______,
        _______, _______,  _______,                              EE_CLR,                             _______, _______,    _______, RM_SPDD, RM_VALD, RM_SPDU),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_B] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = {ENCODER_CCW_CW(_______, _______) },
    [MAC_B] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = {ENCODER_CCW_CW(_______, _______) },
};
#endif // ENCODER_MAP_ENABLE

// clang-format on

static uint8_t  single_blink_cnt   = 0;
static uint8_t  single_blink_index = 0;
static RGB      single_blink_color = {0};
static uint32_t single_blink_time  = 0;
static uint8_t  all_blink_cnt      = 0;
static RGB      all_blink_color    = {0};
static uint32_t all_blink_time     = 0;

static uint16_t pressed_code_user;
static uint16_t pressed_time_user = 0;
static bool     W2UP_flag         = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_W2UP:
            if (record->event.pressed) {
                pressed_code_user = KC_W2UP;
                pressed_time_user = timer_read();
            } else {
                pressed_time_user = 0;
            }
            break;
        case KC_W:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_UP, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_UP);
                    } else {
                        unregister_code(KC_UP);
                    }
                }
                return false;
            }
            break;
        case KC_A:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_LEFT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_LEFT);
                    } else {
                        unregister_code(KC_LEFT);
                    }
                }
                return false;
            }
            break;
        case KC_S:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_DOWN, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_DOWN);
                    } else {
                        unregister_code(KC_DOWN);
                    }
                }
                return false;
            }
            break;
        case KC_D:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_RGHT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_RGHT);
                    } else {
                        unregister_code(KC_RGHT);
                    }
                }
                return false;
            }
            break;
        case KC_UP:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_W, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_W);
                    } else {
                        unregister_code(KC_W);
                    }
                }
                return false;
            }
            break;
        case KC_LEFT:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_A, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_A);
                    } else {
                        unregister_code(KC_A);
                    }
                }
                return false;
            }
            break;
        case KC_DOWN:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_S, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_S);
                    } else {
                        unregister_code(KC_S);
                    }
                }
                return false;
            }
            break;
        case KC_RGHT:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_D, record->event.pressed, dev_info.devs, keymap_config.no_gui, KC_NUM);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_D);
                    } else {
                        unregister_code(KC_D);
                    }
                }
                return false;
            }
            break;

        case RM_VALU:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){100, 100, 100};
                    if (timer_elapsed32(all_blink_time) >= 300) {
                        all_blink_time = timer_read32();
                    }
                }
            }
            break;
        case RM_VALD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){100, 100, 100};
                    if (timer_elapsed32(all_blink_time) >= 300) {
                        all_blink_time = timer_read32();
                    }
                }
            }
            break;
        case RM_SPDU:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){100, 100, 100};
                    if (timer_elapsed32(all_blink_time) >= 300) {
                        all_blink_time = timer_read32();
                    }
                }
            }
            break;
        case RM_SPDD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){100, 100, 100};
                    if (timer_elapsed32(all_blink_time) >= 300) {
                        all_blink_time = timer_read32();
                    }
                }
            }
            break;
        default:
            break;
    }

    return true;
}

void housekeeping_task_user(void) {
    switch (pressed_code_user) {
        case KC_W2UP:
            if ((timer_elapsed32(pressed_time_user) > 3000) && (pressed_time_user)) {
                W2UP_flag         = !W2UP_flag;
                pressed_time_user = 0;
            }
        default:
            break;
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (all_blink_cnt) {
        rgb_matrix_set_color_all(0, 0, 0);
        if (timer_elapsed32(all_blink_time) > 300) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt & 0x1) {
            rgb_matrix_set_color_all(all_blink_color.r, all_blink_color.g, all_blink_color.b);
        } else {
            rgb_matrix_set_color_all(RGB_OFF);
        }
    }

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
