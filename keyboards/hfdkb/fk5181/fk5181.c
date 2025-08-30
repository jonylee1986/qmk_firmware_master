// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include <stdlib.h>

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
        setPinOutputPushPull(INDLED_BT1_PIN);
        writePinLow(INDLED_BT1_PIN);
        setPinOutputPushPull(INDLED_BT2_PIN);
        writePinLow(INDLED_BT2_PIN);
        setPinOutputPushPull(INDLED_BT3_PIN);
        writePinLow(INDLED_BT3_PIN);
        setPinOutputPushPull(INDLED_VOL_PIN);
        writePinLow(INDLED_VOL_PIN);
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // // Set our LED pins as input
        writePinLow(INDLED_BT1_PIN);
        writePinLow(INDLED_BT2_PIN);
        writePinLow(INDLED_BT3_PIN);
        writePinLow(INDLED_VOL_PIN);
        writePinLow(LED_CAPS_LOCK_PIN);

        led_inited = false;
    }
}

void set_led_state(void) {
    if (led_inited) {
        bt_indicator_led();
    }
}
extern bool ALL_KEY_LOCK;

uint16_t dip_pressed_time;
bool     dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    switch (index) {
        case 0:
            default_layer_set(1UL << (active ? 2 : 0));
            if (active) {
                ALL_KEY_LOCK         = 0;
                keymap_config.no_gui = 0;
                eeconfig_update_keymap(&keymap_config);
            }
            break;
        default:
            break;
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
    switch (keycode) {
        case BT_SW: {
            if (record->event.pressed) {
                if (dip_pressed_time == 0) {
                    dip_pressed_time = timer_read();
                }
            } else {
                if (dip_pressed_time && (timer_elapsed(dip_pressed_time) < 1500)) {
                    if (dev_info.devs == DEVS_USB) {
                        bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                    } else if (dev_info.devs == DEVS_HOST1) {
                        bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                    } else if (dev_info.devs == DEVS_HOST2) {
                        bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                    } else if (dev_info.devs == DEVS_HOST3) {
                        bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                    } else if (dev_info.devs == DEVS_2_4G) {
                        bt_switch_mode(dev_info.devs, DEVS_USB, false);
                    }
                }
                dip_pressed_time = 0;
            }
        } break;
        case C(KC_UP):
            if (dev_info.devs) {
                bts_process_keys(KC_LCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_UP, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case BRGB_TOG:
            if (record->event.pressed) {
                dev_info.rgb_toggle = !dev_info.rgb_toggle;
                eeconfig_update_user(dev_info.raw);
            }
            break;
    }
    return true;
}

// void lp_recovery_hook(void) {
//     extern void open_rgb(void);
//     bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
//     open_rgb();
// }

void matrix_init_kb(void) {
#ifdef WS2812_EN_PIN
    setPinOutput(WS2812_EN_PIN);
    writePinLow(WS2812_EN_PIN);
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
    housekeeping_task_user();
    if (dip_pressed_time && (timer_elapsed(dip_pressed_time) >= 3000)) {
        dip_pressed_time = 0;
        if (dev_info.devs != DEVS_USB) {
            bt_switch_mode(dev_info.devs, dev_info.devs, true);
            writePinLow(LED_CAPS_LOCK_PIN);
        }
    }
#ifdef BT_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}
// clang-format off
#ifdef RGB_MATRIX_ENABLE

// clang-format on

uint8_t              rgb_test_en               = false;
uint8_t              rgb_test_index            = 0;
static const uint8_t rgb_test_color_table[][3] = {
    {RGB_RED},
    {RGB_GREEN},
    {RGB_BLUE},
    {RGB_WHITE},
};

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    if (keymap_config.no_gui) rgb_matrix_set_color(82, 0xFF, 0xFF, 0xFF); // L_WIN

    if ((rgb_test_en) && (rgb_test_index > 0)) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, rgb_test_color_table[rgb_test_index - 1][0], rgb_test_color_table[rgb_test_index - 1][1], rgb_test_color_table[rgb_test_index - 1][2]);
        }

        return false;
    }
    if (dev_info.rgb_toggle) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif
    return true;
}
#endif
