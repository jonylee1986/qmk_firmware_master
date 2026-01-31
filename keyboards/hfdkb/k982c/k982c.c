/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

#include "quantum.h"
#ifdef MM_MODE_ENABLE
#    include "bt_task.h"
#    include "lcd.h"
#    include "uart3.h"
#    include "usb_main.h"
#    include "uart.h"
#endif

static void    low_power_indicator(void);
static uint8_t get_pvol_from_uart(void);

bool is_charging(void);
bool is_fully_charged(void);
void Charge_Chat(void);

extern void    led_config_all(void);
extern void    led_deconfig_all(void);
extern uint8_t get_enc_blink_cnt(void);

static bool low_bat_vol     = false;
static bool low_bat_vol_off = false;

uint8_t pvol = 94;

extern bool low_vol_offed_sleep;

// static uint8_t  rChr_ChkBuf = 0;
// static uint8_t  rChr_OldBuf = 0;
// static uint16_t rChr_Cnt    = 0;
// static uint8_t  f_ChargeOn  = 0;
// static uint8_t  f_Charged   = 0;

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? 2 : 0));
        if (get_highest_layer(default_layer_state) == 2) {
            if (keymap_config.no_gui) {
                keymap_config.no_gui = false;
                eeconfig_update_keymap(&keymap_config);
            }
        }
    }
    return true;
}
#endif

#if defined(RGB_MATRIX_ENABLE)

static uint8_t color_tab[][3] = {
    {HSV_RED},    // RED
    {HSV_ORANGE}, // ORANGE
    {HSV_YELLOW}, // YELLOW
    {HSV_GREEN},  // GREEN
    {HSV_CYAN},   // CYAN
    {HSV_BLUE},   // BLUE
    {HSV_PURPLE}, // PURPLE
};

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (low_bat_vol_off) {
        bts_process_keys(keycode, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
        return false;
    }

    switch (keycode) {
#    ifdef RGB_MATRIX_ENABLE
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
#    endif
        case RM_HUEU:
            if (record->event.pressed) {
                if (dev_info.rgb_test_en) {
                    dev_info.rgb_test_en = 0;
                }
                dev_info.color_index++;
                if (dev_info.color_index >= sizeof(color_tab) / sizeof(color_tab[0])) {
                    dev_info.color_index = 0;
                }
                rgb_matrix_config.hsv.h = color_tab[dev_info.color_index][0];
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        case RM_HUED:
            if (record->event.pressed) {
                if (dev_info.rgb_test_en) {
                    dev_info.rgb_test_en = 0;
                }
                if (dev_info.color_index == 0) {
                    dev_info.color_index = sizeof(color_tab) / sizeof(color_tab[0]) - 1;
                } else {
                    dev_info.color_index--;
                }
                rgb_matrix_config.hsv.h = color_tab[dev_info.color_index][0];
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        case RM_PREV:
        case RM_NEXT:
            if (record->event.pressed) {
                if (dev_info.rgb_test_en) {
                    dev_info.rgb_test_en = 0;
                    eeconfig_update_user(dev_info.raw);
                }
            }
            return true;

        default:
            break;
    }

    if (!process_record_user(keycode, record)) {
        return false;
    }

#    ifdef MM_MODE_ENABLE
    if (process_record_bt(keycode, record) != true) {
        return false;
    }
#    endif

    return true;
}

void keyboard_post_init_kb(void) {
    // Apply the saved color
#    ifdef RGB_MATRIX_ENABLE
    rgb_matrix_config.hsv.h = color_tab[dev_info.color_index][0];
#    endif

    // Call existing initialization
    LCD_init();

    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
        eeconfig_update_keymap(&keymap_config);
    }

    LCD_IND_update();
    LCD_charge_update();
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if ((rgb_matrix_get_flags() == LED_FLAG_NONE) || low_bat_vol) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    rgb_matrix_set_color(LED_PWR_LOW_INDEX, 0, 0, 0);

    if (dev_info.rgb_test_en) {
        uint8_t brightness = rgb_matrix_get_val();
        for (uint8_t i = 0; i <= 83; i++) {
            rgb_matrix_set_color(i, brightness, brightness, brightness);
        }
    }

    if (keymap_config.no_gui) {
        rgb_matrix_set_color(73, 60, 60, 60);
    }

    // static uint32_t full_charged_time = 0;

    // if (!get_enc_blink_cnt()) {
    //     if (!dev_info.ind_toggle) {
    //         if (is_charging()) {
    //             low_bat_vol     = false;
    //             low_bat_vol_off = false;

    //             rgb_matrix_set_color(LED_PWR_LOW_INDEX, 100, 0, 0);
    //             full_charged_time = timer_read32();
    //         } else {
    //             if (is_fully_charged()) {
    //                 if (timer_elapsed32(full_charged_time) < 5000) {
    //                     rgb_matrix_set_color(LED_PWR_LOW_INDEX, 0, 100, 0);
    //                 }
    //             } else {
    //                 if (pvol <= 10) {
    //                     low_bat_vol = true;
    //                 }
    //                 if (pvol < 1) {
    //                     low_bat_vol_off = true;
    //                 }

    //                 low_power_indicator();
    //             }
    //         }
    //     }
    // }

    // static uint32_t full_charged_time = 0;
    // static uint32_t entry_full_time   = 0;

    // if (f_ChargeOn) {
    //     low_bat_vol     = false;
    //     low_bat_vol_off = false;

    //     if (f_Charged && pvol < 100) {
    //         entry_full_time   = timer_read32();
    //         full_charged_time = timer_read32();
    //         rgb_matrix_set_color(LED_PWR_LOW_INDEX, 100, 0, 0);
    //     } else {
    //         if (timer_elapsed32(entry_full_time) >= 2000) {
    //             if (timer_elapsed32(full_charged_time) < 5000) {
    //                 rgb_matrix_set_color(LED_PWR_LOW_INDEX, 0, 100, 0);
    //             }
    //         }
    //     }
    // } else {
    //     entry_full_time   = timer_read32();
    //     full_charged_time = timer_read32();

    //     if (pvol <= 10) {
    //         low_bat_vol = true;
    //     }
    //     if (pvol < 1) {
    //         low_bat_vol_off = true;
    //     }

    //     if (dev_info.devs != DEVS_USB) low_power_indicator();
    // }

    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }

    return true;
}

