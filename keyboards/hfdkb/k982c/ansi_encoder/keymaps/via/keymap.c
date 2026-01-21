/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

#ifdef MM_MODE_ENABLE
#    include "bt_task.h"
#    include "lcd.h"
#endif

// clang-format off

enum layers{
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

#define KC_SPOT KC_SPOTLIGHT
#define BT_1 BT_HOST1
#define BT_2 BT_HOST2
#define BT_3 BT_HOST3
#define WL_2G4 BT_2_4G

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_BASE] = LAYOUT_ansi(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,      KC_F10,   KC_F11,   KC_F12,   ENC_MODE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,       KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,       KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_END,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,    KC_QUOT,            KC_ENT,   KC_PGUP,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,     KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
        KC_LCTL,  KC_LCMD,  KC_LALT,                                KC_SPC,                       KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_ansi(
        EE_CLR,             KC_MYCM,  KC_WHOM,  KC_MAIL,  KC_CALC,  KC_MSEL,  KC_MSTP,  KC_MPRV,  KC_MPLY,  KC_MNXT,    KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,
        _______,  BT_1,     BT_2,     BT_3,     WL_2G4,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  RM_TOGG,  KC_PSCR,
        RGB_TEST, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  RM_NEXT,  KC_SCRL,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,            RM_HUEU,  _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RM_VALU,  _______,
        _______,  GU_TOGG,  _______,                                _______,                      IND_TOGG, _______,    BT_VOL,   RM_SPDD,  RM_VALD,  RM_SPDU),

    [MAC_BASE] = LAYOUT_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  KC_SIRI,  KC_SPOT,  KC_MPRV,  KC_MPLY,  KC_MNXT,    KC_MUTE,  KC_VOLD,  KC_VOLU,  ENC_MODE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,       KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,       KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_END,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,    KC_QUOT,            KC_ENT,   KC_PGUP,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,     KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
        KC_LCTL,  KC_LOPT,  KC_LCMD,                                KC_SPC,                       KC_RCMD,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_ansi(
        EE_CLR,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,      KC_F10,   KC_F11,   KC_F12,   _______,
        _______,  BT_1,     BT_2,     BT_3,     WL_2G4,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  RM_TOGG,  KC_PSCR,
        RGB_TEST, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  RM_NEXT,  KC_SCRL,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,            RM_HUEU,  _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RM_VALU,  _______,
        _______,  _______,  _______,                                _______,                      IND_TOGG, _______,    BT_VOL,   RM_SPDD,  RM_VALD,  RM_SPDU),

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = { ENCODER_CCW_CW(RM_VALD, _______)},
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = { ENCODER_CCW_CW(RM_VALD, _______)},
};
#endif // ENCODER_MAP_ENABLE

// clang-format on

static bool     mode_long_pressed_flag = false;
static uint32_t mode_long_pressed_time = 0;

static uint8_t  single_blink_cnt   = 0;
static uint8_t  single_blink_index = 0;
static RGB      single_blink_color = {0};
static uint32_t single_blink_time  = 0;

static uint8_t enc_blink_cnt   = 0;
static uint8_t enc_blink_index = 0;
// static RGB      enc_blink_color[] = {{0}, {0}, {0}};
static RGB      enc_blink_color = {0};
static uint32_t enc_blink_time  = 0;

static bool     is_siri_active = false;
static uint32_t siri_timer     = 0;

