// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include "usb_main.h"
#include "common/wb32_wwdg.h"
// clang-format off

#ifdef RGB_MATRIX_ENABLE
const is31fl3733_led_t PROGMEM g_is31fl3733_leds[RGB_MATRIX_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *   driver
 *   |   R location
 *   |   |          G location
 *   |   |          |          B location
 *   |   |          |          | */
    {1, SW1_CS1,   SW2_CS1,   SW3_CS1},
    {1, SW1_CS3,   SW2_CS3,   SW3_CS3},
    {1, SW1_CS4,   SW2_CS4,   SW3_CS4},
    {1, SW1_CS5,   SW2_CS5,   SW3_CS5},
    {1, SW1_CS6,   SW2_CS6,   SW3_CS6},
    {1, SW1_CS7,   SW2_CS7,   SW3_CS7},
    {1, SW1_CS8,   SW2_CS8,   SW3_CS8},
    {1, SW1_CS9,   SW2_CS9,   SW3_CS9},
    {1, SW1_CS10,  SW2_CS10,  SW3_CS10},
    {1, SW1_CS11,  SW2_CS11,  SW3_CS11},
    {1, SW1_CS12,  SW2_CS12,  SW3_CS12},
    {1, SW1_CS13,  SW2_CS13,  SW3_CS13},
    {1, SW1_CS14,  SW2_CS14,  SW3_CS14},
    {1, SW4_CS11,  SW5_CS11,  SW6_CS11},
    {1, SW4_CS12,  SW5_CS12,  SW6_CS12},
    {1, SW4_CS13,  SW5_CS13,  SW6_CS13},
    {1, SW7_CS7,   SW8_CS7,   SW9_CS7}, // logo1
    {1, SW7_CS16,  SW8_CS16,  SW9_CS16}, // logo2
//2
    {0, SW1_CS1,   SW2_CS1,   SW3_CS1},
    {0, SW1_CS2,   SW2_CS2,   SW3_CS2},
    {0, SW1_CS3,   SW2_CS3,   SW3_CS3},
    {0, SW1_CS4,   SW2_CS4,   SW3_CS4},
    {0, SW1_CS5,   SW2_CS5,   SW3_CS5},
    {0, SW1_CS6,   SW2_CS6,   SW3_CS6},
    {0, SW1_CS7,   SW2_CS7,   SW3_CS7},
    {0, SW1_CS8,   SW2_CS8,   SW3_CS8},
    {0, SW1_CS9,   SW2_CS9,   SW3_CS9},
    {0, SW1_CS10,  SW2_CS10,  SW3_CS10},
    {0, SW1_CS11,  SW2_CS11,  SW3_CS11},
    {0, SW1_CS12,  SW2_CS12,  SW3_CS12},
    {0, SW1_CS13,  SW2_CS13,  SW3_CS13},
    {0, SW1_CS14,  SW2_CS14,  SW3_CS14},
    {1, SW4_CS4,   SW5_CS4,   SW6_CS4},
    {1, SW4_CS5,   SW5_CS5,   SW6_CS5},
    {1, SW4_CS6,   SW5_CS6,   SW6_CS6},
    {1, SW4_CS7,   SW5_CS7,   SW6_CS7},
//3
    {0, SW4_CS1,   SW5_CS1,   SW6_CS1},
    {0, SW4_CS2,   SW5_CS2,   SW6_CS2},
    {0, SW4_CS3,   SW5_CS3,   SW6_CS3},
    {0, SW4_CS4,   SW5_CS4,   SW6_CS4},
    {0, SW4_CS5,   SW5_CS5,   SW6_CS5},
    {0, SW4_CS6,   SW5_CS6,   SW6_CS6},
    {0, SW4_CS7,   SW5_CS7,   SW6_CS7},
    {0, SW4_CS8,   SW5_CS8,   SW6_CS8},
    {0, SW4_CS9,   SW5_CS9,   SW6_CS9},
    {0, SW4_CS10,  SW5_CS10,  SW6_CS10},
    {0, SW4_CS11,  SW5_CS11,  SW6_CS11},
    {0, SW4_CS12,  SW5_CS12,  SW6_CS12},
    {0, SW4_CS13,  SW5_CS13,  SW6_CS13},
    {0, SW4_CS14,  SW5_CS14,  SW6_CS14},
    {1, SW4_CS8,   SW5_CS8,   SW6_CS8},
    {1, SW4_CS9,   SW5_CS9,   SW6_CS9},
    {1, SW4_CS10,  SW5_CS10,  SW6_CS10},
    {1, SW4_CS15,  SW5_CS15,  SW6_CS15},
//4
    {0, SW7_CS1,   SW8_CS1,   SW9_CS1},
    {0, SW7_CS2,   SW8_CS2,   SW9_CS2},
    {0, SW7_CS3,   SW8_CS3,   SW9_CS3},
    {0, SW7_CS4,   SW8_CS4,   SW9_CS4},
    {0, SW7_CS5,   SW8_CS5,   SW9_CS5},
    {0, SW7_CS6,   SW8_CS6,   SW9_CS6},
    {0, SW7_CS7,   SW8_CS7,   SW9_CS7},
    {0, SW7_CS8,   SW8_CS8,   SW9_CS8},
    {0, SW7_CS9,   SW8_CS9,   SW9_CS9},
    {0, SW7_CS10,  SW8_CS10,  SW9_CS10},
    {0, SW7_CS11,  SW8_CS11,  SW9_CS11},
    {0, SW7_CS12,  SW8_CS12,  SW9_CS12},
    {0, SW7_CS13,  SW8_CS13,  SW9_CS13},
    {1, SW7_CS4,   SW8_CS4,   SW9_CS4},
    {1, SW7_CS5,   SW8_CS5,   SW9_CS5},
    {1, SW7_CS6,   SW8_CS6,   SW9_CS6},
//5
    {0, SW10_CS1,  SW11_CS1,  SW12_CS1},
    {0, SW10_CS2,  SW11_CS2,  SW12_CS2},
    {0, SW10_CS3,  SW11_CS3,  SW12_CS3},
    {0, SW10_CS4,  SW11_CS4,  SW12_CS4},
    {0, SW10_CS5,  SW11_CS5,  SW12_CS5},
    {0, SW10_CS6,  SW11_CS6,  SW12_CS6},
    {0, SW10_CS7,  SW11_CS7,  SW12_CS7},
    {0, SW10_CS8,  SW11_CS8,  SW12_CS8},
    {0, SW10_CS9,  SW11_CS9,  SW12_CS9},
    {0, SW10_CS10, SW11_CS10, SW12_CS10},
    {0, SW10_CS11, SW11_CS11, SW12_CS11},
    {0, SW10_CS12, SW11_CS12, SW12_CS12},
    {1, SW10_CS4,  SW11_CS4,  SW12_CS4},
    {1, SW10_CS6,  SW11_CS6,  SW12_CS6},
    {1, SW10_CS8,  SW11_CS8,  SW12_CS8},
    {1, SW10_CS9,  SW11_CS9,  SW12_CS9},
    {1, SW10_CS10, SW11_CS10, SW12_CS10},
//6
    {0, SW10_CS13, SW11_CS13, SW12_CS13},
    {0, SW10_CS14, SW11_CS14, SW12_CS14},
    {0, SW10_CS15, SW11_CS15, SW12_CS15},
    {0, SW10_CS16, SW11_CS16, SW12_CS16},
    {0, SW7_CS14,  SW8_CS14,  SW9_CS14},
    {0, SW7_CS15,  SW8_CS15,  SW9_CS15},
    {0, SW7_CS16,  SW8_CS16,  SW9_CS16},
    {1, SW10_CS1,  SW11_CS1,  SW12_CS1},
    {1, SW10_CS2,  SW11_CS2,  SW12_CS2},
    {1, SW10_CS3,  SW11_CS3,  SW12_CS3},
    {1, SW10_CS5,  SW11_CS5,  SW12_CS5}
};
#endif
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
        setPinOutput(RGB_DRIVER_SDB_PIN);
        writePinHigh(RGB_DRIVER_SDB_PIN);

        setPinOutput(INDLED_POWER_PIN);
        writePinLow(INDLED_POWER_PIN);

        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // Set our LED pins as input
        writePinLow(RGB_DRIVER_SDB_PIN);
        writePinLow(LED_NUM_LOCK_PIN);
        writePinLow(LED_CAPS_LOCK_PIN);
        writePinLow(INDLED_POWER_PIN);

        led_inited = false;
    }
}

