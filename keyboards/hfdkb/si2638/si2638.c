// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// clang-format off

#ifdef RGB_MATRIX_ENABLE
const is31_led PROGMEM g_is31_leds[RGB_MATRIX_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *   driver
 *   |   R location
 *   |   |     G location
 *   |   |     |     B location
 *   |   |     |     | */
    {1, A_1,   B_1,   C_1},
    {1, A_3,   B_3,   C_3},
    {1, A_4,   B_4,   C_4},
    {1, A_5,   B_5,   C_5},
    {1, A_6,   B_6,   C_6},
    {1, A_7,   B_7,   C_7},
    {1, A_8,   B_8,   C_8},
    {1, A_9,   B_9,   C_9},
    {1, A_10,  B_10,  C_10},
    {1, A_11,  B_11,  C_11},
    {1, A_12,  B_12,  C_12},
    {1, A_13,  B_13,  C_13},
    {1, A_14,  B_14,  C_14},
    {1, D_11,  E_11,  F_11},
    {1, D_12,  E_12,  F_12},
    {1, D_13,  E_13,  F_13},
    {1, G_7,   H_7,   I_7},
    {1, G_16,  H_16,  I_16},
//2
    {0, A_1,   B_1,   C_1},
    {0, A_2,   B_2,   C_2},
    {0, A_3,   B_3,   C_3},
    {0, A_4,   B_4,   C_4},
    {0, A_5,   B_5,   C_5},
    {0, A_6,   B_6,   C_6},
    {0, A_7,   B_7,   C_7},
    {0, A_8,   B_8,   C_8},
    {0, A_9,   B_9,   C_9},
    {0, A_10,  B_10,  C_10},
    {0, A_11,  B_11,  C_11},
    {0, A_12,  B_12,  C_12},
    {0, A_13,  B_13,  C_13},
    {0, A_14,  B_14,  C_14},
    {1, D_4,   E_4,   F_4},
    {1, D_5,   E_5,   F_5},
    {1, D_6,   E_6,   F_6},
    {1, D_7,   E_7,   F_7},
//3
    {0, D_1,   E_1,   F_1},
    {0, D_2,   E_2,   F_2},
    {0, D_3,   E_3,   F_3},
    {0, D_4,   E_4,   F_4},
    {0, D_5,   E_5,   F_5},
    {0, D_6,   E_6,   F_6},
    {0, D_7,   E_7,   F_7},
    {0, D_8,   E_8,   F_8},
    {0, D_9,   E_9,   F_9},
    {0, D_10,  E_10,  F_10},
    {0, D_11,  E_11,  F_11},
    {0, D_12,  E_12,  F_12},
    {0, D_13,  E_13,  F_13},
    {0, D_14,  E_14,  F_14},
    {1, D_8,   E_8,   F_8},
    {1, D_9,   E_9,   F_9},
    {1, D_10,  E_10,  F_10},
    {1, D_15,  E_15,  F_15},


//4
    {0, G_1,   H_1,   I_1},
    {0, G_2,   H_2,   I_2},
    {0, G_3,   H_3,   I_3},
    {0, G_4,   H_4,   I_4},
    {0, G_5,   H_5,   I_5},
    {0, G_6,   H_6,   I_6},
    {0, G_7,   H_7,   I_7},
    {0, G_8,   H_8,   I_8},
    {0, G_9,   H_9,   I_9},
    {0, G_10,  H_10,  I_10},
    {0, G_11,  H_11,  I_11},
    {0, G_12,  H_12,  I_12},
    {0, G_13,  H_13,  I_13},
    {1, G_4,   H_4,   I_4},
    {1, G_5,   H_5,   I_5},
    {1, G_6,   H_6,   I_6},


//5
    {0, J_1,   K_1,   L_1},
    {0, J_2,   K_2,   L_2},
    {0, J_3,   K_3,   L_3},
    {0, J_4,   K_4,   L_4},
    {0, J_5,   K_5,   L_5},
    {0, J_6,   K_6,   L_6},
    {0, J_7,   K_7,   L_7},
    {0, J_8,   K_8,   L_8},
    {0, J_9,   K_9,   L_9},
    {0, J_10,  K_10,  L_10},
    {0, J_11,  K_11,  L_11},
    {0, J_12,  K_12,  L_12},
    {1, J_4,   K_4,   L_4},
    {1, J_6,   K_6,   L_6},
    {1, J_8,   K_8,   L_8},
    {1, J_9,   K_9,   L_9},
    {1, J_10,  K_10, L_10},

//6
    {0, J_13,  K_13,  L_13},
    {0, J_14,  K_14,  L_14},
    {0, J_15,  K_15,  L_15},
    {0, J_16,  K_16,  L_16},

    {0, G_14,  H_14,  I_14},
    {0, G_15,   H_15,   I_15},
    {0, G_16,  H_16,  I_16},

    {1, J_1,  K_1,  L_1},
    {1, J_2,  K_2,  L_2},
    {1, J_3,  K_3,  L_3},
    {1, J_5,  K_5,  L_5}

};
#endif
// clang-format on

