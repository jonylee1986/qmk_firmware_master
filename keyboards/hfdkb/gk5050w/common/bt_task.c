/**
 * @file bt_task.c
 * @brief
 * @author JoyLee
 * @version 2.0.0
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2023 Westberry Technology Corp., Ltd
 */

#include QMK_KEYBOARD_H

#include "uart.h"
#include "common/bt_task.h"
#include "command.h"
#include "usb_main.h"
#include "../lcd_drv/lcd.h"
#include "../lcd_drv/uart3.h"

#define NUM_LONG_PRESS_KEYS (sizeof(long_pressed_keys) / sizeof(long_pressed_keys_t))

#ifdef BT_DEBUG_MODE
#    define BT_DEBUG_INFO(fmt, ...) dprintf(fmt, ##__VA_ARGS__)
#else
#    define BT_DEBUG_INFO(fmt, ...)
#endif

typedef struct {
    uint32_t press_time;
    uint16_t keycode;
    void (*event_cb)(uint16_t);
} long_pressed_keys_t;

uint32_t   bt_init_time = 0;
dev_info_t dev_info     = {0};
bts_info_t bts_info     = {
        .bt_name        = {"Model 1-$", "Model 1-$", "Model 1-$"},
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
void        open_rgb(void);
static void close_rgb(void);
#endif

static void bt_mousekey_task(void);
// clang-format off
long_pressed_keys_t long_pressed_keys[] = {
  {.keycode = BT_HOST1, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = BT_HOST2, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = BT_HOST3, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = BT_2_4G, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = EE_CLR, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = SW_OS1, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = RGB_TEST, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = WL_2_4G, .press_time = 0, .event_cb = long_pressed_keys_cb},
};
// clang-format on

static uint32_t USB_switch_time = 0;

static uint32_t key_press_time;
static uint32_t close_rgb_time;

static bool bak_rgb_toggle;
static bool led_inited;
static bool sober                = true;
static bool kb_sleep_flag        = false;
static bool backlight_sleep_flag = false;

bool low_battery_vol     = false;
bool low_battery_vol_off = false;
bool bat_vol_full        = false;

#if 1
void register_mouse(uint8_t mouse_keycode, bool pressed);
/** \brief Utilities for actions. (FIXME: Needs better description)
 *
 * FIXME: Needs documentation.
 */
__attribute__((weak)) void register_code(uint8_t code) {
    if (dev_info.devs) {
        bts_process_keys(code, 1, dev_info.devs, keymap_config.no_gui);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
    } else {
        if (code == KC_NO) {
            return;

#    ifdef LOCKING_SUPPORT_ENABLE
        } else if (KC_LOCKING_CAPS_LOCK == code) {
#        ifdef LOCKING_RESYNC_ENABLE
            // Resync: ignore if caps lock already is on
            if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) return;
#        endif
            add_key(KC_CAPS_LOCK);
            send_keyboard_report();
            wait_ms(TAP_HOLD_CAPS_DELAY);
            del_key(KC_CAPS_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_NUM_LOCK == code) {
#        ifdef LOCKING_RESYNC_ENABLE
            if (host_keyboard_leds() & (1 << USB_LED_NUM_LOCK)) return;
#        endif
            add_key(KC_NUM_LOCK);
            send_keyboard_report();
            wait_ms(100);
            del_key(KC_NUM_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_SCROLL_LOCK == code) {
#        ifdef LOCKING_RESYNC_ENABLE
            if (host_keyboard_leds() & (1 << USB_LED_SCROLL_LOCK)) return;
#        endif
            add_key(KC_SCROLL_LOCK);
            send_keyboard_report();
            wait_ms(100);
            del_key(KC_SCROLL_LOCK);
            send_keyboard_report();
#    endif

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

#    ifdef EXTRAKEY_ENABLE
        } else if (IS_SYSTEM_KEYCODE(code)) {
            host_system_send(KEYCODE2SYSTEM(code));
        } else if (IS_CONSUMER_KEYCODE(code)) {
            host_consumer_send(KEYCODE2CONSUMER(code));
#    endif

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
        bts_process_keys(code, 0, dev_info.devs, keymap_config.no_gui);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
            wait_ms(1);
        }
    } else {
        if (code == KC_NO) {
            return;

#    ifdef LOCKING_SUPPORT_ENABLE
        } else if (KC_LOCKING_CAPS_LOCK == code) {
#        ifdef LOCKING_RESYNC_ENABLE
            // Resync: ignore if caps lock already is off
            if (!(host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK))) return;
#        endif
            add_key(KC_CAPS_LOCK);
            send_keyboard_report();
            del_key(KC_CAPS_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_NUM_LOCK == code) {
#        ifdef LOCKING_RESYNC_ENABLE
            if (!(host_keyboard_leds() & (1 << USB_LED_NUM_LOCK))) return;
#        endif
            add_key(KC_NUM_LOCK);
            send_keyboard_report();
            del_key(KC_NUM_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_SCROLL_LOCK == code) {
#        ifdef LOCKING_RESYNC_ENABLE
            if (!(host_keyboard_leds() & (1 << USB_LED_SCROLL_LOCK))) return;
#        endif
            add_key(KC_SCROLL_LOCK);
            send_keyboard_report();
            del_key(KC_SCROLL_LOCK);
            send_keyboard_report();
#    endif

        } else if (IS_BASIC_KEYCODE(code)) {
            del_key(code);
            send_keyboard_report();
        } else if (IS_MODIFIER_KEYCODE(code)) {
            del_mods(MOD_BIT(code));
            send_keyboard_report();

#    ifdef EXTRAKEY_ENABLE
        } else if (IS_SYSTEM_KEYCODE(code)) {
            host_system_send(0);
        } else if (IS_CONSUMER_KEYCODE(code)) {
            host_consumer_send(0);
#    endif

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
                bts_process_keys(KC_RCTL, 1, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LCTL, 1, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(code) & 0x2) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(code) & 0x4) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RALT, 1, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LALT, 1, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(code) & 0x8) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RGUI, 1, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LGUI, 1, dev_info.devs, keymap_config.no_gui);
        }
        bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(code), 1, dev_info.devs, keymap_config.no_gui);
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
                bts_process_keys(KC_RCTL, 0, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LCTL, 0, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(code) & 0x2) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RSFT, 0, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LSFT, 0, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(code) & 0x4) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RALT, 0, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LALT, 0, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(code) & 0x8) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RGUI, 0, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LGUI, 0, dev_info.devs, keymap_config.no_gui);
        }
        bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(code), 0, dev_info.devs, keymap_config.no_gui);
    } else {
        unregister_code(code);
        if (IS_MODIFIER_KEYCODE(code) || code == KC_NO) {
            do_code16(code, unregister_mods);
        } else {
            do_code16(code, unregister_weak_mods);
        }
    }
}
#endif

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
    bt_init_time = timer_read32();
    chThdCreateStatic(waThread1, sizeof(waThread1), HIGHPRIO, Thread1, NULL);
    bt_scan_mode();
    bts_send_name(DEVS_HOST1);
    wait_ms(10);
    if (dev_info.devs != DEVS_USB) {
        usbDisconnectBus(&USB_DRIVER);
        usbStop(&USB_DRIVER);
        writePinHigh(A12);
    }
    if (dev_info.devs == DEVS_USB) {
        writePinLow(A14);
    } else {
        writePinHigh(A14);
    }
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

        bts_send_name(DEVS_HOST1);

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
        bts_send_vendor(v_en_sleep_bt);
        // LCD_charge_update();
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

    bt_mousekey_task();
    long_pressed_keys_hook();
    bt_scan_mode();
}