#endif

void set_led_state(void) {
    static uint8_t now_led_stuts = 0;
    static uint8_t old_led_stuts = 0;

    uint8_t IND_data[5] = {0};

    IND_data[0] = 0xAA;
    IND_data[1] = 0x02;

    if (host_keyboard_led_state().num_lock) {
        now_led_stuts |= 0x01;
        IND_data[2] |= 0x01;
    } else {
        now_led_stuts &= ~0x01;
        IND_data[2] &= ~0x01;
    }
    if (host_keyboard_led_state().caps_lock) {
        if (dev_info.devs == DEVS_USB && USB_DRIVER.state != USB_ACTIVE) {
            now_led_stuts &= ~0x02;
            IND_data[2] &= ~0x02;
        } else {
            now_led_stuts |= 0x02;
            IND_data[2] |= 0x02;
        }
    } else {
        now_led_stuts &= ~0x02;
        IND_data[2] &= ~0x02;
    }
    if (host_keyboard_led_state().scroll_lock) {
        now_led_stuts |= 0x04;
        IND_data[2] |= 0x04;
    } else {
        now_led_stuts &= ~0x04;
        IND_data[2] &= ~0x04;
    }
    if (keymap_config.no_gui) {
        now_led_stuts |= 0x08;
        IND_data[2] |= 0x08;
    } else {
        now_led_stuts &= ~0x08;
        IND_data[2] &= ~0x08;
    }

    if ((get_highest_layer(default_layer_state) == 2)) {
        now_led_stuts |= 0x10;
        IND_data[2] |= 0x10;
    } else {
        now_led_stuts &= ~0x10;
        IND_data[2] &= ~0x10;
    }

    now_led_stuts &= ~0xE0;
    IND_data[2] &= ~0xE0;
    switch (dev_info.devs) {
        case DEVS_HOST1: {
            now_led_stuts |= 0x20;
            IND_data[2] |= 0x20;
        } break;
        case DEVS_HOST2: {
            now_led_stuts |= 0x40;
            IND_data[2] |= 0x40;
        } break;
        case DEVS_HOST3: {
            now_led_stuts |= 0x80;
            IND_data[2] |= 0x80;
        } break;
        case DEVS_2_4G: {
            now_led_stuts |= 0xE0;
            IND_data[2] |= 0xE0;
        } break;
        default:
            break;
    }
    if (now_led_stuts != old_led_stuts) {
        old_led_stuts = now_led_stuts;
        IND_data[3]   = IND_data[1] + IND_data[2];
        IND_data[4]   = 0xBB;
        uart3_transmit(IND_data, 5);
    }
    static uint16_t power_update_time = 0;
    if (timer_elapsed(power_update_time) >= 4000) {
        power_update_time = timer_read();

        pvol = get_pvol_from_uart();

        LCD_IND_update();
        LCD_charge_update();
    } else {
#if defined(MM_CABLE_PIN)

        static bool charging_old_satus = false;
        static bool charging_now_satus = false;

        static uint32_t full_charged_time = 0;
        static uint32_t entry_full_time   = 0;
        // static uint32_t entry_charge_time = 0;

        if (!readPin(MM_CABLE_PIN)) {
            charging_now_satus = 1;

            low_bat_vol     = false;
            low_bat_vol_off = false;

            if (!readPin(MM_CHARGE_PIN) && pvol < 100) {
                // if (timer_elapsed(entry_charge_time) > 500) {
                entry_full_time   = timer_read32();
                full_charged_time = timer_read32();
                rgb_matrix_set_color(LED_PWR_LOW_INDEX, 100, 0, 0);
                // }
            } else {
                if (timer_elapsed32(entry_full_time) >= 2000) {
                    // entry_charge_time = timer_read32();
                    if (timer_elapsed32(full_charged_time) < 5000) {
                        rgb_matrix_set_color(LED_PWR_LOW_INDEX, 0, 100, 0);
                    }
                }
            }
        } else {
            charging_now_satus = 0;

            // entry_charge_time = timer_read32();
            entry_full_time   = timer_read32();
            full_charged_time = timer_read32();

            if (dev_info.devs != DEVS_USB) {
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

                low_power_indicator();
            }
        }
        if (charging_old_satus != charging_now_satus) {
            charging_old_satus = charging_now_satus;
            LCD_charge_update();
        }

        // static uint32_t full_charged_time = 0;
        // static uint32_t entry_full_time   = 0;

        // if (!get_enc_blink_cnt()) {
        //     if (!dev_info.ind_toggle) {
        //         if (is_charging()) {
        //             low_bat_vol     = false;
        //             low_bat_vol_off = false;

        //             rgb_matrix_set_color(LED_PWR_LOW_INDEX, 100, 0, 0);
        //             full_charged_time = timer_read32();
        //             entry_full_time   = timer_read32();
        //         } else {
        //             if (is_fully_charged()) {
        //                 if (timer_elapsed32(entry_full_time) >= 2000) {
        //                     if (timer_elapsed32(full_charged_time) < 5000) {
        //                         rgb_matrix_set_color(LED_PWR_LOW_INDEX, 0, 100, 0);
        //                     }
        //                 }
        //             } else {
        //                 if (pvol <= 10) {
        //                     low_bat_vol = true;
        //                 }
        //                 if (pvol < 1) {
        //                     low_bat_vol_off = true;
        //                 }

        //                 low_power_indicator();
        //             }
        //         }
        //     }
        // }

#endif
    }

    if (!get_enc_blink_cnt()) {
        if (dev_info.ind_toggle || low_bat_vol_off) {
            writePinLow(LED_CAPS_LOCK_IND_PIN);
            writePinLow(LED_MAC_OS_IND_PIN);
        } else {
            if (dev_info.devs == DEVS_USB && USB_DRIVER.state != USB_ACTIVE) {
                writePinLow(LED_CAPS_LOCK_IND_PIN);
                writePinLow(LED_MAC_OS_IND_PIN);
            } else {
                writePin(LED_CAPS_LOCK_IND_PIN, host_keyboard_led_state().caps_lock);
                if ((get_highest_layer(default_layer_state) == 2)) {
                    writePinHigh(LED_MAC_OS_IND_PIN);
                } else {
                    writePinLow(LED_MAC_OS_IND_PIN);
                }
            }
        }
    }
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

        uart_transmit(uart_data_send, 3);

        return uart_data_read[1];
    }

    return 94;
}