bool low_vol_off = false;

void set_led_state(void) {
    if (led_inited) {
        writePin(LED_NUM_LOCK_PIN, (host_keyboard_led_state().num_lock));
        writePin(LED_CAPS_LOCK_PIN, (host_keyboard_led_state().caps_lock));
#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
        // 充电接入
        static uint32_t charging_time = 0;
        static bool     Low_power_bink;
        static uint16_t Low_power_time;
        // bts_info.bt_info.low_vol = false;
        // Low_power_bink = false;

        if (!readPin(BT_CABLE_PIN)) {
            if (!readPin(BT_CHARGE_PIN)) {
                writePinHigh(INDLED_POWER_PIN);
            } else {
                charging_time = timer_read32();
            }
            if (charging_time && timer_elapsed(charging_time) > 500) {
                writePinLow(INDLED_POWER_PIN);
            } else {
                charging_time = 0;
            }
            if (low_vol_off) low_vol_off = false;
        } else {
            if ((bts_info.bt_info.low_vol) && readPin(BT_CABLE_PIN)) {
                // if (timer_elapsed(Low_power_time) >= 300) {
                //     Low_power_bink = !Low_power_bink;
                //     Low_power_time = timer_read32();
                // }
                if (!low_vol_off) low_vol_off = true;
                // rgb_matrix_set_color_all(0, 0, 0);
                // writePin(INDLED_POWER_PIN, Low_power_bink);
            } else {
                if (bts_info.bt_info.paired) low_vol_off = false;
                // writePinHigh(RGB_DRIVER_SDB_PIN); // 重新打开背光
                writePinLow(INDLED_POWER_PIN);
            }
            if (low_vol_off) {
                if (timer_elapsed(Low_power_time) >= 300) {
                    Low_power_bink = !Low_power_bink;
                    Low_power_time = timer_read32();
                }
                writePin(INDLED_POWER_PIN, Low_power_bink);
            }
        }
#endif
    }
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    led_deconfig_all();
    wb32_wwdg_stop();
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    led_config_all();
    wb32_wwdg_start();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    extern bool low_bat_vol_off;
    if (low_bat_vol_off) {
        bts_process_keys(keycode, 0, dev_info.devs, keymap_config.no_gui);
        bts_task(dev_info.devs);
        return false;
    }

    switch (keycode) {
#ifdef RGB_MATRIX_ENABLE
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
                rgb_matrix_set_flags(LED_FLAG_ALL);
                rgb_matrix_enable();
            }
            return false;
#endif
        default:
            break;
    }

    if (process_record_user(keycode, record) != true) {
        return false;
    }
    switch (keycode) {}
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
    set_led_state();
