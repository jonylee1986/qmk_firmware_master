// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include "usb_main.h"

// clang-format off
#ifdef RGB_MATRIX_ENABLE
const snled27351_led_t PROGMEM g_snled27351_leds[SNLED27351_LED_COUNT] = {
/* Refer to IS31 manual for these locations
*    driver
*    |   R location
*    |   |          G location
*    |   |          |          B location
*    |   |          |          | */
    {1, CB1_CA1,   CB2_CA1,   CB3_CA1},
    {1, CB1_CA3,   CB2_CA3,   CB3_CA3},
    {1, CB1_CA4,   CB2_CA4,   CB3_CA4},
    {1, CB1_CA5,   CB2_CA5,   CB3_CA5},
    {1, CB1_CA6,   CB2_CA6,   CB3_CA6},
    {1, CB1_CA7,   CB2_CA7,   CB3_CA7},
    {1, CB1_CA8,   CB2_CA8,   CB3_CA8},
    {1, CB1_CA9,   CB2_CA9,   CB3_CA9},
    {1, CB1_CA10,  CB2_CA10,  CB3_CA10},
    {1, CB1_CA11,  CB2_CA11,  CB3_CA11},
    {1, CB1_CA12,  CB2_CA12,  CB3_CA12},
    {1, CB1_CA13,  CB2_CA13,  CB3_CA13},
    {1, CB1_CA14,  CB2_CA14,  CB3_CA14},

    {0, CB1_CA1,   CB2_CA1,   CB3_CA1},
    {0, CB1_CA2,   CB2_CA2,   CB3_CA2},
    {0, CB1_CA3,   CB2_CA3,   CB3_CA3},
    {0, CB1_CA4,   CB2_CA4,   CB3_CA4},
    {0, CB1_CA5,   CB2_CA5,   CB3_CA5},
    {0, CB1_CA6,   CB2_CA6,   CB3_CA6},
    {0, CB1_CA7,   CB2_CA7,   CB3_CA7},
    {0, CB1_CA8,   CB2_CA8,   CB3_CA8},
    {0, CB1_CA9,   CB2_CA9,   CB3_CA9},
    {0, CB1_CA10,  CB2_CA10,  CB3_CA10},
    {0, CB1_CA11,  CB2_CA11,  CB3_CA11},
    {0, CB1_CA12,  CB2_CA12,  CB3_CA12},
    {0, CB1_CA13,  CB2_CA13,  CB3_CA13},
    {0, CB1_CA14,  CB2_CA14,  CB3_CA14},
    {1, CB4_CA1,   CB5_CA1,   CB6_CA1},
    {1, CB4_CA2,   CB5_CA2,   CB6_CA2},
    {1, CB4_CA3,   CB5_CA3,   CB6_CA3},

    {0, CB4_CA1,   CB5_CA1,   CB6_CA1},
    {0, CB4_CA2,   CB5_CA2,   CB6_CA2},
    {0, CB4_CA3,   CB5_CA3,   CB6_CA3},
    {0, CB4_CA4,   CB5_CA4,   CB6_CA4},
    {0, CB4_CA5,   CB5_CA5,   CB6_CA5},
    {0, CB4_CA6,   CB5_CA6,   CB6_CA6},
    {0, CB4_CA7,   CB5_CA7,   CB6_CA7},
    {0, CB4_CA8,   CB5_CA8,   CB6_CA8},
    {0, CB4_CA9,   CB5_CA9,   CB6_CA9},
    {0, CB4_CA10,  CB5_CA10,  CB6_CA10},
    {0, CB4_CA11,  CB5_CA11,  CB6_CA11},
    {0, CB4_CA12,  CB5_CA12,  CB6_CA12},
    {0, CB4_CA13,  CB5_CA13,  CB6_CA13},
    {0, CB4_CA14,  CB5_CA14,  CB6_CA14},
    {1, CB4_CA8,   CB5_CA8,   CB6_CA8},
    {1, CB4_CA9,   CB5_CA9,   CB6_CA9},
    {1, CB4_CA10,  CB5_CA10,  CB6_CA10},

    {0, CB7_CA1,   CB8_CA1,   CB9_CA1},
    {0, CB7_CA2,   CB8_CA2,   CB9_CA2},
    {0, CB7_CA3,   CB8_CA3,   CB9_CA3},
    {0, CB7_CA4,   CB8_CA4,   CB9_CA4},
    {0, CB7_CA5,   CB8_CA5,   CB9_CA5},
    {0, CB7_CA6,   CB8_CA6,   CB9_CA6},
    {0, CB7_CA7,   CB8_CA7,   CB9_CA7},
    {0, CB7_CA8,   CB8_CA8,   CB9_CA8},
    {0, CB7_CA9,   CB8_CA9,   CB9_CA9},
    {0, CB7_CA10,  CB8_CA10,  CB9_CA10},
    {0, CB7_CA11,  CB8_CA11,  CB9_CA11},
    {0, CB7_CA12,  CB8_CA12,  CB9_CA12},
    {0, CB7_CA13,  CB8_CA13,  CB9_CA13},

    {0, CB10_CA1,  CB11_CA1,  CB12_CA1},
    {0, CB10_CA2,  CB11_CA2,  CB12_CA2},
    {0, CB10_CA3,  CB11_CA3,  CB12_CA3},
    {0, CB10_CA4,  CB11_CA4,  CB12_CA4},
    {0, CB10_CA5,  CB11_CA5,  CB12_CA5},
    {0, CB10_CA6,  CB11_CA6,  CB12_CA6},
    {0, CB10_CA7,  CB11_CA7,  CB12_CA7},
    {0, CB10_CA8,  CB11_CA8,  CB12_CA8},
    {0, CB10_CA9,  CB11_CA9,  CB12_CA9},
    {0, CB10_CA10, CB11_CA10, CB12_CA10},
    {0, CB10_CA11, CB11_CA11, CB12_CA11},
    {0, CB10_CA12, CB11_CA12, CB12_CA12},
    {1, CB10_CA4,  CB11_CA4,  CB12_CA4},

    {0, CB10_CA13, CB11_CA13, CB12_CA13},
    {0, CB10_CA14, CB11_CA14, CB12_CA14},
    {0, CB10_CA15, CB11_CA15, CB12_CA15},
    {0, CB10_CA16, CB11_CA16, CB12_CA16},
    {0, CB7_CA15,  CB8_CA15,  CB9_CA15},
    {0, CB7_CA16,  CB8_CA16,  CB9_CA16},
    {1, CB10_CA1,  CB11_CA1,  CB12_CA1},
    {1, CB10_CA2,  CB11_CA2,  CB12_CA2},
    {1, CB10_CA3,  CB11_CA3,  CB12_CA3},
    {1, CB10_CA5,  CB11_CA5,  CB12_CA5},
    {1, CB10_CA6,  CB11_CA6,  CB12_CA6},

    {1, CB4_CA11,  CB5_CA11,  CB6_CA11},
    {1, CB4_CA14,  CB5_CA14,  CB6_CA14},
    {1, CB4_CA15,  CB5_CA15,  CB6_CA15},
};
#endif
// clang-format on

