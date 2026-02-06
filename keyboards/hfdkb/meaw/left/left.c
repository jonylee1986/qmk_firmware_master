// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include <stdlib.h>
#include "usb_main.h"

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
        // setPinInput(A14);
        led_inited = false;
    }
}

void set_led_state(void) {
    if (led_inited) {
        // writePin(LED_CAPS_LOCK_PIN, host_keyboard_led_state().caps_lock);
        // writePin(LED_WIN_LOCK_PIN, keymap_config.no_gui);
        // writePin(LED_POWER_PIN, get_highest_layer(default_layer_state) == 2);
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (process_record_user(keycode, record) != true) {
        return false;
    }
    switch (keycode) {
        default:
            break;
    }
#ifdef BT_MODE_ENABLE
    if (process_record_bt(keycode, record) != true) {
        return false;
    }
#endif
    return true;
}

void keyboard_post_init_kb(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
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
    set_led_state();
#endif
    matrix_scan_user();
}

static uint32_t usb_suspend_timer = 0;
static uint32_t usb_suspend       = false;
static uint32_t rgb_status_saved  = 0;

void housekeeping_task_kb(void) {
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

#ifdef USB_CHECK_SUSPEND_STATE
    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE || USB_DRIVER.state == USB_SUSPENDED) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend      = true;
                    rgb_status_saved = rgb_matrix_is_enabled();
                    if (rgb_status_saved) {
                        rgb_matrix_disable_noeeprom();
                    }
                    // led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;
                if (rgb_status_saved) {
                    if (!rgb_matrix_is_enabled()) {
                        rgb_matrix_enable_noeeprom();
                    }
                }
                // led_config_all();
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
            if (rgb_status_saved) {
                if (!rgb_matrix_is_enabled()) {
                    rgb_matrix_enable_noeeprom();
                }
            }
            // led_config_all();
        }
    }
#endif
}

uint8_t  rgb_test_en    = false;
uint8_t  rgb_test_index = 0;
uint32_t rgb_test_time  = 0;

static const uint8_t rgb_test_color_table[][3] = {
    {200, 200, 200},
    {200, 0, 0},
    {0, 200, 0},
    {0, 0, 200},
};

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    // if (!rgb_matrix_get_flags())
    rgb_matrix_set_color_all(0, 0, 0);

    if (rgb_test_en) {
        if (timer_elapsed32(rgb_test_time) > 3000 * (rgb_test_index + 1)) {
            rgb_test_index++;
        }
        // clang-format off
        rgb_matrix_set_color_all(rgb_test_color_table[rgb_test_index % 4 - 1][0],
        rgb_test_color_table[rgb_test_index % 4 - 1][1],
        rgb_test_color_table[rgb_test_index % 4 - 1][2]);
        // clang-format on
        if (timer_elapsed32(rgb_test_time) > 30 * 1000) {
            rgb_test_en = false;
        }
        return false;
    }

#ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#endif

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    return true;
}
