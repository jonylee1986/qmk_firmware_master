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

#include "quantum.h"
#include "uart.h"
#include "report.h"
#include "usb_main.h"
#include "bt_task.h"

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
        .bt_name        = {"YUNZII QL75 BT$", "YUNZII QL75 BT$", "YUNZII QL75 BT$"},
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
static void bt_mousekey_task(void);

#ifdef RGB_MATRIX_ENABLE
void        open_rgb(void);
static void close_rgb(void);
#endif
// clang-format off
long_pressed_keys_t long_pressed_keys[] = {
    {.keycode = BT_HOST1, .press_time = 0, .event_cb = long_pressed_keys_cb},
    {.keycode = BT_HOST2, .press_time = 0, .event_cb = long_pressed_keys_cb},
    {.keycode = BT_HOST3, .press_time = 0, .event_cb = long_pressed_keys_cb},
    {.keycode = BT_2_4G, .press_time = 0, .event_cb = long_pressed_keys_cb},
    // {.keycode = GU_TOGG, .press_time = 0, .event_cb = long_pressed_keys_cb},
    {.keycode = EE_CLR, .press_time = 0, .event_cb = long_pressed_keys_cb},
    {.keycode = TO(0), .press_time = 0, .event_cb = long_pressed_keys_cb},
    {.keycode = TO(2), .press_time = 0, .event_cb = long_pressed_keys_cb},
    // {.keycode = RGB_TEST, .press_time = 0, .event_cb = long_pressed_keys_cb},
};
// clang-format on

#include "command.h"
#include "action.h"

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
        bts_process_keys(code, 0, dev_info.devs, keymap_config.no_gui);
        bts_task(dev_info.devs);
        while (bts_is_busy()) {
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

    if (dev_info.devs != DEVS_USB) {
        usbDisconnectBus(&USB_DRIVER);
        usbStop(&USB_DRIVER);
        // writePinHigh(A12);
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
                // bts_send_name(DEVS_HOST2);
            } break;
            case DEVS_HOST3: {
                bts_send_vendor(v_host3);
                // bts_send_name(DEVS_HOST3);
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
    }

    /* Execute every 1ms */
    if (timer_elapsed32(last_time) >= 1) {
        last_time = timer_read32();

        if (dev_info.devs != DEVS_USB) {
            uint8_t keyboard_led_state = 0;
            led_t  *kb_leds            = (led_t *)&keyboard_led_state;
            kb_leds->raw               = bts_info.bt_info.indictor_rgb_s;
            usb_device_state_set_leds(keyboard_led_state);
        }

        // bts_task(dev_info.devs);
        bt_mousekey_task();

        // if (dev_info.devs != DEVS_USB) {
        // 设定指示灯状态
#ifdef RGB_MATRIX_ENABLE
        close_rgb();
#endif
        // }
    }

    long_pressed_keys_hook();
    bt_scan_mode();
}

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
    }
    // clang-format on
    retval = process_record_other(keycode, record);

    if (dev_info.devs != DEVS_USB) {
        if (retval != false) {
            retval = bts_process_keys(keycode, record->event.pressed, dev_info.devs, keymap_config.no_gui);
        }
#ifdef RGB_MATRIX_ENABLE
        open_rgb();
#endif
    }

    return retval;
}

