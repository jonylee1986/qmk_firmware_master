// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include "usb_main.h"
// clang-format off

#ifdef RGB_MATRIX_ENABLE
const snled27351_led_t PROGMEM g_snled27351_leds[SNLED27351_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *   driver
 *   |   R location
 *   |   |          G location
 *   |   |          |          B location
 *   |   |          |          | */
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
    {1, CB4_CA11,  CB5_CA11,  CB6_CA11},
    {1, CB4_CA12,  CB5_CA12,  CB6_CA12},
    {1, CB4_CA13,  CB5_CA13,  CB6_CA13},
    {1, CB7_CA7,   CB8_CA7,   CB9_CA7}, // logo1
    {1, CB7_CA16,  CB8_CA16,  CB9_CA16}, // logo2
//2
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
    {1, CB4_CA4,   CB5_CA4,   CB6_CA4},
    {1, CB4_CA5,   CB5_CA5,   CB6_CA5},
    {1, CB4_CA6,   CB5_CA6,   CB6_CA6},
    {1, CB4_CA7,   CB5_CA7,   CB6_CA7},
//3
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
    {1, CB4_CA15,  CB5_CA15,  CB6_CA15},
//4
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
    {1, CB7_CA4,   CB8_CA4,   CB9_CA4},
    {1, CB7_CA5,   CB8_CA5,   CB9_CA5},
    {1, CB7_CA6,   CB8_CA6,   CB9_CA6},
//5
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
    {1, CB10_CA6,  CB11_CA6,  CB12_CA6},
    {1, CB10_CA8,  CB11_CA8,  CB12_CA8},
    {1, CB10_CA9,  CB11_CA9,  CB12_CA9},
    {1, CB10_CA10, CB11_CA10, CB12_CA10},
//6
    {0, CB10_CA13, CB11_CA13, CB12_CA13},
    {0, CB10_CA14, CB11_CA14, CB12_CA14},
    {0, CB10_CA15, CB11_CA15, CB12_CA15},
    {0, CB10_CA16, CB11_CA16, CB12_CA16},
    {0, CB7_CA14,  CB8_CA14,  CB9_CA14},
    {0, CB7_CA15,  CB8_CA15,  CB9_CA15},
    {0, CB7_CA16,  CB8_CA16,  CB9_CA16},
    {1, CB10_CA1,  CB11_CA1,  CB12_CA1},
    {1, CB10_CA2,  CB11_CA2,  CB12_CA2},
    {1, CB10_CA3,  CB11_CA3,  CB12_CA3},
    {1, CB10_CA5,  CB11_CA5,  CB12_CA5}
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
        // setPinOutput(RGB_DRIVER_SDB_PIN);
        // writePinHigh(RGB_DRIVER_SDB_PIN);
        // rgb_matrix_init();

        setPinOutput(INDLED_POWER_PIN);
        writePinLow(INDLED_POWER_PIN);

        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // Set our LED pins as input
        // writePinLow(RGB_DRIVER_SDB_PIN);
        writePinLow(LED_NUM_LOCK_PIN);
        writePinLow(LED_CAPS_LOCK_PIN);
        writePinLow(INDLED_POWER_PIN);

        led_inited = false;
    }
}

#define CHR_DEBOUNCE 100

static uint8_t  rChr_ChkBuf = 0;
static uint8_t  rChr_OldBuf = 0;
static uint16_t rChr_Cnt    = 0;
// static uint8_t  f_ChargeOn  = 0;
static uint8_t f_Charged = 0;

void Charge_Chat(void) {
    uint8_t i = 0;

    // if (!readPin(BT_CABLE_PIN)) i |= 0x01;
    if (!readPin(BT_CHARGE_PIN)) i |= 0x02;

    if (rChr_ChkBuf != i) {
        rChr_Cnt    = CHR_DEBOUNCE;
        rChr_ChkBuf = i;
    } else {
        if (rChr_Cnt != 0) {
            rChr_Cnt--;
            if (rChr_Cnt == 0) {
                i = rChr_ChkBuf ^ rChr_OldBuf;

                if (i != 0) {
                    rChr_OldBuf = rChr_ChkBuf;

                    if (i & 0x3) {
                        // f_ChargeOn = (rChr_ChkBuf & 0x01) ? 1 : 0;
                        f_Charged = (rChr_ChkBuf & 0x02) ? 1 : 0;
                    }
                }
            }
        }
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
        // static uint32_t full_time     = 0;
        static bool     Low_power_bink;
        static uint16_t Low_power_time;

        if (!readPin(BT_CABLE_PIN)) {
            if (f_Charged) {
                writePinHigh(INDLED_POWER_PIN);
                charging_time = timer_read32();
            } else {
                if (timer_elapsed32(charging_time) > 1000) {
                    writePinLow(INDLED_POWER_PIN);
                }
            }

            // if (charging_time && timer_elapsed(charging_time) > 1000) {
            //     writePinLow(INDLED_POWER_PIN);
            // } else {
            //     charging_time = 0;
            // }

            if (low_vol_off) low_vol_off = false;
        } else {
            if ((bts_info.bt_info.low_vol) && readPin(BT_CABLE_PIN)) {
                if (!low_vol_off) low_vol_off = true;
            } else {
                writePinLow(INDLED_POWER_PIN);
            }
            if (low_vol_off) {
                if (timer_elapsed(Low_power_time) >= 300) {
                    Low_power_bink = !Low_power_bink;
                    Low_power_time = timer_read();
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
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    led_config_all();
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

    static uint32_t charg_chat_time = 0;
    if (timer_elapsed32(charg_chat_time) >= 2) {
        charg_chat_time = timer_read32();
        Charge_Chat();
    }

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
                rgb_matrix_init();
#    endif
            }
        }

        if ((USB_DRIVER.state != USB_ACTIVE)) {
            if (!usb_suspend_timer) {
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
                rgb_matrix_init();
#    endif
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
#    ifdef RGB_DRIVER_SDB_PIN
            writePinHigh(RGB_DRIVER_SDB_PIN);
            rgb_matrix_init();
#    endif
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
        uint8_t brightness = (rgb_matrix_get_val() - rgb_matrix_get_val() / 4);
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

void keyboard_post_init_kb(void) {
    snled27351_pwm_phase_delay_enable(SNLED27351_I2C_ADDRESS_1);
    snled27351_pwm_phase_delay_enable(SNLED27351_I2C_ADDRESS_2);

    keyboard_post_init_user();
}
