// Copyright 2026 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "usb_main.h"
#include "uart.h"
// clang-format off

#ifdef RGB_MATRIX_ENABLE
const snled27351_led_t PROGMEM g_snled27351_leds[SNLED27351_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *   driver
 *   |  R location
 *   |  |      G location
 *   |  |      |      B location
 *   |  |      |      | */
    {1, CB1_CA1,   CB2_CA1,   CB3_CA1},
    {1, CB1_CA2,   CB2_CA2,   CB3_CA2},
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
    {1, CB4_CA4,   CB5_CA4,   CB6_CA4},

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
    {1, CB4_CA5,   CB5_CA5,   CB6_CA5},
    {1, CB4_CA6,   CB5_CA6,   CB6_CA6},
    {1, CB4_CA7,   CB5_CA7,   CB6_CA7},
    {1, CB4_CA8,   CB5_CA8,   CB6_CA8},

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
    {1, CB7_CA1,   CB8_CA1,   CB9_CA1},
    {1, CB7_CA2,   CB8_CA2,   CB9_CA2},
    {1, CB7_CA3,   CB8_CA3,   CB9_CA3},

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
    {1, CB10_CA1,  CB11_CA1,  CB12_CA1},
    {1, CB7_CA4,   CB8_CA4,   CB9_CA4},
    {1, CB7_CA5,   CB8_CA5,   CB9_CA5},
    {1, CB7_CA6,   CB8_CA6,   CB9_CA6},
    {1, CB7_CA7,   CB8_CA7,   CB9_CA7},

    {0, CB10_CA13, CB11_CA13, CB12_CA13},
    {0, CB10_CA14, CB11_CA14, CB12_CA14},
    {0, CB10_CA15, CB11_CA15, CB12_CA15},
    {0, CB10_CA16, CB11_CA16, CB12_CA16},
    {0, CB7_CA15,  CB8_CA15,  CB9_CA15},
    {0, CB7_CA16,  CB8_CA16,  CB9_CA16},
    {0, CB4_CA15,  CB5_CA15,  CB6_CA15},
    {0, CB4_CA16,  CB5_CA16,  CB6_CA16},
    {1, CB10_CA2,  CB11_CA2,  CB12_CA2},
    {1, CB10_CA3,  CB11_CA3,  CB12_CA3},
    {1, CB10_CA4,  CB11_CA4,  CB12_CA4},
    {1, CB10_CA5,  CB11_CA5,  CB12_CA5},
};
#endif
// clang-format on

bool led_inited = false;

uint8_t pvol = 94;

bool low_bat_vol_off = false;
bool low_bat_vol     = false;

static uint32_t usb_suspend_timer = 0;
static uint32_t usb_suspend       = false;
static bool     LCD_Sleep_Flag    = false;

#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)

static uint8_t  rChr_ChkBuf = 0;
static uint8_t  rChr_OldBuf = 0;
static uint16_t rChr_Cnt    = 0;

uint8_t f_ChargeOn = 0;
uint8_t f_Charged  = 0;

#    define CHR_DEBOUNCE 100

void Charge_Chat(void) {
    uint8_t i = 0;

    if (readPin(BT_CABLE_PIN)) i |= 0x01;
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
                        f_ChargeOn = (rChr_ChkBuf & 0x01) ? 1 : 0;
                        f_Charged  = (rChr_ChkBuf & 0x02) ? 1 : 0;
                    }
                }
            }
        }
    }
}
#endif

void led_config_all(void) {
    if (!led_inited) {
        // Set our LED pins as output
        setPinOutput(A14);
        if (dev_info.devs == DEVS_USB) {
            writePinLow(A14);
        } else {
            writePinHigh(A14);
        }
        // setPinOutputOpenDrain(RGB_DRIVER_SDB_PIN);
        // writePinHigh(RGB_DRIVER_SDB_PIN);
        // setPinOutput(LED_WIN_LOCK_PIN); // LDE2 Caps Lock
        // writePinLow(LED_WIN_LOCK_PIN);
        // setPinOutput(LED_MAC_MODE_PIN); // LDE2 Caps Lock
        // writePinLow(LED_MAC_MODE_PIN);
        // LCD_start();
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // Set our LED pins as input
        // writePinLow(RGB_DRIVER_SDB_PIN);
        // writePinLow(LED_NUM_LOCK_PIN);
        // writePinLow(LED_CAPS_LOCK_PIN);
        // writePinLow(LED_WIN_LOCK_PIN);
        // writePinLow(LED_MAC_MODE_PIN);
        // LCD_stop();
        led_inited = false;
    }
}

// 拨动开关选择系统模式
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? 2 : 0));
    }
    if (active) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
    return true;
}