uint8_t get_enc_blink_cnt(void) {
    return enc_blink_cnt;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ENC_MODE:
            if (record->event.pressed) {
                mode_long_pressed_time = timer_read32();
            } else {
                mode_long_pressed_time = 0;
                if (mode_long_pressed_flag) {
                    mode_long_pressed_flag = false;
                } else {
                    if (dev_info.encoder_mode == 0) {
                        tap_code(KC_MUTE);
                    } else {
                        rgb_matrix_step();
                        if (dev_info.rgb_test_en) {
                            dev_info.rgb_test_en = 0;
                            eeconfig_update_user(dev_info.raw);
                        }
                    }
                }
            }
            return false;

        case IND_TOGG:
            if (record->event.pressed) {
                if (dev_info.ind_toggle) {
                    dev_info.ind_toggle = 0;
                } else {
                    dev_info.ind_toggle = 1;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case RM_VALU:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    single_blink_cnt   = 6;
                    single_blink_index = 70;
                    single_blink_color = (RGB){100, 100, 100};
                    single_blink_time  = timer_read32();
                }
            }
            break;
        case RM_VALD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    single_blink_cnt   = 6;
                    single_blink_index = 82;
                    single_blink_color = (RGB){100, 100, 100};
                    single_blink_time  = timer_read32();
                }
            }
            break;
        case RM_SPDU:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    single_blink_cnt   = 6;
                    single_blink_index = 83;
                    single_blink_color = (RGB){100, 100, 100};
                    single_blink_time  = timer_read32();
                }
            }
            break;
        case RM_SPDD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    single_blink_cnt   = 6;
                    single_blink_index = 81;
                    single_blink_color = (RGB){100, 100, 100};
                    single_blink_time  = timer_read32();
                }
            }
            break;

        case LCD_HOME: {
            if (record->event.pressed) {
                dev_info.LCD_Page = 0;
                LCD_Page_update(dev_info.LCD_Page);
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }
        case LCD_PAGE: {
            if (record->event.pressed) {
                dev_info.LCD_Page = 1;
                LCD_Page_update(dev_info.LCD_Page);
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }
        case LCD_SW: {
            if (record->event.pressed) {
                LCD_command_update(LCD_SWITCH);
            }
            return false;
        }

        case KC_SIRI:
            if (record->event.pressed) {
                if (!is_siri_active) {
                    is_siri_active = true;
                    register_code(KC_LCMD);
                    register_code(KC_SPACE);
                }
                siri_timer = timer_read32();
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key

        case KC_SPOTLIGHT:
            if (dev_info.devs == DEVS_USB) {
                if (record->event.pressed) {
                    host_consumer_send(0x0221);
                } else {
                    host_consumer_send(0);
                }
            } else {
                if (record->event.pressed) {
                    bts_send_consumer(0x221);
                } else {
                    bts_send_consumer(0);
                }
            }
            return false;

        default:
            break;
    }

    return true;
}

void housekeeping_task_user(void) {
#ifdef ENCODER_ENABLE
    if (mode_long_pressed_time && (timer_elapsed32(mode_long_pressed_time) >= 5000)) {
        mode_long_pressed_time = 0;
        mode_long_pressed_flag = true;
        if (dev_info.encoder_mode == 0) {
            dev_info.encoder_mode = 1;
            eeconfig_update_user(dev_info.raw);

            for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer += 2) {
                dynamic_keymap_set_encoder(layer, 0, true, RM_VALU);
                dynamic_keymap_set_encoder(layer, 0, false, RM_VALD);
            }
            enc_blink_cnt  = 6;
            enc_blink_time = timer_read32();
            // enc_blink_index[0] = CHRG_LOW_LED_INDEX;
            // enc_blink_index[1] = CAPS_LOCK_LED_INDEX;
            enc_blink_index = OS_LED_INDEX;
            enc_blink_color = (RGB){100, 100, 100};
        } else {
            dev_info.encoder_mode = 0;
            eeconfig_update_user(dev_info.raw);

            for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer += 2) {
                dynamic_keymap_set_encoder(layer, 0, true, KC_VOLU);
                dynamic_keymap_set_encoder(layer, 0, false, KC_VOLD);
            }
            enc_blink_cnt  = 6;
            enc_blink_time = timer_read32();
            // enc_blink_index[0] = CHRG_LOW_LED_INDEX;
            // enc_blink_index[1] = CAPS_LOCK_LED_INDEX;
            enc_blink_index = OS_LED_INDEX;
            enc_blink_color = (RGB){100, 100, 100};
        }
    }
#endif

    if (is_siri_active) {
        if (timer_elapsed32(siri_timer) >= 500) {
            unregister_code(KC_LCMD);
            unregister_code(KC_SPACE);
            is_siri_active = false;
        }
    }
}

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (enc_blink_cnt) {
        if (timer_elapsed32(enc_blink_time) > 500) {
            enc_blink_time = timer_read32();
            enc_blink_cnt--;
        }
        if (enc_blink_cnt % 2) {
            writePinHigh(LED_CHRG_LOW_PWR_PIN);
            writePinHigh(LED_CAPS_LOCK_IND_PIN);
            rgb_matrix_set_color(enc_blink_index, enc_blink_color.r, enc_blink_color.g, enc_blink_color.b);

        } else {
            writePinLow(LED_CHRG_LOW_PWR_PIN);
            writePinLow(LED_CAPS_LOCK_IND_PIN);
            rgb_matrix_set_color(enc_blink_index, 0, 0, 0);
        }
    }

    if (single_blink_cnt) {
        if (timer_elapsed32(single_blink_time) > 500) {
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
#endif
