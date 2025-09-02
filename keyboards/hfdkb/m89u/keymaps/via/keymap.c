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
#ifdef MULTIMODE_ENABLE
#    include "bt_task.h"
#endif
#include "lib/lib8tion/lib8tion.h"
#include "usb_main.h"

enum __layers {
    WIN_B,
    WIN_B1,
    WIN_FN,
    MAC_B,
    MAC_FN,
};

#define IND_HUE INDICATOR_HUE
#define IND_VAL INDICATOR_BRIGHTNESS
#define KEY_SLP KEYBOARD_SLEEP
#define KEY_ECO ECO
#define FACTORY FACTORY_RESET
#define KEY_RES KEYBOARD_RESET
#define BLE_RES BLE_RESET

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_numpad_6x4(
        KC_ESC,  KC_TAB,  KC_BSPC, MO(WIN_FN),
        KC_NUM,  KEY_EQL, KC_PSLS, KC_PAST,
        KC_P7,   KC_P8,   KC_P9,   KC_PMNS,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,
                 KC_P0,   KC_PDOT
    ),

    [WIN_B1] = LAYOUT_numpad_6x4(
        KC_ESC,  KC_TAB,  KC_BSPC, MO(WIN_FN),
        KC_NUM,  KEY_EQL, KC_PSLS, KC_PAST,
        KC_HOME, KC_UP,   KC_PGUP, KC_PMNS,
        KC_LEFT, KC_NO,   KC_RIGHT,KC_PPLS,
        KC_END,  KC_DOWN, KC_PGDN, KC_PENT,
                 KC_INS,  KC_DEL
    ),

    [WIN_FN] = LAYOUT_numpad_6x4(
        NK_TOGG, _______, KEY_ECO, _______,
        RGB_TOG, BLE_RES, KEY_RES, FACTORY,
        RGB_HUI, RGB_VAI, RGB_MOD, RGB_SAI,
        BT_2_4G, BT_USB,  RGB_SPI, BT_VOL,
        BT_HOST1,BT_HOST2,BT_HOST3,_______,
        SW_OS,   RGB_TEST
    ),

    [MAC_B] = LAYOUT_numpad_6x4(
        KC_ESC,  KC_TAB,  KC_BSPC, MO(MAC_FN),
        KC_NUM,  KC_PEQL, KC_PSLS, KC_PAST,
        KC_P7,   KC_P8,   KC_P9,   KC_PMNS,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,
                 KC_P0,   KC_PDOT
    ),

    [MAC_FN] = LAYOUT_numpad_6x4(
        NK_TOGG, _______, KEY_ECO, _______,
        RGB_TOG, BLE_RES, KEY_RES, FACTORY,
        RGB_HUI, RGB_VAI, RGB_MOD, RGB_SAI,
        BT_2_4G, BT_USB,  RGB_SPI, BT_VOL,
        BT_HOST1,BT_HOST2,BT_HOST3,_______,
        SW_OS,   RGB_TEST
    )
};
// clang-format on

// static uint16_t navigation_tab[][3] = {
//     [0] = {KC_HOME, KC_UP, KC_PGUP},
//     [1] = {KC_LEFT, KC_NO, KC_RIGHT},
//     [2] = {KC_END, KC_DOWN, KC_PGDN},
//     [3] = {KC_INS, KC_DEL},
// };

const uint8_t indicator_color_tab[][3] = {
    {HSV_BLUE},    // BLUE
    {HSV_PURPLE},  // PURPLE
    {HSV_MAGENTA}, // MAGENTA
    {HSV_RED},     // RED
    {HSV_ORANGE},  // ORANGE
    {HSV_YELLOW},  // YELLOW
    {HSV_GREEN},   // GREEN
    {HSV_CYAN},    // CYAN
};

static uint8_t  single_blink_index = 0;
static uint8_t  single_blink_cnt   = 0;
static uint32_t single_blink_time  = 0;
RGB             single_blink_color = {0};

