/* Copyright (C) 2023 Westberry Technology (ChangZhou) Corp., Ltd
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

#include "quantum.h"
#include "uart.h"
#include "report.h"
#include "usb_main.h"
#include "command.h"
#include "common/bt_task.h"
#include "lib/lib8tion/lib8tion.h"

#define NUM_LONG_PRESS_KEYS (sizeof(long_pressed_keys) / sizeof(long_pressed_keys_t))

#ifdef BT_DEBUG_MODE
#    define BT_DEBUG_INFO(fmt, ...) dprintf(fmt, ##__VA_ARGS__)
#else
#    define BT_DEBUG_INFO(fmt, ...)
#endif

uint32_t bt_init_time = 0;

dev_info_t dev_info = {0};
bts_info_t bts_info = {
    .bt_name        = {"Code One", "Code One", "Code One"},
    .uart_init      = uart_init,
    .uart_read      = uart_read,
    .uart_transmit  = uart_transmit,
    .uart_receive   = uart_receive,
    .uart_available = uart_available,
    .timer_read32   = timer_read32,
};

static void long_pressed_keys_hook(void);
static void long_pressed_keys_cb(uint16_t keycode);
static bool process_record_other(uint16_t keycode, keyrecord_t *record);
static void bt_scan_mode(void);
static void bt_used_pin_init(void);

#ifdef RGB_MATRIX_ENABLE
static void open_rgb(void);
static void close_rgb(void);
// static void rgb_driver_wake(void);
#endif

#if 0
static bool bt_soft_switch_enabled(void);
#endif

// blink led set
static uint8_t  all_blink_cnt      = 0;
static uint32_t all_blink_time     = 0;
static RGB      all_blink_color    = {0};
uint8_t         single_blink_cnt   = 0;
uint8_t         single_blink_index = 0;
RGB             single_blink_color = {0};
uint32_t        single_blink_time  = 0;

static uint32_t USB_switch_time = 0;
uint8_t         USB_blink_cnt   = 0;

static uint32_t key_press_time = 0;
static uint32_t close_rgb_time = 0;

static bool bak_rgb_toggle = false;
static bool sober          = true;

static bool kb_sleep_flag = false;

static const uint8_t rgb_index_table[]          = {USB_LED_INDEX, BT1_LED_INDEX, BT2_LED_INDEX, BT3_LED_INDEX, BT4_LED_INDEX};
static const uint8_t rgb_index_color_table[][3] = {
    USB_LED_COLOR, BT1_LED_COLOR, BT2_LED_COLOR, BT3_LED_COLOR, BT4_LED_COLOR,
};

static uint32_t last_total_time           = 0;
static uint8_t  indicator_status          = 2;
static uint8_t  indicator_reset_last_time = false;

uint32_t EE_CLR_press_cnt  = 0;
uint32_t EE_CLR_press_time = 0;
bool     EE_CLR_flag       = false;

bool query_vol_flag           = false;
bool battery_low_warning_flag = false;

// static const RGB caps_lock_indicator_color = {120, 255, 0};
// static const RGB gui_lock_indicator_color  = {255, 0, 140};

// static bool    rgb_test_en    = false;
// static uint8_t rgb_test_index = 0;

// static const uint8_t rgb_test_color_table[][3] = {
//     {RGB_WHITE},
//     {RGB_RED},
//     {RGB_GREEN},
//     {RGB_BLUE},
// };

static long_pressed_keys_t long_pressed_keys[] = {
    {
        .keycode         = BT_HOST1,
        .press_hold_time = 3000,
        .event_cb        = long_pressed_keys_cb,
    },
    {
        .keycode         = BT_HOST2,
        .press_hold_time = 3000,
        .event_cb        = long_pressed_keys_cb,
    },
    {
        .keycode         = BT_HOST3,
        .press_hold_time = 3000,
        .event_cb        = long_pressed_keys_cb,
    },
    {
        .keycode         = EE_CLR,
        .press_hold_time = 5000,
        .event_cb        = long_pressed_keys_cb,
    },
};

void long_press_start(uint16_t keycode, long_press_source_t source) {
    for (uint8_t i = 0; i < NUM_LONG_PRESS_KEYS; i++) {
        long_pressed_keys_t *item = &long_pressed_keys[i];

        if (item->keycode == keycode) {
            item->press_time = timer_read32();
            item->source     = source;
            item->active     = true;
            return;
        }
    }
}

void long_press_cancel(uint16_t keycode, long_press_source_t source) {
    for (uint8_t i = 0; i < NUM_LONG_PRESS_KEYS; i++) {
        long_pressed_keys_t *item = &long_pressed_keys[i];

        if (item->keycode == keycode) {
            if (item->source == source) {
                item->active = false;
            }
            return;
        }
    }
}

extern void register_mouse(uint8_t mouse_keycode, bool pressed);
/** \brief Utilities for actions. (FIXME: Needs better description)
 *
 * FIXME: Needs documentation.
 */