#endif
    matrix_scan_user();
}

void housekeeping_task_kb(void) {
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef USB_SUSPEND_CHECK_ENABLE

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
                // rgb_matrix_init();
#    endif
                wb32_wwdg_start();
            }
        }

        if ((USB_DRIVER.state != USB_ACTIVE)) {
            if (!usb_suspend_timer) {
                wb32_wwdg_stop();
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
#    ifdef RGB_DRIVER_SDB_PIN
                    writePinLow(RGB_DRIVER_SDB_PIN);
#    endif
                }
                led_deconfig_all();
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;
#    ifdef RGB_DRIVER_SDB_PIN
                writePinHigh(RGB_DRIVER_SDB_PIN);
                // rgb_matrix_init();
#    endif
                wb32_wwdg_start();
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
#    ifdef RGB_DRIVER_SDB_PIN
            writePinHigh(RGB_DRIVER_SDB_PIN);
            // rgb_matrix_init();
#    endif
            wb32_wwdg_start();
        }
    }
#endif

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}

#ifdef RGB_MATRIX_ENABLE

uint8_t rgb_test_en    = false;
uint8_t rgb_test_index = 0;

static const uint8_t rgb_test_color_table[][3] = {
    {RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS},
    {RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0, 0},
    {0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0},
    {0, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS},
};

#    define NUM_BLINK_RGBS (sizeof(blink_rgbs) / sizeof(blink_rgb_t))

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_get_flags() || low_vol_off) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    if (dev_info.rgb_white_light && !low_vol_off) {
        // 强制所有LED为白色（但亮度跟随系统）
        uint8_t brightness = rgb_matrix_get_val();
        rgb_matrix_set_color_all(brightness, brightness, brightness);
    }

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    if (rgb_test_en) {
        // clang-format off
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, rgb_test_color_table[rgb_test_index - 1][0],
            rgb_test_color_table[rgb_test_index - 1][1],
            rgb_test_color_table[rgb_test_index - 1][2]);
        }
        return false;
    }
    // clang-format on
    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    return true;
}
#endif