uint32_t pressed_time = 0;
bool     rgb_status_save;

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
        if (!rgb_matrix_config.enable) {
            if (rgb_status_save) {
                rgb_matrix_enable_noeeprom();
            }
            // LCD_command_update(LCD_WEAKUP);
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
                        bts_process_keys(KC_RCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    else
                        bts_process_keys(KC_LCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                }
                if (QK_MODS_GET_MODS(keycode) & 0x2) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    else
                        bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                }
                if (QK_MODS_GET_MODS(keycode) & 0x4) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RALT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    else
                        bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                }
                if (QK_MODS_GET_MODS(keycode) & 0x8) {
                    if (QK_MODS_GET_MODS(keycode) & 0x10)
                        bts_process_keys(KC_RGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    else
                        bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                }
                retval = bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(keycode), record->event.pressed, dev_info.devs, keymap_config.no_gui);
            } else {
                retval = bts_process_keys(keycode, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
        }
    }

#ifdef RGB_MATRIX_ENABLE
    open_rgb();
#endif

    return retval;
}

void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset) {
    bool usb_sws = !!last_mode ? !now_mode : !!now_mode;
    if (!rgb_matrix_config.enable) {
        if (rgb_status_save) {
            rgb_matrix_enable_noeeprom();
        }
    }

    LCD_IND_update();

    if (backlight_sleep_flag) {
        backlight_sleep_flag = false;
        LCD_command_update(LCD_LIGHT_WAKEUP);
    }

    LCD_command_update(LCD_WAKEUP);
    pressed_time = timer_read32();

    extern uint8_t indicator_status;
    extern uint8_t indicator_reset_last_time;

    if (usb_sws) {
        if (!!now_mode) {
            usbDisconnectBus(&USB_DRIVER);
            usbStop(&USB_DRIVER);
            // writePinHigh(A12);
        } else {
            init_usb_driver(&USB_DRIVER);
        }
    }

    dev_info.devs = now_mode;

    if (dev_info.devs != dev_info.last_devs) {
        extern uint32_t last_total_time;
        last_total_time = timer_read32();
    }

    if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
        dev_info.last_devs = dev_info.devs;
    } else if (dev_info.devs == DEVS_USB) {
        USB_switch_time = timer_read32();
    }

    if (dev_info.devs == DEVS_USB) {
        writePinLow(A14);
    } else {
        writePinHigh(A14);
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
            } else if (last_mode != DEVS_HOST1) {
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
            } else if (last_mode != DEVS_HOST2) {
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
            } else if (last_mode != DEVS_HOST3) {
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
            } else if (last_mode != DEVS_2_4G) {
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
typedef struct {
    bool dir;
    bool pressed;
} move_t;
typedef struct {
    bool dir;
    bool pressed;
} wheel_t;
typedef struct {
    uint16_t pressed_time;
    move_t   move_x;
    move_t   move_y;
    wheel_t  wheel_x;
    wheel_t  wheel_y;
    uint8_t  data[5];
} bt_mousekey_t;
static bt_mousekey_t bt_mousekey;

uint16_t bt_mousekey_move_send_time;
uint16_t bt_mousekey_wheel_send_time;
// static uint8_t bt_mousekey_send_speed;

void bt_mousekey_task(void) {
    if (bt_mousekey.move_x.pressed || bt_mousekey.move_y.pressed) {
        if (bt_mousekey.move_x.pressed) {
            if (bt_mousekey.move_x.dir)
                bt_mousekey.data[1] = 0xfe;
            else
                bt_mousekey.data[1] = 0x02;
        } else {
            bt_mousekey.data[1] = 0;
        }
        if (bt_mousekey.move_y.pressed) {
            if (bt_mousekey.move_y.dir)
                bt_mousekey.data[2] = 0xfe;
            else
                bt_mousekey.data[2] = 0x02;
        } else {
            bt_mousekey.data[2] = 0;
        }
        bt_mousekey.data[4] = 0;
        bt_mousekey.data[3] = 0;
        if (timer_elapsed(bt_mousekey_move_send_time) >= 10) {
            bt_mousekey_move_send_time = timer_read();
            bts_send_mouse_report(bt_mousekey.data);
        }
    }
    if (bt_mousekey.wheel_x.pressed || bt_mousekey.wheel_y.pressed) {
        bt_mousekey.data[1] = 0;
        bt_mousekey.data[2] = 0;
        if (bt_mousekey.wheel_x.pressed) {
            if (bt_mousekey.wheel_x.dir)
                bt_mousekey.data[4] = 0xff;
            else
                bt_mousekey.data[4] = 0x01;
        } else {
            bt_mousekey.data[4] = 0;
        }
        if (bt_mousekey.wheel_y.pressed) {
            if (bt_mousekey.wheel_y.dir)
                bt_mousekey.data[3] = 0xfe;
            else
                bt_mousekey.data[3] = 0x01;
        } else {
            bt_mousekey.data[3] = 0;
        }
        if (timer_elapsed(bt_mousekey_wheel_send_time) >= 100) {
            bt_mousekey_wheel_send_time = timer_read();
            bts_send_mouse_report(bt_mousekey.data);
        }
    }
}

uint32_t EE_CLR_press_cnt  = 0;
uint32_t EE_CLR_press_time = 0;
bool     EE_CLR_flag       = false;
bool     query_vol_flag    = false;
uint8_t  single_blink_cnt;
uint8_t  single_blink_index;
uint32_t single_blink_time;
RGB      single_blink_color = {0};
uint8_t  all_blink_cnt;
uint32_t all_blink_time;
RGB      all_blink_color = {0};

// static uint16_t bt_tap_time;

static bool process_record_other(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < NUM_LONG_PRESS_KEYS; i++) {
        if (keycode == long_pressed_keys[i].keycode) {
            if (record->event.pressed) {
                long_pressed_keys[i].press_time = timer_read32();
            } else {
                long_pressed_keys[i].press_time = 0;
            }
            break;
        }
    }

    switch (keycode) {
        case BT_HOST1: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_HOST1) && (!readPin(RF_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                }
            }
        } break;
        case BT_HOST2: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_HOST2) && (!readPin(RF_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                }
            }
        } break;
        case BT_HOST3: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_HOST3) && (!readPin(RF_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                }
            }
        } break;
        case BT_2_4G: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_2_4G) && (!readPin(RF_MODE_SW_PIN))) {
                    bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                }
            }
        } break;
        case BT_USB: {
            // if (record->event.pressed) {
            //     if (dev_info.devs != DEVS_USB) {
            //         bt_switch_mode(dev_info.devs, DEVS_USB, false);
            //     }
            // }
        } break;
        case BT_VOL: {
            if (record->event.pressed) {
                bts_send_vendor(v_query_vol);
                query_vol_flag = true;
            } else {
                query_vol_flag = false;
            }
        } break;
        case WL_2_4G: {
        } break;
        case RGB_TEST: {
        } break;
        case EE_CLR: {
        } break;
        case SW_OS1: {
        } break;

        default:
            return true;
    }

    return false;
}