__attribute__((weak)) void register_code(uint8_t code) {
    if (dev_info.devs) {
        bts_process_keys(code, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
    } else {
        if (code == KC_NO) {
            return;

#ifdef LOCKING_SUPPORT_ENABLE
        } else if (KC_LOCKING_CAPS_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            // Resync: ignore if caps lock already is on
            if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) return;
#    endif
            add_key(KC_CAPS_LOCK);
            send_keyboard_report();
            wait_ms(TAP_HOLD_CAPS_DELAY);
            del_key(KC_CAPS_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_NUM_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (host_keyboard_leds() & (1 << USB_LED_NUM_LOCK)) return;
#    endif
            add_key(KC_NUM_LOCK);
            send_keyboard_report();
            wait_ms(100);
            del_key(KC_NUM_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_SCROLL_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (host_keyboard_leds() & (1 << USB_LED_SCROLL_LOCK)) return;
#    endif
            add_key(KC_SCROLL_LOCK);
            send_keyboard_report();
            wait_ms(100);
            del_key(KC_SCROLL_LOCK);
            send_keyboard_report();
#endif

        } else if (IS_BASIC_KEYCODE(code)) {
            // TODO: should push command_proc out of this block?
            if (command_proc(code)) return;

            // Force a new key press if the key is already pressed
            // without this, keys with the same keycode, but different
            // modifiers will be reported incorrectly, see issue #1708
            if (is_key_pressed(code)) {
                del_key(code);
                send_keyboard_report();
            }
            add_key(code);
            send_keyboard_report();
        } else if (IS_MODIFIER_KEYCODE(code)) {
            add_mods(MOD_BIT(code));
            send_keyboard_report();

#ifdef EXTRAKEY_ENABLE
        } else if (IS_SYSTEM_KEYCODE(code)) {
            host_system_send(KEYCODE2SYSTEM(code));
        } else if (IS_CONSUMER_KEYCODE(code)) {
            host_consumer_send(KEYCODE2CONSUMER(code));
#endif

        } else if (IS_MOUSE_KEYCODE(code)) {
            register_mouse(code, true);
        }
    }
}

/** \brief Utilities for actions. (FIXME: Needs better description)
 *
 * FIXME: Needs documentation.
 */
__attribute__((weak)) void unregister_code(uint8_t code) {
    if (dev_info.devs) {
        bts_process_keys(code, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
    } else {
        if (code == KC_NO) {
            return;

#ifdef LOCKING_SUPPORT_ENABLE
        } else if (KC_LOCKING_CAPS_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            // Resync: ignore if caps lock already is off
            if (!(host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK))) return;
#    endif
            add_key(KC_CAPS_LOCK);
            send_keyboard_report();
            del_key(KC_CAPS_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_NUM_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (!(host_keyboard_leds() & (1 << USB_LED_NUM_LOCK))) return;
#    endif
            add_key(KC_NUM_LOCK);
            send_keyboard_report();
            del_key(KC_NUM_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_SCROLL_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (!(host_keyboard_leds() & (1 << USB_LED_SCROLL_LOCK))) return;
#    endif
            add_key(KC_SCROLL_LOCK);
            send_keyboard_report();
            del_key(KC_SCROLL_LOCK);
            send_keyboard_report();
#endif

        } else if (IS_BASIC_KEYCODE(code)) {
            del_key(code);
            send_keyboard_report();
        } else if (IS_MODIFIER_KEYCODE(code)) {
            del_mods(MOD_BIT(code));
            send_keyboard_report();

#ifdef EXTRAKEY_ENABLE
        } else if (IS_SYSTEM_KEYCODE(code)) {
            host_system_send(0);
        } else if (IS_CONSUMER_KEYCODE(code)) {
            host_consumer_send(0);
#endif

        } else if (IS_MOUSE_KEYCODE(code)) {
            register_mouse(code, false);
        }
    }
}

extern void do_code16(uint16_t code, void (*f)(uint8_t));

__attribute__((weak)) void register_code16(uint16_t code) {
    if (dev_info.devs) {
        if (QK_MODS_GET_MODS(code) & 0x1) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RCTL, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LCTL, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x2) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x4) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RALT, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LALT, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x8) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RGUI, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LGUI, 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(code), 1, dev_info.devs, keymap_config.no_gui, KEY_NUM);
    } else {
        if (IS_MODIFIER_KEYCODE(code) || code == KC_NO) {
            do_code16(code, register_mods);
        } else {
            do_code16(code, register_weak_mods);
        }
        register_code(code);
    }
}

__attribute__((weak)) void unregister_code16(uint16_t code) {
    if (dev_info.devs) {
        if (QK_MODS_GET_MODS(code) & 0x1) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RCTL, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LCTL, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x2) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RSFT, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LSFT, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x4) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RALT, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LALT, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x8) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RGUI, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LGUI, 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(code), 0, dev_info.devs, keymap_config.no_gui, KEY_NUM);
    } else {
        unregister_code(code);
        if (IS_MODIFIER_KEYCODE(code) || code == KC_NO) {
            do_code16(code, unregister_mods);
        } else {
            do_code16(code, unregister_weak_mods);
        }
    }
}

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
    (void)arg;
    chRegSetThreadName("blinker");
    while (true) {
        bts_task(dev_info.devs);
        chThdSleepMilliseconds(1);
    }
}

/**
 * @brief bluetooth 初始化函数
 * @param None
 * @return None
 */
void bt_init(void) {
    bts_init(&bts_info);
    bt_used_pin_init();

    // Read the user config from EEPROM
    dev_info.raw = eeconfig_read_user();
    if (!dev_info.raw) {
        dev_info.devs      = DEVS_USB;
        dev_info.last_devs = DEVS_HOST1;
        eeconfig_update_user(dev_info.raw);
    }

    chThdCreateStatic(waThread1, sizeof(waThread1), HIGHPRIO, Thread1, NULL);

    bts_send_name(DEVS_HOST1);
    wait_ms(10);
    bt_scan_mode();

    if (dev_info.devs != DEVS_USB) {
        usbDisconnectBus(&USB_DRIVER);
        usbStop(&USB_DRIVER);
    }

    gpio_set_pin_output(A14);
    if (dev_info.devs == DEVS_USB) {
        gpio_write_pin_low(A14);
    } else {
        gpio_write_pin_high(A14);
    }

    bt_init_time = timer_read32();
}

/**
 * @brief bluetooth交互任务
 * @param event 当前事件
 * @return None
 */
void bt_task(void) {
    static uint32_t last_time = 0;

    if ((bt_init_time != 0) && (timer_elapsed32(bt_init_time) >= 2000)) {
        bt_init_time = 0;

        // bts_send_vendor(v_en_sleep_wl);
        bts_send_vendor(v_en_sleep_bt);

        switch (dev_info.devs) {
            case DEVS_HOST1: {
                bts_send_vendor(v_host1);
            } break;
            case DEVS_HOST2: {
                bts_send_vendor(v_host2);
            } break;
            case DEVS_HOST3: {
                bts_send_vendor(v_host3);
            } break;
            case DEVS_2_4G: {
                bts_send_vendor(v_2_4g);
            } break;
            default: {
                bts_send_vendor(v_usb);
                dev_info.devs = DEVS_USB;
                eeconfig_update_user(dev_info.raw);
            } break;
        }
    }

    /* Execute every 1ms */
    if (timer_elapsed32(last_time) >= 1) {
        last_time = timer_read32();

        if (dev_info.devs != DEVS_USB) {
            uint8_t keyboard_led_state = 0;
            led_t  *kb_leds            = (led_t *)&keyboard_led_state;
            kb_leds->raw               = bts_info.bt_info.indictor_rgb_s;
            usb_device_state_set_leds(keyboard_led_state);

#ifdef RGB_MATRIX_ENABLE
            close_rgb();
#endif
        }
    }

    long_pressed_keys_hook();
    bt_scan_mode();
}

uint32_t pressed_time = 0;

