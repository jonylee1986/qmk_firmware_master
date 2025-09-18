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
#include <lib/lib8tion/lib8tion.h>
#include "dynamic_keymap.h"
#include "usb_main.h"

extern uint8_t sleep_time;

// clang-format off

#define IND_HUE INDICATOR_HUE
#define IND_VAL INDICATOR_BRIGHTNESS
#define KEY_SLP KEYBOARD_SLEEP
#define KEY_ECO ECO
#define FACTORY FACTORY_RESET
#define KEY_RES KEYBOARD_RESET
#define BLE_RES BLE_RESET

#define KC_SPOT MAC_Spotlight
#define KC_DICT MAC_Dictation
#define KC_DND MAC_Do_Not_Disturb

enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN
};

 const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_104_ansi( /* Base */
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,  KC_PSCR, KC_SCRL, KC_PAUS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSPC, KC_INS,  KC_HOME, KC_PGUP,  KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,  KC_BSLS, KC_DEL,  KC_END,  KC_PGDN,  KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,           KC_ENT,                              KC_P4,   KC_P5,   KC_P6,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,      KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT,          KC_UP,             KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_LCTL, KC_LWIN, KC_LALT,                              KC_SPC,                             KC_RALT, KC_APP,MO(WIN_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT,           KC_P0,   KC_PDOT),

    [WIN_FN] = LAYOUT_104_ansi( /* FN */
        NK_TOGG,          KC_BRID, KC_BRIU, KC_NO,   KC_NO,     KC_NO,   KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU, _______, _______, _______,
        FN_FUN,  BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, BT_USB,    _______, _______, _______, IND_VAL, IND_HUE, RGB_HUD, RGB_HUI,  FACTORY, _______, _______, _______,  _______, _______, _______, _______,
        BT_VOL,  _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, RGB_SAD, RGB_SAI,  RGB_MOD, BLE_RES, KEY_RES, _______,  _______, _______, _______, _______,
        RGB_TEST,_______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, KEY_ECO,           RGB_TOG,                             _______, _______, _______,
        _______,          _______, _______, _______, _______,   _______, _______, _______, _______, SW_OS,   FN_MENU,           _______,          RGB_VAI,           _______, _______, _______, _______,
        _______, WIN_LOCK,_______,                              _______,                            _______, KC_RWIN,MO(WIN_FN),_______, RGB_SPD, RGB_VAD, RGB_SPI,           _______, _______),

    [MAC_B] = LAYOUT_104_ansi( /* Base */
        KC_ESC,           KC_BRID, KC_BRIU, KC_MCTL, KC_NO,     KC_NO,   KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU, _______, _______, _______,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSPC, KC_INS,  KC_HOME, KC_PGUP,  KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,  KC_BSLS, KC_DEL,  KC_END,  KC_PGDN,  KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,           KC_ENT,                              KC_P4,   KC_P5,   KC_P6,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,      KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, KC_UP,                      KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_LCTL, KC_LOPT, KC_LCMD,                              KC_SPC,                             KC_RCMD, KC_APP, MO(MAC_FN),KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT,           KC_P0,   KC_PDOT),

    [MAC_FN] = LAYOUT_104_ansi( /* mac fn */
        NK_TOGG,           KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12, KC_PSCR, KC_SCRL, KC_PAUS,
        FN_FUN,  BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, BT_USB,    _______, _______, _______, IND_VAL, IND_HUE, RGB_HUD, RGB_HUI,  FACTORY, _______, _______, _______,  _______, _______, _______, _______,
        BT_VOL,  _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, RGB_SAD, RGB_SAI,  RGB_MOD, BLE_RES, KEY_RES, _______,  _______, _______, _______, _______,
        RGB_TEST,_______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, KEY_ECO,           RGB_TOG,                             _______, _______, _______,
        _______,          _______, _______, _______, _______,   _______, _______, _______, SW_OS,   _______, FN_MENU,           _______,          RGB_VAI,           _______, _______, _______, _______,
        _______, _______, _______,                              _______,                            _______, KC_ROPT, _______,  _______, RGB_SPD, RGB_VAD, RGB_SPI,           _______, _______),

 };