void blink_led_set(uint8_t index);
typedef struct {
    uint8_t  index;
    uint32_t start_time;
    uint32_t total_time;
    uint32_t node_time;
    uint32_t interval;
    uint8_t  flip;
} blink_led_t;
blink_led_t blink_leds = {.index = 0xFF, .interval = 200, .total_time = 2200, .start_time = 0, .node_time = 0}; // ALL

void blink_led_set(uint8_t index) {
    blink_leds.index      = index;
    blink_leds.start_time = timer_read32();
    blink_leds.node_time  = timer_read32();
}
bool blink_led_advanced(void) {
    if (!led_inited) {
        return false;
    }
    if (blink_leds.start_time != 0) {
        if (timer_elapsed32(blink_leds.start_time) <= blink_leds.total_time) {
            if (timer_elapsed32(blink_leds.node_time) >= blink_leds.interval) {
                blink_leds.flip      = !blink_leds.flip;
                blink_leds.node_time = timer_read32();
                switch (blink_leds.index) {
                    // case 1: {
                    //     writePin(LED_NUM_LOCK_PIN, !blink_leds.flip);
                    // } break;
                    // case 2: {
                    //     writePin(LED_CAPS_LOCK_PIN, !blink_leds.flip);
                    // } break;
                    // case 3: {
                    //     writePin(LED_WIN_LOCK_PIN, !blink_leds.flip);
                    // } break;
                    // case 0xFF: {
                    //     writePin(LED_NUM_LOCK_PIN, !blink_leds.flip);
                    //     writePin(LED_CAPS_LOCK_PIN, !blink_leds.flip);
                    //     writePin(LED_WIN_LOCK_PIN, !blink_leds.flip);
                    // } break;
                    default:
                        break;
                }
            }
        } else {
            blink_leds.start_time = 0;
            blink_leds.node_time  = 0;
            // writePin(LED_NUM_LOCK_PIN, (host_keyboard_led_state().num_lock));
            // writePin(LED_CAPS_LOCK_PIN, (host_keyboard_led_state().caps_lock));
            // writePin(LED_WIN_LOCK_PIN, keymap_config.no_gui);
        }
    }
    return true;
}

static uint8_t get_pvol_from_uart(void) {
    uint8_t uart_data_read[3] = {0};
    uint8_t uart_data_send[3] = {0};

    if (uart3_available()) {
        uart3_receive(uart_data_read, 3);
    }
    if ((uart_data_read[0] == 0xA7) && (uart_data_read[2] == ((uart_data_read[0] + uart_data_read[1]) & 0xFF))) {
        uart_data_send[0] = uart_data_read[0];
        uart_data_send[1] = uart_data_read[1];
        uart_data_send[2] = (uart_data_send[0] + uart_data_send[1]) & 0xFF;

        if (dev_info.devs == DEVS_USB || ((dev_info.devs != DEVS_USB) && !get_kb_sleep_flag() && bts_info.bt_info.paired)) {
            uart_transmit(uart_data_send, 3);
        }

        return uart_data_read[1];
    }

    return 94;
}