bool process_record_bt(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;
    // clang-format off
    if (record->event.pressed) {
        BT_DEBUG_INFO("\n\nkeycode = [0x%x], pressed time: [%d]\n\n", keycode, record->event.time);
        BT_DEBUG_INFO("\n devs     = [%d] \
                    \n sleeped       = [%d] \
                    \n low_vol       = [%d] \
                    \n low_vol_offed = [%d] \
                    \n normal_vol    = [%d] \
                    \n pairing       = [%d] \
                    \n paired        = [%d] \
                    \n come_back     = [%d] \
                    \n come_back_err = [%d] \
                    \n mode_switched = [%d] \
                    \n pvol          = [%d]\n\n\n",
                    dev_info.devs,
                    bts_info.bt_info.sleeped,
                    bts_info.bt_info.low_vol,
                    bts_info.bt_info.low_vol_offed,
                    bts_info.bt_info.normal_vol,
                    bts_info.bt_info.pairing,
                    bts_info.bt_info.paired,
                    bts_info.bt_info.come_back,
                    bts_info.bt_info.come_back_err,
                    bts_info.bt_info.mode_switched,
                    bts_info.bt_info.pvol);
        // clang-format on
        pressed_time = timer_read32();

        if (indicator_status != 0) {
            last_total_time = timer_read32();
        }
    }
    retval = process_record_other(keycode, record);

    if (dev_info.devs != DEVS_USB) {
        if (retval != false) {
            while (bts_is_busy()) {
                wait_ms(1);
            }

            if ((keycode > QK_MODS) && (keycode <= QK_MODS_MAX)) {
                if (QK_MODS_GET_MODS(keycode) & 0x1) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                    else
                        bts_process_keys(KC_LCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                }
                if (QK_MODS_GET_MODS(keycode) & 0x2) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                    else
                        bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                }
                if (QK_MODS_GET_MODS(keycode) & 0x4) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RALT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                    else
                        bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                }
                if (QK_MODS_GET_MODS(keycode) & 0x8) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                    else
                        bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                }
                retval = bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(keycode), record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            } else if (IS_BASIC_KEYCODE(keycode)) {
                if (record->event.pressed) {
                    register_code(keycode);
                } else {
                    unregister_code(keycode);
                }
            } else {
                retval = bts_process_keys(keycode, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            }
        }
    }

#ifdef RGB_MATRIX_ENABLE
    open_rgb();
#endif

    return retval;
}

void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset) {
    last_total_time = timer_read32();

    bool usb_sws = !!last_mode ? !now_mode : !!now_mode;

    if (usb_sws) {
        if (!!now_mode) {
            usbDisconnectBus(&USB_DRIVER);
            usbStop(&USB_DRIVER);
        } else {
            init_usb_driver(&USB_DRIVER);
        }
    }

    dev_info.devs = now_mode;

    if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
        dev_info.last_devs = dev_info.devs;
    } else if (dev_info.devs == DEVS_USB) {
        USB_switch_time = timer_read32();
        USB_blink_cnt   = 0;
    }

    if (dev_info.devs == DEVS_USB) {
        gpio_write_pin_low(A14);
    } else {
        gpio_write_pin_high(A14);
    }

    bts_info.bt_info.pairing       = false;
    bts_info.bt_info.paired        = false;
    bts_info.bt_info.come_back     = false;
    bts_info.bt_info.come_back_err = false;
    bts_info.bt_info.mode_switched = false;

    bts_info.bt_info.indictor_rgb_s = 0;
    eeconfig_update_user(dev_info.raw);

    switch (dev_info.devs) {
        case DEVS_HOST1: {
            if (reset != false) {
                indicator_status          = 1;
                indicator_reset_last_time = true;
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_host1);
            }
        } break;
        case DEVS_HOST2: {
            if (reset != false) {
                indicator_status          = 1;
                indicator_reset_last_time = 0;
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_host2);
            }
        } break;
        case DEVS_HOST3: {
            if (reset != false) {
                indicator_status          = 1;
                indicator_reset_last_time = true;
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_host3);
            }
        } break;
        case DEVS_2_4G: {
            if (reset != false) {
                indicator_status          = 1;
                indicator_reset_last_time = true;
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_2_4g);
            }
        } break;
        case DEVS_USB: {
            bts_send_vendor(v_usb);
        } break;
        default:
            break;
    }
}

#if 0
static bool bt_soft_switch_enabled(void) {
#    ifdef BT_MODE_SW_PIN
    return gpio_read_pin(BT_MODE_SW_PIN) && gpio_read_pin(RF_MODE_SW_PIN);
#    else
    return false;
#    endif
}
#endif