bool            key_eql_pressed       = false;
static uint32_t key_eql_numlock_timer = 0;
static uint8_t  host_numlock_state    = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef MULTIMODE_ENABLE
    if (!bt_process_record(keycode, record)) {
        return false;
    }
#endif

    switch (keycode) {
        case RGB_VAI: {
            if (record->event.pressed) {
                if (rgb_matrix_get_val() == RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                    rgb_matrix_config.hsv.v = RGB_MATRIX_VAL_STEP;
                    eeconfig_update_rgb_matrix(&rgb_matrix_config);

                } else {
                    rgb_matrix_increase_val();
                }
            }
            return false;
        }

        case RGB_SPI: {
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() == UINT8_MAX) {
                    rgb_matrix_config.speed = RGB_MATRIX_SPD_STEP;
                    eeconfig_update_rgb_matrix(&rgb_matrix_config);
                } else {
                    rgb_matrix_increase_speed();
                }
            }
            return false;
        }

        case IND_VAL: {
            if (record->event.pressed) {
                dev_info.ind_brightness = qadd8(dev_info.ind_brightness, RGB_MATRIX_VAL_STEP);
                dev_info.ind_brightness = (dev_info.ind_brightness > RGB_MATRIX_MAXIMUM_BRIGHTNESS) ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : dev_info.ind_brightness;
                eeconfig_update_user(dev_info.raw);
                if (dev_info.ind_brightness == RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                    dev_info.ind_brightness = 0x00;
                }
            }
            return false;
        }

        case IND_HUE: {
            if (record->event.pressed) {
                dev_info.ind_color_index++;
                if (dev_info.ind_color_index >= sizeof(indicator_color_tab) / sizeof(indicator_color_tab[0])) {
                    dev_info.ind_color_index = 0;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        }

        case RGB_HUI: {
            if (record->event.pressed) {
                dev_info.smd_color_index++;
                if (dev_info.smd_color_index >= sizeof(indicator_color_tab) / sizeof(indicator_color_tab[0])) {
                    dev_info.smd_color_index = 0;
                }
                eeconfig_update_user(dev_info.raw);
                rgb_matrix_config.hsv.h = indicator_color_tab[dev_info.smd_color_index][0];
            }
            return false;
        }

        case RGB_SAI: {
            if (record->event.pressed) {
                if (rgb_matrix_get_sat() >= UINT8_MAX) {
                    rgb_matrix_config.hsv.s = 64; // 设置为最小饱和度
                    eeconfig_update_rgb_matrix(&rgb_matrix_config);
                } else {
                    rgb_matrix_increase_sat();
                }
            }
            return false;
        }

        case KEY_ECO: {
            if (record->event.pressed) {
                dev_info.eco_tog_flag = !dev_info.eco_tog_flag;
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        }

        case KEY_EQL: {
            if (get_highest_layer(default_layer_state) == 0) {
                if (record->event.pressed) {
                    key_eql_pressed    = true;
                    host_numlock_state = host_keyboard_led_state().num_lock;
                    if (!host_numlock_state) {
                        register_code(KC_NUM_LOCK);
                    }
                    register_code(KC_LALT);
                    register_code(KC_P6);
                    register_code(KC_P1);
                    // if (dev_info.devs) {
                    //     unregister_code(KC_NUM_LOCK);
                    // }
                    key_eql_numlock_timer = timer_read32(); // 50ms delay
                }
            }
            return false;
        }

        case KC_NUM: {
            if (dev_info.unsync) {
                if (record->event.pressed) {
                    dev_info.num_unsync = !dev_info.num_unsync;
                    // uprintf("num_unsync: %d", dev_info.num_unsync);
                    eeconfig_update_user(dev_info.raw);
                    return false;
                }
            }
            break;
        }

        default: {
            // 处理其他按键
            return true; // 允许默认处理
        }
    }

    return true;
}

bool charge_full = false;

#ifdef RGB_MATRIX_ENABLE
static void num_lock_indicator(void) {
    static HSV hsv;
    static RGB rgb;

    hsv.h = indicator_color_tab[dev_info.ind_color_index][0];
    hsv.s = indicator_color_tab[dev_info.ind_color_index][1];
    hsv.v = dev_info.ind_brightness;
    rgb   = hsv_to_rgb(hsv);

    // NumLock 指示逻辑
    bool should_show_numlock = false;

    if (!dev_info.eco_tog_flag && !charge_full) {
        if (dev_info.unsync) {
            should_show_numlock = (dev_info.num_unsync && (bts_info.bt_info.paired || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state == USB_ACTIVE))));
        } else {
            should_show_numlock = (host_keyboard_led_state().num_lock && (bts_info.bt_info.paired || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state == USB_ACTIVE))));
        }
    }

    if (should_show_numlock) {
        rgb_matrix_set_color(NUM_LOCK_INDICATE_INDEX, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_matrix_set_color(NUM_LOCK_INDICATE_INDEX, RGB_OFF);
    }
}

