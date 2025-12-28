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
// clang-format off
#include "common/bt_task.h"
enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define KC_SIRI LGUI(KC_SPC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_ansi( /* Base */
		         KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,
		KC_ESC,  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
		KC_PGUP, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
		KC_PGDN, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
		MO(1),   KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC),

    [WIN_FN] = LAYOUT_ansi( /* FN */
		         _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
		EE_CLR,  BT_VOL,  SW_OS1,  _______, _______, _______, _______,
		_______, CHG_TOG, _______, _______, _______, _______, _______,
		_______, _______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, BT_USB,
		_______, WL_2_4G, GU_TOGG, _______,                   _______),

    [MAC_B] = LAYOUT_ansi( /* Base */
		         KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,
		KC_ESC,  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
		KC_PGUP, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
		KC_PGDN, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
		MO(3),   KC_LCTL, KC_LALT, KC_LGUI,                   KC_SPC),

    [MAC_FN] = LAYOUT_ansi( /* FN */
		         _______, KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, _______, _______,
		EE_CLR,  BT_VOL,  SW_OS1,  _______, _______, _______, _______,
		_______, CHG_TOG, _______, _______, _______, _______, _______,
		_______, _______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, BT_USB,
		_______, WL_2_4G, _______, _______,                   _______),
};

// clang-format on
uint16_t pressed_code_user;
uint16_t pressed_time_user = 0;
bool     W2UP_flag;
uint8_t  VAL_OUT_LEDINDEX;
uint8_t  VAL_OUT_blink_cnt;
uint32_t VAL_OUT_blink_time;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case G(KC_TAB):
            if (dev_info.devs) {
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_TAB, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case C(KC_UP):
            if (dev_info.devs) {
                bts_process_keys(KC_LCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_UP, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key

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
                    bts_process_keys(KC_UP, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_LEFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_DOWN, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_RGHT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_W, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_A, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_S, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
                    bts_process_keys(KC_D, record->event.pressed, dev_info.devs, keymap_config.no_gui);
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
        // case RGB_VAI: {
        //     if (record->event.pressed && (rgb_matrix_get_val() == RGB_MATRIX_MAXIMUM_BRIGHTNESS)) {
        //         if (timer_elapsed32(VAL_OUT_blink_time) > 200) {
        //             VAL_OUT_blink_time = timer_read32();
        //         }
        //         VAL_OUT_blink_cnt = 6;
        //         VAL_OUT_LEDINDEX  = 71;
        //     }
        // } break;
        // case RGB_VAD: {
        //     if (record->event.pressed && (rgb_matrix_get_val() == 0x00)) {
        //         if (timer_elapsed32(VAL_OUT_blink_time) > 200) {
        //             VAL_OUT_blink_time = timer_read32();
        //         }
        //         VAL_OUT_blink_cnt = 6;
        //         VAL_OUT_LEDINDEX  = 81;
        //     }
        // } break;
        // case RGB_SPI: {
        //     if (record->event.pressed && (rgb_matrix_get_speed() == 0xff)) {
        //         if (timer_elapsed32(VAL_OUT_blink_time) > 200) {
        //             VAL_OUT_blink_time = timer_read32();
        //         }
        //         VAL_OUT_blink_cnt = 6;
        //         VAL_OUT_LEDINDEX  = 82;
        //     }
        // } break;
        // case RGB_SPD: {
        //     if (record->event.pressed && (rgb_matrix_get_speed() == 0x00)) {
        //         if (timer_elapsed32(VAL_OUT_blink_time) > 200) {
        //             VAL_OUT_blink_time = timer_read32();
        //         }
        //         VAL_OUT_blink_cnt = 6;
        //         VAL_OUT_LEDINDEX  = 80;
        //     }
        // } break;
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
    if (VAL_OUT_blink_cnt) {
        if (timer_elapsed32(VAL_OUT_blink_time) > 200) {
            VAL_OUT_blink_time = timer_read32();
            VAL_OUT_blink_cnt--;
        }
        if (VAL_OUT_blink_cnt % 2) {
            rgb_matrix_set_color(VAL_OUT_LEDINDEX, 100, 100, 100);
        } else {
            rgb_matrix_set_color(VAL_OUT_LEDINDEX, 0, 0, 0);
        }
    }

    return true;
}
