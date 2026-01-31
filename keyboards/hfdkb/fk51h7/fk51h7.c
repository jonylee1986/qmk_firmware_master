// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef USB_SUSPEND_CHECK
#    include "usb_main.h"
#endif

bool led_inited = false;

void led_config_all(void) {
    if (!led_inited) {
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
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
        // if (!active) {
        //     keymap_config.no_gui = 0;
        //     eeconfig_update_keymap(&keymap_config);
        // }
    }
    return true;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (process_record_user(keycode, record) != true) {
        return false;
    }

    switch (keycode) {
        case RM_TOGG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                    } break;
                }
            }
            if (!rgb_matrix_is_enabled()) {
                rgb_matrix_enable();
            }
            return false;
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
#ifdef RGB_DRIVER_SDB_PIN
    setPinOutputOpenDrain(RGB_DRIVER_SDB_PIN);
    writePinHigh(RGB_DRIVER_SDB_PIN);
#endif

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
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
}

void housekeeping_task_kb(void) {
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef USB_SUSPEND_CHECK
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE || USB_DRIVER.state == USB_SUSPENDED) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
                    led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend_timer) {
                usb_suspend_timer = 0;
                if (usb_suspend) {
                    usb_suspend = false;
                    led_config_all();
                }
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
            led_config_all();
        }
    }
#endif

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = 83; i < 87; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_led() != true) {
        return false;
    }
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    // caps lock red
    if (host_keyboard_led_state().caps_lock && (dev_info.devs == DEVS_USB || bts_info.bt_info.paired)) {
        rgb_matrix_set_color(LED_CAPS_LOCK_IND_INDEX, 100, 100, 100);
    }
    // gui lock red
    if (keymap_config.no_gui && (dev_info.devs == DEVS_USB || bts_info.bt_info.paired) && (get_highest_layer(default_layer_state) == 0)) {
        rgb_matrix_set_color(LED_GUI_LOCK_IND_INDEX, 100, 100, 100);
    }

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    return true;
}
#endif

void suspend_power_down_kb(void) {
#ifdef RGB_DRIVER_SDB_PIN
    writePinLow(RGB_DRIVER_SDB_PIN);
#endif
}

void suspend_wakeup_init_kb(void) {
#ifdef RGB_DRIVER_SDB_PIN
    writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
}