static bool process_record_other(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        long_press_start(keycode, LONG_PRESS_SOURCE_DIRECT);
    } else {
        long_press_cancel(keycode, LONG_PRESS_SOURCE_DIRECT);
    }

    switch (keycode) {
        case BT_HOST1: {
            if (record->event.pressed) {
                // if ((dev_info.devs != DEVS_HOST1) && (bt_soft_switch_enabled() || !gpio_read_pin(BT_MODE_SW_PIN))) {
                if ((dev_info.devs != DEVS_HOST1) && (!gpio_read_pin(BT_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                    single_blink_cnt   = 6;
                    single_blink_index = BT1_LED_INDEX;
                    single_blink_color = (RGB)BT1_LED_COLOR;
                    single_blink_time  = timer_read32();
                }
            }
        } break;
        case BT_HOST2: {
            if (record->event.pressed) {
                // if ((dev_info.devs != DEVS_HOST2) && (bt_soft_switch_enabled() || !gpio_read_pin(BT_MODE_SW_PIN))) {
                if ((dev_info.devs != DEVS_HOST2) && (!gpio_read_pin(BT_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                    single_blink_cnt   = 6;
                    single_blink_index = BT2_LED_INDEX;
                    single_blink_color = (RGB)BT2_LED_COLOR;
                    single_blink_time  = timer_read32();
                }
            }
        } break;
        case BT_HOST3: {
            if (record->event.pressed) {
                // if ((dev_info.devs != DEVS_HOST3) && (bt_soft_switch_enabled() || !gpio_read_pin(BT_MODE_SW_PIN))) {
                if ((dev_info.devs != DEVS_HOST3) && (!gpio_read_pin(BT_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                    single_blink_cnt   = 6;
                    single_blink_index = BT3_LED_INDEX;
                    single_blink_color = (RGB)BT3_LED_COLOR;
                    single_blink_time  = timer_read32();
                }
            }
        } break;
            // case BT_2_4G: {
            //     if (record->event.pressed) {
            //         // if ((dev_info.devs != DEVS_2_4G) && (bt_soft_switch_enabled() || !gpio_read_pin(RF_MODE_SW_PIN))) {
            //         if ((dev_info.devs != DEVS_2_4G) && (!gpio_read_pin(RF_MODE_SW_PIN))) {
            //             bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
            //             single_blink_cnt   = 6;
            //             single_blink_color = (RGB)BT4_LED_COLOR;
            //             if (timer_elapsed32(single_blink_time) >= 300) {
            //                 single_blink_time = timer_read32();
            //             }
            //             single_blink_index = BT4_LED_INDEX;
            //         }
            //     }
            // } break;
            // case BT_USB: {
            //     if (record->event.pressed) {
            //         // if ((dev_info.devs != DEVS_USB) && bt_soft_switch_enabled()) {
            //         if ((dev_info.devs != DEVS_USB) && (gpio_read_pin(BT_MODE_SW_PIN) && gpio_read_pin(RF_MODE_SW_PIN))) {
            //             bt_switch_mode(dev_info.devs, DEVS_USB, false);
            //             single_blink_cnt   = 6;
            //             single_blink_color = (RGB)USB_LED_COLOR;
            //             if (timer_elapsed32(single_blink_time) >= 300) {
            //                 single_blink_time = timer_read32();
            //             }
            //             single_blink_index = USB_LED_INDEX;
            //         }
            //     }
            // } break;
            // case BT_VOL: {
            //     if (record->event.pressed) {
            //         bts_send_vendor(v_query_vol);
            //         query_vol_flag = true;
            //     } else {
            //         query_vol_flag = false;
            //     }
            // } break;

            // case KC_PGDN: {
            //     if (record->event.pressed) {
            //         if (rgb_test_en) {
            //             rgb_test_en = false;
            //             return false;
            //         }
            //     }
            //     return true;
            // }
            // case KC_DOWN: {
            //     if (record->event.pressed) {
            //         if (rgb_test_en) {
            //             rgb_test_index++;
            //             if (rgb_test_index > 4) rgb_test_index = 1;
            //             return false;
            //         } else {
            //         }
            //     }
            //     return true;
            // }

            // case RGB_TEST: {
            // } break;
            // case EE_CLR: {
            // } break;

            // case OS_SWITCH: {
            //     if (record->event.pressed) {
            //         if (get_highest_layer(default_layer_state) == 0) { // WIN_BASE
            //             set_single_persistent_default_layer(2);
            //             if (keymap_config.no_gui) {
            //                 keymap_config.no_gui = false;
            //             }
            //             single_blink_cnt   = 6;
            //             single_blink_color = (RGB){100 / 3, 100, 100};
            //             if (timer_elapsed32(single_blink_time) >= 300) {
            //                 single_blink_time = timer_read32();
            //             }
            //             single_blink_index = GUI_LOCK_LED_INDEX;
            //             eeconfig_update_keymap(&keymap_config);
            //         } else if (get_highest_layer(default_layer_state) == 2) { // MAC_BASE
            //             set_single_persistent_default_layer(0);
            //         }
            //     }
            // } break;

            // case RM_SPDU:
            //     if (record->event.pressed) {
            //         if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
            //             single_blink_cnt   = 6;
            //             single_blink_color = (RGB){100 / 3, 100, 100};
            //             if (timer_elapsed32(single_blink_time) >= 300) {
            //                 single_blink_time = timer_read32();
            //             }
            //             single_blink_index = GUI_LOCK_LED_INDEX;
            //         }
            //         return true;
            //     }
            // case RM_SPDD:
            //     if (record->event.pressed) {
            //         if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
            //             single_blink_cnt   = 6;
            //             single_blink_color = (RGB){100 / 3, 100, 100};
            //             if (timer_elapsed32(single_blink_time) >= 300) {
            //                 single_blink_time = timer_read32();
            //             }
            //             single_blink_index = GUI_LOCK_LED_INDEX;
            //         }
            //         return true;
            //     }

        default:
            return true;
    }

    return false;
}

static void long_pressed_keys_cb(uint16_t keycode) {
    switch (keycode) {
        case BT_HOST1: {
            if (dev_info.devs == DEVS_HOST1) {
                bt_switch_mode(dev_info.devs, DEVS_HOST1, true);
            }
        } break;
        case BT_HOST2: {
            if (dev_info.devs == DEVS_HOST2) {
                bt_switch_mode(dev_info.devs, DEVS_HOST2, true);
            }
        } break;
        case BT_HOST3: {
            if (dev_info.devs == DEVS_HOST3) {
                bt_switch_mode(dev_info.devs, DEVS_HOST3, true);
            }
        } break;
            // case BT_2_4G: {
            //     if (dev_info.devs == DEVS_2_4G) {
            //         bt_switch_mode(dev_info.devs, DEVS_2_4G, true);
            //     }
            // } break;

        case EE_CLR: {
            if (!EE_CLR_flag) {
                EE_CLR_flag       = true;
                EE_CLR_press_time = timer_read32();
                EE_CLR_press_cnt  = 1;
            }
        } break;

            // case RGB_TEST: {
            //     if (rgb_test_en != true) {
            //         rgb_test_en    = true;
            //         rgb_test_index = 1;
            //     }
            // } break;

        default:
            break;
    }
}

static void long_pressed_keys_hook(void) {
    for (uint8_t i = 0; i < NUM_LONG_PRESS_KEYS; i++) {
        long_pressed_keys_t *item = &long_pressed_keys[i];

        if (item->active && timer_elapsed32(item->press_time) >= item->press_hold_time) {
            // 先结束计时，确保持续按住只触发一次。
            item->active = false;
            item->event_cb(item->keycode);
        }
    }
}

static void bt_used_pin_init(void) {
#ifdef BT_MODE_SW_PIN
    gpio_set_pin_input_high(BT_MODE_SW_PIN);
    gpio_set_pin_input_high(RF_MODE_SW_PIN);
#endif

#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
    gpio_set_pin_input_high(BT_CABLE_PIN);
    gpio_set_pin_input(BT_CHARGE_PIN);
#endif

#ifdef RGB_DRIVER_SDB_PIN
    gpio_set_pin_output_push_pull(RGB_DRIVER_SDB_PIN);
    gpio_write_pin_high(RGB_DRIVER_SDB_PIN);
#endif

#ifdef RGB_DRIVER_RESET_PIN
    gpio_set_pin_output_push_pull(RGB_DRIVER_RESET_PIN);
    gpio_write_pin_high(RGB_DRIVER_RESET_PIN);
#endif
}

// void lp_recovery_hook(void) {
//     bt_used_pin_init();
// }

/**
 * @brief 根据波动开关判断工作模式
 * @param None
 * @return None
 */
static void bt_scan_mode(void) {
#ifdef BT_MODE_SW_PIN
#    if 0
    static bool    usb_cable_was_plugged  = false;
    static bool    usb_cable_forced_usb   = false;
    static uint8_t usb_cable_restore_mode = DEVS_USB;
    static bool    mode_sw_initialized    = false;
    static bool    last_bt_mode_sw        = false;
    static bool    last_rf_mode_sw        = false;

    bool bt_mode_sw        = gpio_read_pin(BT_MODE_SW_PIN);
    bool rf_mode_sw        = gpio_read_pin(RF_MODE_SW_PIN);
    bool usb_mode_sw       = !gpio_read_pin(BT_CABLE_PIN);
    bool enter_usb_mode_sw = mode_sw_initialized && bt_mode_sw && rf_mode_sw && (!last_bt_mode_sw || !last_rf_mode_sw);

    mode_sw_initialized = true;
    last_bt_mode_sw     = bt_mode_sw;
    last_rf_mode_sw     = rf_mode_sw;

    if (bt_mode_sw && rf_mode_sw) {
        if (usb_mode_sw) {
            if (!usb_cable_was_plugged || enter_usb_mode_sw) {
                usb_cable_was_plugged  = true;
                usb_cable_forced_usb   = true;
                usb_cable_restore_mode = dev_info.devs;

                if (dev_info.devs != DEVS_USB) {
                    bt_switch_mode(dev_info.devs, DEVS_USB, false); // Force USB once when cable is plugged in.
                }
            } else if (usb_cable_forced_usb && (dev_info.devs != DEVS_USB)) {
                usb_cable_forced_usb = false; // User manually selected wireless while cable is still plugged in.
            }
            return;
        }

        if (usb_cable_was_plugged) {
            uint8_t restore_mode = usb_cable_restore_mode;

            usb_cable_was_plugged  = false;
            usb_cable_restore_mode = DEVS_USB;

            if (usb_cable_forced_usb && (dev_info.devs == DEVS_USB) && ((restore_mode == DEVS_HOST1) || (restore_mode == DEVS_HOST2) || (restore_mode == DEVS_HOST3) || (restore_mode == DEVS_2_4G))) {
                bt_switch_mode(dev_info.devs, restore_mode, false); // Restore previous mode only if USB was auto-forced.
            }

            usb_cable_forced_usb = false;
            return;
        }

        if (dev_info.devs == DEVS_USB) {
            bt_switch_mode(dev_info.devs, dev_info.last_devs, false);
        }
        return;
    }

    if (!bt_mode_sw && rf_mode_sw) {
        if ((dev_info.devs == DEVS_USB) || (dev_info.devs == DEVS_2_4G)) {
            bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // BT mode
        }
        return;
    }
    if (!rf_mode_sw && bt_mode_sw) {
        if (dev_info.devs != DEVS_2_4G) {
            bt_switch_mode(dev_info.devs, DEVS_2_4G, false); // 2_4G mode
        }
        return;
    }

    usb_cable_was_plugged  = usb_mode_sw;
    usb_cable_forced_usb   = false;
    usb_cable_restore_mode = DEVS_USB;

#    else
    // uint8_t        now_mode  = 0;
    // static uint8_t last_mode = 0;
    if (!gpio_read_pin(BT_MODE_SW_PIN) && gpio_read_pin(RF_MODE_SW_PIN)) {
        if ((dev_info.devs == DEVS_USB) || (dev_info.devs == DEVS_2_4G)) {
            bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // BT mode
            // now_mode = 1;
        }
    } else if ((!gpio_read_pin(RF_MODE_SW_PIN) && gpio_read_pin(BT_MODE_SW_PIN)) || (gpio_read_pin(BT_MODE_SW_PIN) && gpio_read_pin(RF_MODE_SW_PIN))) {
        // if (dev_info.devs != DEVS_2_4G) {
        // bt_switch_mode(dev_info.devs, DEVS_2_4G, false); // 2_4G mode
        if (dev_info.devs != DEVS_USB)
            bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
                                                            // now_mode = 2;
        // }
    }
    // if (gpio_read_pin(BT_MODE_SW_PIN) && gpio_read_pin(RF_MODE_SW_PIN)) {
    //     if (dev_info.devs != DEVS_USB) bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
    // now_mode = 0;
    // }
#    endif
    // if ((last_mode != now_mode) && !battery_low_warning_flag) {
    //     last_mode = now_mode;
    //     gpio_write_pin_low(RGB_DRIVER_SDB_PIN);
    //     wait_ms(10);
    //     gpio_write_pin_high(RGB_DRIVER_SDB_PIN);
    //     rgb_matrix_init();
    // }
#endif
}

static void close_rgb(void) {
    extern bool low_vol_offed_sleep;
    static bool switch_to_usb = false;

    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }

    if (sober) {
        if (kb_sleep_flag || (timer_elapsed32(key_press_time) >= (5 * 60 * 1000))) {
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();
#ifdef RGB_DRIVER_SDB_PIN
            gpio_write_pin_low(RGB_DRIVER_SDB_PIN);
#endif
        }
    } else {
        if (!rgb_matrix_config.enable) {
            if (low_vol_offed_sleep && !switch_to_usb) {
                bts_send_vendor(v_usb);
                switch_to_usb = true;
            }
            if (timer_elapsed32(close_rgb_time) >= ENTRY_STOP_TIMEOUT) {
#ifdef ENTRY_STOP_MODE
                lp_system_sleep();
#endif

                if (low_vol_offed_sleep && switch_to_usb) {
                    switch_to_usb = false;
                    if (dev_info.devs == DEVS_2_4G) {
                        bt_switch_mode(dev_info.last_devs, DEVS_2_4G, 0);
                    } else {
                        bt_switch_mode(dev_info.last_devs, dev_info.devs, 0);
                    }
                }

                extern void open_rgb(void);
                open_rgb();
            }
        }
    }
}

void open_rgb(void) {
    key_press_time = timer_read32();

    if (!sober) {
#ifdef RGB_DRIVER_SDB_PIN
        gpio_set_pin_output_push_pull(RGB_DRIVER_SDB_PIN);
        // gpio_set_pin_output_push_pull(RGB_DRIVER_RESET_PIN);
        gpio_write_pin_high(RGB_DRIVER_SDB_PIN);
        wait_ms(10);
        // gpio_write_pin_low(RGB_DRIVER_RESET_PIN);
        // wait_ms(2);
        // gpio_write_pin_high(RGB_DRIVER_RESET_PIN);
        // wait_ms(20);

        // rgb_matrix_init();
#endif

        kb_sleep_flag = false;
        extern bool low_vol_offed_sleep;
        low_vol_offed_sleep = false;

        if (bak_rgb_toggle) {
            rgb_matrix_enable_noeeprom();
        }

        sober = true;
    }
}

// Device connection indication
static void indicator_device_connection(void) {
    // if (dev_info.devs != DEVS_USB) {
    //     if ((get_highest_layer(default_layer_state | layer_state) == 0) || (get_highest_layer(default_layer_state | layer_state) == 3)) {
    //         rgb_matrix_set_color(rgb_index_table[dev_info.devs], rgb_index_color_table[dev_info.devs][0], rgb_index_color_table[dev_info.devs][1], rgb_index_color_table[dev_info.devs][2]);
    //     }
    // }
}

// Factory reset indication
static void indicator_factory_reset(void) {
    if (EE_CLR_flag) {
        if (timer_elapsed32(EE_CLR_press_time) >= 300) {
            EE_CLR_press_time = timer_read32();
            EE_CLR_press_cnt++;
        }

        if (EE_CLR_press_cnt > 7) {
            EE_CLR_press_time = 0;
            EE_CLR_press_cnt  = 0;
            EE_CLR_flag       = false;

            eeconfig_init();

            if (keymap_config.no_gui) {
                keymap_config.no_gui = false;
            }

            // if (indicator_status != 0) {
            //     last_total_time = timer_read32();
            // }
            if ((dev_info.devs != DEVS_USB) && !bts_info.bt_info.paired) {
                if (dev_info.devs == DEVS_2_4G) {
                    bt_switch_mode(dev_info.last_devs, DEVS_2_4G, false);
                } else {
                    bt_switch_mode(dev_info.last_devs, dev_info.devs, false);
                }
            }
        }

        uint8_t brightness = (EE_CLR_press_cnt % 2) ? 0 : 100;

        rgb_matrix_set_color_all(brightness / 3, brightness, brightness);
    }
}

// Query battery voltage every 4 seconds
static void battery_voltage_query(void) {
    static uint32_t query_vol_time = 0;
    if (gpio_read_pin(BT_CABLE_PIN)) {
        if (!bt_init_time && !kb_sleep_flag && bts_info.bt_info.paired && (timer_elapsed32(query_vol_time) > 4000)) {
            query_vol_time = timer_read32();
            bts_send_vendor(v_query_vol);
        }
    }
}

uint8_t ledNum;

// Display battery voltage on LEDs
// static void battery_voltage_display(void) {
//     if (query_vol_flag && gpio_read_pin(BT_CABLE_PIN)) {
//         static bool     bat_query_blink = false;
//         static uint32_t bat_query_time  = 0;

//         uint8_t        pvol                    = bts_info.bt_info.pvol;
//         uint8_t        led_count               = 0;
//         uint8_t        i                       = 0;
//         static uint8_t LED_POWER_LEVEL_TABLE[] = BAT_VOL_LED_INDEX;

//         rgb_matrix_set_color_all(0, 0, 0);

//         if (pvol <= 10)
//             led_count = 1;
//         else if (pvol <= 20)
//             led_count = 2;
//         else if (pvol < 40)
//             led_count = 3;
//         else if (pvol >= 100)
//             led_count = 10;
//         else
//             led_count = pvol / 10;

//         if (timer_elapsed32(bat_query_time) >= 1000) {
//             bat_query_blink = !bat_query_blink;
//             bat_query_time  = timer_read32();
//         }

//         if (bat_query_blink) {
//             for (i = 0; i < 10; i++) {
//                 if (i < led_count) {
//                     // if (pvol < 30)
//                     //     rgb_matrix_set_color(LED_POWER_LEVEL_TABLE[i], 100, 0, 0);
//                     // else if (pvol < 60)
//                     //     rgb_matrix_set_color(LED_POWER_LEVEL_TABLE[i], 0, 0, 100);
//                     // else
//                     rgb_matrix_set_color(LED_POWER_LEVEL_TABLE[i], 0xFF, 0xF4, 0xE5);
//                 }
//             }
//         }
//     }
// }
/* ================================================== */
static void battery_voltage_display(void) {
    static bool was_querying = false;
    static bool bat_query_on = false;
    // static uint32_t bat_query_time = 0;

    const bool querying = query_vol_flag && gpio_read_pin(BT_CABLE_PIN);

    /*
     * 不在查询状态时复位。
     * 下次查询一定从“亮”开始。
     */
    if (!querying) {
        was_querying = false;
        bat_query_on = false;
        return;
    }

    /*
     * 检测一次新的电量查询。
     */
    if (!was_querying) {
        was_querying = true;
        bat_query_on = true;
        // bat_query_time = timer_read32();
    }

    /*
     * 查询显示期间先关闭所有灯，
     * 后面只点亮表示电量的数字键。
     */
    rgb_matrix_set_color_all(0, 0, 0);

    uint8_t        pvol                    = bts_info.bt_info.pvol;
    uint8_t        led_count               = 0;
    static uint8_t LED_POWER_LEVEL_TABLE[] = BAT_VOL_LED_INDEX;

    if (pvol <= 10) {
        led_count = 1;
    } else if (pvol <= 20) {
        led_count = 2;
    } else if (pvol < 40) {
        led_count = 3;
    } else if (pvol >= 100) {
        led_count = 10;
    } else {
        led_count = pvol / 10;
    }

    /*
     * 每 1 秒切换一次亮灭状态。
     * 因为新查询时 bat_query_on=true，所以必定先亮。
     */
    // if (timer_elapsed32(bat_query_time) >= 1000) {
    //     bat_query_time = timer_read32();
    //     bat_query_on   = !bat_query_on;
    // }

    if (bat_query_on) {
        for (uint8_t i = 0; i < led_count; i++) {
            rgb_matrix_set_color(LED_POWER_LEVEL_TABLE[i], 0xFF / 3, 0xFF, 0xFF);
        }
    }
}
/* ================================================== */

// static void battery_low_warning(void) {
//     static bool     Low_power_blink  = false;
//     static uint32_t Low_power_time   = 0;
//     static uint32_t charge_last_time = 0;

//     if (!gpio_read_pin(BT_CABLE_PIN)) {
//         if (!gpio_read_pin(BT_CHARGE_PIN)) {
//             if (timer_elapsed32(charge_last_time) < 3000) {
//                 rgb_matrix_set_color(81, 100, 0, 0);
//             }
//         }
//         battery_low_warning_flag = false;
//     } else {
//         charge_last_time = timer_read32();

//         if (bts_info.bt_info.low_vol) {
//             battery_low_warning_flag = true;
//         }

//         if (battery_low_warning_flag) {
//             if (timer_elapsed32(Low_power_time) >= 300) {
//                 Low_power_blink = !Low_power_blink;
//                 Low_power_time  = timer_read32();
//             }
//             if (Low_power_blink) {
//                 rgb_matrix_set_color(81, 100, 0, 0);
//             } else {
//                 rgb_matrix_set_color(81, 0, 0, 0);
//             }
//         } else {
//             Low_power_blink = 0;
//         }

//         if (bts_info.bt_info.low_vol_offed) {
//             if (timer_elapsed32(pressed_time) >= 2000) {
//                 kb_sleep_flag = true;
//             }
//             extern bool low_vol_offed_sleep;
//             low_vol_offed_sleep = true;
//         }
//     }
// }
/* ============================================== */
typedef enum {
    BAT_NOTICE_NONE,
    BAT_NOTICE_15,
    BAT_NOTICE_10,
    BAT_NOTICE_5,
} battery_notice_t;

static void set_battery_warning_leds(uint8_t r, uint8_t g, uint8_t b) {
    rgb_matrix_set_color(FLASK_LED_INDEX, r, g, b);
    // rgb_matrix_set_color(LOGO_LED_INDEX, r, g, b);
}

static void battery_low_warning(void) {
    static uint8_t          next_threshold = 15;
    static battery_notice_t active_notice  = BAT_NOTICE_NONE;
    static uint32_t         notice_started = 0;
    static uint32_t         blink_timer    = 0;
    static bool             blink_on       = false;

    static bool     cable_initialized     = false;
    static bool     cable_was_present     = false;
    static uint32_t charge_notice_started = 0;
    static uint32_t charge_blink_timer    = 0;
    static bool     charge_blink_on       = false;

    const bool    cable_present = !gpio_read_pin(BT_CABLE_PIN);
    const bool    charging      = cable_present && !gpio_read_pin(BT_CHARGE_PIN);
    const uint8_t percent       = bts_info.bt_info.pvol;

    /*
     * Type-C 插入边沿。
     * cable_initialized 保证键盘上电时线已经插着，也会产生一次充电提示。
     */
    if (!cable_initialized) {
        cable_initialized = true;
        cable_was_present = false;
    }

    if (cable_present && !cable_was_present) {
        active_notice            = BAT_NOTICE_NONE;
        battery_low_warning_flag = false;

        charge_notice_started = timer_read32();
        charge_blink_timer    = timer_read32();
        charge_blink_on       = true;

        /*
         * 插线视为开始一个新的充放电周期。
         * 下次拔线后允许重新检测 15/10/5% 节点。
         */
        next_threshold = 15;
    }

    cable_was_present = cable_present;

    /*
     * 充电提示：
     * 仅在插入 Type-C 且充电脚有效时，C 键标准白闪烁约 2.4 秒。
     * 不检测充满状态。
     */
    if (charging && timer_elapsed32(charge_notice_started) < CHARGE_NOTICE_TIME) {
        if (timer_elapsed32(charge_blink_timer) >= CHARGE_BLINK_INTERVAL) {
            charge_blink_timer = timer_read32();
            charge_blink_on    = !charge_blink_on;
        }

        if (charge_blink_on) {
            rgb_matrix_set_color(CHARGE_LED_INDEX, CHARGE_INDICATOR_COLOR);
        } else {
            rgb_matrix_set_color(CHARGE_LED_INDEX, 0, 0, 0);
        }
    }

    /*
     * 插线立即终止低电提醒。
     */
    if (cable_present) {
        active_notice            = BAT_NOTICE_NONE;
        battery_low_warning_flag = false;
        blink_on                 = false;
        return;
    }

    /*
     * 低于 3% 优先级最高：直到模块执行低电关机前持续闪烁。
     */
    if (percent < 3 || bts_info.bt_info.low_vol_offed) {
        active_notice            = BAT_NOTICE_NONE;
        battery_low_warning_flag = true;

        if (timer_elapsed32(blink_timer) >= LOW_BATTERY_BLINK_INTERVAL) {
            blink_timer = timer_read32();
            blink_on    = !blink_on;
        }

        if (blink_on) {
            set_battery_warning_leds(LOW_BATTERY_COLOR);
        } else {
            set_battery_warning_leds(0, 0, 0);
        }

        /*
         * 保留原固件的低电关机逻辑。
         */
        if (bts_info.bt_info.low_vol_offed) {
            if (timer_elapsed32(pressed_time) >= 2000) {
                kb_sleep_flag = true;
            }

            extern bool low_vol_offed_sleep;
            low_vol_offed_sleep = true;
        }

        return;
    }

    // 电量恢复到 20% 及以上，重新允许下一次 15% 提醒。
    // 留出 5 个百分点，避免 15% 附近波动造成反复提醒。
    if (percent >= 20) {
        next_threshold           = 15;
        battery_low_warning_flag = false;
    }
    /*
     * 每个电量节点只触发一次。
     * 若电量一次从 16% 跳到 9%，会先提示 15%，结束后再提示 10%，
     * 因而不会丢失任何节点提醒。
     */
    if (active_notice == BAT_NOTICE_NONE) {
        if (next_threshold == 15 && percent <= 15) {
            active_notice  = BAT_NOTICE_15;
            next_threshold = 10;
        } else if (next_threshold == 10 && percent <= 10) {
            active_notice  = BAT_NOTICE_10;
            next_threshold = 5;
        } else if (next_threshold == 5 && percent <= 5) {
            active_notice  = BAT_NOTICE_5;
            next_threshold = 0;
        }

        if (active_notice != BAT_NOTICE_NONE) {
            notice_started = timer_read32();
            blink_timer    = timer_read32();
            blink_on       = true;
        }
    }

    if (active_notice != BAT_NOTICE_NONE) {
        if (timer_elapsed32(notice_started) >= LOW_BATTERY_NOTICE_TIME) {
            // battery_low_warning_flag = false;
            active_notice = BAT_NOTICE_NONE;
            blink_on      = false;
            set_battery_warning_leds(0, 0, 0);
            return;
        }

        if (active_notice == BAT_NOTICE_5) battery_low_warning_flag = true;

        if (timer_elapsed32(blink_timer) >= LOW_BATTERY_BLINK_INTERVAL) {
            blink_timer = timer_read32();
            blink_on    = !blink_on;
        }

        if (blink_on) {
            set_battery_warning_leds(LOW_BATTERY_COLOR);
        } else {
            set_battery_warning_leds(0, 0, 0);
        }
    }
}

/* ============================================== */

// All indicator blink
static void indicator_all_blink(void) {
    if (!all_blink_cnt) return;

    rgb_matrix_set_color_all(0, 0, 0);
    if (timer_elapsed32(all_blink_time) > 300) {
        all_blink_time = timer_read32();
        all_blink_cnt--;
    }

    if (all_blink_cnt % 2) {
        rgb_matrix_set_color_all(all_blink_color.r, all_blink_color.g, all_blink_color.b);
    }
}

// Single indicator blink
static void indicator_single_blink(void) {
    if (!single_blink_cnt) return;

    if (timer_elapsed32(single_blink_time) > 300) {
        single_blink_time = timer_read32();
        single_blink_cnt--;
    }

    if (single_blink_cnt % 2) {
        rgb_matrix_set_color(single_blink_index, single_blink_color.r, single_blink_color.g, single_blink_color.b);
    } else {
        rgb_matrix_set_color(single_blink_index, 0, 0, 0);
    }
}

// Bluetooth connection indication
// static void indicator_usb_connection(void) {
//     static uint32_t USB_blink_time = 0;
//     static bool     USB_blink      = false;

//     if ((USB_DRIVER.state != USB_ACTIVE)) {
//         if (USB_blink_cnt <= 20) {
//             if (timer_elapsed32(USB_blink_time) >= 500) {
//                 USB_blink_cnt++;
//                 USB_blink      = !USB_blink;
//                 USB_blink_time = timer_read32();
//             }
//             uint8_t brightness = USB_blink ? 100 : 0;
//             rgb_matrix_set_color(rgb_index_table[DEVS_USB], brightness, brightness, brightness);
//         }
//         USB_switch_time = timer_read32();
//     } else {
//         if (timer_elapsed32(USB_switch_time) < (3 * 1000)) {
//             rgb_matrix_set_color(rgb_index_table[DEVS_USB], 100, 100, 100);
//         }
//     }
// }

static void indicator_rgb_test(void) {
    // if (rgb_test_en && (rgb_test_index > 0)) {
    //     rgb_matrix_set_color_all(rgb_test_color_table[rgb_test_index - 1][0], rgb_test_color_table[rgb_test_index - 1][1], rgb_test_color_table[rgb_test_index - 1][2]);
    // }
}

// Bluetooth connection indication
static void indicator_bluetooth_connection(void) {
    if (dev_info.devs != DEVS_USB) {
        uint8_t         rgb_index      = rgb_index_table[dev_info.devs];
        static uint32_t last_time      = 0;
        static uint32_t last_long_time = 0;
        static uint8_t  last_status    = 0;
        static bool     rgb_flip       = false;
        static RGB      rgb            = {0};

        if (last_status != indicator_status) {
            last_status     = indicator_status;
            last_total_time = timer_read32();
        }

        if (indicator_reset_last_time != false) {
            indicator_reset_last_time = false;
            last_time                 = 0;
            last_total_time           = timer_read32();
        }

        switch (indicator_status) {
            case 1: { // 闪烁模式 5Hz 重置
                if ((last_time == 0) || (timer_elapsed32(last_time) >= 125)) {
                    last_time = timer_read32();
                    rgb_flip  = !rgb_flip;
                    rgb       = rgb_flip ? (RGB){rgb_index_color_table[dev_info.devs][0], rgb_index_color_table[dev_info.devs][1], rgb_index_color_table[dev_info.devs][2]} : (RGB){0, 0, 0};
                }

                if (bts_info.bt_info.paired) {
                    last_long_time   = timer_read32();
                    indicator_status = 3;
                    break;
                }

                if (timer_elapsed32(last_total_time) >= (60 * 1000)) {
                    indicator_status = 0;
                    kb_sleep_flag    = true;
                }
            } break;

            case 2: { // 闪烁模式 2Hz 回连
                if ((last_time == 0) || (timer_elapsed32(last_time) >= 250)) {
                    last_time = timer_read32();
                    rgb_flip  = !rgb_flip;
                    rgb       = rgb_flip ? (RGB){rgb_index_color_table[dev_info.devs][0], rgb_index_color_table[dev_info.devs][1], rgb_index_color_table[dev_info.devs][2]} : (RGB){0, 0, 0};
                }

                if (bts_info.bt_info.paired) {
                    last_long_time   = timer_read32();
                    indicator_status = 3;
                    break;
                }

                if (timer_elapsed32(last_total_time) >= (60 * 1000)) {
                    indicator_status = 0;
                    kb_sleep_flag    = true;
                }
            } break;

            case 3: { // 长亮模式
                if (timer_elapsed32(last_long_time) < (2 * 1000)) {
                    rgb = (RGB){rgb_index_color_table[dev_info.devs][0], rgb_index_color_table[dev_info.devs][1], rgb_index_color_table[dev_info.devs][2]};
                } else {
                    indicator_status = 0;
                }
            } break;

            case 4: { // 长灭模式
                rgb = (RGB){0, 0, 0};
            } break;

            default: {
                rgb_flip = false;
                if (!kb_sleep_flag) {
                    if (!bts_info.bt_info.paired) {
                        if (!bts_info.bt_info.pairing) {
                            indicator_status = 2;
                            break;
                        }
                        indicator_status = 2;
                        if (!bts_info.bt_info.pairing) {
                            if (dev_info.devs == DEVS_2_4G) {
                                bt_switch_mode(dev_info.last_devs, DEVS_2_4G, false);
                            } else {
                                bt_switch_mode(dev_info.last_devs, dev_info.devs, false);
                            }
                        }
                        break;
                    }
                }
            } break;
        }

        if (indicator_status) rgb_matrix_set_color(rgb_index, rgb.r, rgb.g, rgb.b);
    }
}

#if 0
static void indicator_usb_connection(void) {
    if (dev_info.devs == DEVS_USB) {
        static uint32_t USB_blink_time = 0;
        static bool     USB_blink      = false;

        if ((USB_DRIVER.state != USB_ACTIVE)) {
            if (USB_blink_cnt <= 20) {
                if (timer_elapsed32(USB_blink_time) >= 500) {
                    USB_blink_cnt++;
                    USB_blink      = !USB_blink;
                    USB_blink_time = timer_read32();
                }
                if (USB_blink) {
                    rgb_matrix_set_color(rgb_index_table[DEVS_USB], rgb_index_color_table[DEVS_USB][0], rgb_index_color_table[DEVS_USB][1], rgb_index_color_table[DEVS_USB][2]);
                } else {
                    rgb_matrix_set_color(rgb_index_table[DEVS_USB], 0, 0, 0);
                }
            }
            USB_switch_time = timer_read32();
        } else {
            if (timer_elapsed32(USB_switch_time) < (2 * 1000)) {
                rgb_matrix_set_color(rgb_index_table[DEVS_USB], rgb_index_color_table[DEVS_USB][0], rgb_index_color_table[DEVS_USB][1], rgb_index_color_table[DEVS_USB][2]);
            }
        }
    }
}
#endif

uint8_t bt_indicator_rgb(uint8_t led_min, uint8_t led_max) {
    battery_voltage_query();

    battery_low_warning();

    battery_voltage_display();

    indicator_device_connection();

    indicator_bluetooth_connection();

    // indicator_usb_connection();

    // caps lock red
    if (host_keyboard_led_state().caps_lock && (((dev_info.devs != DEVS_USB) && bts_info.bt_info.paired && !kb_sleep_flag) || ((dev_info.devs == DEVS_USB) && (USB_DRIVER.state != USB_SUSPENDED)))) {
        // rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, caps_lock_indicator_color.r, caps_lock_indicator_color.g, caps_lock_indicator_color.b);
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, RGB_CYAN);
    }
    // GUI lock red
    if (keymap_config.no_gui) {
        // rgb_matrix_set_color(GUI_LOCK_LED_INDEX, gui_lock_indicator_color.r, gui_lock_indicator_color.g, gui_lock_indicator_color.b);
        rgb_matrix_set_color(GUI_LOCK_LED_INDEX, RGB_CYAN);
    }

    indicator_all_blink();

    indicator_single_blink();

    indicator_rgb_test();

    indicator_factory_reset();

    return true;
}