bool rgb_matrix_indicators_user(void) {
    if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color_all(RGB_OFF);
    }

    num_lock_indicator();

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
#    ifdef MULTIMODE_ENABLE
    if (!bt_indicators_advanced(led_min, led_max)) {
        return false;
    }
#    endif

    // All LEDs blink
    if (single_blink_cnt) {
        if (timer_elapsed32(single_blink_time) > 300) {
            single_blink_time = timer_read32();
            single_blink_cnt--;
        }
        if (single_blink_cnt & 0x1) {
            rgb_matrix_set_color(single_blink_index, single_blink_color.r, single_blink_color.g, single_blink_color.b);
        }
    }

    return true;
}
#endif // RGB_MATRIX_ENABLE

void housekeeping_task_user(void) {
#ifdef NKRO_ENABLE
    if (dev_info.devs) {
        static uint8_t nkro_mode = true;
        do {
            nkro_mode = keymap_config.nkro;
            if (nkro_mode) {
                bts_set_nkro(true);
            } else {
                bts_set_nkro(false);
            }
        } while (nkro_mode != keymap_config.nkro);
    }
#endif // NKRO_ENABLE

    if (host_keyboard_led_state().num_lock && !dev_info.num_unsync && dev_info.unsync) {
        if (IS_LAYER_OFF(WIN_B1)) {
            layer_on(WIN_B1);
        }
    } else {
        if (IS_LAYER_ON(WIN_B1)) {
            layer_off(WIN_B1);
        }
    }

    if (key_eql_numlock_timer && (timer_elapsed32(key_eql_numlock_timer) >= 50)) {
        unregister_code(KC_P6);
        unregister_code(KC_P1);
        unregister_code(KC_LALT);

        if (dev_info.devs) {
            unregister_code(KC_NUM_LOCK);
        }
        if (key_eql_pressed && !host_numlock_state) {
            // USB mode - send NumLock toggle
            register_code(KC_NUM_LOCK);
            wait_ms(10);
            unregister_code(KC_NUM_LOCK);
        }
        key_eql_numlock_timer = 0; // Reset timer
        key_eql_pressed       = false;
    }
}

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        if (active) {
            if (get_highest_layer(default_layer_state) == WIN_B) {
                dev_info.unsync = true;
                eeconfig_update_user(dev_info.raw);
            }
        } else {
            dev_info.unsync = false;
            eeconfig_update_user(dev_info.raw);
        }
    }
    return true;
}
#endif // DIP_SWITCH_ENABLE

void keyboard_post_init_user(void) {
    rgb_matrix_config.hsv.h = indicator_color_tab[dev_info.smd_color_index][0];
}

void eeconfig_init_user(void) { // EEPROM is getting reset!
    dev_info.ind_brightness  = RGB_MATRIX_VAL_STEP * 3;
    dev_info.smd_color_index = 0;
    dev_info.ind_color_index = 0;
    dev_info.eco_tog_flag    = false;
    dev_info.manual_usb_mode = false;
    dev_info.unsync          = false;
    dev_info.num_unsync      = true;
    eeconfig_update_user(dev_info.raw);
}
