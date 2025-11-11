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
#include "usb_main.h"

static uint8_t  all_blink_cnt   = 0;
static uint32_t all_blink_time  = 0;
static RGB      all_blink_color = {0};
static bool     lcd_toggle      = false;

#define ALARM_COLOR_WHITE 0x64, 0x64, 0x64

void set_led_state(void);

extern bool low_battery_vol_off;

extern void led_config_all(void);
extern void led_deconfig_all(void);

uint32_t uart_resend_interval = 0;
uint8_t  uart_resend_times    = 0;

void uart_resend_task(void) {
    if (uart_resend_times) {
        if (timer_elapsed32(uart_resend_interval) > 50) {
            uart_resend_interval = timer_read32();
            LCD_command_update(LCD_LIGHT_WAKEUP);
            uart_resend_times--;
        }
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (low_battery_vol_off) {
        // clear_keyboard();
        bts_process_keys(keycode, 0, dev_info.devs, keymap_config.no_gui);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
        return false;
    }

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
                dev_info.enable = !dev_info.enable;
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_MOD:
            if (record->event.pressed) {
                dev_info.mode = (dev_info.mode + 1) % SLED_MODE_COUNT;
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_HUI:
            if (record->event.pressed) {
                dev_info.color = (dev_info.color == COLOR_WHITE) ? COLOR_RAINBOW : (dev_info.color + 1);
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_VAI:
            if (record->event.pressed) {
                if (dev_info.brightness >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    dev_info.brightness = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
                    all_blink_cnt       = 6;
                    all_blink_color     = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time      = timer_read32();
                } else {
                    dev_info.brightness += RGB_MATRIX_VAL_STEP;
                }
                // dev_info.brightness = qadd8(dev_info.brightness, RGB_MATRIX_VAL_STEP);
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_VAD:
            if (record->event.pressed) {
                if (dev_info.brightness <= RGB_MATRIX_VAL_STEP) {
                    dev_info.brightness = 0;
                    all_blink_cnt       = 6;
                    all_blink_color     = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time      = timer_read32();
                } else {
                    dev_info.brightness -= RGB_MATRIX_VAL_STEP;
                }
                // dev_info.brightness = qsub8(dev_info.brightness, RGB_MATRIX_VAL_STEP);
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_SPI:
            if (record->event.pressed) {
                // dev_info.speed = qadd8(dev_info.speed, RGB_MATRIX_SPD_STEP);
                if (dev_info.speed >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    dev_info.speed  = UINT8_MAX;
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time  = timer_read32();
                } else {
                    dev_info.speed += RGB_MATRIX_SPD_STEP;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case SLED_SPD:
            if (record->event.pressed) {
                // dev_info.speed = qsub8(dev_info.speed, RGB_MATRIX_SPD_STEP);
                if (dev_info.speed <= RGB_MATRIX_SPD_STEP) {
                    dev_info.speed  = 0;
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time  = timer_read32();
                } else {
                    dev_info.speed -= RGB_MATRIX_SPD_STEP;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case RM_VALU:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case RM_VALD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case RM_SPDU:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case RM_SPDD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    all_blink_cnt   = 6;
                    all_blink_color = (RGB){ALARM_COLOR_WHITE};
                    all_blink_time  = timer_read32();
                }
            }
            return true;

        case LCD_TOGG:
            if (record->event.pressed) {
                lcd_toggle = !lcd_toggle;
                if (lcd_toggle) {
                    LCD_command_update(LCD_LIGHT_SLEEP);
                } else {
                    LCD_command_update(LCD_LIGHT_WAKEUP);
                }
            }
            return false;

        case LCD_LEFT:
            if (record->event.pressed) {
                if (dev_info.LCD_PAGE > 0) {
                    dev_info.LCD_PAGE--;
                    LCD_Page_update(dev_info.LCD_PAGE);
                } else if (dev_info.LCD_PAGE == 0) {
                    dev_info.LCD_PAGE = 1;
                    LCD_Page_update(dev_info.LCD_PAGE);
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;

        case LCD_RIGHT:
            if (record->event.pressed) {
                if (dev_info.LCD_PAGE < 1) {
                    dev_info.LCD_PAGE++;
                    LCD_Page_update(dev_info.LCD_PAGE);
                } else if (dev_info.LCD_PAGE >= 1) {
                    dev_info.LCD_PAGE = 0;
                    LCD_Page_update(dev_info.LCD_PAGE);
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;
#ifdef CONSOLE_ENABLE
        case KC_A: {
            if (record->event.pressed) {
                // dev_info.raw = eeconfig_read_user();
                uprintf("dev: %d, last_dev: %d\n", dev_info.devs, dev_info.last_devs);
                uprintf("bri: %d, spd: %d, col: %d, mod: %d, enb: %d\n", dev_info.brightness, dev_info.speed, dev_info.color, dev_info.mode, dev_info.enable);
            }
            return true;
        }
#endif

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

    uart3_init(115200);

    keyboard_pre_init_user();
}

void set_led_state(void) {
    static uint8_t now_led_stuts = 0;
    static uint8_t old_led_stuts = 0;
    uint8_t        IND_data[3]   = {0};

    if (low_battery_vol_off) {
        return;
    }

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

    if ((get_highest_layer(default_layer_state) == 3)) {
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
    // if (get_led_inited()) {
    if (now_led_stuts != old_led_stuts) {
        old_led_stuts = now_led_stuts;
        // LCD_IND_update();
        IND_data[2] = IND_data[0] + IND_data[1];
        uart3_transmit(IND_data, 3);
    }
    // }
    static uint16_t power_update_time;
    if (timer_elapsed(power_update_time) >= 4000) {
        power_update_time = timer_read();
        LCD_IND_update();
        LCD_charge_update();
    } else {
#if defined(BT_CABLE_PIN)
        static bool charging_old_satus = 0;
        static bool charging_now_satus = 0;

        if (!readPin(BT_CABLE_PIN)) {
            charging_now_satus = 1;
        } else {
            charging_now_satus = 0;
        }
        if (charging_old_satus != charging_now_satus) {
            charging_old_satus = charging_now_satus;
            LCD_charge_update();
        }
#endif
    }
}

void matrix_scan_kb(void) {
#ifdef MULTI_MODE_ENABLE
    bt_task();
#endif

    // set_led_state();

    matrix_scan_user();
}

void matrix_init_kb(void) {
    //     uart3_init(115200);

#ifdef MULTI_MODE_ENABLE
    bt_init();
#endif

    matrix_init_user();
}

void keyboard_post_init_kb(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
        eeconfig_update_keymap(&keymap_config);
    }
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    set_led_state();

#ifdef MULTI_MODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

    uart_resend_task();

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;
    static bool     bak_rgb_status    = false;

    if (dev_info.devs == DEVS_USB) {
        if ((USB_DRIVER.state != USB_ACTIVE) || (USB_DRIVER.state == USB_SUSPENDED)) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 8000) {
                if (!usb_suspend) {
                    usb_suspend    = true;
                    bak_rgb_status = rgb_matrix_is_enabled();
                    rgb_matrix_disable_noeeprom();
                    led_deconfig_all();
                    LCD_command_update(LCD_LIGHT_SLEEP);
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend_timer) {
                usb_suspend_timer = 0;
                if (usb_suspend) {
                    usb_suspend = false;
                    led_config_all();
                    if (bak_rgb_status) {
                        rgb_matrix_enable_noeeprom();
                    }
                    LCD_command_update(LCD_LIGHT_WAKEUP);
                    LCD_IND_update();
                }
            }
        }
    } else {
        usb_suspend_timer = 0;
        if (usb_suspend) {
            usb_suspend = false;
            if (bak_rgb_status) {
                rgb_matrix_enable_noeeprom();
            }
            led_config_all();
            LCD_command_update(LCD_LIGHT_WAKEUP);
            LCD_IND_update();
        }
    }
#endif
}

extern bool low_battery_vol;

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_get_flags() || low_battery_vol) {
        rgb_matrix_set_color_all(RGB_OFF);
    }

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(LED_CAPS_LOCK, RGB_WHITE);
    }

    if (keymap_config.no_gui) {
        rgb_matrix_set_color(LED_WIN_LOCK, RGB_CYAN);
    }

    if (!rgb_matrix_indicators_user()) {
        return false;
    }

    return true;
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    // if (dev_info.enble) {
    if (dev_info.enable && !low_battery_vol) {
        SLed_task();
    } else {
        for (uint8_t i = 83; i <= 129; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    if (!bt_indicator_rgb(led_min, led_max)) {
        return false;
    }

    if (all_blink_cnt) {
        for (uint8_t i = led_min; i <= 82; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
        if (timer_elapsed32(all_blink_time) > 300) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt & 0x1) {
            for (uint8_t i = led_min; i <= 82; i++) {
                rgb_matrix_set_color(i, all_blink_color.r, all_blink_color.g, all_blink_color.b);
            }
        }
    }

    return true;
}

void eeconfig_init_kb(void) {
    eeconfig_init_user();
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    extern void led_deconfig_all(void);
    led_deconfig_all();
    LCD_command_update(LCD_LIGHT_SLEEP);
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    led_config_all();
    uart_resend_times = 3;
    // LCD_command_update(LCD_LIGHT_WAKEUP);
    // set_led_state();
    LCD_IND_update();
}
