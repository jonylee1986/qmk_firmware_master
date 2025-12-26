// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"

bool led_inited = false;

void led_config_all(void) {
    if (!led_inited) {
        // Set our LED pins as output
        setPinOutput(RGB_DRIVER_SDB_PIN);
        writePinHigh(RGB_DRIVER_SDB_PIN);
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        // Set our LED pins as input
        writePinLow(RGB_DRIVER_SDB_PIN);
        led_inited = false;
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (process_record_user(keycode, record) != true) {
        return false;
    }
    switch (keycode) {
        case KC_DICT:
            if (dev_info.devs == DEVS_USB) {
                if (record->event.pressed) {
                    host_consumer_send(0x00CF);
                } else {
                    host_consumer_send(0x0000);
                }
            } else {
                if (record->event.pressed) {
                    bts_send_consumer(0x00CF);
                } else {
                    bts_send_consumer(0x0000);
                }
            }
            break;
        case KC_DNDT:
            if (dev_info.devs == DEVS_USB) {
                if (record->event.pressed) {
                    host_system_send(0x009B);
                } else {
                    host_system_send(0x0000);
                }
            } else {
                if (record->event.pressed) {
                    bts_send_system(0x9B);
                } else {
                    bts_send_system(0x00);
                }
            }
            break;

        case A(KC_TAB):
            if (dev_info.devs) {
                bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_TAB, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case A(S(KC_TAB)):
            if (dev_info.devs) {
                bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_TAB, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case G(KC_E):
            if (dev_info.devs) {
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_E, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case G(KC_L):
            if (dev_info.devs) {
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_L, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case G(KC_UP):
            if (dev_info.devs) {
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_UP, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case G(KC_SPC):
            if (dev_info.devs) {
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_SPC, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case A(G(KC_SPC)):
            if (dev_info.devs) {
                bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_SPC, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
        case G(S(KC_3)):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_process_keys(KC_3, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
            return true; // Skip all further processing of this key
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
    // #ifdef RGB_DRIVER_SDB_PIN
    //     setPinOutputOpenDrain(RGB_DRIVER_SDB_PIN);
    //     writePinHigh(RGB_DRIVER_SDB_PIN);
    // #endif

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

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    rgb_matrix_set_color_all(RGB_BLACK);

    // rgb_matrix_set_color(5, 160, 160, 160);

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

#    ifdef BT_MODE_ENABLE
    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }
#    endif

    // caps lock red
    if ((host_keyboard_led_state().caps_lock) && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
        RGB_MATRIX_INDICATOR_SET_COLOR(4, 100, 100, 100);
    }
    // GUI lock red
    // if (keymap_config.no_gui) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(73, 100, 100, 100);
    // }

    return true;
}
#endif
