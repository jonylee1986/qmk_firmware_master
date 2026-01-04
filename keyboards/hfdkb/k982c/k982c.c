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
#endif

static uint32_t uart_resend_interval = 0;
static uint8_t  uart_resend_times    = 0;

static void uart_resend_task(void) {
    if (uart_resend_times) {
        if (timer_elapsed32(uart_resend_interval) > 100) {
            uart_resend_interval = timer_read32();
            LCD_command_update(LCD_LIGHT_WAKEUP);
            uart_resend_times--;
        }
    }
}

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
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

#if defined(RGB_MATRIX_ENABLE) && defined(CAPS_LOCK_LED_INDEX)

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
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

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
    }
    // clang-format off
    for (uint8_t i = 0; i < 5; i++) {
        RGB_MATRIX_INDICATOR_SET_COLOR(
            ((uint8_t[]){CHRG_LOW_LED_INDEX, CAPS_LOCK_LED_INDEX, OS_LED_INDEX, NUM_LOCK_LED_INDEX, GUI_LOCK_LED_INDEX})[i],
            0, 0, 0);
    }
    // clang-format on

    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 100, 100, 100);
    }
    // if (host_keyboard_led_state().num_lock) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(NUM_LOCK_LED_INDEX, 100, 100, 100);
    // }
    // if (keymap_config.no_gui) {
    //     RGB_MATRIX_INDICATOR_SET_COLOR(GUI_LOCK_LED_INDEX, 100, 100, 100);
    // }
    if (get_highest_layer(default_layer_state) == 2) {
        RGB_MATRIX_INDICATOR_SET_COLOR(OS_LED_INDEX, 100, 100, 100);
    }
    if (keymap_config.no_gui) {
        RGB_MATRIX_INDICATOR_SET_COLOR(73, 100, 100, 100);
    }

    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    if (bt_indicator_rgb(led_min, led_max) != true) {
        return false;
    }

    return true;
}

#endif

#ifdef MM_MODE_ENABLE

void set_led_state(void) {
    static uint8_t now_led_stuts = 0;
    static uint8_t old_led_stuts = 0;

    uint8_t IND_data[3] = {0};

    IND_data[0] = 0x02;
    if (((dev_info.devs != DEVS_USB) && bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB)) {
        if (host_keyboard_led_state().num_lock) {
            now_led_stuts |= 0x01;
            IND_data[1] |= 0x01;
        } else {
            now_led_stuts &= ~0x01;
            IND_data[1] &= ~0x01;
        }
        if (host_keyboard_led_state().caps_lock) {
            now_led_stuts |= 0x02;
            IND_data[1] |= 0x02;
        } else {
            now_led_stuts &= ~0x02;
            IND_data[1] &= ~0x02;
        }
        if (host_keyboard_led_state().scroll_lock) {
            now_led_stuts |= 0x04;
            IND_data[1] |= 0x04;
        } else {
            now_led_stuts &= ~0x04;
            IND_data[1] &= ~0x04;
        }
        if (keymap_config.no_gui) {
            now_led_stuts |= 0x08;
            IND_data[1] |= 0x08;
        } else {
            now_led_stuts &= ~0x08;
            IND_data[1] &= ~0x08;
        }
    }

    if ((get_highest_layer(default_layer_state) == 2)) {
        now_led_stuts |= 0x10;
        IND_data[1] |= 0x10;
    } else {
        now_led_stuts &= ~0x10;
        IND_data[1] &= ~0x10;
    }

    now_led_stuts &= ~0xE0;
    IND_data[1] &= ~0xE0;
    switch (dev_info.devs) {
        case DEVS_HOST1: {
            now_led_stuts |= 0x20;
            IND_data[1] |= 0x20;
        } break;
        case DEVS_HOST2: {
            now_led_stuts |= 0x40;
            IND_data[1] |= 0x40;
        } break;
        case DEVS_HOST3: {
            now_led_stuts |= 0x80;
            IND_data[1] |= 0x80;
        } break;
        case DEVS_2_4G: {
            now_led_stuts |= 0xE0;
            IND_data[1] |= 0xE0;
        } break;
        default:
            break;
    }
    if (now_led_stuts != old_led_stuts) {
        old_led_stuts = now_led_stuts;
        IND_data[2]   = IND_data[0] + IND_data[1];
        uart3_transmit(IND_data, 3);
    }
    static uint16_t power_update_time = 0;
    if (timer_elapsed(power_update_time) >= 4000) {
        power_update_time = timer_read();
        LCD_IND_update();
        LCD_charge_update();
    } else {
#    if defined(MM_CABLE_PIN)
        static bool charging_old_satus = 0;
        static bool charging_now_satus = 0;

        if (!readPin(MM_CABLE_PIN)) {
            charging_now_satus = 1;
        } else {
            charging_now_satus = 0;
        }
        if (charging_old_satus != charging_now_satus) {
            charging_old_satus = charging_now_satus;
            LCD_charge_update();
        }
#    endif
    }
}

void keyboard_post_init_kb(void) {
#    ifdef RGB_MATRIX_DRIVER_SDB_PIN
    setPinOutputOpenDrain(RGB_MATRIX_DRIVER_SDB_PIN);
    writePinHigh(RGB_MATRIX_DRIVER_SDB_PIN);
#    endif

    bt_init();

    LCD_init();

    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
        eeconfig_update_keymap(&keymap_config);
    }
}

void matrix_scan_kb(void) {
    bt_task();

    matrix_scan_user();
}

void housekeeping_task_kb(void) {
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();

    if (((dev_info.devs != DEVS_USB) && !get_kb_sleep_flag()) || (dev_info.devs == DEVS_USB)) {
        set_led_state();
    }

    uart_resend_task();
}

#endif

void suspend_power_down_kb(void) {
#ifdef RGB_MATRIX_DRIVER_SDB_PIN
    writePinLow(RGB_MATRIX_DRIVER_SDB_PIN);
#endif
    // LCD_command_update(LCD_SLEEP);
    LCD_command_update(LCD_LIGHT_SLEEP);
}

void suspend_wakeup_init_kb(void) {
#ifdef RGB_MATRIX_DRIVER_SDB_PIN
    writePinHigh(RGB_MATRIX_DRIVER_SDB_PIN);
#endif
    // LCD_command_update(LCD_WAKEUP);
    // LCD_command_update(LCD_LIGHT_WAKEUP);
    uart_resend_times = 5;

    LCD_IND_update();
}
