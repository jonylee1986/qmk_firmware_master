/* Copyright (C) 2025 jonylee@hfd
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

#include QMK_KEYBOARD_H
#include "lcd_drv/lcd.h"
#include "lcd_drv/uart3.h"
#ifdef MULTI_MODE_ENABLE
#    include "common/bt_task.h"
#endif

void set_led_state(void);

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }

    if (!process_record_bt(keycode, record)) {
        return false;
    }

    switch (keycode) {
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
        default:
            break;
    }
    return true;
}

void keyboard_pre_init_kb() {
#ifdef RGB_MATRIX_SDB_PIN
    setPinOutputPushPull(RGB_MATRIX_SDB_PIN);
    writePinHigh(RGB_MATRIX_SDB_PIN);
#endif
    keyboard_pre_init_user();
}

void set_led_state(void) {
    LCD_IND_update();
}

void matrix_scan_kb(void) {
#ifdef MULTI_MODE_ENABLE
    bt_task();
#endif
    // set_led_state();
    matrix_scan_user();
}

void matrix_init_kb(void) {
#ifdef MULTI_MODE_ENABLE
    bt_init();
#endif
    matrix_init_user();
}

void keyboard_post_init_kb(void) {
    uart3_init(115200);
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    set_led_state();
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    if (!bt_indicator_rgb(led_min, led_max)) {
        return false;
    }

    return true;
}
