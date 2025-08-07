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
#include "common/bt_task.h"

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
        .bt_name        = {"TH85 MAX-$", "TH85 MAX-$", "TH85 MAX-$"},
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
//   {.keycode = GU_TOGG, .press_time = 0, .event_cb = long_pressed_keys_cb},
//   {.keycode = SW_OS1, .press_time = 0, .event_cb = long_pressed_keys_cb},
//   {.keycode = TO(0), .press_time = 0, .event_cb = long_pressed_keys_cb},
//   {.keycode = TO(2), .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = SLP_TG, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = RGB_TEST, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = EE_CLR, .press_time = 0, .event_cb = long_pressed_keys_cb},
};
// clang-format on

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

#if 0
uint16_t bt_mod_tap_pressed_time;
uint16_t bt_mod_tap_released_time;
uint16_t bt_mod_tap_keycode;
void     bt_modcode_send(uint16_t keycode, bool pressed) {
    if ((keycode > QK_MODS) && (keycode <= QK_MODS_MAX)) {
        if (QK_MODS_GET_MODS(keycode) & 0x1) {
            if (QK_MODS_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RCTL, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LCTL, pressed, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(keycode) & 0x2) {
            if (QK_MODS_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RSFT, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LSFT, pressed, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(keycode) & 0x4) {
            if (QK_MODS_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RALT, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LALT, pressed, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MODS_GET_MODS(keycode) & 0x8) {
            if (QK_MODS_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RGUI, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LGUI, pressed, dev_info.devs, keymap_config.no_gui);
        }
    } else if ((keycode > QK_MOD_TAP) && (keycode <= QK_MOD_TAP_MAX)) {
        if (QK_MOD_TAP_GET_MODS(keycode) & 0x1) {
            if (QK_MOD_TAP_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RCTL, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LCTL, pressed, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MOD_TAP_GET_MODS(keycode) & 0x2) {
            if (QK_MOD_TAP_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RSFT, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LSFT, pressed, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MOD_TAP_GET_MODS(keycode) & 0x4) {
            if (QK_MOD_TAP_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RALT, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LALT, pressed, dev_info.devs, keymap_config.no_gui);
        }
        if (QK_MOD_TAP_GET_MODS(keycode) & 0x8) {
            if (QK_MOD_TAP_GET_MODS(keycode) & 0x10)
                bts_process_keys(KC_RGUI, pressed, dev_info.devs, keymap_config.no_gui);
            else
                bts_process_keys(KC_LGUI, pressed, dev_info.devs, keymap_config.no_gui);
        }
    }
}
#endif
/**
 * @brief bluetooth交互任务
 * @param event 当前事件
 * @return None
 */
void bt_task(void) {
    static uint32_t last_time = 0;

    if ((bt_init_time != 0) && (timer_elapsed32(bt_init_time) >= 2000)) {
        bt_init_time = 0;

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

        // 设定指示灯状态

        // bts_task(dev_info.devs);

        if (dev_info.devs != DEVS_USB) {
            uint8_t keyboard_led_state = 0;
            led_t  *kb_leds            = (led_t *)&keyboard_led_state;
            kb_leds->raw               = bts_info.bt_info.indictor_rgb_s;
            usb_device_state_set_leds(keyboard_led_state);

#ifdef RGB_MATRIX_ENABLE
            if (!dev_info.sleep_off_flag) {
                close_rgb();
            }
#endif
        }
    }

    // if (bts_info.bt_info.low_vol) {
    //     void blink_rgb_set(uint8_t, uint8_t);
    //     blink_rgb_set(3, 0);
    // }
    bt_mousekey_task();
    long_pressed_keys_hook();
    bt_scan_mode();
}
uint32_t pressed_time = 0;
bool     rgb_status_save;
bool     process_record_bt(uint16_t keycode, keyrecord_t *record) {
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
            extern bool LCD_DONT_SEND;
            LCD_DONT_SEND = 0;
            LCD_command_update(LCD_WEAKUP);
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
    extern bool LCD_DONT_SEND;
    LCD_DONT_SEND = 0;
    LCD_command_update(LCD_WEAKUP);
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
    // 设定指示灯状态
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

uint32_t        EE_CLR_press_cnt  = 0;
uint32_t        EE_CLR_press_time = 0;
bool            EE_CLR_flag       = false;
bool            query_vol_flag    = false;
static uint8_t  BT_SW_blink_cnt   = 0;
static uint32_t BT_SW_blink_time  = 0;
uint8_t         single_blink_cnt;
uint8_t         single_blink_index;
uint32_t        single_blink_time;
uint8_t         SLP_TG_CNT;

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
                if ((dev_info.devs != DEVS_2_4G) && (dev_info.devs != DEVS_USB)) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                }
            }
        } break;
        case BT_HOST2: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_2_4G) && (dev_info.devs != DEVS_USB)) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                }
            }
        } break;
        case BT_HOST3: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_2_4G) && (dev_info.devs != DEVS_USB)) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                }
            }
        } break;
        case BT_2_4G: {
            // if (record->event.pressed) {
            //     if (dev_info.devs != DEVS_2_4G) {
            //         bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
            //     }
            // }
        } break;
        case BT_USB: {
            if (record->event.pressed) {
                bt_switch_mode(dev_info.devs, DEVS_USB, false);
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
        case RGB_TEST: {
            // if (record->event.pressed) {
            //     extern uint8_t rgb_test_en;
            //     if (rgb_test_en) {
            //         rgb_test_en = false;
            //     }
            // }
        } break;
        case EE_CLR: {
        } break;
        case RGB_TOG: {
            if (record->event.pressed) {
                rgb_status_save = !rgb_matrix_config.enable;
            }
            return true;
        }
        case SLP_TG: {
            if (record->event.pressed) {
                SLP_TG_CNT++;
            } else {
                if (SLP_TG_CNT > 0) SLP_TG_CNT--;
            }
        } break;
        case GU_TOGG: {
            if (record->event.pressed) {
                keymap_config.no_gui = !keymap_config.no_gui;
            }
            return false;
        }
        case BT_SW: {
            if (record->event.pressed) {
                BT_SW_blink_cnt  = 6;
                BT_SW_blink_time = 0;
                if (dev_info.devs == DEVS_USB) {
                    bt_switch_mode(dev_info.devs, dev_info.last_devs, false);
                } else if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                    bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                } else if (dev_info.devs == DEVS_2_4G) {
                    bt_switch_mode(dev_info.devs, DEVS_USB, false);
                }
            }
        } break;
#if 0
        case TO(0): { // WIN_BASE
            if (record->event.pressed) {
                set_single_persistent_default_layer(0);
            }
        } break;
        case TO(2): { // MAC_BASE
            if (record->event.pressed) {
                set_single_persistent_default_layer(2);
                keymap_config.no_gui = 0;
                eeconfig_update_keymap(&keymap_config);
            }
        } break;
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_LSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_9, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_RSFT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_process_keys(KC_0, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
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
                    if (timer_elapsed(bt_tap_time) <= 100) {
                        bts_process_keys(KC_ENT, 1, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                        // for (uint16_t i = TAP_CODE_DELAY; i > 0; i--) {
                        //     wait_ms(1);
                        // }
                        bts_process_keys(KC_ENT, 0, dev_info.devs, keymap_config.no_gui);
                        bts_task(dev_info.devs);
                    }
                }
                return false; // 通过返回false阻止对该键的其它处理
            }
            return true;
#endif
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
                    bt_mousekey.wheel_y.pressed = 1;
                    bt_mousekey.wheel_y.dir     = 0;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.wheel_y.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
                return false;
            }
            return true;
        case KC_WH_D:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.wheel_y.pressed = 1;
                    bt_mousekey.wheel_y.dir     = 1;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.wheel_y.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
                return false;
            }
            return true;
        case KC_WH_L:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.wheel_x.pressed = 1;
                    bt_mousekey.wheel_x.dir     = 1;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.wheel_x.pressed = 0;
                    // bt_mousekey.pressed_time   = 0;
                }
                return false;
            }
            return true;
        case KC_WH_R:
            if (dev_info.devs) {
                if (record->event.pressed) {
                    bt_mousekey.wheel_x.pressed = 1;
                    bt_mousekey.wheel_x.dir     = 0;
                    // if (!bt_mousekey.pressed_time) bt_mousekey.pressed_time = timer_read();
                } else {
                    bt_mousekey.wheel_x.pressed = 0;
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

static void long_pressed_keys_cb(uint16_t keycode) {
    switch (keycode) {
        case BT_HOST1: {
            if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                bt_switch_mode(dev_info.devs, DEVS_HOST1, true);
            }
        } break;
        case BT_HOST2: {
            if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                bt_switch_mode(dev_info.devs, DEVS_HOST2, true);
            }
        } break;
        case BT_HOST3: {
            if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                bt_switch_mode(dev_info.devs, DEVS_HOST3, true);
            }
        } break;
        case BT_2_4G: {
            if (dev_info.devs == DEVS_2_4G) {
                bt_switch_mode(dev_info.devs, DEVS_2_4G, true);
            }
        } break;
        // case GU_TOGG: {
        //     keymap_config.no_gui = !keymap_config.no_gui;
        // } break;
        // case TO(0): { // WIN_BASE
        //     set_single_persistent_default_layer(0);
        // } break;
        // case TO(2): { // MAC_BASE
        //     set_single_persistent_default_layer(2);
        //     keymap_config.no_gui = 0;
        //     eeconfig_update_keymap(keymap_config.raw);
        // } break;
        // case SW_OS1: {
        //     if (get_highest_layer(default_layer_state) == 0) { // WIN_BASE
        //         set_single_persistent_default_layer(2);
        //         extern void blink_rgb_set(uint8_t, uint8_t);
        //         blink_rgb_set(1, 0);
        //     } else if (get_highest_layer(default_layer_state) == 2) { // MAC_BASE
        //         set_single_persistent_default_layer(0);
        //         extern void blink_rgb_set(uint8_t, uint8_t);
        //         blink_rgb_set(0, 0);
        //     }
        // } break;
        case RGB_TEST: {
            if (dev_info.devs == DEVS_USB) {
                extern void    blink_rgb_set(uint8_t, uint8_t);
                extern uint8_t rgb_test_en;
                extern uint8_t rgb_test_index;

                if (rgb_test_en != true) {
                    // blink_rgb_set(2, 0);
                    rgb_test_en    = true;
                    rgb_test_index = 1;
                }
            }
        } break;
        case EE_CLR: {
            if (!EE_CLR_flag) {
                EE_CLR_flag       = true;
                EE_CLR_press_time = timer_read32();
                EE_CLR_press_cnt  = 1;
            }
        } break;
        case SLP_TG: {
            if (SLP_TG_CNT > 1) {
                dev_info.sleep_off_flag = !dev_info.sleep_off_flag;
                eeconfig_update_kb(dev_info.raw);
                SLP_TG_CNT         = 0;
                single_blink_time  = timer_read32();
                single_blink_cnt   = 6;
                single_blink_index = 0;
            }
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
    setPinInputHigh(BT_MODE_SW_PIN);
    setPinInputHigh(RF_MODE_SW_PIN);
#endif

#if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
    setPinInputHigh(BT_CABLE_PIN);
    setPinInputHigh(BT_CHARGE_PIN);
#endif
}

/**
 * @brief 根据波动开关判断工作模式
 * @param None
 * @return None
 */
static void bt_scan_mode(void) {
#ifdef BT_MODE_SW_PIN
    if (readPin(BT_MODE_SW_PIN) && !readPin(RF_MODE_SW_PIN)) {
        if (dev_info.devs != DEVS_2_4G) bt_switch_mode(dev_info.devs, DEVS_2_4G, false); // 2_4G mode
    }
    if (readPin(RF_MODE_SW_PIN) && !readPin(BT_MODE_SW_PIN)) {
        if ((dev_info.devs == DEVS_USB) || (dev_info.devs == DEVS_2_4G)) bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // BT mode
    }
    if (readPin(BT_MODE_SW_PIN) && readPin(RF_MODE_SW_PIN)) {
        if (dev_info.devs != DEVS_USB) bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
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

bool get_kb_sleep_flag(void) {
    return kb_sleep_flag;
}

static const uint8_t rgb_index_table[] = {
    LED_USB, LED_BT1, LED_BT2, LED_BT3, LED_2G,
};
static const uint8_t rgb_index_color_table[][3] = {
    {RGB_WHITE}, {RGB_WHITE}, {RGB_WHITE}, {RGB_WHITE}, {RGB_WHITE},
};
static void close_rgb(void) {
    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }
    /*************************************************************************************/
    if (timer_elapsed32(pressed_time) >= ((5 * 60 - 40) * 1000)) { // 超时，关闭灯光
        rgb_matrix_disable_noeeprom();
        LCD_command_update(LCD_LIGHT_SLEEP);
        extern bool LCD_DONT_SEND;
        LCD_DONT_SEND = 1;
    } else {
        rgb_status_save = rgb_matrix_config.enable; // 记录超时前状态
    }
    /*************************************************************************************/
    if (sober) {
        if (kb_sleep_flag || (timer_elapsed32(key_press_time) >= ((30 * 60 - 40) * 1000))) { // 30 minutes
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();
            // writePinLow(RGB_DRIVER_SDB_PIN);
            LCD_command_update(LCD_SLEEP);
            extern bool LCD_DONT_SEND;
            LCD_DONT_SEND = 1;
        }
    } else {
        if (!rgb_matrix_config.enable) {
            if (timer_elapsed32(close_rgb_time) >= ENTRY_STOP_TIMEOUT) {
                /* Turn off all indicators led */
                if (led_inited) {
                    led_deconfig_all();
                    uart3_stop();
                }

#ifdef ENTRY_STOP_MODE
                lp_system_sleep();
#endif
                extern void open_rgb(void);
                // bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
                open_rgb();
                // LCD_charge_update();
                extern bool LCD_DONT_SEND;
                LCD_DONT_SEND = 0;
                LCD_command_update(LCD_WEAKUP);
            }
        }
    }
}

void open_rgb(void) {
    key_press_time = timer_read32();
    if (!sober) {
        // writePinHigh(RGB_DRIVER_SDB_PIN);
        if (bak_rgb_toggle) {
            kb_sleep_flag = false;
            rgb_matrix_enable_noeeprom();
        }
        if (!led_inited) {
            led_config_all();
            uart3_start();
        }
        sober = true;
    }
}

uint8_t  indicator_status          = 2;
uint8_t  indicator_reset_last_time = false;
uint32_t last_total_time           = 0;

void led_ble(void) {
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
    }
    switch (indicator_status) {
        case LED_BLE_PAIR: { // 闪烁模式 5Hz 重置
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

            /* 超时60s退出 */
            if (timer_elapsed32(last_total_time) >= BLE_PAIR_TIMEOUT) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;

        case LED_BLE_CONN: { // 闪烁模式 2Hz 回连
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
            // if (dev_info.devs == DEVS_2_4G) bts_process_keys(KC_LGUI, 0, DEVS_2_4G, 1);
            if (bts_info.bt_info.paired) {
                last_long_time   = timer_read32();
                indicator_status = 3;
                break;
            }

            /* 超时30s退出 */
            if (timer_elapsed32(last_total_time) >= BLE_CONN_TIMEOUT) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;
        case 3: { // 长亮模式
            if ((timer_elapsed32(last_long_time) < (3 * 1000))) {
                rgb.r = rgb_index_color_table[dev_info.devs][0];
                rgb.g = rgb_index_color_table[dev_info.devs][1];
                rgb.b = rgb_index_color_table[dev_info.devs][2];
            } else {
                indicator_status = 0;
            }
        } break;
        case 4: { // 长灭模式
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
            // return true;
            break;
    }
    if (indicator_status) rgb_matrix_set_color(rgb_index, rgb.r, rgb.g, rgb.b);
}

uint8_t bt_indicator_rgb(uint8_t led_min, uint8_t led_max) {
    /*************************************************************************************/
    if (EE_CLR_flag) {
        if (timer_elapsed32(EE_CLR_press_time) >= EE_CLR_press_cnt * 300) {
            EE_CLR_press_cnt++;
        }
        if (EE_CLR_press_cnt >= 7) {
            EE_CLR_press_time  = 0;
            EE_CLR_press_cnt   = 0;
            EE_CLR_flag        = false;
            uint8_t layer_save = get_highest_layer(default_layer_state);
            LCD_command_update(LCD_RESET);
            eeconfig_init();
            eeconfig_update_rgb_matrix_default();
            dev_info.LCD_PAGE       = 0;
            dev_info.sleep_off_flag = 0;
            eeconfig_update_kb(dev_info.raw);
            LCD_Page_update(dev_info.LCD_PAGE);
            set_single_persistent_default_layer(layer_save);
        }
        if (EE_CLR_press_cnt & 0x1) {
            if (EE_CLR_press_cnt == 1) {
                rgb_matrix_set_color_all(100, 0, 0);
            } else if (EE_CLR_press_cnt == 3) {
                rgb_matrix_set_color_all(0, 0, 100);
            } else if (EE_CLR_press_cnt == 5) {
                rgb_matrix_set_color_all(0, 100, 0);
            }
        } else {
            rgb_matrix_set_color_all(0, 0, 0);
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
            rgb_matrix_set_color(single_blink_index, 100, 100, 100);
        } else {
            rgb_matrix_set_color(single_blink_index, 0, 0, 0);
        }
    }
    /*************************************************************************************/
    static uint32_t charg_full_time = 0;
    static uint32_t charging_time   = 0;
    if (!readPin(BT_CABLE_PIN)) {
        if (!readPin(BT_CHARGE_PIN)) {
            // 正在充电
            if (timer_elapsed32(charging_time) > 2000) {
                rgb_matrix_set_color(0, 100, 0, 0);
            }
            charg_full_time = timer_read32();
        } else {
            // 充满
            if (timer_elapsed32(charg_full_time) > 2000) {
                rgb_matrix_set_color(0, 0, 100, 0);
            }
            charging_time = timer_read32();
        }
    } else {
        charging_time   = timer_read32();
        charg_full_time = timer_read32();
    }

    if (dev_info.devs != DEVS_USB) {
        // FN 按下时显示当前设备状态
        // if ((get_highest_layer(layer_state) == 1) || (get_highest_layer(layer_state) == 3)) {
        //     rgb_matrix_set_color(rgb_index_table[dev_info.devs], RGB_WHITE);
        // }

        if (query_vol_flag) {
            for (int i = 16; i < 16 + (bts_info.bt_info.pvol / 10); i++) {
                if (bts_info.bt_info.pvol > 70) {
                    rgb_matrix_set_color(i, RGB_GREEN);
                } else if ((bts_info.bt_info.pvol <= 70) && (bts_info.bt_info.pvol > 30)) {
                    rgb_matrix_set_color(i, RGB_GREEN);
                } else {
                    rgb_matrix_set_color(i, RGB_RED);
                }
            }
        }

        if ((bts_info.bt_info.low_vol) && readPin(BT_CABLE_PIN)) {
            rgb_matrix_set_color_all(0, 0, 0);
            static bool     Low_power_bink;
            static uint16_t Low_power_time;
            if (timer_elapsed(Low_power_time) >= 300) {
                Low_power_bink = !Low_power_bink;
                Low_power_time = timer_read32();
            }
            if (Low_power_bink) {
                rgb_matrix_set_color(0, 100, 0, 0);
            }
        }
        extern bool low_vol_offed_sleep;
        if (bts_info.bt_info.low_vol_offed && readPin(BT_CABLE_PIN)) {
            kb_sleep_flag       = true;
            low_vol_offed_sleep = true;
        }
    }
    if (BT_SW_blink_cnt == 0) {
        if (dev_info.devs != DEVS_USB) {
            led_ble();
        }
    } else {
        if ((BT_SW_blink_time == 0) || (timer_elapsed32(BT_SW_blink_time) >= 200)) {
            BT_SW_blink_time = timer_read32();
            if (BT_SW_blink_cnt > 0) BT_SW_blink_cnt--;
        }
        switch (dev_info.devs) {
            case DEVS_USB:
                // if (BT_SW_blink_cnt & 0x01)
                //     rgb_matrix_set_color(rgb_index_table[DEVS_USB], RGB_RED);
                // else
                //     rgb_matrix_set_color(rgb_index_table[DEVS_USB], 0, 0, 0);
                break;
            case DEVS_HOST1:
            case DEVS_HOST2:
            case DEVS_HOST3:
                if (BT_SW_blink_cnt & 0x01) {
                    rgb_matrix_set_color(rgb_index_table[DEVS_HOST1], RGB_WHITE);
                    rgb_matrix_set_color(rgb_index_table[DEVS_HOST2], RGB_WHITE);
                    rgb_matrix_set_color(rgb_index_table[DEVS_HOST3], RGB_WHITE);
                } else {
                    rgb_matrix_set_color(rgb_index_table[DEVS_HOST1], RGB_OFF);
                    rgb_matrix_set_color(rgb_index_table[DEVS_HOST2], RGB_OFF);
                    rgb_matrix_set_color(rgb_index_table[DEVS_HOST3], RGB_OFF);
                }
                break;
            case DEVS_2_4G:
                if (BT_SW_blink_cnt & 0x01)
                    rgb_matrix_set_color(rgb_index_table[DEVS_2_4G], RGB_WHITE);
                else
                    rgb_matrix_set_color(rgb_index_table[DEVS_2_4G], RGB_OFF);
                break;
            default:
                break;
        }
    }
    return true;
}