// clang-format on
static uint8_t  VAL_OUT_LEDINDEX;
static uint8_t  VAL_OUT_blink_cnt;
static RGB      VAL_OUT_blink_color;
static uint32_t VAL_OUT_blink_time;

// bool     is_siri_active = false;
// uint32_t siri_timer     = 0;

#define KC_F1_ROW 0
#define KC_F1_COL 2
#define KC_MENU_ROW 5
#define KC_MENU_COL 10

uint16_t FN_FUN_table[][12] = {
    {KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12},
    {KC_BRID, KC_BRIU, KC_NO, KC_NO, KC_NO, KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU},
    {KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12},
    {KC_BRID, KC_BRIU, KC_MCTL, KC_NO, KC_NO, KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU},
};

uint16_t FN_MENU_table[][1] = {
    {KC_APP},
    {KC_RWIN},
    {KC_APP},
    {KC_ROPT},
};

static uint8_t indicator_color_tab[][3] = {
    {HSV_BLUE},    // BLUE
    {HSV_PURPLE},  // PURPLE
    {HSV_MAGENTA}, // MAGENTA
    {HSV_RED},     // RED
    {HSV_ORANGE},  // ORANGE
    {HSV_YELLOW},  // YELLOW
    {HSV_GREEN},   // GREEN
    {HSV_CYAN},    // CYAN
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FN_FUN: {
            if (record->event.pressed) {
                if (get_highest_layer(default_layer_state) == WIN_B) {
                    if (dynamic_keymap_get_keycode(WIN_B, KC_F1_ROW, KC_F1_COL) == KC_F1) {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(WIN_B, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[WIN_FN][i]);
                            dynamic_keymap_set_keycode(WIN_FN, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[WIN_B][i]);
                        }
                        VAL_OUT_blink_time  = timer_read32();
                        VAL_OUT_blink_cnt   = 6;
                        VAL_OUT_LEDINDEX    = 16;
                        VAL_OUT_blink_color = (RGB){0, 0, 100};
                    } else {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(WIN_B, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[WIN_B][i]);
                            dynamic_keymap_set_keycode(WIN_FN, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[WIN_FN][i]);
                        }
                        VAL_OUT_blink_time  = timer_read32();
                        VAL_OUT_blink_cnt   = 6;
                        VAL_OUT_LEDINDEX    = 16;
                        VAL_OUT_blink_color = (RGB){100, 100, 100};
                    }
                } else if (get_highest_layer(default_layer_state) == MAC_B) {
                    if (dynamic_keymap_get_keycode(MAC_B, KC_F1_ROW, KC_F1_COL) == KC_F1) {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(MAC_B, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[MAC_FN][i]);
                            dynamic_keymap_set_keycode(MAC_FN, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[MAC_B][i]);
                        }
                        VAL_OUT_blink_time  = timer_read32();
                        VAL_OUT_blink_cnt   = 6;
                        VAL_OUT_LEDINDEX    = 16;
                        VAL_OUT_blink_color = (RGB){0, 0, 100};
                    } else {
                        for (size_t i = 0; i < 12; i++) {
                            dynamic_keymap_set_keycode(MAC_B, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[MAC_B][i]);
                            dynamic_keymap_set_keycode(MAC_FN, KC_F1_ROW, i + KC_F1_COL, FN_FUN_table[MAC_FN][i]);
                        }
                        VAL_OUT_blink_time  = timer_read32();
                        VAL_OUT_blink_cnt   = 6;
                        VAL_OUT_LEDINDEX    = 16;
                        VAL_OUT_blink_color = (RGB){100, 100, 100};
                    }
                }
            }
        }
            return false;
        case FN_MENU: {
            if (record->event.pressed) {
                if (get_highest_layer(default_layer_state) == WIN_B) {
                    if (dynamic_keymap_get_keycode(WIN_B, KC_MENU_ROW, KC_MENU_COL) == KC_APP) {
                        dynamic_keymap_set_keycode(WIN_B, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[WIN_FN][0]);
                        dynamic_keymap_set_keycode(WIN_FN, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[WIN_B][0]);
                    } else {
                        dynamic_keymap_set_keycode(WIN_B, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[WIN_B][0]);
                        dynamic_keymap_set_keycode(WIN_FN, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[WIN_FN][0]);
                    }
                } else if (get_highest_layer(default_layer_state) == MAC_B) {
                    if (dynamic_keymap_get_keycode(MAC_B, KC_MENU_ROW, KC_MENU_COL) == KC_APP) {
                        dynamic_keymap_set_keycode(MAC_B, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[MAC_FN][0]);
                        dynamic_keymap_set_keycode(MAC_FN, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[MAC_B][0]);
                    } else {
                        dynamic_keymap_set_keycode(MAC_B, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[MAC_B][0]);
                        dynamic_keymap_set_keycode(MAC_FN, KC_MENU_ROW, KC_MENU_COL, FN_MENU_table[MAC_FN][0]);
                    }
                }
            }
        }
            return false;
        case WIN_LOCK: {
            if (record->event.pressed) {
                keymap_config.no_gui = !keymap_config.no_gui;
                eeconfig_update_keymap(&keymap_config);
            }
        }
            return false;
        case RGB_VAD: {
            if (record->event.pressed) {
                rgb_matrix_config.hsv.v = rgb_matrix_get_val() - RGB_MATRIX_VAL_STEP;
                if (rgb_matrix_get_val() <= 40) rgb_matrix_config.hsv.v = 40;
            }
        }
            return false;
        case RGB_SPD: {
            if (record->event.pressed) {
                rgb_matrix_config.speed = rgb_matrix_get_speed() - RGB_MATRIX_SPD_STEP;
                eeconfig_update_rgb_matrix(&rgb_matrix_config);
                if (rgb_matrix_get_speed() <= 51) rgb_matrix_config.speed = 51;
            }
        }
            return false;
        case RGB_SAD: {
            if (record->event.pressed) {
                rgb_matrix_config.hsv.s = rgb_matrix_get_sat() - RGB_MATRIX_SAT_STEP;
                eeconfig_update_rgb_matrix(&rgb_matrix_config);
                if (rgb_matrix_get_sat() <= 64) rgb_matrix_config.hsv.s = 64;
            }
        }
            return false;
        case IND_VAL: {
            if (record->event.pressed) {
                dev_info.ind_brightness += RGB_MATRIX_VAL_STEP;
                if (dev_info.ind_brightness > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                    dev_info.ind_brightness = RGB_MATRIX_VAL_STEP;
                }
                eeconfig_update_user(dev_info.raw);
            }
        }
            return false;
        case IND_HUE: {
            if (record->event.pressed) {
                dev_info.ind_color_index++;
                if (dev_info.ind_color_index >= sizeof(indicator_color_tab) / sizeof(indicator_color_tab[0])) {
                    dev_info.ind_color_index = 0;
                }
                eeconfig_update_user(dev_info.raw);
            }
        }
            return false;
        case RGB_HUI: {
            if (record->event.pressed) {
                dev_info.smd_color_index++;
                if (dev_info.smd_color_index >= sizeof(indicator_color_tab) / sizeof(indicator_color_tab[0])) {
                    dev_info.smd_color_index = 0;
                }
                eeconfig_update_user(dev_info.raw);
                rgb_matrix_config.hsv.h = indicator_color_tab[dev_info.smd_color_index][0];
            }
        }
            return false;
        case RGB_HUD: {
            if (record->event.pressed) {
                if (dev_info.smd_color_index == 0) {
                    dev_info.smd_color_index = sizeof(indicator_color_tab) / sizeof(indicator_color_tab[0]) - 1;
                } else {
                    dev_info.smd_color_index--;
                }
                eeconfig_update_user(dev_info.raw);
                rgb_matrix_config.hsv.h = indicator_color_tab[dev_info.smd_color_index][0];
            }
        }
            return false;
        case KEY_ECO: {
            if (record->event.pressed) {
                dev_info.eco_tog_flag = !dev_info.eco_tog_flag;
                eeconfig_update_user(dev_info.raw);
            }
        }
            return false;

        case KC_DICT: {
            if (record->event.pressed) {
                host_consumer_send(0x00CF);
            } else {
                host_consumer_send(0x0000);
            }
        }
            return false; // Skip all further processing of this key
        case MAC_Spotlight: {
            if (record->event.pressed) {
                host_consumer_send(0x0221);
            } else {
                host_consumer_send(0x0000);
            }
        }
            return false; // Skip all further processing of this key
        case MAC_Do_Not_Disturb: {
            if (record->event.pressed) {
                host_system_send(0x009B);
            } else {
                host_system_send(0x0000);
            }
        }
            return false; // Skip all further processing of this key

        default:
            break;
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // GUI lock red
    if (keymap_config.no_gui) {
        rgb_matrix_set_color(92, 160, 160, 160);
    }

    static HSV hsv;
    static RGB rgb;

    hsv.h = indicator_color_tab[dev_info.ind_color_index][0];
    hsv.s = indicator_color_tab[dev_info.ind_color_index][1];
    hsv.v = dev_info.ind_brightness;
    rgb   = hsv_to_rgb(hsv);

    if (!dev_info.eco_tog_flag && host_keyboard_led_state().num_lock && ((bts_info.bt_info.paired) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state == USB_ACTIVE)))) {
        rgb_matrix_set_color(NUM_LOCK_IND_INDEX, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_matrix_set_color(NUM_LOCK_IND_INDEX, 0, 0, 0);
    }
    if (!dev_info.eco_tog_flag && host_keyboard_led_state().caps_lock && ((bts_info.bt_info.paired) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state == USB_ACTIVE)))) {
        rgb_matrix_set_color(CAPS_LOCK_IND_INDEX, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_matrix_set_color(CAPS_LOCK_IND_INDEX, 0, 0, 0);
    }
    if (!dev_info.eco_tog_flag && host_keyboard_led_state().scroll_lock && ((bts_info.bt_info.paired) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state == USB_ACTIVE)))) {
        rgb_matrix_set_color(SCROLL_LOCK_IND_INDEX, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_matrix_set_color(SCROLL_LOCK_IND_INDEX, 0, 0, 0);
    }

    if (VAL_OUT_blink_cnt) {
        if (timer_elapsed32(VAL_OUT_blink_time) > 500) {
            VAL_OUT_blink_time = timer_read32();
            VAL_OUT_blink_cnt--;
        }
        if (VAL_OUT_blink_cnt % 2) {
            rgb_matrix_set_color(VAL_OUT_LEDINDEX, VAL_OUT_blink_color.r, VAL_OUT_blink_color.g, VAL_OUT_blink_color.b);
        } else {
            rgb_matrix_set_color(VAL_OUT_LEDINDEX, 0, 0, 0);
        }
    }

    return true;
}

void keyboard_post_init_user() {
    rgb_matrix_config.hsv.h = indicator_color_tab[dev_info.smd_color_index][0];
    snled27351_pwm_phase_delay_enable(0);
    snled27351_pwm_phase_delay_enable(1);
}

void eeconfig_init_user(void) {
    // 设置默认值
    dev_info.ind_brightness  = RGB_MATRIX_VAL_STEP * 3;
    dev_info.smd_color_index = 0;
    dev_info.ind_color_index = 0;
    dev_info.eco_tog_flag    = false;
    dev_info.manual_usb_mode = false;
    eeconfig_update_user(dev_info.raw);
}
