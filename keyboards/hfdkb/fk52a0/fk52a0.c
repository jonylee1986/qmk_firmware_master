// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

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
        if (!active) {
            keymap_config.no_gui = 0;
            eeconfig_update_keymap(&keymap_config);
        }
    }
    return true;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (process_record_user(keycode, record) != true) {
        return false;
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

#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}

void suspend_power_down_kb(void) {
    // led_deconfig_all();
#ifdef RGB_DRIVER_SDB_PIN
    writePinLow(RGB_DRIVER_SDB_PIN);
#endif
}

void suspend_wakeup_init_kb(void) {
    // led_config_all();
#ifdef RGB_DRIVER_SDB_PIN
    writePinHigh(RGB_DRIVER_SDB_PIN);
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

bool rgb_matrix_indicators_kb(void) {
    if (rgb_matrix_indicators_user() != true) {
        return false;
    }
    if (bt_indicator_led() != true) {
        return false;
    }
    return true;
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
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

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    // caps lock red
    if ((host_keyboard_led_state().caps_lock) && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
        RGB_MATRIX_INDICATOR_SET_COLOR(LED_CAPS_IND_INDEX, 60, 60, 60);
    }
    // gui lock red
    if (keymap_config.no_gui && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB)) && (get_highest_layer(default_layer_state) == 0)) {
        RGB_MATRIX_INDICATOR_SET_COLOR(LED_WIN_IND_INDEX, 60, 60, 60);
    }

    return true;
}
#endif
