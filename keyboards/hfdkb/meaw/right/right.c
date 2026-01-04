// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include <stdlib.h>
#include "usb_main.h"
// clang-format on

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
        case LOGO_MOD: {
            if (record->event.pressed) {
                dev_info.rgb_logo_mode++;
                dev_info.rgb_logo_mode %= 9;
                eeconfig_update_user(dev_info.raw);
            }
        }
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

void housekeeping_task_kb(void) {
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef USB_CHECK_SUSPEND_STATE

    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE || USB_DRIVER.state == USB_SUSPENDED) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
                    // rgb_matrix_disable_noeeprom();
                    // rgb_matrix_set_flags(LED_FLAG_NONE);
                    // rgb_matrix_set_color_all(0, 0, 0);
                    // led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend_timer) {
                usb_suspend_timer = 0;
                if (usb_suspend) {
                    usb_suspend = false;
                    // led_config_all();
                    // rgb_matrix_set_flags(LED_FLAG_ALL);
                }
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
            // led_config_all();
            // rgb_matrix_set_flags(LED_FLAG_ALL);
        }
    }
#endif

#ifdef CONSOLE_ENABLE
    debug_enable = true;
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
static const uint8_t rgb_logo_color_table[][3] = {
    {92, 0, 0}, {92, 92, 0}, {46, 92, 0}, {0, 92, 0}, {0, 92, 92}, {0, 0, 92}, {92, 0, 92}, {0, 0, 0},
};
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (dev_info.RGB_off) {
        for (uint8_t i = led_min; i <= led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    if (dev_info.rgb_logo_mode) {
        rgb_matrix_set_color(0, rgb_logo_color_table[dev_info.rgb_logo_mode - 1][0], rgb_logo_color_table[dev_info.rgb_logo_mode - 1][1], rgb_logo_color_table[dev_info.rgb_logo_mode - 1][2]);
    }

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

    if (usb_suspend) {
        for (uint8_t i = led_min; i <= led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    return true;
}

bool via_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
    switch (*command_id) {
        case 0x07: {
            if (data[1] == 0x03 && data[2] == 0x02 && data[3] == 0x00) {
                dev_info.RGB_off = 1;
                eeconfig_update_user(dev_info.raw);
            } else if (data[1] == 0x03 && data[2] == 0x02 && data[3] != 0x00) {
                dev_info.RGB_off = 0;
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        }
        default:
            break;
    }
    return false;
}