static void long_pressed_keys_cb(uint16_t keycode) {
    switch (keycode) {
        case BT_HOST1: {
            // if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
            if (dev_info.devs == DEVS_HOST1) {
                bt_switch_mode(dev_info.devs, DEVS_HOST1, true);
            }
        } break;
        case BT_HOST2: {
            // if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
            if (dev_info.devs == DEVS_HOST2) {
                bt_switch_mode(dev_info.devs, DEVS_HOST2, true);
            }
        } break;
        case BT_HOST3: {
            // if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
            if (dev_info.devs == DEVS_HOST3) {
                bt_switch_mode(dev_info.devs, DEVS_HOST3, true);
            }
        } break;
        case BT_2_4G: {
            if (dev_info.devs == DEVS_2_4G) {
                bt_switch_mode(dev_info.devs, DEVS_2_4G, true);
            }
        } break;
        case EE_CLR: {
            if (!EE_CLR_flag) {
                EE_CLR_flag       = true;
                EE_CLR_press_time = timer_read32();
                EE_CLR_press_cnt  = 1;
            }
        } break;
        case SW_OS1: {
            if (get_highest_layer(default_layer_state) == 0) {
                set_single_persistent_default_layer(3);
                keymap_config.no_gui = 0;
                eeconfig_update_keymap(&keymap_config);
                all_blink_time  = timer_read32();
                all_blink_cnt   = 6;
                all_blink_color = (RGB){100, 100, 100};
            } else if (get_highest_layer(default_layer_state) == 3) {
                set_single_persistent_default_layer(0);
                all_blink_time  = timer_read32();
                all_blink_cnt   = 6;
                all_blink_color = (RGB){100, 100, 100};
            }
        } break;
        case WL_2_4G: {
            uint8_t data[3];
            data[0] = 0xB1;
            data[1] = 0x10;
            data[2] = 0xC1;
            uart_transmit(data, 3);
            single_blink_cnt   = 6;
            single_blink_time  = timer_read32();
            single_blink_color = (RGB){100, 100, 100};
            single_blink_index = 50;
        } break;

        default:
            break;
    }
}