void set_led_state(void) {
    static uint8_t now_led_stuts = 0;
    static uint8_t old_led_stuts = 0;

    if (host_keyboard_led_state().num_lock)
        now_led_stuts |= 0x01;
    else
        now_led_stuts &= ~0x01;
    if (host_keyboard_led_state().caps_lock)
        now_led_stuts |= 0x02;
    else
        now_led_stuts &= ~0x02;
    if (host_keyboard_led_state().scroll_lock)
        now_led_stuts |= 0x04;
    else
        now_led_stuts &= ~0x04;
    if (keymap_config.no_gui)
        now_led_stuts |= 0x08;
    else
        now_led_stuts &= ~0x08;
    if ((get_highest_layer(default_layer_state) == 2))
        now_led_stuts |= 0x10;
    else
        now_led_stuts &= ~0x10;
    now_led_stuts &= ~0xE0;
    switch (dev_info.devs) {
        case DEVS_HOST1: {
            now_led_stuts |= 0x20;
        } break;
        case DEVS_HOST2: {
            now_led_stuts |= 0x40;
        } break;
        case DEVS_HOST3: {
            now_led_stuts |= 0x80;
        } break;
        case DEVS_2_4G: {
            now_led_stuts |= 0xE0;
        } break;
        default:
            break;
    }
    if (led_inited) {
        if (now_led_stuts != old_led_stuts) {
            old_led_stuts = now_led_stuts;
            LCD_IND_update();
        }
    }
    static uint32_t power_update_time = 0;
    if (timer_elapsed32(power_update_time) >= 4000) {
        power_update_time = timer_read32();

        pvol = get_pvol_from_uart();

        LCD_charge_update();
        LCD_IND_update();
    } else {
#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
        static bool charging_old_satus = false;
        static bool charging_now_satus = false;

        if (readPin(BT_CABLE_PIN)) {
            charging_now_satus = 1;

            low_bat_vol     = false;
            low_bat_vol_off = false;
        } else {
            if (pvol <= 10) {
                low_bat_vol = true;
            } else {
                low_bat_vol = false;
            }

            if (pvol < 1) {
                low_bat_vol_off = true;
            } else {
                low_bat_vol_off = false;
            }

            charging_now_satus = 0;
        }
        if (charging_old_satus != charging_now_satus) {
            charging_old_satus = charging_now_satus;
            LCD_charge_update();
        }
#endif
    }
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    if (!LCD_Sleep_Flag) {
        LCD_Sleep_Flag = true;
        LCD_command_update(LCD_LIGHT_SLEEP);
    }
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    // led_config_all();
    if (LCD_Sleep_Flag) {
        LCD_Sleep_Flag = false;
        LCD_command_update(LCD_LIGHT_WAKEUP);
    }
    // set_led_state();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (low_bat_vol_off) {
        bts_process_keys(keycode, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        bts_task(dev_info.devs);

        return false;
    }

    if (process_record_user(keycode, record) != true) {
        return false;
    }

    switch (keycode) {
        case LCD_HOME: {
            if (record->event.pressed) {
                dev_info.LCD_PAGE = 0;
                LCD_Page_update(dev_info.LCD_PAGE);
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }
        case LCD_PAGE: {
            if (record->event.pressed) {
                dev_info.LCD_PAGE = !dev_info.LCD_PAGE;
                LCD_Page_update(dev_info.LCD_PAGE);
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }
        case RM_VALU: {
            if (record->event.pressed && (rgb_matrix_get_val() == RGB_MATRIX_MAXIMUM_BRIGHTNESS)) {
                blink_led_set(3);
                dprintf("brightness out in\r\n");
            }
        } break;
        case RM_VALD: {
            if (record->event.pressed && (rgb_matrix_get_val() == 0x00)) {
                blink_led_set(3);
                dprintf("brightness out in\r\n");
            }
        } break;
        case RM_SPDU: {
            if (record->event.pressed && (rgb_matrix_get_speed() == 0xff)) {
                blink_led_set(3);
                dprintf("speed out in\r\n");
            }
        } break;
        case RM_SPDD: {
            if (record->event.pressed && (rgb_matrix_get_speed() == 0x00)) {
                blink_led_set(3);
                dprintf("speed out in\r\n");
            }
        } break;
        case KC_ESC: {
            if (record->event.pressed) {
                LCD_command_update(3);
            }
        } break;

        case RM_TOGG: {
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
    LCD_init();
    led_config_all();
#endif
    matrix_init_user();
}

void matrix_scan_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_task();
    set_led_state();
    blink_led_advanced();
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

#ifdef USB_SUSPEND_CHECK_ENABLE
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
                LCD_Sleep_Flag    = false;
                usb_suspend_timer = 0;
#    ifdef RGB_DRIVER_SDB_PIN
                writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
                LCD_command_update(LCD_LIGHT_WAKEUP);
                LCD_IND_update();
                LCD_charge_update();
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

                    LCD_command_update(LCD_LIGHT_SLEEP);
                    if (!LCD_Sleep_Flag) {
                        LCD_Sleep_Flag = true;
                    }
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;
                LCD_Sleep_Flag    = false;

#    ifdef RGB_DRIVER_SDB_PIN
                writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
                LCD_command_update(LCD_LIGHT_WAKEUP);
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
            LCD_Sleep_Flag    = false;
#    ifdef RGB_DRIVER_SDB_PIN
            writePinHigh(RGB_DRIVER_SDB_PIN);
#    endif
            LCD_command_update(LCD_LIGHT_WAKEUP);
        }
    }
#endif

    static uint32_t charg_chat_time = 0;
    if (timer_elapsed32(charg_chat_time) >= 2) {
        charg_chat_time = timer_read32();
        Charge_Chat();
    }

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}

#ifdef RGB_MATRIX_ENABLE
bool    rgb_test_en    = false;
uint8_t rgb_test_index = 0;

static const uint8_t rgb_test_color_table[][3] = {
    {RGB_WHITE},
    {RGB_RED},
    {RGB_GREEN},
    {RGB_BLUE},
};

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_get_flags() || low_bat_vol) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
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

    if (host_keyboard_led_state().caps_lock && ((bts_info.bt_info.paired) || USB_DRIVER.state == USB_ACTIVE)) {
        rgb_matrix_set_color(50, 0x77, 0, 0);
    }
    if (host_keyboard_led_state().num_lock && ((bts_info.bt_info.paired) || USB_DRIVER.state == USB_ACTIVE)) {
        rgb_matrix_set_color(28, 0x77, 0, 0);
    }
    if (keymap_config.no_gui) {
        rgb_matrix_set_color(84, 0x77, 0, 0);
    }

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    return true;
}
#endif
