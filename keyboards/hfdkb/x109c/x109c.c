// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// #endif
// clang-format on
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (!active ? 0 : 2));
        if (!active) {
            keymap_config.no_gui = 0;
        }
    }
    return true;
}

bool via_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
    switch (*command_id) {
        case 0x07: {
            if (data[1] == 0x03 && data[2] == 0x02 && data[3] == 0x00) {
                extern bool rgb_status_save;
                rgb_status_save = !rgb_matrix_config.enable;
                // return false;
            }
            return false;
        }
        default:
            break;
    }
    return false;
}

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
        // setPinOutputOpenDrain(RGB_DRIVER_SDB_PIN);
        // writePinHigh(RGB_DRIVER_SDB_PIN);
        setPinOutputPushPull(WS2812_EN_PIN);
        writePinHigh(WS2812_EN_PIN);
        // setPinOutput(LED_MAC_OS_PIN); // LDE2 MAC\WIN
        // writePinLow(LED_MAC_OS_PIN);
        // setPinOutput(LED_WIN_LOCK_PIN); // LED3 Win Lock
        // writePinLow(LED_WIN_LOCK_PIN);
        // writePin(LED_CAPS_LOCK_PIN, host_keyboard_led_state().caps_lock);
        // writePin(LED_MAC_OS_PIN, default_layer_state & (1<<2));
        // writePin(LED_WIN_LOCK_PIN, keymap_config.no_gui);

        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // Set our LED pins as input
        setPinOutputPushPull(WS2812_EN_PIN);
        writePinLow(WS2812_EN_PIN);
        // writePinLow(LED_CAPS_LOCK_PIN);
        // writePinLow(LED_MAC_OS_PIN);
        // writePinLow(LED_WIN_LOCK_PIN);

        // writePinLow(RGB_DRIVER_SDB_PIN);
        led_inited = false;
    }
}

void set_led_state(void) {
    static uint8_t now_led_stuts;
    static uint8_t old_led_stuts;
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
        // extern uint32_t EE_CLR_press_cnt;
        extern bool EE_CLR_flag;
        if (EE_CLR_flag) {
            // writePin(LED_CAPS_LOCK_PIN, EE_CLR_press_cnt & 0x1);
            // writePin(LED_MAC_OS_PIN, EE_CLR_press_cnt & 0x1);
            // writePin(LED_WIN_LOCK_PIN, EE_CLR_press_cnt & 0x1);
        } else {
            // writePin(LED_CAPS_LOCK_PIN, host_keyboard_led_state().caps_lock);
            // writePin(LED_MAC_OS_PIN, default_layer_state & (1<<2));
            // writePin(LED_WIN_LOCK_PIN, keymap_config.no_gui);
            if (now_led_stuts != old_led_stuts) {
                old_led_stuts = now_led_stuts;
                LCD_IND_update();
            }
        }
    }
    static uint16_t power_update_time;
    static uint16_t query_vol_time;
    if (!get_kb_sleep_flag() && timer_elapsed(query_vol_time) >= 4000 && bts_info.bt_info.paired) {
        query_vol_time = timer_read();
        bts_send_vendor(v_query_vol);
    }
    if (timer_elapsed(power_update_time) >= 4000) {
        power_update_time = timer_read();
        LCD_IND_update();
        LCD_charge_update();
        // LCD_DATA_receive();
        // lcd_bat_receive();
    } else {
#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
        // 充电接入
        static bool charging_old_satus;
        static bool charging_now_satus;
        if (!readPin(BT_CABLE_PIN)) {
            charging_now_satus = 1;
        } else {
            charging_now_satus = 0;
        }
        if (charging_old_satus != charging_now_satus) {
            charging_old_satus = charging_now_satus;
            LCD_charge_update();
            // LCD_DATA_receive();
            // lcd_cable_send();
        }
#endif
        // LCD_DATA_receive();
    }
}
bool LCD_DONT_SEND;
void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    led_deconfig_all();
    LCD_command_update(LCD_SLEEP);
    LCD_DONT_SEND = 1;
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    led_config_all();
    LCD_DONT_SEND = 0;
    LCD_command_update(LCD_WEAKUP);
    set_led_state();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
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
                dev_info.LCD_PAGE = 1;
                LCD_Page_update(dev_info.LCD_PAGE);
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }
        case LCD_SW: {
            if (record->event.pressed) {
                LCD_command_update(LCD_SWITCH);
            }
            return false;
        }
        // case KC_VOLD:
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_VOLD, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_task(dev_info.devs);
        //         while (bts_is_busy()) {
        //         }
        //     }
        //     if (record->event.pressed) {
        //         LCD_vol_update(false);
        //     }
        //     return true;
        // case KC_VOLU:
        //     if (dev_info.devs) {
        //         bts_process_keys(KC_VOLU, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        //         bts_task(dev_info.devs);
        //         while (bts_is_busy()) {
        //         }
        //     }
        //     if (record->event.pressed) {
        //         LCD_vol_update(true);
        //     }
        //     return true;
        case KC_END: {
            if (record->event.pressed) {
                extern uint8_t rgb_test_en;
                if (rgb_test_en) {
                    rgb_test_en = false;
                    return false;
                }
            }
        } break;
        case KC_DOWN: {
            if (record->event.pressed) {
                extern uint8_t rgb_test_en;
                extern uint8_t rgb_test_index;
                if (rgb_test_en) {
                    rgb_test_index++;
                    if (rgb_test_index > 4) rgb_test_index = 1;
                    return false;
                }
            }
        } break;
    }
#ifdef BT_MODE_ENABLE
    if (process_record_bt(keycode, record) != true) {
        return false;
    }
#endif
    return true;
}

void lp_recovery_hook(void) {
    // extern void open_rgb(void);

    // bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
    // open_rgb();
}

void matrix_init_kb(void) {
#ifdef WS2812_EN_PIN
    setPinOutput(WS2812_EN_PIN);
    writePinHigh(WS2812_EN_PIN);
#endif
#ifdef RGB_DRIVER_SDB_PIN
    setPinOutputOpenDrain(RGB_DRIVER_SDB_PIN);
    writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
    LCD_init();
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
#ifdef CONSOLE_ENABLE
    debug_enable = true;
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

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }
#    ifdef BT_MODE_ENABLE

#    endif
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

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    // led_cap_lock_set();
    // led_gui_lock_set();

    // caps lock red
    // if ((host_keyboard_led_state().caps_lock) && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(48, 160, 160, 160);
    // }

    // GUI lock red
    // if (keymap_config.no_gui) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(75, 160, 160, 160);
    // }

    return true;
}
#endif