static void long_pressed_keys_hook(void) {
    for (uint8_t i = 0; i < NUM_LONG_PRESS_KEYS; i++) {
        if ((long_pressed_keys[i].press_time != 0) && (timer_elapsed32(long_pressed_keys[i].press_time) >= (3 * 1000))) {
            long_pressed_keys[i].event_cb(long_pressed_keys[i].keycode);
            long_pressed_keys[i].press_time = 0;
        }
    }
}

static void bt_used_pin_init(void) {
#ifdef RF_MODE_SW_PIN
    // setPinInputHigh(BT_MODE_SW_PIN);
    setPinInputHigh(RF_MODE_SW_PIN);
#endif

#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
    setPinInputHigh(BT_CABLE_PIN);
    setPinInput(BT_CHARGE_PIN);
#endif

#if defined(LED_CHRG_PIN) && defined(LED_DOWN_PIN)
    setPinOutputPushPull(LED_CHRG_PIN);
    setPinOutputPushPull(LED_DOWN_PIN);
    setPinOutputPushPull(LED_UNUSED_PIN);

    writePinHigh(LED_CHRG_PIN);
    writePinHigh(LED_DOWN_PIN);
    writePinHigh(LED_UNUSED_PIN);
#endif
}

/**
 * @brief 根据波动开关判断工作模式
 * @param None
 * @return None
 */

