// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include "usb_main.h"

// clang-format off
#ifdef RGB_MATRIX_ENABLE
#if 1
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
    {1, CB1_CA2,   CB2_CA2,   CB3_CA2},
    {1, CB4_CA11,  CB5_CA11,  CB6_CA11},
    {1, CB4_CA12,  CB5_CA12,  CB6_CA12},
    {1, CB4_CA13,  CB5_CA13,  CB6_CA13},

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
    {1, CB4_CA4,   CB5_CA4,   CB6_CA4},
    {1, CB4_CA5,   CB5_CA5,   CB6_CA5},
    {1, CB4_CA6,   CB5_CA6,   CB6_CA6},
    {1, CB4_CA7,   CB5_CA7,   CB6_CA7},

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
    {1, CB7_CA1,   CB8_CA1,   CB9_CA1},
    {1, CB7_CA2,   CB8_CA2,   CB9_CA2},
    {1, CB7_CA3,   CB8_CA3,   CB9_CA3},
    {1, CB7_CA4,   CB8_CA4,   CB9_CA4},

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
    {1, CB7_CA5,   CB8_CA5,   CB9_CA5},
    {1, CB7_CA6,   CB8_CA6,   CB9_CA6},
    {1, CB7_CA7,   CB8_CA7,   CB9_CA7},

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
    {1, CB7_CA8,   CB8_CA8,   CB9_CA8},
    {1, CB7_CA9,   CB8_CA9,   CB9_CA9},
    {1, CB7_CA10,  CB8_CA10,  CB9_CA10},
    {1, CB7_CA11,  CB8_CA11,  CB9_CA11},

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
    {1, CB7_CA12,  CB8_CA12,  CB9_CA12},
    {1, CB7_CA13,  CB8_CA13,  CB9_CA13},

    {1, CB4_CA14,  CB5_CA14,  CB6_CA14},
    {1, CB4_CA15,  CB5_CA15,  CB6_CA15},
    {1, CB4_CA16,  CB5_CA16,  CB6_CA16},
};
#else
const is31fl3733_led_t PROGMEM g_is31fl3733_leds[IS31FL3733_LED_COUNT] = {
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
    {1, SW1_CS2,   SW2_CS2,   SW3_CS2},
    {1, SW4_CS11,  SW5_CS11,  SW6_CS11},
    {1, SW4_CS12,  SW5_CS12,  SW6_CS12},
    {1, SW4_CS13,  SW5_CS13,  SW6_CS13},

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
    {1, SW4_CS1,   SW5_CS1,   SW6_CS1},
    {1, SW4_CS2,   SW5_CS2,   SW6_CS2},
    {1, SW4_CS3,   SW5_CS3,   SW6_CS3},
    {1, SW4_CS4,   SW5_CS4,   SW6_CS4},
    {1, SW4_CS5,   SW5_CS5,   SW6_CS5},
    {1, SW4_CS6,   SW5_CS6,   SW6_CS6},
    {1, SW4_CS7,   SW5_CS7,   SW6_CS7},

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
    {1, SW7_CS1,   SW8_CS1,   SW9_CS1},
    {1, SW7_CS2,   SW8_CS2,   SW9_CS2},
    {1, SW7_CS3,   SW8_CS3,   SW9_CS3},
    {1, SW7_CS4,   SW8_CS4,   SW9_CS4},

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
    {1, SW7_CS5,   SW8_CS5,   SW9_CS5},
    {1, SW7_CS6,   SW8_CS6,   SW9_CS6},
    {1, SW7_CS7,   SW8_CS7,   SW9_CS7},

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
    {1, SW7_CS8,   SW8_CS8,   SW9_CS8},
    {1, SW7_CS9,   SW8_CS9,   SW9_CS9},
    {1, SW7_CS10,  SW8_CS10,  SW9_CS10},
    {1, SW7_CS11,  SW8_CS11,  SW9_CS11},

    {0, SW10_CS13, SW11_CS13, SW12_CS13},
    {0, SW10_CS14, SW11_CS14, SW12_CS14},
    {0, SW10_CS15, SW11_CS15, SW12_CS15},
    {0, SW10_CS16, SW11_CS16, SW12_CS16},
    {0, SW7_CS15,  SW8_CS15,  SW9_CS15},
    {0, SW7_CS16,  SW8_CS16,  SW9_CS16},
    {1, SW10_CS1,  SW11_CS1,  SW12_CS1},
    {1, SW10_CS2,  SW11_CS2,  SW12_CS2},
    {1, SW10_CS3,  SW11_CS3,  SW12_CS3},
    {1, SW10_CS5,  SW11_CS5,  SW12_CS5},
    {1, SW10_CS6,  SW11_CS6,  SW12_CS6},
    {1, SW7_CS12,  SW8_CS12,  SW9_CS12},
    {1, SW7_CS13,  SW8_CS13,  SW9_CS13},

    {1, SW4_CS14,  SW5_CS14,  SW6_CS14},
    {1, SW4_CS15,  SW5_CS15,  SW6_CS15},
    {1, SW4_CS16,  SW5_CS16,  SW6_CS16},
};
#endif
#endif
// clang-format on