static void low_power_indicator(void) {
    static bool     Low_power_bink = false;
    static uint32_t Low_power_time = 0;

    if (low_bat_vol && !low_bat_vol_off) {
        if (timer_elapsed32(Low_power_time) >= 1000) {
            Low_power_bink = !Low_power_bink;
            Low_power_time = timer_read32();
        }
        if (Low_power_bink) {
            rgb_matrix_set_color(LED_PWR_LOW_INDEX, 100, 0, 0);
        } else {
            rgb_matrix_set_color(LED_PWR_LOW_INDEX, 0, 0, 0);
        }
    } else {
        Low_power_bink = 0;
    }

    if (low_bat_vol_off) {
        low_vol_offed_sleep = true;
        if (timer_elapsed32(get_key_press_time()) > 2000) {
            set_kb_sleep_flag(true);
        }
    }
}

void matrix_init_kb(void) {
    bt_init();

    matrix_init_user();
}

void matrix_scan_kb(void) {
    bt_task();

    // if (low_bat_vol_off) {
    //     bts_process_keys(KC_NO, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
    //     bts_task(dev_info.devs);
    //     while (bts_is_busy()) {
    //         wait_ms(1);
    //     }
    // }

    matrix_scan_user();
}

static uint32_t usb_suspend_timer = 0;
static uint32_t usb_suspend       = false;
static bool     LCD_Sleep_Flag    = false;