// ===========================================
// Helper functions for hardware switch
// ===========================================

/**
 * @brief Check if current device mode is wireless
 * @return true if current mode is wireless
 */
static bool is_current_mode_wireless(void) {
    return (dev_info.devs >= DEVS_HOST1 && dev_info.devs <= DEVS_2_4G);
}

/**
 * @brief Check if a given device mode is wireless
 * @param devs The device mode to check
 * @return true if the given mode is wireless
 */
static bool is_current_mode_wireless_by_devs(uint8_t devs) {
    return (devs >= DEVS_HOST1 && devs <= DEVS_2_4G);
}

static void bt_scan_mode(void) {
#ifdef RF_MODE_SW_PIN
    // if (readPin(RF_MODE_SW_PIN)) {
    //     if (dev_info.devs != DEVS_USB) bt_switch_mode(dev_info.devs, DEVS_USB, false);
    // } else {
    //     if (dev_info.devs == DEVS_USB) bt_switch_mode(dev_info.devs, dev_info.last_devs, false);
    // }

    // static bool last_switch_state = false; // Track previous switch state

    bool switch_state = readPin(RF_MODE_SW_PIN); // false = switch ON (force wired), true = switch OFF (allow all modes)

    // Handle switch state changes
    // if (last_switch_state != switch_state) {
    //     last_switch_state = switch_state;

    if (switch_state) {
        // Switch turned ON -> Force wired mode immediately
        if (dev_info.devs != DEVS_USB) {
            // Save current wireless mode for restoration when switch turns OFF
            if (is_current_mode_wireless()) {
                dev_info.last_devs = dev_info.devs;
            }
            bt_switch_mode(dev_info.devs, DEVS_USB, false);
            eeconfig_update_user(dev_info.raw);
        }
    } else {
        // Switch turned OFF -> Restore last wireless mode ONLY if it was wireless AND USB wasn't manually selected
        if (dev_info.devs == DEVS_USB && is_current_mode_wireless_by_devs(dev_info.last_devs)) {
            // Only auto-switch back if:
            // 1. Currently in USB mode
            // 2. Last mode was wireless
            // 3. USB mode wasn't manually selected by user
            bt_switch_mode(dev_info.devs, dev_info.last_devs, false);
        }
        // If USB was manually selected or last mode was USB, stay in USB mode
    }
    // }
#endif
    // #ifdef BT_MODE_SW_PIN
    //     if (readPin(BT_MODE_SW_PIN) && !readPin(RF_MODE_SW_PIN)) {
    //         if (dev_info.devs != DEVS_2_4G) bt_switch_mode(dev_info.devs, DEVS_2_4G, false); // 2_4G mode
    //     }
    //     if (readPin(RF_MODE_SW_PIN) && !readPin(BT_MODE_SW_PIN)) {
    //         if ((dev_info.devs == DEVS_USB) || (dev_info.devs == DEVS_2_4G)) bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // BT mode
    //     }
    //     if (readPin(BT_MODE_SW_PIN) && readPin(RF_MODE_SW_PIN)) {
    //         if (dev_info.devs != DEVS_USB) bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
    //     }
    // #endif
}

bool get_kb_sleep_flag(void) {
    return kb_sleep_flag;
}
bool get_led_inited(void) {
    return led_inited;
}

static const uint8_t rgb_index_table[] = {
    LED_USB, LED_BT1, LED_BT2, LED_BT3, LED_2G,
};

static const uint8_t rgb_index_color_table[][3] = {
    {RGB_WHITE}, {RGB_RED}, {RGB_BLUE}, {RGB_YELLOW}, {RGB_GREEN},
};

