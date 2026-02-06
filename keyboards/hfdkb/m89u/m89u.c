// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "bt_task.h"

// clang-format off

#ifdef RGB_MATRIX_ENABLE
const snled27351_led_t PROGMEM g_snled27351_leds[SNLED27351_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *   driver
 *   |   R location
 *   |   |        G location
 *   |   |        |        B location
 *   |   |        |        | */

    {0, CB1_CA1, CB2_CA1, CB3_CA1},
    {0, CB1_CA2, CB2_CA2, CB3_CA2},
    {0, CB1_CA3, CB2_CA3, CB3_CA3},
    {0, CB1_CA4, CB2_CA4, CB3_CA4},

    {0, CB1_CA5, CB2_CA5, CB3_CA5},
    {0, CB1_CA6, CB2_CA6, CB3_CA6},
    {0, CB1_CA7, CB2_CA7, CB3_CA7},
    {0, CB1_CA8, CB2_CA8, CB3_CA8},

    {0, CB4_CA1, CB5_CA1, CB6_CA1},
    {0, CB4_CA2, CB5_CA2, CB6_CA2},
    {0, CB4_CA3, CB5_CA3, CB6_CA3},
    {0, CB4_CA4, CB5_CA4, CB6_CA4},

    {0, CB4_CA5, CB5_CA5, CB6_CA5},
    {0, CB4_CA6, CB5_CA6, CB6_CA6},
    {0, CB4_CA7, CB5_CA7, CB6_CA7},
    {0, CB4_CA8, CB5_CA8, CB6_CA8},

    {0, CB7_CA1, CB8_CA1, CB9_CA1},
    {0, CB7_CA2, CB8_CA2, CB9_CA2},
    {0, CB7_CA3, CB8_CA3, CB9_CA3},
    {0, CB7_CA4, CB8_CA4, CB9_CA4},

    {0, CB7_CA5, CB8_CA5, CB9_CA5},
    {0, CB7_CA6, CB8_CA6, CB9_CA6},

    {0, CB1_CA9, CB2_CA9, CB3_CA9},
};
#endif
// clang-format on

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // extern bool key_eql_pressed;
    // if (key_eql_pressed) {
    //     return false;
    // }

    if (!process_record_user(keycode, record)) {
        return false;
    }

    switch (keycode) {
        // case QK_UNDERGLOW_TOGGLE:
        case RGB_TOG:
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
        default:
            break;
    }

    return true;
}
