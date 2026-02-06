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
enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_iso( /* Base */
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,    KC_F11,  KC_F12,           KC_HOME,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,  KC_BSPC,          KC_END,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,                   KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   KC_NUHS, KC_ENT,           KC_PGDN,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,          KC_UP,   KC_DEL,
        KC_LCTL, KC_LWIN, KC_LALT,                            KC_SPC,                             KC_RALT, MO(WIN_FN),KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT_iso( /* FN */
        EE_CLR,           KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD,   KC_VOLU, KC_MAIL,          RM_TOGG,
        _______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, BT_USB,  _______, _______, _______, _______, _______, RM_SPDD,   RM_SPDU, _______,          _______,
        _______, _______, KC_W2UP, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,                   _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MC_1,    MC_2,      RM_NEXT, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,          RM_VALU, _______,
        _______, GU_TOGG, _______,                            _______,                            BT_VOL,  _______,   _______, RM_HUED, RM_VALD, RM_HUEU),

    [MAC_B] = LAYOUT_iso( /* Base */
        KC_ESC,           KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, KC_NO,   KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,   KC_VOLD, KC_VOLU,          KC_HOME,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,  KC_BSPC,          KC_END,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,                   KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   KC_NUHS, KC_ENT,           KC_PGDN,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,          KC_UP,   KC_DEL,
        KC_LCTL, KC_LOPT, KC_LCMD,                            KC_SPC,                             KC_ROPT, MO(MAC_FN),KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT_iso( /* FN */
        EE_CLR,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,    KC_F11,  KC_F12,           RM_TOGG,
        _______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, BT_USB,  _______, _______, _______, _______, _______, RM_SPDD,   RM_SPDU, _______,          _______,
        _______, _______, KC_W2UP, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,                   _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MC_1,    MC_2,      RM_NEXT, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,          RM_VALU, _______,
        _______, _______, _______,                            _______,                            BT_VOL,  _______,   _______, RM_HUED, RM_VALD, RM_HUEU),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_B]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN] = {ENCODER_CCW_CW(_______, _______) },
    [MAC_B]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN] = {ENCODER_CCW_CW(_______, _______) },
};
#endif // ENCODER_MAP_ENABLE
// clang-format on

static uint8_t  VAL_OUT_LEDINDEX;
static uint8_t  VAL_OUT_blink_cnt;
static RGB      VAL_OUT_blink_color;
static uint32_t VAL_OUT_blink_time;

static bool W2UP_flag = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_W2UP:
            if (record->event.pressed) {
                W2UP_flag = !W2UP_flag;
            }
            return false;

        case KC_W:
            if (W2UP_flag) {
                if (dev_info.devs) {
                    bts_process_keys(KC_UP, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_LEFT, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_DOWN, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_RGHT, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_W, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_A, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_S, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    bts_process_keys(KC_D, record->event.pressed, dev_info.devs, keymap_config.no_gui, 8);
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
                    VAL_OUT_LEDINDEX    = LED_CAPS_LOCK_IND_INDEX;
                    VAL_OUT_blink_cnt   = 6;
                    VAL_OUT_blink_color = (RGB){100, 100, 100};
                    VAL_OUT_blink_time  = timer_read32();
                }
            }
            break;
        case RM_VALD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    VAL_OUT_LEDINDEX    = LED_CAPS_LOCK_IND_INDEX;
                    VAL_OUT_blink_cnt   = 6;
                    VAL_OUT_blink_color = (RGB){100, 100, 100};
                    VAL_OUT_blink_time  = timer_read32();
                }
            }
            break;
        case RM_SPDU:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    VAL_OUT_LEDINDEX    = LED_CAPS_LOCK_IND_INDEX;
                    VAL_OUT_blink_cnt   = 6;
                    VAL_OUT_blink_color = (RGB){100, 100, 100};
                    VAL_OUT_blink_time  = timer_read32();
                }
            }
            break;
        case RM_SPDD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    VAL_OUT_LEDINDEX    = LED_CAPS_LOCK_IND_INDEX;
                    VAL_OUT_blink_cnt   = 6;
                    VAL_OUT_blink_color = (RGB){100, 100, 100};
                    VAL_OUT_blink_time  = timer_read32();
                }
            }
            break;

        default:
            break;
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (VAL_OUT_blink_cnt > 0) {
        // Set the color every frame based on current blink state
        if (VAL_OUT_blink_cnt % 2) {
            RGB_MATRIX_INDICATOR_SET_COLOR(VAL_OUT_LEDINDEX, VAL_OUT_blink_color.r, VAL_OUT_blink_color.g, VAL_OUT_blink_color.b);
        } else {
            RGB_MATRIX_INDICATOR_SET_COLOR(VAL_OUT_LEDINDEX, 0, 0, 0);
        }

        // Toggle state every 200ms
        if (timer_elapsed32(VAL_OUT_blink_time) >= 200) {
            VAL_OUT_blink_time = timer_read32();
            VAL_OUT_blink_cnt--;
        }
    }

    return true;
}