void led_config_all(void) {
    if (!led_inited) {
        led_inited = true;
#ifdef RGB_MATRIX_SDB_PIN
        setPinOutputPushPull(RGB_MATRIX_SDB_PIN);
        writePinHigh(RGB_MATRIX_SDB_PIN);
#endif
        // if (dev_info.devs == DEVS_USB) LCD_command_update(LCD_WAKEUP);
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
        led_inited = false;
#ifdef RGB_MATRIX_SDB_PIN
        setPinOutputPushPull(RGB_MATRIX_SDB_PIN);
        writePinLow(RGB_MATRIX_SDB_PIN);
#endif
        // if (dev_info.devs == DEVS_USB) LCD_command_update(LCD_SLEEP);
    }
}

static void close_rgb(void) {
    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }
    /*************************************************************************************/
    if (timer_elapsed32(pressed_time) >= ((3 * 60 - 34) * 1000)) {
        rgb_matrix_disable_noeeprom();
        if (!backlight_sleep_flag) {
            backlight_sleep_flag = true;
            LCD_command_update(LCD_LIGHT_SLEEP);
        }
        // writePinHigh(LED_CHRG_PIN);
    } else {
        rgb_status_save = rgb_matrix_config.enable;
    }
    /*************************************************************************************/
    if (sober) {
        if (kb_sleep_flag || (timer_elapsed32(key_press_time) >= (30 * 60 * 1000))) { // 30 minutes
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();
            writePinLow(RGB_MATRIX_SDB_PIN);
            // LCD_command_update(LCD_LIGHT_SLEEP);
            // for (uint8_t i = 0; i < 3; i++) {
            LCD_command_update(LCD_SLEEP);
            // wait_ms(10);
            // }
            // writePinHigh(LED_CHRG_PIN);
        }
    } else {
        if (!rgb_matrix_config.enable) {
            if (timer_elapsed32(close_rgb_time) >= ENTRY_STOP_TIMEOUT) {
                uart3_stop();
                setPinOutputPushPull(SD3_TX_PIN);

                /* Turn off all indicators led */
                if (led_inited) {
                    led_deconfig_all();
                    // LCD_command_update(LCD_SLEEP);
                }

#ifdef ENTRY_STOP_MODE
                lp_system_sleep();
#endif

                for (uint8_t i = 0; i < 5; i++) {
                    writePinHigh(SD3_TX_PIN);
                    wait_ms(5);
                    writePinLow(SD3_TX_PIN);
                    wait_ms(5);
                }

                extern void open_rgb(void);
                uart3_start();
                open_rgb();
                LCD_command_update(LCD_WAKEUP);
                // LCD_command_update(LCD_LIGHT_WAKEUP);
            }
        }
    }
}

void open_rgb(void) {
    key_press_time = timer_read32();

    if (backlight_sleep_flag) {
        backlight_sleep_flag = false;
        LCD_command_update(LCD_LIGHT_WAKEUP);
    }

    if (!sober) {
        // writePinHigh(RGB_DRIVER_SDB_PIN);
        if (bak_rgb_toggle) {
            kb_sleep_flag = false;
            extern bool low_vol_offed_sleep;
            low_vol_offed_sleep = false;
            rgb_matrix_enable_noeeprom();
        }
        if (!led_inited) {
            led_config_all();
        }
        sober = true;
    }
}

uint8_t  indicator_status          = LED_BLE_CONN;
uint8_t  indicator_reset_last_time = false;
uint32_t last_total_time           = 0;

void usb_indicate(void) {
    if (USB_DRIVER.state == USB_ACTIVE) {
        if (timer_elapsed32(USB_switch_time) < 2000) {
            rgb_matrix_set_color(rgb_index_table[DEVS_USB], rgb_index_color_table[DEVS_USB][0], rgb_index_color_table[DEVS_USB][1], rgb_index_color_table[DEVS_USB][2]);
        }
    } else {
        USB_switch_time = timer_read32();
    }
}