// void led_init_ports(void) {
//     // Set our LED pins as output
//     // setPinOutput(D2); // Num Lock
//     // writePinLow(D2);
//     // setPinOutput(C11); // Scroll Lock
//     // writePinLow(C11);
//     setPinOutput(C10); // Caps Lock
//     writePinLow(C10);
// }

// bool led_update_kb(led_t led_state) {
//     bool res = led_update_user(led_state);
//     if(res) {
//         // writePin(D2, led_state.num_lock);
//         writePin(C10, led_state.caps_lock);
//         // writePin(C11, led_state.scroll_lock);
//     }
//     return res;
// }

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

        // setPinInput(A14);
        // writePinLow(A14);
        led_inited = false;
    }
}
// 拨动开关选择系统模式
// bool dip_switch_update_kb(uint8_t index, bool active) {
//     if (!dip_switch_update_user(index, active)) {
//         return false;
//     }
//     if (index == 0) {
//         default_layer_set(1UL << ((!active) ? 2 : 0));
//     }
//     if (!active) {
//         keymap_config.no_gui = 0;
//         eeconfig_update_keymap(keymap_config.raw);
//     }
//     return true;
// }


void set_led_state(void) {
    if (led_inited) {
        writePin(LED_NUM_LOCK_PIN, (host_keyboard_led_state().num_lock));
        writePin(LED_CAPS_LOCK_PIN, (host_keyboard_led_state().caps_lock));
#    if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
// 充电接入
        static uint32_t charging_time = 0;
        static bool     Low_power_bink;
        static uint16_t Low_power_time;
        bts_info.bt_info.low_vol = false;
        Low_power_bink = false;

        if (!readPin(BT_CABLE_PIN))
        {

            {
                if(!readPin(BT_CHARGE_PIN)){
                    writePinHigh(INDLED_POWER_PIN);
                }else
                {
                    charging_time = timer_read32();
                }
                if(charging_time&&timer_elapsed(charging_time)>500)
                {
                    writePinLow(INDLED_POWER_PIN);
                }
                else
                {
                   charging_time=0;
                }

            }

        }
        else
        {
            if ((bts_info.bt_info.low_vol) && readPin(BT_CABLE_PIN)) {

                if (timer_elapsed(Low_power_time) >= 300) {
                    Low_power_bink = !Low_power_bink;
                    Low_power_time = timer_read32();
                }
                    rgb_matrix_set_color_all(0, 0, 0);
                    writePin(INDLED_POWER_PIN, Low_power_bink);
                }
                else
                {
                    writePinHigh(RGB_DRIVER_SDB_PIN);           //重新打开背光
                    writePinLow(INDLED_POWER_PIN);
                }
            }
#    endif
    }
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    led_deconfig_all();

}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    led_config_all();
    // led_inited = true;  // 确保 LED 控制逻辑执行
    // set_led_state();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
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

void lp_recovery_hook(void) {
    // extern void open_rgb(void);

    // bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
    // open_rgb();
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

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}

#ifdef RGB_MATRIX_ENABLE


uint8_t rgb_test_en = false;
uint8_t rgb_test_index = 0;

static const uint8_t rgb_test_color_table[][3] = {
    {RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS},
    {RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0, 0},
    {0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0},
    {0, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS},
};

#    define NUM_BLINK_RGBS (sizeof(blink_rgbs) / sizeof(blink_rgb_t))

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
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

    // // caps lock red
    // if ((host_keyboard_led_state().caps_lock) && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(44, 160, 160, 160);
    // }
    // // GUI lock red
    // if (keymap_config.no_gui) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(73, 160, 160, 160);
    // }

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    return true;
}
#endif