void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset) {
    bool usb_sws = !!last_mode ? !now_mode : !!now_mode;

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
    if (dev_info.devs != DEVS_USB) {
        dev_info.last_devs = dev_info.devs;
    }
    bts_info.bt_info.pairing       = false;
    bts_info.bt_info.paired        = false;
    bts_info.bt_info.come_back     = false;
    bts_info.bt_info.come_back_err = false;
    bts_info.bt_info.mode_switched = false;
    // 设定指示灯状态
    bts_info.bt_info.indictor_rgb_s = 0;
    eeconfig_update_user(dev_info.raw);
    if (dev_info.devs == DEVS_USB) {
        writePinLow(A14);
    } else {
        writePinHigh(A14);
    }

    switch (dev_info.devs) {
        case DEVS_HOST1: {
            if (reset != false) {
                indicator_status          = 1;
                indicator_reset_last_time = true;
                // bts_send_name(DEVS_HOST1);
                // bts_send_vendor(v_host1);
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
                indicator_reset_last_time = true;
                // bts_send_name(DEVS_HOST2);
                // bts_send_vendor(v_host2);
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
                // bts_send_name(DEVS_HOST3);
                // bts_send_vendor(v_host3);
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
                // bts_send_name(DEVS_2_4G);
                // bts_send_vendor(v_2_4g);
                bts_send_vendor(v_pair);
            } else if (last_mode != DEVS_2_4G) {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_2_4g);
            }
        } break;
        case DEVS_USB: {
            bts_send_vendor(v_usb);
            indicator_status = 1;
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

static uint16_t bt_tap_time;

bool        query_vol_flag;
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
#ifndef BT_MODE_SW_PIN
                if (dev_info.devs != DEVS_HOST1) {
#else
                if ((dev_info.devs != DEVS_HOST1) && (dev_info.devs != DEVS_USB)) {
#endif
                    bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                }
            }
        } break;
        case BT_HOST2: {
            if (record->event.pressed) {
#ifndef BT_MODE_SW_PIN
                if (dev_info.devs != DEVS_HOST2) {
#else
                if ((dev_info.devs != DEVS_HOST2) && (dev_info.devs != DEVS_USB)) {
#endif
                    bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                }
            }
        } break;
        case BT_HOST3: {
            if (record->event.pressed) {
#ifndef BT_MODE_SW_PIN
                if (dev_info.devs != DEVS_HOST3) {
#else
                if ((dev_info.devs != DEVS_HOST3) && (dev_info.devs != DEVS_USB)) {
#endif
                    bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                }
            }
        } break;
        case BT_2_4G: {
            if (record->event.pressed) {
#ifndef BT_MODE_SW_PIN
                if (dev_info.devs != DEVS_2_4G) {
#else
                if ((dev_info.devs != DEVS_2_4G) && (dev_info.devs != DEVS_USB)) {
#endif
                    bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                }
            }
        } break;
        case BT_USB: {
            if (record->event.pressed) {
                bt_switch_mode(dev_info.devs, DEVS_USB, false);
            }
        } break;
        case RGB_TEST: {
            extern uint8_t rgb_test_en;
            extern uint8_t rgb_test_index;
            if (record->event.pressed) {
                rgb_test_en = !rgb_test_en;
                if (rgb_test_en) {
                    rgb_test_index = 4;
                } else {
                    rgb_test_index = 0;
                }
            }
        } break;
        case BT_VOL: {
            if (record->event.pressed) {
                bts_send_vendor(v_query_vol);
                query_vol_flag = true;
            } else {
                query_vol_flag = false;
            }
        } break;
        case EE_CLR:
        case TO(0): // WIN_BASE
        case TO(2): // MAC_BASE
            break;
        case S(KC_1):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_1, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_2):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_2, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_3):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_3, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_4):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_4, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_5):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_5, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_6):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_6, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_7):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_7, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_8):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_8, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_9):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_9, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_0):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_0, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_MINS):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_MINS, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_EQL):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_EQL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_LEFT_BRACKET):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_LEFT_BRACKET, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_RIGHT_BRACKET):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_RIGHT_BRACKET, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_BACKSLASH):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_BACKSLASH, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_SEMICOLON):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_SEMICOLON, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_QUOTE):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_QUOTE, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_COMMA):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_COMMA, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_DOT):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_DOT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case S(KC_SLASH):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                bts_process_keys(KC_SLASH, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true; // Skip all further processing of this key
        case KC_LGUI:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    add_mods(MOD_BIT(KC_LGUI));
                } else {
                    del_mods(MOD_BIT(KC_LGUI));
                }
            }
            return true;
        case KC_LSFT:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    add_mods(MOD_BIT(KC_LSFT));
                } else {
                    del_mods(MOD_BIT(KC_LSFT));
                }
            }
            return true;
        case KC_RGUI:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    add_mods(MOD_BIT(KC_RGUI));
                } else {
                    del_mods(MOD_BIT(KC_RGUI));
                }
            }
            return true;
        case KC_RSFT:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    add_mods(MOD_BIT(KC_RSFT));
                } else {
                    del_mods(MOD_BIT(KC_RSFT));
                }
            }
            return true;
        case QK_GRAVE_ESCAPE:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    if ((get_mods() & MOD_MASK_SG)) {
                        bts_process_keys(KC_GRAVE, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    } else {
                        bts_process_keys(KC_ESCAPE, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    }
                } else {
                    bts_process_keys(KC_GRAVE, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                    bts_process_keys(KC_ESCAPE, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                }
            }
            return true; // Skip all further processing of this key
        case LM(1, MOD_LCTL):
        case LM(2, MOD_LCTL):
        case LM(3, MOD_LCTL):
            if (dev_info.devs) {
                bts_process_keys(KC_LCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_LSFT):
        case LM(2, MOD_LSFT):
        case LM(3, MOD_LSFT):
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_LALT):
        case LM(2, MOD_LALT):
        case LM(3, MOD_LALT):
            if (dev_info.devs) {
                bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_LGUI):
        case LM(2, MOD_LGUI):
        case LM(3, MOD_LGUI):
            if (dev_info.devs) {
                bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_RCTL):
        case LM(2, MOD_RCTL):
        case LM(3, MOD_RCTL):
            if (dev_info.devs) {
                bts_process_keys(KC_RCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_RSFT):
        case LM(2, MOD_RSFT):
        case LM(3, MOD_RSFT):
            if (dev_info.devs) {
                bts_process_keys(KC_RSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_RALT):
        case LM(2, MOD_RALT):
        case LM(3, MOD_RALT):
            if (dev_info.devs) {
                bts_process_keys(KC_RALT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LM(1, MOD_RGUI):
        case LM(2, MOD_RGUI):
        case LM(3, MOD_RGUI):
            if (dev_info.devs) {
                bts_process_keys(KC_RGUI, record->event.pressed, dev_info.devs, keymap_config.no_gui);
            }
            return true;
        case LT(1, KC_SPC):
        case LT(2, KC_SPC):
        case LT(3, KC_SPC):
            if (dev_info.devs) {
                if (record->tap.count && record->event.pressed) {
                    bts_process_keys(KC_SPC, 1, dev_info.devs, keymap_config.no_gui);
                    bts_task(dev_info.devs);
                    for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        wait_ms(1);
                    }
                    bts_process_keys(KC_SPC, 0, dev_info.devs, keymap_config.no_gui);
                    bts_task(dev_info.devs);
                    return false; // 通过返回false阻止对该键的其它处理
                }
            }
            return true;
        case SC_LCPO:
            if (dev_info.devs) {
                bts_process_keys(KC_LCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_LSFT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case SC_LSPO:
            if (dev_info.devs) {
                bts_process_keys(KC_LSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_LSFT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case SC_LAPO:
            if (dev_info.devs) {
                bts_process_keys(KC_LALT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_LSFT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case SC_RCPC:
            if (dev_info.devs) {
                bts_process_keys(KC_RCTL, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_RSFT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case SC_RSPC:
            if (dev_info.devs) {
                bts_process_keys(KC_RSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_RSFT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case SC_RAPC:
            if (dev_info.devs) {
                bts_process_keys(KC_RALT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_RSFT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case SC_SENT:
            if (dev_info.devs) {
                bts_process_keys(KC_RSFT, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                if (record->event.pressed) {
                    bt_tap_time = timer_read();
                } else {
                    if (timer_elapsed(bt_tap_time) <= 200) {
                        bts_process_keys(KC_ENT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                            wait_ms(1);
                        }
                        bts_process_keys(KC_ENT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
        case KC_MS_U:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.move_y.pressed = 1;
                    bt_mousekey.move_y.dir     = 1;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    if (bt_mousekey.move_y.dir) {
                        bt_mousekey.move_y.pressed = 0;
                        // bt_mousekey.pressed_time   = 0;
                    }
                }
                return false;
            }
            return true;
        case KC_MS_D:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.move_y.pressed = 1;
                    bt_mousekey.move_y.dir     = 0;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    if (!bt_mousekey.move_y.dir) {
                        bt_mousekey.move_y.pressed = 0;
                        // bt_mousekey.pressed_time   = 0;
                    }
                }
                return false;
            }
            return true;
        case KC_MS_L:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.move_x.pressed = 1;
                    bt_mousekey.move_x.dir     = 1;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    if (bt_mousekey.move_x.dir) {
                        bt_mousekey.move_x.pressed = 0;
                        // bt_mousekey.pressed_time   = 0;
                    }
                }
                return false;
            }
            return true;
        case KC_MS_R:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.move_x.pressed = 1;
                    bt_mousekey.move_x.dir     = 0;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    if (!bt_mousekey.move_x.dir) {
                        bt_mousekey.move_x.pressed = 0;
                        // bt_mousekey.pressed_time   = 0;
                    }
                }
                return false;
            }
            return true;
        case KC_WH_U:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    // bt_mousekey.data[1] = 0;
                    // bt_mousekey.data[2] = 0;
                    // bt_mousekey.data[3] = 0;
                    bt_mousekey.data[3] = 0x01;
                    bts_send_mouse_report(bt_mousekey.data);
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.data[3] = 0;
                    bts_send_mouse_report(bt_mousekey.data);
                    // bt_mousekey.wheel_y.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
                return false;
            }
            return true;
        case KC_WH_D:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    // bt_mousekey.data[1] = 0;
                    // bt_mousekey.data[2] = 0;
                    // bt_mousekey.data[3] = 0;
                    bt_mousekey.data[3] = 0xfe;
                    bts_send_mouse_report(bt_mousekey.data);
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.data[3] = 0;
                    bts_send_mouse_report(bt_mousekey.data);
                    // bt_mousekey.wheel_y.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
                return false;
            }
            return true;
        case KC_WH_L:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    // bt_mousekey.data[1] = 0;
                    // bt_mousekey.data[2] = 0;
                    // bt_mousekey.data[3] = 0;
                    bt_mousekey.data[4] = 0xfe;
                    bts_send_mouse_report(bt_mousekey.data);
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.data[4] = 0;
                    bts_send_mouse_report(bt_mousekey.data);
                    // bt_mousekey.wheel_y.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
            }
            return true;
        case KC_WH_R:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    // bt_mousekey.data[1] = 0;
                    // bt_mousekey.data[2] = 0;
                    // bt_mousekey.data[3] = 0;
                    bt_mousekey.data[4] = 0x01;
                    bts_send_mouse_report(bt_mousekey.data);
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.data[4] = 0;
                    bts_send_mouse_report(bt_mousekey.data);
                    // bt_mousekey.wheel_y.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
                return false;
            }
            return true;
        default:
            return true;
    }

    return false;
}
uint16_t    EE_CLR_time;
uint8_t     EE_CLR_cnt;
static void long_pressed_keys_cb(uint16_t keycode) {
    switch (keycode) {
        case BT_HOST1: {
            if (dev_info.devs != DEVS_USB) {
                bt_switch_mode(dev_info.devs, DEVS_HOST1, true);
            }
        } break;
        case BT_HOST2: {
            if (dev_info.devs != DEVS_USB) {
                bt_switch_mode(dev_info.devs, DEVS_HOST2, true);
            }
        } break;
        case BT_HOST3: {
            if (dev_info.devs != DEVS_USB) {
                bt_switch_mode(dev_info.devs, DEVS_HOST3, true);
            }
        } break;
        case BT_2_4G: {
            if (dev_info.devs != DEVS_USB) {
                bt_switch_mode(dev_info.devs, DEVS_2_4G, true);
            }
        } break;
        case EE_CLR: {
            EE_CLR_time = timer_read();
            EE_CLR_cnt  = 7;
        } break;
        case TO(0): { // WIN_BASE
            set_single_persistent_default_layer(0);
        } break;
        case TO(2): { // MAC_BASE
            set_single_persistent_default_layer(2);
        } break;
        case RGB_TEST: {
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
#ifdef BT_MODE_SW_PIN
    setPinInput(BT_MODE_SW_PIN);
#endif

#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
    setPinInput(BT_CABLE_PIN);
    setPinInput(BT_CHARGE_PIN);
#endif
}

/**
 * @brief 根据波动开关判断工作模式
 * @param None
 * @return None
 */
static void bt_scan_mode(void) {
#ifdef BT_MODE_SW_PIN
    uint8_t now_mode = false; // 高电平为USB模式
    uint8_t usb_sws  = false;

    now_mode = readPin(BT_MODE_SW_PIN);
    usb_sws  = !!dev_info.devs ? !now_mode : now_mode;

    if (usb_sws) {
        if (now_mode) {
            bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // ble mode
        } else {
            bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
        }
    }
#endif
}

static uint32_t key_press_time;
static uint32_t close_rgb_time;
static bool     bak_rgb_toggle;
static bool     sober         = true;
static bool     kb_sleep_flag = false;
extern bool     led_inited;
extern void     led_config_all(void);
extern void     led_deconfig_all(void);

static void close_rgb(void) {
    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }

    if (sober) {
        if (kb_sleep_flag || (timer_elapsed32(key_press_time) >= (3 * 60000) && dev_info.devs != DEVS_USB)) { // 3 minutes
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();
#ifdef WS2812_EN_PIN
            writePinHigh(WS2812_EN_PIN);
            // writePinHigh(A14);
#endif
        }
    } else {
        if (!rgb_matrix_config.enable) {
            if (timer_elapsed32(close_rgb_time) >= ENTRY_STOP_TIMEOUT) {
                /* Turn off all indicators led */
                if (led_inited) {
                    led_deconfig_all();
                }

#ifdef ENTRY_STOP_MODE
                lp_system_sleep();
                if (dev_info.devs != DEVS_USB) {
                    bt_switch_mode(DEVS_USB, dev_info.last_devs, false); // ble mode
                } else {
                    bt_switch_mode(dev_info.last_devs, DEVS_USB, false); // usb mode
                }
                extern void open_rgb(void);
                open_rgb();
#endif
            }
        }
    }
}

void open_rgb(void) {
    key_press_time = timer_read32();
    if (!sober) {
#ifdef WS2812_EN_PIN
        setPinOutput(WS2812_EN_PIN);
        writePinLow(WS2812_EN_PIN);
#endif
        if (bak_rgb_toggle) {
            kb_sleep_flag = false;
            rgb_matrix_enable_noeeprom();
        }
        if (!led_inited) {
            led_config_all();
        }
        sober = true;
    }
}

uint8_t indicator_status          = 2;
uint8_t indicator_reset_last_time = false;

uint8_t bt_indicator_rgb(uint8_t led_min, uint8_t led_max) {
    // FN 按下时显示当前设备状态
    if ((query_vol_flag) && (dev_info.devs != DEVS_USB)) { // 电量显示
        rgb_matrix_set_color_all(0, 0, 0);
        for (uint8_t i = 1; i <= (bts_info.bt_info.pvol / 10); i++) {
            rgb_matrix_set_color(i, 60, 60, 60);
        }
    }
    if (EE_CLR_cnt) {
        if (EE_CLR_cnt & 1) {
            rgb_matrix_set_color_all(60, 60, 60);
        } else {
            rgb_matrix_set_color_all(0, 0, 0);
        }
        if (timer_elapsed(EE_CLR_time) > 300) {
            if (EE_CLR_cnt > 0) {
                EE_CLR_time = timer_read();
                EE_CLR_cnt--;
            }
            if (!EE_CLR_cnt) {
                extern bool ALL_KEY_LOCK;
                ALL_KEY_LOCK         = 0;
                keymap_config.no_gui = 0;
                eeconfig_update_keymap(&keymap_config);

                //    eeconfig_init();
                eeconfig_update_rgb_matrix_default();
                dev_info.last_devs = DEVS_HOST1;
                eeconfig_update_user(dev_info.raw);
            }
        }
    }
    return true;
}

void bt_indicator_led(void) {
#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
    // 充电接入
    if (readPin(BT_CABLE_PIN)) {
        if (readPin(BT_CHARGE_PIN)) {
            // 充满
            writePinHigh(INDLED_VOL_PIN);

        } else {
            // 未满
            static uint16_t charge_time = 0;
            static bool     charge_flip = false;
            if (timer_elapsed(charge_time) > 500) {
                charge_time = timer_read();
                charge_flip = !charge_flip;
            }
            writePin(INDLED_VOL_PIN, charge_flip);
        }
    } else {
        if (bts_info.bt_info.low_vol) {
            static uint16_t low_vol_time = 0;
            static bool     low_vol_flip = false;
            if (timer_elapsed(low_vol_time) > 200) {
                low_vol_time = timer_read();
                low_vol_flip = !low_vol_flip;
                writePin(INDLED_VOL_PIN, low_vol_flip);
            }
        } else {
            writePinLow(INDLED_VOL_PIN);
        }
    }
#endif

    static uint32_t last_time       = 0;
    static uint32_t last_long_time  = 0;
    static uint32_t last_total_time = 0;
    static uint8_t  last_status     = 0;
    static bool     led_flip        = false;

    if (last_status != indicator_status) {
        last_status     = indicator_status;
        last_total_time = timer_read32();
    }

    if (indicator_reset_last_time != false) {
        indicator_reset_last_time = false;
        last_time                 = 0;
    }

    switch (indicator_status) {
        case 1: { // 闪烁模式 5Hz 重置
            if ((last_time == 0) || (timer_elapsed32(last_time) >= 200)) {
                last_time = timer_read32();
                led_flip  = !led_flip;
            }
            if (dev_info.devs == DEVS_USB) {
                if (timer_elapsed32(last_total_time) >= (7 * 200)) {
                    last_long_time = timer_read32();
                    if (USB_DRIVER.state == USB_ACTIVE) {
                        indicator_status = 3;
                    } else {
                        indicator_status = 0;
                        kb_sleep_flag    = true;
                    }
                    break;
                }
            } else {
                writePinLow(LED_CAPS_LOCK_PIN);
            }
            if ((timer_elapsed32(last_total_time) >= (500)) && (bts_info.bt_info.paired)) {
                last_long_time   = timer_read32();
                indicator_status = 3;
                break;
            }

            /* 超时60s退出 */
            if (timer_elapsed32(last_total_time) >= (60 * 1000)) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;
        case 2: { // 闪烁模式 2Hz 回连
            if ((last_time == 0) || (timer_elapsed32(last_time) >= 500)) {
                last_time = timer_read32();
                led_flip  = !led_flip;
            }

            if (bts_info.bt_info.paired) {
                last_long_time   = timer_read32();
                indicator_status = 3;
                break;
            }
            if (dev_info.devs == DEVS_USB && USB_DRIVER.state == USB_ACTIVE) {
                last_long_time   = timer_read32();
                indicator_status = 3;
                break;
            }

            /* 超时30s退出 */
            if (timer_elapsed32(last_total_time) >= (30 * 1000)) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;
        case 3: { // 长亮模式
            if ((timer_elapsed32(last_long_time) < (5 * 1000))) {
                led_flip = true;
            } else {
                indicator_status = 0;
            }
            writePin(LED_CAPS_LOCK_PIN, (host_keyboard_led_state().caps_lock));
        } break;
        case 4: { // 长灭模式
            led_flip = false;
        } break;
        default:
            if (dev_info.devs == DEVS_USB) {
                led_flip = false;
                if (!readPin(BT_CABLE_PIN)) {
                    bt_switch_mode(dev_info.last_devs, DEVS_USB, false);
                }
            } else {
                led_flip = true;
                if (!bts_info.bt_info.paired) {
                    bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
                }
            }
            break;
    }
    if (EE_CLR_cnt) {
        writePin(INDLED_BT1_PIN, EE_CLR_cnt & 1);
        writePin(INDLED_BT2_PIN, EE_CLR_cnt & 1);
        writePin(INDLED_BT3_PIN, EE_CLR_cnt & 1);
    } else {
        switch (dev_info.devs) {
            case DEVS_USB: {
                writePin(INDLED_BT1_PIN, led_flip);
                writePin(INDLED_BT2_PIN, led_flip);
                writePin(INDLED_BT3_PIN, led_flip);
                break;
            }
            case DEVS_HOST1: {
                writePin(INDLED_BT1_PIN, led_flip);
                writePin(INDLED_BT2_PIN, false);
                writePin(INDLED_BT3_PIN, false);
                break;
            }
            case DEVS_HOST2: {
                writePin(INDLED_BT1_PIN, false);
                writePin(INDLED_BT2_PIN, led_flip);
                writePin(INDLED_BT3_PIN, false);
                break;
            }
            case DEVS_HOST3: {
                writePin(INDLED_BT1_PIN, false);
                writePin(INDLED_BT2_PIN, false);
                writePin(INDLED_BT3_PIN, led_flip);
                break;
            }
            case DEVS_2_4G: {
                writePin(INDLED_BT1_PIN, led_flip);
                writePin(INDLED_BT2_PIN, led_flip);
                writePin(INDLED_BT3_PIN, led_flip);
                break;
            }
            default:
                break;
        }
    }
}