void led_ble(void) {
    uint8_t rgb_index = rgb_index_table[dev_info.devs];

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
    }
    switch (indicator_status) {
        case LED_BLE_PAIR: {
            if ((last_time == 0) || (timer_elapsed32(last_time) >= LED_BLE_PAIR_INTVL_MS)) {
                last_time = timer_read32();
                rgb_flip  = !rgb_flip;
                if (rgb_flip) {
                    rgb.r = rgb_index_color_table[dev_info.devs][0];
                    rgb.g = rgb_index_color_table[dev_info.devs][1];
                    rgb.b = rgb_index_color_table[dev_info.devs][2];
                } else {
                    rgb = (RGB){.r = 0, .g = 0, .b = 0};
                }
            }

            if (bts_info.bt_info.paired) {
                last_long_time   = timer_read32();
                indicator_status = 3;
                break;
            }

            if (timer_elapsed32(last_total_time) >= BLE_PAIR_TIMEOUT) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;

        case LED_BLE_CONN: {
            if ((last_time == 0) || (timer_elapsed32(last_time) >= LED_BLE_CONN_INTVL_MS)) {
                last_time = timer_read32();
                rgb_flip  = !rgb_flip;
                if (rgb_flip) {
                    rgb.r = rgb_index_color_table[dev_info.devs][0];
                    rgb.g = rgb_index_color_table[dev_info.devs][1];
                    rgb.b = rgb_index_color_table[dev_info.devs][2];
                } else {
                    rgb = (RGB){.r = 0, .g = 0, .b = 0};
                }
            }
            if (bts_info.bt_info.paired) {
                last_long_time   = timer_read32();
                indicator_status = 3;
                break;
            }

            if (timer_elapsed32(last_total_time) >= BLE_CONN_TIMEOUT) {
                indicator_status = 0;
                kb_sleep_flag    = true;
                // writePinLow(LED_UNUSED_PIN);
            }
        } break;
        case 3: {
            if ((timer_elapsed32(last_long_time) < (2 * 1000))) {
                rgb.r = rgb_index_color_table[dev_info.devs][0];
                rgb.g = rgb_index_color_table[dev_info.devs][1];
                rgb.b = rgb_index_color_table[dev_info.devs][2];
            } else {
                indicator_status = 0;
            }
        } break;
        case 4: {
            rgb = (RGB){.r = 0, .g = 0, .b = 0};
        } break;

        default:
            rgb_flip = false;
            if (!kb_sleep_flag) {
                if (!bts_info.bt_info.paired) {
                    if (!bts_info.bt_info.pairing) {
                        indicator_status = 2;
                        break;
                    }
                    indicator_status = 2;
                    if (dev_info.devs == DEVS_2_4G)
                        bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                    else
                        bt_switch_mode(dev_info.devs, dev_info.last_devs, false);
                    break;
                }
            }
            break;
    }
    if (indicator_status) rgb_matrix_set_color(rgb_index, rgb.r, rgb.g, rgb.b);
}

