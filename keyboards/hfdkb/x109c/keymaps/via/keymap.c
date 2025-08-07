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
enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define KC_SIRI LGUI(KC_SPC)

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

key_combination_t key_comb_list[] = {
    {3, {KC_LSFT, KC_LCMD, KC_S}}, // KC_SNAP
    {3, {KC_LSFT, KC_LCMD, KC_4}}, // KC_SSHOT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_78_ansi( /* Base */
		KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_MUTE,
		KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,
		KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(WIN_FN), KC_RCTL,       KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT_78_ansi( /* FN */
		EE_CLR,  KC_BRID, KC_BRIU, KC_TASK, KC_MYCM, KC_WSCH, KC_SNAP, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, LCD_MOD,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SAD,
		_______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, _______, _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______, RGB_SAI,
		_______, _______, _______, _______, _______, _______, _______, _______, KC_INS,  KC_HOME, KC_PGUP, _______,          RGB_HUI,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN,                   RGB_MOD, RGB_VAI,
		_______, GU_TOGG, _______,                   _______,                            _______, _______, _______,          RGB_SPD, RGB_VAD, RGB_SPI),

    [MAC_B] = LAYOUT_78_ansi( /* Base */
        KC_ESC,  KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, KC_SIRI, KC_SSHOT,KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,  KC_MUTE,
		KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,
		KC_LCTL, KC_LOPT, KC_LCMD,                   KC_SPC,                                      KC_RCMD,MO(MAC_FN),KC_RCTL,KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT_78_ansi( /* FN */
        EE_CLR,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, LCD_MOD,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SAD,
		_______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, _______, _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______, RGB_SAI,
		_______, _______, _______, _______, _______, _______, _______, _______, KC_INS,  KC_HOME, KC_PGUP, _______,          RGB_HUI,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN,                   RGB_MOD, RGB_VAI,
		_______, GU_TOGG, _______,                   _______,                            _______, _______, _______,          RGB_SPD, RGB_VAD, RGB_SPI),
};

//clang-format on
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [WIN_B]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN] = { ENCODER_CCW_CW(_______, _______) },
    [MAC_B]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN] = { ENCODER_CCW_CW(_______, _______) },
};
#endif

// bool     ENC_VOL_MODE = 0;
// void encoder_layer_set(void) {
//     ENC_VOL_MODE = !ENC_VOL_MODE;
// }
static bool lcd_mode_enabled = false;

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
            // case EN_MODE: {
            //     if (record->event.pressed) {
            //         encoder_layer_set();
            //     }
            //     return false;
            // }

            // case EN_VOLU: {
            //     if (ENC_VOL_MODE) {
            //         if (dev_info.devs) {
            //             bts_process_keys(KC_VOLU, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            //         } else {
            //             if (record->event.pressed) {
            //                 register_code(KC_VOLU);
            //             } else {
            //                 unregister_code(KC_VOLU);
            //             }
            //         }
            //     } else {
            //         if (record->event.pressed) {
            //             dev_info.LCD_PAGE = !dev_info.LCD_PAGE;
            //             LCD_Page_update(dev_info.LCD_PAGE);
            //             eeconfig_update_kb(dev_info.raw);
            //         }
            //     }
            //     return false;
            //     break;
            // }

            // case EN_VOLD: {
            //     if (ENC_VOL_MODE) {
            //         if (dev_info.devs) {
            //             bts_process_keys(KC_VOLD, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            //         } else {
            //             if (record->event.pressed) {
            //                 register_code(KC_VOLD);
            //             } else {
            //                 unregister_code(KC_VOLD);
            //             }
            //         }
            //     } else {
            //         if (record->event.pressed) {
            //             dev_info.LCD_PAGE = !dev_info.LCD_PAGE;
            //             LCD_Page_update(dev_info.LCD_PAGE);
            //             eeconfig_update_kb(dev_info.raw);
            //         }
            //     }
            //     return false;
            //     break;
            // }

        case KC_SNAP:
        case KC_SSHOT: {
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_SNAP].len; i++) {
                    register_code(key_comb_list[keycode - KC_SNAP].keycode[i]);
                }
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_SNAP].len; i++) {
                    unregister_code(key_comb_list[keycode - KC_SNAP].keycode[i]);
                }
            }
            return false; // Skip all further processing of this key
        }
        // case LGUI(KC_TAB): {
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_TAB, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //     }
        //     return true;
        //     break;
        // }
        // case G(KC_E): {
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_E, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //     }
        //     return true;
        //     break;
        // }
        // case G(KC_SPC): {
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_SPC, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //     }
        //     return true;
        //     break;
        // }
        // case G(S(KC_4)): {
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_4, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //     }
        //     return true;
        //     break;
        // }
        // case G(S(KC_S)): {
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_process_keys(KC_S, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //     }
        //     return true;
        //     break;
        // }
        // case KC_VOLD:
        //     if (record->event.pressed) {
        //         LCD_vol_update(false);
        //     }
        //     return true;
        // case KC_VOLU:
        //     if (record->event.pressed) {
        //         LCD_vol_update(true);
        //     }
        //     return true;
        case LCD_MOD: {
            if (record->event.pressed) {
                lcd_mode_enabled = !lcd_mode_enabled;
            }
        }
        default:
            break;
    }
    return true;
}
void housekeeping_task_user(void) {}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (!lcd_mode_enabled) {
        if (index == 0) { /* First encoder */
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
        return false; // Skip further processing for this encoder
    } else {
        if (index == 0) { /* First encoder */
            if (clockwise) {
                // tap_code(KC_PGDN);
                // unregister_code(KC_VOLU);
                if (dev_info.LCD_PAGE == 1) {
                    dev_info.LCD_PAGE = 0;
                } else {
                    dev_info.LCD_PAGE++;
                }
                eeconfig_update_user(dev_info.raw);
                LCD_Page_update(dev_info.LCD_PAGE);
                // LCD_command_update(LCD_SWITCH);
            } else {
                // tap_code(KC_PGUP);
                // unregister_code(KC_VOLD);
                if (dev_info.LCD_PAGE == 0) {
                    dev_info.LCD_PAGE = 1;
                } else {
                    dev_info.LCD_PAGE--;
                }
                eeconfig_update_user(dev_info.raw);
                LCD_Page_update(dev_info.LCD_PAGE);
                // LCD_command_update(LCD_SWITCH);
            }
        }
        return false;
    }
}
