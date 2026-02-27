// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "usb_main.h"
#include "common/bt_task.h"
#include "lib/lib8tion/lib8tion.h"

static uint8_t color_tab[][3] = {
    {HSV_RED},    // RED
    {HSV_GREEN},  // GREEN
    {HSV_BLUE},   // BLUE
    {HSV_YELLOW}, // YELLOW
    // {HSV_WHITE},  // WHITE
    {HSV_PURPLE}, // PURPLE
    {HSV_CYAN},   // CYAN
};

enum led_light_color {
    LED_COLOR_RED,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
    LED_COLOR_YELLOW,
    // LED_COLOR_WHITE,
    LED_COLOR_PURPLE,
    LED_COLOR_CYAN,
    LED_COLOR_COUNT,
};

bool led_inited = false;

void led_config_all(void) {
    if (!led_inited) {
        // Set our LED pins as output
        setPinOutput(A14);
        if (dev_info.devs == DEVS_USB) {
            writePinLow(A14);
        } else {
            writePinHigh(A14);
        }
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // Set our LED pins as input
        led_inited = false;
    }
}

// 拨动开关选择系统模式
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
        if (!active) {
            keymap_config.no_gui = 0;
            eeconfig_update_keymap(&keymap_config);
        }
    }
    return true;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (get_low_vol_off()) {
        bts_process_keys(keycode, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
        return false;
    }

    switch (keycode) {
        case QK_RGB_MATRIX_TOGGLE: {
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_set_color_all(RGB_OFF);
                    } break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                    } break;
                }
            }
            if (!rgb_matrix_is_enabled()) {
                rgb_matrix_set_flags(LED_FLAG_ALL);
                rgb_matrix_enable();
            }
            return false;
        }

        case RM_HUEU:
            if (record->event.pressed) {
                dev_info.color_index++;
                if (dev_info.color_index >= LED_COLOR_COUNT) {
                    dev_info.color_index = LED_COLOR_RED;
                }
                // if (dev_info.color_index != LED_COLOR_WHITE)
                rgb_matrix_config.hsv.h = color_tab[dev_info.color_index][0];
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        case RM_HUED:
            if (record->event.pressed) {
                if (dev_info.color_index == LED_COLOR_RED) {
                    dev_info.color_index = LED_COLOR_COUNT - 1;
                } else {
                    dev_info.color_index--;
                }
                // if (dev_info.color_index != LED_COLOR_WHITE)
                rgb_matrix_config.hsv.h = color_tab[dev_info.color_index][0];
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        default:
            break;
    }

    if (process_record_user(keycode, record) != true) {
        return false;
    }

#ifdef BT_MODE_ENABLE
    if (process_record_bt(keycode, record) != true) {
        return false;
    }
#endif
    return true;
}

void matrix_init_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_init();
    led_config_all();
#endif
    matrix_init_user();
}

void matrix_scan_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_task();
#endif
    matrix_scan_user();
}

void keyboard_post_init_kb(void) {
#ifdef RGB_MATRIX_ENABLE
    dev_info.raw            = eeconfig_read_user();
    rgb_matrix_config.hsv.h = color_tab[dev_info.color_index][0];
#endif

    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
}

void eeconfig_init_kb(void) {
    dev_info.color_index = LED_COLOR_RED;
    eeconfig_update_user(dev_info.raw);
}

void housekeeping_task_kb(void) {
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

#ifdef USB_SUSPEND_STATE_CHECK
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (usb_suspend) {
            bool wakeup = false;
            for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
                if (matrix_get_row(r)) {
                    wakeup = true;
                    break;
                }
            }
            if (wakeup) {
                // usbWakeupHost(&USB_DRIVER);
                // restart_usb_driver(&USB_DRIVER);
                usb_suspend       = false;
                usb_suspend_timer = 0;
#    ifdef RGB_DRIVER_SDB_PIN
                writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
            }
        }

        if ((USB_DRIVER.state != USB_ACTIVE) || (USB_DRIVER.state == USB_SUSPENDED)) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
#    ifdef RGB_DRIVER_SDB_PIN
                    writePinLow(RGB_DRIVER_SDB_PIN);
#    endif
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;

#    ifdef RGB_DRIVER_SDB_PIN
                writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
#    ifdef RGB_DRIVER_SDB_PIN
            writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
        }
    }
#endif
}

#ifdef RGB_MATRIX_ENABLE
uint8_t rgb_test_en    = false;
uint8_t rgb_test_index = 0;

static const uint8_t rgb_test_color_table[][3] = {
    {RGB_WHITE},
    {RGB_RED},
    {RGB_GREEN},
    {RGB_BLUE},
};

bool rgb_matrix_indicators_kb(void) {
    if (rgb_matrix_indicators_user() != true) {
        return false;
    }

    return true;
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    extern bool EE_CLR_flag;
    extern bool Low_power;

    if ((rgb_matrix_get_flags() != LED_FLAG_NONE) && !EE_CLR_flag && !Low_power) {
        uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_get_speed() / 4, 1));
        for (uint8_t i = 102; i <= 104; i++) {
            HSV hsv = {g_led_config.point[i].x - time, 255, rgb_matrix_get_val()};
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }

    // if ((dev_info.color_index == LED_COLOR_WHITE) && !Low_power) {
    //     uint8_t brightness = rgb_matrix_get_val();
    //     llv_rgb_matrix_set_color_all(brightness / 2, brightness / 2, brightness / 2);
    // }

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    // caps lock red
    if (host_keyboard_led_state().caps_lock && (((dev_info.devs != DEVS_USB) && bts_info.bt_info.paired && !get_kb_sleep_flag()) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state != USB_SUSPENDED)))) {
        rgb_matrix_set_color(LED_CAPS_IND_INDEX, 100, 100, 100);
    }
    // gui lock red
    if (keymap_config.no_gui && (((dev_info.devs != DEVS_USB) && bts_info.bt_info.paired && !get_kb_sleep_flag()) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state != USB_SUSPENDED)))) {
        rgb_matrix_set_color(LED_WIN_IND_INDEX, 100, 100, 100);
    }
    // num lock red
    if (host_keyboard_led_state().num_lock && (((dev_info.devs != DEVS_USB) && bts_info.bt_info.paired && !get_kb_sleep_flag()) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state != USB_SUSPENDED)))) {
        rgb_matrix_set_color(LED_NUM_LOCK_IND_INDEX, 100, 100, 100);
    }

    if ((rgb_test_en) && (rgb_test_index > 0)) {
        // clang-format off
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, rgb_test_color_table[rgb_test_index - 1][0],
            rgb_test_color_table[rgb_test_index - 1][1],
            rgb_test_color_table[rgb_test_index - 1][2]);
        }
        // clang-format on
        return false;
    }

    return true;
}
#endif
