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
#include "lib/lib8tion/lib8tion.h"
#ifdef MULTI_MODE_ENABLE
#    include "common/bt_task.h"
#endif

static uint8_t  all_blink_cnt   = 0;
static uint32_t all_blink_time  = 0;
static RGB      all_blink_color = {0};

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

        case SLED_TOG:
            if (record->event.pressed) {
                dev_info.SLed_info.toggle ^= dev_info.SLed_info.toggle;
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_MOD:
            if (record->event.pressed) {
                dev_info.SLed_info.mode = (dev_info.SLed_info.mode + 1) % SLED_MODE_COUNT;
                eeconfig_update_user(dev_info.raw);
#ifdef CONSOLE_ENABLE
                uprintf("sled mode: %d\r\n", dev_info.SLed_info.mode);
#endif
            }
            return false;

        case SLED_HUI:
            if (record->event.pressed) {
                dev_info.SLed_info.color = (dev_info.SLed_info.color == COLOR_WHITE) ? COLOR_RAINBOW : (dev_info.SLed_info.color + 1);
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_VAI:
            if (record->event.pressed) {
                if (dev_info.SLed_info.brightness >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    dev_info.SLed_info.brightness = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
                    all_blink_cnt                 = 6;
                    all_blink_color               = (RGB){RGB_WHITE};
                    all_blink_time                = timer_read32();
                } else {
                    dev_info.SLed_info.brightness += RGB_MATRIX_VAL_STEP;
                }
                // dev_info.SLed_info.brightness = qadd8(dev_info.SLed_info.brightness, RGB_MATRIX_VAL_STEP);
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_VAD:
            if (record->event.pressed) {
                if (dev_info.SLed_info.brightness <= RGB_MATRIX_VAL_STEP) {
                    dev_info.SLed_info.brightness = 0;
                    all_blink_cnt                 = 6;
                    all_blink_color               = (RGB){RGB_WHITE};
                    all_blink_time                = timer_read32();
                } else {
                    dev_info.SLed_info.brightness -= RGB_MATRIX_VAL_STEP;
                }
                // dev_info.SLed_info.brightness = qsub8(dev_info.SLed_info.brightness, RGB_MATRIX_VAL_STEP);
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_SPI:
            if (record->event.pressed) {
                // dev_info.SLed_info.speed = qadd8(dev_info.SLed_info.speed, RGB_MATRIX_SPD_STEP);
                if (dev_info.SLed_info.speed >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    dev_info.SLed_info.speed = UINT8_MAX;
                    all_blink_cnt            = 6;
                    all_blink_color          = (RGB){RGB_WHITE};
                    all_blink_time           = timer_read32();
                } else {
                    dev_info.SLed_info.speed += RGB_MATRIX_SPD_STEP;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_SPD:
            if (record->event.pressed) {
                // dev_info.SLed_info.speed = qsub8(dev_info.SLed_info.speed, RGB_MATRIX_SPD_STEP);
                if (dev_info.SLed_info.speed <= RGB_MATRIX_SPD_STEP) {
                    dev_info.SLed_info.speed = 0;
                    all_blink_cnt            = 6;
                    all_blink_color          = (RGB){RGB_WHITE};
                    all_blink_time           = timer_read32();
                } else {
                    dev_info.SLed_info.speed -= RGB_MATRIX_SPD_STEP;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case RM_VALU:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){RGB_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case RM_VALD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){RGB_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case RM_SPDU:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){RGB_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case RM_SPDD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){RGB_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

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

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color_all(RGB_OFF);
    }

    if (!rgb_matrix_indicators_user()) {
        return false;
    }

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(LED_CAPS_LOCK, RGB_WHITE);
    }

    if (keymap_config.no_gui) {
        rgb_matrix_set_color(LED_WIN_LOCK, RGB_CYAN);
    }

    return true;
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    if (!dev_info.SLed_info.toggle && rgb_matrix_get_flags()) {
        SLed_task();
    }

#ifdef CONSOLE_ENABLE
    for (uint8_t i = 83; i < 130; i++) {
        rgb_matrix_set_color(i, RGB_RED);
    }
#endif

    if (!bt_indicator_rgb(led_min, led_max)) {
        return false;
    }

    if (all_blink_cnt) {
        rgb_matrix_set_color_all(0, 0, 0);
        if (timer_elapsed32(all_blink_time) > 300) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt & 0x1) {
            rgb_matrix_set_color_all(all_blink_color.r, all_blink_color.g, all_blink_color.b);
        }
    }

    return true;
}

void eeconfig_init_kb(void) {
    SLed_eeconfig_init();

    eeconfig_init_user();
}