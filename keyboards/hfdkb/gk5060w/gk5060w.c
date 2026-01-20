// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include "common/cw2017.h"

bool led_inited = false;

void led_config_all(void) {
    if (!led_inited) {
// Set our LED pins as output
#ifdef RGB_DRIVER_SDB_PIN
        // setPinOutput(RGB_DRIVER_SDB_PIN);
        // writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
// Set our LED pins as input
#ifdef RGB_DRIVER_SDB_PIN
        // setPinOutput(RGB_DRIVER_SDB_PIN);
        // writePinLow(RGB_DRIVER_SDB_PIN);
#endif
        led_inited = false;
    }
}

static bool grave_esc_was_shifted = false;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (get_low_vol_off()) {
        bts_process_keys(keycode, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
        return false;
    }

    switch (keycode) {
        case KC_DICTATION:
            if (dev_info.devs == DEVS_USB) {
                if (record->event.pressed) {
                    host_consumer_send(0x00CF);
                } else {
                    host_consumer_send(0);
                }
            } else {
                if (record->event.pressed) {
                    bts_send_consumer(0x00CF);
                } else {
                    bts_send_consumer(0);
                }
            }
            return false;
        case KC_DO_NOT_DISTURB:
            if (dev_info.devs == DEVS_USB) {
                if (record->event.pressed) {
                    host_system_send(0x009B);
                } else {
                    host_system_send(0);
                }
            } else {
                if (record->event.pressed) {
                    bts_send_system(0x9B);
                } else {
                    bts_send_system(0);
                }
            }
            return false;
        case KC_SPOTLIGHT:
            if (dev_info.devs == DEVS_USB) {
                if (record->event.pressed) {
                    host_consumer_send(0x0221);
                } else {
                    host_consumer_send(0);
                }
            } else {
                if (record->event.pressed) {
                    bts_send_consumer(0x221);
                } else {
                    bts_send_consumer(0);
                }
            }
            return false;

        case KC_LSFT:
        case KC_RSFT:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    grave_esc_was_shifted = true;
                } else {
                    grave_esc_was_shifted = false;
                }
            }
            break;
        case QK_GESC:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    if (grave_esc_was_shifted) {
                        bts_process_keys(KC_GRV, 1, dev_info.devs, keymap_config.no_gui, KC_NUM);
                        // bts_process_keys(KC_GRV, 1, dev_info.devs, keymap_config.no_gui);
                    } else {
                        bts_process_keys(KC_ESC, 1, dev_info.devs, keymap_config.no_gui, KC_NUM);
                        // bts_process_keys(KC_ESC, 1, dev_info.devs, keymap_config.no_gui);
                    }
                } else {
                    if (grave_esc_was_shifted) {
                        bts_process_keys(KC_GRV, 0, dev_info.devs, keymap_config.no_gui, KC_NUM);
                        // bts_process_keys(KC_GRV, 0, dev_info.devs, keymap_config.no_gui);
                    } else {
                        bts_process_keys(KC_ESC, 0, dev_info.devs, keymap_config.no_gui, KC_NUM);
                        // bts_process_keys(KC_ESC, 0, dev_info.devs, keymap_config.no_gui);
                    }
                }
                return false;
            }
            break;

        default:
            break;
    }

#ifdef BT_MODE_ENABLE
    if (process_record_bt(keycode, record) != true) {
        return false;
    }
#endif

    if (process_record_user(keycode, record) != true) {
        return false;
    }

    return true;
}

void matrix_init_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_init();
#endif
    matrix_init_user();
}

void keyboard_post_init_kb(void) {
    cw2017_init();

    // Customise these values to desired behaviour
    // debug_enable = true;
    // debug_matrix = true;
    // debug_keyboard=true;
    // debug_mouse=true;
}

void matrix_scan_kb(void) {
#ifdef BT_MODE_ENABLE
    bt_task();
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

void suspend_power_down_kb(void) {
    led_deconfig_all();
}

void suspend_wakeup_init_kb(void) {
    led_config_all();
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    // rgb_matrix_set_color(5, RGB_PINK);

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    // caps lock red
    // if ((host_keyboard_led_state().caps_lock) && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(4, 100, 100, 100);
    //     RGB_MATRIX_INDICATOR_SET_COLOR(5, 100, 100, 100);
    // }
    // GUI lock red
    // if (keymap_config.no_gui) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(73, 100, 100, 100);
    // }

    return true;
}
#endif