bool led_inited = false;

void led_config_all(void) {
    if (!led_inited) {
        // #ifdef RGB_DRIVER_SDB_PIN
        //         writePinHigh(RGB_DRIVER_SDB_PIN);
        // #endif
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // #ifdef RGB_DRIVER_SDB_PIN
        //         writePinLow(RGB_DRIVER_SDB_PIN);
        // #endif
        led_inited = false;
    }
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    // led_deconfig_all();
    // clear_keyboard();
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    // led_config_all();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (process_record_user(keycode, record) != true) {
        return false;
    }

    switch (keycode) {
        case RGB_TOG:
        case QK_RGB_MATRIX_TOGGLE:
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

            // case KC_A: {
            //     if (record->event.pressed) {
            //         // Do something when the key is pressed
            //         rgb_matrix_enable();
            //     } else {
            //         // Do something when the key is released
            //     }
            //     return false; // Return false to prevent further processing of this key
            // }

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
    // led_config_all();
#endif
}

void matrix_scan_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_task();
#endif

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;
    // static uint8_t  usb_was_connected     = false;
    static bool usb_driver_was_active = false;

    if (dev_info.devs == DEVS_USB) {
        if (usb_driver_was_active && USB_DRIVER.state != USB_ACTIVE) {
            clear_keyboard();
            layer_clear();
        }
        usb_driver_was_active = (USB_DRIVER.state == USB_ACTIVE);

        if (usb_suspend) {
            bool wakeup = false;
            for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
                if (matrix_get_row(r)) {
                    wakeup = true;
                    // clear_keyboard();
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
        // usb_was_connected = true;
    } else {
        // USB disconnected - clear all key states if we were previously connected
        // if (usb_was_connected) {
        //     clear_keyboard();
        //     layer_clear();
        //     usb_was_connected = false;
        // }

        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
#    ifdef RGB_DRIVER_SDB_PIN
            writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
        }
    }
#endif

    static uint32_t chrg_check_time = 0;
    extern void     Charge_Chat(void);
    if (timer_elapsed32(chrg_check_time) >= 2) {
        chrg_check_time = timer_read32();
        Charge_Chat();
    }
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

static bool backlight_shut_down = false;

static uint32_t low_power_entry_time = 0;
// static uint32_t low_power_exit_time  = 0;
extern bool low_vol_off;
// extern bool low_vol_shutdown;

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    if (low_vol_off) {
        if (!backlight_shut_down) {
            backlight_shut_down  = true;
            low_power_entry_time = timer_read32();
        }

        if (timer_elapsed32(low_power_entry_time) > 5000) {
            rgb_matrix_set_color_all(0, 0, 0);
        }
    } else {
        if (backlight_shut_down) {
            backlight_shut_down = false;
        }
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

#if defined(BT_CABLE_PIN) && defined(BT_CABLE_PIN)
static uint8_t  rChr_ChkBuf  = 0;
static uint8_t  rChr_OldBuf  = 0;
static uint16_t rChr_Cnt     = 0;
static uint8_t  f_ChargeOn   = 0;
static uint8_t  f_ChargeFull = 0;

#    define CHR_DEBOUNCE 100

void Charge_Chat(void) {
    uint8_t i = 0;

    if (USBLINK_Status == 0) i |= 0x01;
    if (CHARGE_Status == 1 || (dev_info.devs != DEVS_USB && bts_info.bt_info.pvol >= 100)) i |= 0x02;

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

                    if (i) {
                        f_ChargeOn = (rChr_ChkBuf & 0x01) ? 1 : 0;

                        f_ChargeFull = (rChr_ChkBuf & 0x02) ? 1 : 0;
                    }
                }
            }
        }
    }
}

bool is_charging(void) {
    return f_ChargeOn;
}

bool is_fully_charged(void) {
    return f_ChargeFull;
}
#endif