uint8_t bt_indicator_rgb(uint8_t led_min, uint8_t led_max) {
    uint8_t uart_data_read[3] = {0};
    uint8_t uart_data_send[3] = {0};

    static uint32_t query_vol_time  = 0;
    static uint32_t charg_full_time = 0;
    static uint32_t charging_time   = 0;
    /*************************************************************************************/
    if (EE_CLR_flag) {
        if (timer_elapsed32(EE_CLR_press_time) >= EE_CLR_press_cnt * 300) {
            EE_CLR_press_cnt++;
        }
        if (EE_CLR_press_cnt >= 7) {
            EE_CLR_press_time = 0;
            EE_CLR_press_cnt  = 0;
            EE_CLR_flag       = false;
            LCD_Page_update(0);
            LCD_command_update(LCD_RESET);
            eeconfig_init();
            keymap_config.no_gui = false;

            if (indicator_status) {
                last_total_time = timer_read32();
            }
        }
        if (EE_CLR_press_cnt & 0x1) {
            for (uint8_t i = 0; i < 83; i++) {
                rgb_matrix_set_color(i, 100, 100, 100);
            }
        } else {
            for (uint8_t i = 0; i < 83; i++) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
        return false;
    }
    /*************************************************************************************/
    if (single_blink_cnt) { // 单键闪烁
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
    /*************************************************************************************/
    if (all_blink_cnt) {
        if (timer_elapsed32(all_blink_time) > 300) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt % 2) {
            for (uint8_t i = led_min; i <= 82; i++) {
                rgb_matrix_set_color(i, all_blink_color.r, all_blink_color.g, all_blink_color.b);
            }
        } else {
            for (uint8_t i = led_min; i <= 82; i++) {
                rgb_matrix_set_color(i, RGB_OFF);
            }
        }
    }
    /*************************************************************************************/
    if (!readPin(BT_CABLE_PIN)) {
        if (!readPin(BT_CHARGE_PIN)) {
            if (timer_elapsed32(charging_time) > 1200) {
                writePinLow(LED_CHRG_PIN);
                writePinHigh(LED_DOWN_PIN);
                // if (!bat_vol_full) {
                // } else {
                //     writePinHigh(LED_CHRG_PIN);
                //     writePinHigh(LED_DOWN_PIN);
                // }
                charg_full_time = timer_read32();
            }
        } else {
            if (timer_elapsed32(charg_full_time) > 1200) {
                if (timer_elapsed32(charg_full_time) < 3000) {
                    writePinHigh(LED_CHRG_PIN);
                    writePinLow(LED_DOWN_PIN);
                } else {
                    writePinHigh(LED_CHRG_PIN);
                    writePinHigh(LED_DOWN_PIN);
                }
                charging_time = timer_read32();
            }
        }
        low_battery_vol     = false;
        low_battery_vol_off = false;
    } else {
        if (low_battery_vol && !low_battery_vol_off) {
            static bool     Low_power_bink;
            static uint16_t Low_power_time;
            if (timer_elapsed(Low_power_time) >= 500) {
                Low_power_bink = !Low_power_bink;
                Low_power_time = timer_read32();
            }
            if (Low_power_bink) {
                writePinLow(LED_CHRG_PIN);
            } else {
                writePinHigh(LED_CHRG_PIN);
            }
        } else if (low_battery_vol_off) {
            writePinHigh(LED_CHRG_PIN);
            extern bool low_vol_offed_sleep;
            if (timer_elapsed32(pressed_time) > 2000) {
                kb_sleep_flag = true;
            }
            low_vol_offed_sleep = true;
        } else {
            writePinHigh(LED_CHRG_PIN);
            writePinHigh(LED_DOWN_PIN);
        }
    }
    /*************************************************************************************/
    if (dev_info.devs != DEVS_USB) {
        // Show device status when FN pressed
        if ((get_highest_layer(layer_state) == 1) || (get_highest_layer(layer_state) == 4)) {
            rgb_matrix_set_color(rgb_index_table[dev_info.devs], rgb_index_color_table[dev_info.devs][0], rgb_index_color_table[dev_info.devs][1], rgb_index_color_table[dev_info.devs][2]);
        }

        // if (query_vol_flag) {
        //     for (int i = 16; i < 16 + (bts_info.bt_info.pvol / 10); i++) {
        //         if (bts_info.bt_info.pvol > 70) {
        //             rgb_matrix_set_color(i, RGB_GREEN);
        //         } else if ((bts_info.bt_info.pvol <= 70) && (bts_info.bt_info.pvol > 30)) {
        //             rgb_matrix_set_color(i, RGB_GREEN);
        //         } else {
        //             rgb_matrix_set_color(i, RGB_RED);
        //         }
        //     }
        // }

        // if (!backlight_sleep_flag && !bt_init_time && !kb_sleep_flag && (bts_info.bt_info.paired) && timer_elapsed32(query_vol_time) >= 5000) {
        if (!backlight_sleep_flag && !bt_init_time && !kb_sleep_flag && timer_elapsed32(query_vol_time) >= 4000) {
            query_vol_time = timer_read32();

            if (uart3_available()) {
                uart3_receive(uart_data_read, 3);
            }
            if ((uart_data_read[0] == 0xA7) && (uart_data_read[2] == ((uart_data_read[0] + uart_data_read[1]) & 0xFF))) {
                if ((uart_data_read[1] <= 10) && readPin(BT_CABLE_PIN)) {
                    low_battery_vol = true;
                }
                if ((uart_data_read[1] == 0) && readPin(BT_CABLE_PIN)) {
                    low_battery_vol_off = true;
                }
                // if (uart_data_read[1] == 100) {
                //     bat_vol_full = true;
                // writePinHigh(LED_CHRG_PIN);
                // writePinHigh(LED_DOWN_PIN);
                // } else {
                //     bat_vol_full = false;
                // }

                uart_data_send[0] = uart_data_read[0];
                uart_data_send[1] = uart_data_read[1];
                uart_data_send[2] = (uart_data_send[0] + uart_data_send[1]) & 0xFF;

                uart_transmit(uart_data_send, 3);
            }
        }

        led_ble();
    }

    return true;
}