bool led_inited = false;

void led_config_all(void) {
    if (!led_inited) {
#ifdef RGB_DRIVER_SDB_PIN
        writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
#ifdef RGB_DRIVER_SDB_PIN
        writePinLow(RGB_DRIVER_SDB_PIN);
#endif
        led_inited = false;
    }
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    led_deconfig_all();
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    led_config_all();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (process_record_user(keycode, record) != true) {
        return false;
    }

    switch (keycode) {
        case QK_UNDERGLOW_TOGGLE:
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
                rgb_matrix_set_flags(LED_FLAG_ALL);
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
#ifdef BT_MODE_ENABLE
    bt_init(); // 使用新的初始化函数
    led_config_all();
#endif
}

void matrix_scan_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_task();
#endif

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE || USB_DRIVER.state == USB_SUSPENDED) {
            // USB挂起状态
            if (!usb_suspend_timer) {
                // 开始计时
                usb_suspend_timer = timer_read32();
                dprintf("USB suspended, starting 10s timer\n");
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                // 挂起超过10秒，关闭背光
                dprintf("USB suspended for 10s, turning off lights\n");
                if (!usb_suspend) {
                    // 如果之前没有进入挂起状态，执行挂起操作
                    usb_suspend = true;
                    led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            // USB活跃状态，重置计时器
            if (usb_suspend_timer) {
                dprintf("USB resumed, canceling suspend timer\n");
                usb_suspend_timer = 0;
                if (usb_suspend) {
                    // 如果之前处于挂起状态，恢复背光
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
}

void housekeeping_task_kb(void) {
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

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
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color_all(0x00, 0x00, 0x00);
    }

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

#ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#endif

    return true;
}