void housekeeping_task_kb(void) {
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();

    // clang-format off
    if ( ((dev_info.devs != DEVS_USB) && !get_kb_sleep_flag()) \
    || ((dev_info.devs == DEVS_USB)) ) {
        set_led_state();
    }
    // clang_format on

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
#    ifdef RGB_MATRIX_DRIVER_SDB_PIN
                writePinHigh(RGB_MATRIX_DRIVER_SDB_PIN);
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
#    ifdef RGB_MATRIX_DRIVER_SDB_PIN
                    writePinLow(RGB_MATRIX_DRIVER_SDB_PIN);
#    endif

                    writePinLow(LED_CAPS_LOCK_IND_PIN);
                    writePinLow(LED_MAC_OS_IND_PIN);

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

#    ifdef RGB_MATRIX_DRIVER_SDB_PIN
                writePinHigh(RGB_MATRIX_DRIVER_SDB_PIN);
#    endif
                LCD_command_update(LCD_LIGHT_WAKEUP);
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
            LCD_Sleep_Flag    = false;
#    ifdef RGB_MATRIX_DRIVER_SDB_PIN
            writePinHigh(RGB_MATRIX_DRIVER_SDB_PIN);
#    endif
            LCD_command_update(LCD_LIGHT_WAKEUP);
        }
    }
#endif

    // static uint32_t charg_chat_time = 0;
    // if (timer_elapsed32(charg_chat_time) >= 2) {
    //     charg_chat_time = timer_read32();
    //     Charge_Chat();
    // }
}

void suspend_power_down_kb(void) {
#ifdef RGB_MATRIX_DRIVER_SDB_PIN
    // writePinLow(RGB_MATRIX_DRIVER_SDB_PIN);
#endif
    if (!LCD_Sleep_Flag) {
        LCD_Sleep_Flag = true;
        LCD_command_update(LCD_LIGHT_SLEEP);
    }

    writePinLow(LED_CAPS_LOCK_IND_PIN);
    writePinLow(LED_MAC_OS_IND_PIN);
    // led_deconfig_all();
}

void suspend_wakeup_init_kb(void) {
#ifdef RGB_MATRIX_DRIVER_SDB_PIN
    // writePinHigh(RGB_MATRIX_DRIVER_SDB_PIN);
#endif
    if (LCD_Sleep_Flag) {
        LCD_Sleep_Flag = false;
        LCD_command_update(LCD_LIGHT_WAKEUP);
    }

    // writePin(LED_CAPS_LOCK_IND_PIN, host_keyboard_led_state().caps_lock);

    // LCD_IND_update();
}

#if defined(MM_CABLE_PIN) && defined(MM_CHARGE_PIN)


#    define CHR_DEBOUNCE 100

void Charge_Chat(void) {
    // uint8_t i = 0;

    // if (USBLINK_Status == 0) i |= 0x01;
    // if (CHARGE_Status == 0)  i |= 0x02;

    // if (rChr_ChkBuf != i) {
    //     rChr_Cnt    = CHR_DEBOUNCE;
    //     rChr_ChkBuf = i;
    // } else {
    //     if (rChr_Cnt != 0) {
    //         rChr_Cnt--;
    //         if (rChr_Cnt == 0) {
    //             i = rChr_ChkBuf ^ rChr_OldBuf;

    //             if (i != 0) {
    //                 rChr_OldBuf = rChr_ChkBuf;

    //                 if (i & 0x3) {
    //                     f_ChargeOn = (rChr_ChkBuf & 0x01) ? 1 : 0;
    //                     f_Charged = (rChr_ChkBuf & 0x02) ? 1 : 0;
    //                 }
    //             }
    //         }
    //     }
    // }
}
#endif
