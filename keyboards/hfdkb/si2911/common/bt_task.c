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

#include "bt_task.h"
#include "uart.h"
#include "usb_main.h"
#include "dynamic_keymap.h"
#include "bled/bled.h"

#ifdef BT_DEBUG_MODE
#    define BT_DEBUG_INFO(fmt, ...) dprintf(fmt, ##__VA_ARGS__)
#else
#    define BT_DEBUG_INFO(fmt, ...)
#endif

// ===========================================
// Function declarations
// ===========================================
static void bt_long_pressed_keys_hook(void);
static void bt_long_pressed_keys_cb(uint16_t keycode);
static bool bt_process_record_other(uint16_t keycode, keyrecord_t *record);
static void bt_used_pin_init(void);
static void bt_scan_mode(void);

// static void bt_blink_effects(void);
static void bt_charging_indication(void);

static void bt_bat_low_level_warning(void);
static void bt_bat_low_level_shutdown(void);
static void bt_bat_query_period(void);
static void show_current_keyboard_state(void);
static void bt_indicate(void);
static void factory_reset_indicate(void);
static void usb_indicate(void);

#ifdef RGB_MATRIX_ENABLE
static void led_off_standby(void);
static void open_rgb(void);
static void close_rgb(void);
#endif

// ===========================================
// Constants definitions
// ===========================================
/* Wireless connection timing constants */
#define WL_CONN_TIMEOUT_MS (30 * 1000)   // 30 seconds
#define WL_PAIR_TIMEOUT_MS (60 * 1000)   // 60 seconds
#define WL_PAIR_INTVL_MS (200)           // 5Hz blink for pairing
#define WL_CONN_INTVL_MS (500)           // 2Hz blink for connecting
#define WL_CONNECTED_LAST_MS (2 * 1000)  // Show wl connected status for 2s
#define USB_CONNECTED_LAST_MS (2 * 1000) // Show USB connected status for 2s

/* Sleep and standby timeouts */
#define LED_OFF_STANDBY_TIMEOUT_MS (5 * 60 * 1000) // 5 minutes
#define ENTRY_SLEEP_TIMEOUT_MS (30 * 60 * 1000)    // 30 minutes

/* Array size calculations */
#define NUM_LONG_PRESS_KEYS (sizeof(long_pressed_keys) / sizeof(long_pressed_keys_t))
#define LONG_PRESS_DURATION_MS (3 * 1000)

/* Hardware validation helpers */
#define IS_BT_DEVICE(dev) ((dev) >= DEVS_HOST1 && (dev) <= DEVS_HOST3)
#define IS_VALID_DEVICE(dev) ((dev) >= DEVS_USB && (dev) <= DEVS_2_4G)

#define TASK_UPDATE_INTERVAL_MS 1
#define BT_INIT_WAIT_MS 2000

// ===========================================
// Struct definitions
// ===========================================
typedef struct {
    uint32_t press_time;
    uint16_t keycode;
    void (*event_cb)(uint16_t);
} long_pressed_keys_t;

// Indicator status
typedef enum {
    INDICATOR_OFF        = 0,
    INDICATOR_PAIRING    = 1,
    INDICATOR_CONNECTING = 2,
    INDICATOR_CONNECTED  = 3,
    INDICATOR_DISABLED   = 4,
} indicator_state_t;
static indicator_state_t indicator_status = INDICATOR_CONNECTING;

// ===========================================
// Global variables
// ===========================================

extern keymap_config_t keymap_config;

extern bool low_vol_offed_sleep;

uint32_t   bt_init_time = 0;
dev_info_t dev_info     = {0};
bts_info_t bts_info     = {
        .bt_name        = {MM_BT_HOST1_NAME, MM_BT_HOST2_NAME, MM_BT_HOST3_NAME},
        .uart_init      = uart_init,
        .uart_read      = uart_read,
        .uart_transmit  = uart_transmit,
        .uart_receive   = uart_receive,
        .uart_available = uart_available,
        .timer_read32   = timer_read32,
};

// Long press config
// clang-format off
static long_pressed_keys_t long_pressed_keys[] = {
    {.keycode = BT_HOST1, .press_time = 0, .event_cb = bt_long_pressed_keys_cb},
    {.keycode = BT_HOST2, .press_time = 0, .event_cb = bt_long_pressed_keys_cb},
    {.keycode = BT_HOST3, .press_time = 0, .event_cb = bt_long_pressed_keys_cb},
    {.keycode = BT_2_4G, .press_time = 0, .event_cb = bt_long_pressed_keys_cb},
    {.keycode = EE_CLR, .press_time = 0, .event_cb = bt_long_pressed_keys_cb},
    {.keycode = RGB_TEST, .press_time = 0, .event_cb = bt_long_pressed_keys_cb},
};
// clang-format on
static bool indicator_reset_last_time = false;

// RGB control
static uint32_t key_press_time = 0;
static uint32_t close_rgb_time = 0;

static bool led_inited = false;
// static bool rgb_status_save = false;
static bool bak_rgb_toggle = false;
static bool sober          = true;
static bool kb_sleep_flag  = false;

bool backlight_sleep_flag = false;

// static bool long_pressed_flag = false;

// Device indicator config
static const uint8_t rgb_index_table[] = {MM_BLINK_USB_INDEX, MM_BLINK_HOST1_INDEX, MM_BLINK_HOST2_INDEX, MM_BLINK_HOST3_INDEX, MM_BLINK_2G4_INDEX};
// clang-format off
static const uint8_t rgb_index_color_table[][3] = {
    {MM_BLINK_USB_COLOR},
    {MM_BLINK_HOST1_COLOR},
    {MM_BLINK_HOST2_COLOR},
    {MM_BLINK_HOST3_COLOR},
    {MM_BLINK_2G4_COLOR},
};
// clang-format on

// RGB test related
static const uint8_t rgb_test_color_table[][3] = {
    {100, 0, 0},
    {0, 100, 0},
    {0, 0, 100},
    {90, 100, 100},
};
// static uint8_t  rgb_test_index  = 0;
static bool rgb_test_en = false;
// static uint32_t rgb_test_time   = 0;
bool query_vol_flag = false;

static uint32_t last_total_time = 0;

// static uint32_t caps_blink_time = 0;
// static uint8_t  caps_blink_cnt  = 0;
// static bool     caps_blink_flag = false;

static uint32_t EE_CLR_press_cnt  = 0;
static uint32_t EE_CLR_press_time = 0;
static bool     EE_CLR_flag       = false;

// Battery query
/* Battery level indicator */
uint8_t query_index[10] = BAT_LEVEL_DISPLAY_INDEX;

mode_t mode = MODE_WORKING;

// USB related
static uint32_t USB_switch_time = 0;

typedef struct PACKED {
    uint32_t blink_time;
    uint32_t full_time;
    uint32_t entry_chrg_time;
    uint32_t entry_full_time;
    uint8_t  blink_count;
    bool     triggered;
    bool     full_triggered;
    bool     blink_state;
    bool     completed;
    bool     full_completed;
} charge_complete_warning_t;
static charge_complete_warning_t charge_complete_warning = {0};

// bool show_charging = false;
// bool show_charged  = false;
// bool show_low      = false;

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

void do_code16(uint16_t code, void (*f)(uint8_t));

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

// ===========================================
// Thread definitions
// ===========================================
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
    (void)arg;
    chRegSetThreadName("blinker");
    while (true) {
        bts_task(dev_info.devs);
        chThdSleepMilliseconds(1);
    }
}

// ===========================================
// Initialization functions
// ===========================================
void bt_init(void) {
    bts_init(&bts_info);
    bt_used_pin_init();

    dev_info.raw = eeconfig_read_user();
    if (!dev_info.raw) {
        dev_info.devs      = DEVS_USB;
        dev_info.last_devs = DEVS_HOST1;
        eeconfig_update_user(dev_info.raw);
    }

    bt_init_time = timer_read32();
    chThdCreateStatic(waThread1, sizeof(waThread1), HIGHPRIO, Thread1, NULL);
    // If the device is not USB, we need to set the scan mode
    bt_scan_mode();

    if (dev_info.devs != DEVS_USB) {
        usbDisconnectBus(&USB_DRIVER);
        usbStop(&USB_DRIVER);
    }

    setPinOutput(A14);
    if (dev_info.devs == DEVS_USB) {
        writePinLow(A14);
    } else {
        writePinHigh(A14);
    }

    // rgb_status_save = rgb_matrix_config.enable;
}

// ===========================================
// Bluetooth task functions
// ===========================================
void bt_task(void) {
    static uint32_t last_time = 0;

    // Handle initialization sequence after delay
    if ((bt_init_time != 0) && (timer_elapsed32(bt_init_time) >= BT_INIT_WAIT_MS)) {
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
        // bts_send_vendor(v_en_sleep_wl);
    }

    // Update task at regular intervals
    if (timer_elapsed32(last_time) >= TASK_UPDATE_INTERVAL_MS) {
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

    bt_long_pressed_keys_hook();
    // if (!bt_init_time) bt_scan_mode();
    bt_scan_mode();
}

// ===========================================
// Key processing function
// ===========================================
bool bt_process_record(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;

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
                      dev_info.devs, bts_info.bt_info.sleeped, bts_info.bt_info.low_vol, bts_info.bt_info.low_vol_offed, bts_info.bt_info.normal_vol, bts_info.bt_info.pairing, bts_info.bt_info.paired, bts_info.bt_info.come_back, bts_info.bt_info.come_back_err, bts_info.bt_info.mode_switched, bts_info.bt_info.pvol);

        // if (!rgb_matrix_config.enable) {
        //     if (rgb_status_save) {
        //         rgb_matrix_enable_noeeprom();
        //     }
        // }
    }

    retval = bt_process_record_other(keycode, record);

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

// ===========================================
// Device switch function
// ===========================================
void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset) {
    // Enable RGB if it was previously enabled
    // if (!rgb_matrix_config.enable && rgb_status_save) {
    //     rgb_matrix_enable_noeeprom();
    // }

    // Handle USB driver state changes
    bool usb_sws = !!last_mode ? !now_mode : !!now_mode;
    if (usb_sws) {
        if (!!now_mode) {
            usbDisconnectBus(&USB_DRIVER);
            usbStop(&USB_DRIVER);
        } else {
            init_usb_driver(&USB_DRIVER);
        }
    }

    // Reset wireless indicator timer if switching between different devices
    if (last_mode != now_mode) {
        last_total_time = timer_read32();
    }

    // Update device state
    dev_info.devs = now_mode;
    if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
        dev_info.last_devs = dev_info.devs;
    } else if (dev_info.devs == DEVS_USB) {
        USB_switch_time = timer_read32();
    }
    // Set hardware control pin
    if (dev_info.devs == DEVS_USB) {
        writePinLow(A14);
    } else {
        writePinHigh(A14);
    }

    // Reset BT connection state
    bts_info.bt_info.pairing        = false;
    bts_info.bt_info.paired         = false;
    bts_info.bt_info.come_back      = false;
    bts_info.bt_info.come_back_err  = false;
    bts_info.bt_info.mode_switched  = false;
    bts_info.bt_info.indictor_rgb_s = 0;
    eeconfig_update_user(dev_info.raw);

    // Handle indicator status and send vendor commands
    switch (dev_info.devs) {
        case DEVS_HOST1: {
            if (reset != false) {
                // Reset - enter pairing mode
                indicator_status          = 1;
                indicator_reset_last_time = true;
                // bts_send_name(DEVS_HOST1);
                // bts_send_vendor(v_host1);
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_host1);
            }
        } break;
        case DEVS_HOST2: {
            if (reset != false) {
                // Reset - enter pairing mode
                indicator_status          = 1;
                indicator_reset_last_time = true;
                // bts_send_name(DEVS_HOST2);
                // bts_send_vendor(v_host2);
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_host2);
            }
        } break;
        case DEVS_HOST3: {
            if (reset != false) {
                // Reset - enter pairing mode
                indicator_status          = 1;
                indicator_reset_last_time = true;
                // bts_send_name(DEVS_HOST3);
                // bts_send_vendor(v_host3);
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                bts_send_vendor(v_host3);
            }
        } break;
        case DEVS_2_4G: {
            if (reset != false) {
                // Reset - enter pairing mode
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

// ===========================================
// Other key processing functions
// ===========================================
static bool bt_process_record_other(uint16_t keycode, keyrecord_t *record) {
    if (dev_info.devs == DEVS_2_4G) {
        if ((keycode == BT_USB) || (keycode >= BT_HOST1 && keycode <= BT_HOST3)) return false;
    }
    if (dev_info.devs >= DEVS_HOST1 && dev_info.devs <= DEVS_HOST3) {
        if (keycode == BT_2_4G || keycode == BT_USB) return false;
    }
    if (dev_info.devs == DEVS_USB) {
        if (keycode >= BT_HOST1 && keycode <= BT_2_4G) return false;
    }

    // Update long press time
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
                if (dev_info.devs != DEVS_HOST1) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                }
            }
        } break;
        case BT_HOST2: {
            if (record->event.pressed) {
                if (dev_info.devs != DEVS_HOST2) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                }
            }
        } break;
        case BT_HOST3: {
            if (record->event.pressed) {
                if (dev_info.devs != DEVS_HOST3) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                }
            }
        } break;
        case BT_2_4G: {
            if (record->event.pressed) {
                if (dev_info.devs != DEVS_2_4G) {
                    bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                }
            }
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

        case EE_CLR: {
        } break;

        case RGB_TEST: {
            if (record->event.pressed) {
                if (!rgb_test_en) {
                    rgb_test_en = true;
                } else {
                    rgb_test_en = false;
                }
            }
        } break;

        case RM_NEXT: {
            if (!record->event.pressed) {
                if (rgb_test_en) {
                    rgb_test_en = false;
                    return false;
                }
            }
        }

        default:
            return true;
    }

    return false;
}

// ===========================================
// 长按键处理
// ===========================================
static void bt_long_pressed_keys_cb(uint16_t keycode) {
    switch (keycode) {
        case BT_HOST1:
        case BT_HOST2:
        case BT_HOST3:
        case BT_2_4G: {
            uint8_t target_dev = keycode - BT_HOST1 + DEVS_HOST1;
            if (dev_info.devs == target_dev) {
                bt_switch_mode(dev_info.devs, target_dev, true);
            }
        } break;

        case EE_CLR: {
            if (!EE_CLR_flag) {
                rgb_test_en       = false;
                EE_CLR_flag       = true;
                EE_CLR_press_time = timer_read32();
                EE_CLR_press_cnt  = 1;
                rgb_matrix_enable_noeeprom();
            }
        } break;

            // case RGB_TEST: {
            //     if (rgb_test_en != true) {
            //         rgb_test_en    = true;
            //         rgb_test_index = 1;
            //         rgb_test_time  = timer_read32();
            //     }
            // } break;

        default:
            break;
    }
}

static void bt_long_pressed_keys_hook(void) {
    for (uint8_t i = 0; i < NUM_LONG_PRESS_KEYS; i++) {
        if ((long_pressed_keys[i].press_time != 0) && (timer_elapsed32(long_pressed_keys[i].press_time) >= LONG_PRESS_DURATION_MS)) {
            long_pressed_keys[i].event_cb(long_pressed_keys[i].keycode);
            long_pressed_keys[i].press_time = 0;
        }
    }
}

// ===========================================
// Pin init functions
// ===========================================
static void bt_used_pin_init(void) {
#if defined(MM_BT_MODE_PIN) && defined(MM_2G4_MODE_PIN)
    setPinInputHigh(MM_BT_MODE_PIN);
    setPinInputHigh(MM_2G4_MODE_PIN);
#endif
#if defined(MM_CABLE_PIN) && defined(MM_CHARGE_PIN)
    setPinInputHigh(MM_CABLE_PIN);
    setPinInput(MM_CHARGE_PIN);
#endif
}

static void bt_scan_mode(void) {
#if defined(MM_BT_MODE_PIN) && defined(MM_2G4_MODE_PIN)
    uint8_t        now_mode;
    static uint8_t old_mode;

    if (readPin(MM_BT_MODE_PIN) && !readPin(MM_2G4_MODE_PIN)) {
        now_mode = 0;
        if (dev_info.devs != DEVS_2_4G) bt_switch_mode(dev_info.devs, DEVS_2_4G, false); // 2_4G mode
    }
    if (readPin(MM_2G4_MODE_PIN) && !readPin(MM_BT_MODE_PIN)) {
        now_mode = 1;
        if ((dev_info.devs == DEVS_USB) || (dev_info.devs == DEVS_2_4G)) bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // BT mode
    }
    if (readPin(MM_BT_MODE_PIN) && readPin(MM_2G4_MODE_PIN)) {
        now_mode = 2;
        if (dev_info.devs != DEVS_USB) bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
    }
    if (old_mode != now_mode) {
        old_mode = now_mode;
        writePinLow(RGB_MATRIX_SHUTDOWN_PIN);
        wait_ms(1);
        writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
        rgb_matrix_init();
    }
#endif
}

// ===========================================
// No operation standby functions
// ===========================================
void led_config_all(void) {
    if (!led_inited) {
#ifdef RGB_MATRIX_SHUTDOWN_PIN
        // setPinOutputPushPull(RGB_MATRIX_SHUTDOWN_PIN);
        writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
#endif
        setPinOutputPushPull(LED_CAPS_LOCK_PIN);
        if (host_keyboard_led_state().caps_lock) {
            writePinHigh(LED_CAPS_LOCK_PIN);
        }

        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
#ifdef RGB_MATRIX_SHUTDOWN_PIN
        // setPinOutputOpenDrain(RGB_MATRIX_SHUTDOWN_PIN);
        writePinLow(RGB_MATRIX_SHUTDOWN_PIN);
#endif
        // gpio_write_pin(LED_CAPS_LOCK_PIN, 0);
        setPinOutputOpenDrain(LED_CAPS_LOCK_PIN);

        led_inited = false;
    }
}

static void led_off_standby(void) {
    if (timer_elapsed32(key_press_time) >= LED_OFF_STANDBY_TIMEOUT_MS) {
        if (!backlight_sleep_flag) {
            backlight_sleep_flag = true;
            led_deconfig_all();
        }
    }
}

// ===========================================
// RGB close functions
// ===========================================
static void close_rgb(void) {
    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }

    led_off_standby();

    if (sober) {
        if (kb_sleep_flag || (timer_elapsed32(key_press_time) >= ENTRY_SLEEP_TIMEOUT_MS)) {
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();

#ifdef RGB_MATRIX_SHUTDOWN_PIN
            // setPinOutputOpenDrain(RGB_MATRIX_SHUTDOWN_PIN);
            writePinLow(RGB_MATRIX_SHUTDOWN_PIN);
#endif
        }
    } else {
        if (!rgb_matrix_config.enable) {
            if (timer_elapsed32(close_rgb_time) >= ENTRY_STOP_TIMEOUT) {
                if (led_inited) {
                    led_deconfig_all();
                }
#ifdef ENTRY_STOP_MODE
                lp_system_sleep();
#endif
                open_rgb();
            }
        }
    }
}

// ===========================================
// RGB Open functions
// ===========================================
static void open_rgb(void) {
    key_press_time = timer_read32();

    if (backlight_sleep_flag) {
        backlight_sleep_flag = false;
        led_config_all();
    }

    if (!sober) {
        if (bak_rgb_toggle) {
            kb_sleep_flag       = false;
            low_vol_offed_sleep = false;
            rgb_matrix_enable_noeeprom();

#ifdef RGB_MATRIX_SHUTDOWN_PIN
            // setPinOutputPushPull(RGB_MATRIX_SHUTDOWN_PIN);
            writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
#endif
        }
        if (!led_inited) {
            led_config_all();
        }
        sober = true;
    }
}

// ===========================================
// Bt indicator functions
// ===========================================
static void bt_indicate(void) {
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
        case INDICATOR_PAIRING: {
            if ((last_time == 0) || (timer_elapsed32(last_time) >= WL_PAIR_INTVL_MS)) {
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
                indicator_status = INDICATOR_CONNECTED;
                break;
            }

            if (timer_elapsed32(last_total_time) >= WL_PAIR_TIMEOUT_MS) {
                indicator_status = INDICATOR_OFF;
                kb_sleep_flag    = true;
            }
        } break;

        case INDICATOR_CONNECTING: {
            if ((last_time == 0) || (timer_elapsed32(last_time) >= WL_CONN_INTVL_MS)) {
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
                if (host_keyboard_led_state().caps_lock) {
                    gpio_write_pin(LED_CAPS_LOCK_PIN, 1);
                }
                last_long_time   = timer_read32();
                indicator_status = INDICATOR_CONNECTED;
                break;
            }

            if (host_keyboard_led_state().caps_lock) {
                gpio_write_pin(LED_CAPS_LOCK_PIN, 0);
            }

            if (timer_elapsed32(last_total_time) >= WL_CONN_TIMEOUT_MS) {
                indicator_status = INDICATOR_OFF;
                kb_sleep_flag    = true;
            }
        } break;

        case INDICATOR_CONNECTED: {
            if ((timer_elapsed32(last_long_time) < WL_CONNECTED_LAST_MS)) {
                rgb.r = rgb_index_color_table[dev_info.devs][0];
                rgb.g = rgb_index_color_table[dev_info.devs][1];
                rgb.b = rgb_index_color_table[dev_info.devs][2];
            } else {
                indicator_status = INDICATOR_OFF;
            }
        } break;

        case INDICATOR_DISABLED: {
            rgb = (RGB){.r = 0, .g = 0, .b = 0};
        } break;

        default: {
            rgb_flip = false;
            if (!kb_sleep_flag) {
                if (!bts_info.bt_info.paired) {
                    if (!bts_info.bt_info.pairing) {
                        indicator_status = INDICATOR_CONNECTING;
                        break;
                    }
                    indicator_status = INDICATOR_CONNECTING;
                    if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                        bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
                    }
                    if (dev_info.devs == DEVS_2_4G) {
                        bt_switch_mode(DEVS_USB, DEVS_2_4G, false);
                    }
                }
            }
        } break;
    }

    if (indicator_status) {
        rgb_matrix_set_color(rgb_index, rgb.r, rgb.g, rgb.b);
    }
}

// ===========================================
// USB indicator functions
// ===========================================
static void usb_indicate(void) {
    if (USB_DRIVER.state == USB_ACTIVE) {
        if (timer_elapsed32(USB_switch_time) < USB_CONNECTED_LAST_MS) {
            rgb_matrix_set_color(rgb_index_table[DEVS_USB], rgb_index_color_table[DEVS_USB][0], rgb_index_color_table[DEVS_USB][1], rgb_index_color_table[DEVS_USB][2]);
        }
    } else {
        USB_switch_time = timer_read32();
    }
}

// ===========================================
// Battery low level warning functions
// ===========================================
static void bt_bat_low_level_warning(void) {
    // static bool Low_power_bink = false;
    static bool Low_power = false;
    // static uint32_t Low_power_time = 0;

    if ((bts_info.bt_info.pvol <= 20) && !Low_power) {
        Low_power = true;
    } else if ((bts_info.bt_info.pvol >= 50) && Low_power) {
        Low_power = false;
    }
    // extern uint8_t sled_mode_before_charge;
    // if (bts_info.bt_info.low_vol) {
    if (Low_power) {
        for (uint8_t i = 0; i < 102; i++) {
            rgb_matrix_set_color(i, RGB_OFF);
        }
        bled_low_indicate();
        if (query_vol_flag) {
            for (uint8_t i = 18; i <= 19; i++) {
                rgb_matrix_set_color(i, RGB_RED);
            }
        }
    }
}

static void bt_charging_indication(void) {
    static bool show_chrg_full = false;

    // This flag persists until system restart - once charge full is displayed, it won't trigger again
    static bool charge_full_displayed = false;

    static bool is_in_charging_state   = false;
    static bool is_in_full_power_state = false;

    if (!readPin(MM_CABLE_PIN)) {
        if (!readPin(MM_CHARGE_PIN)) {
            if (!charge_full_displayed) {
                charge_complete_warning.entry_full_time = timer_read32();
                if (timer_elapsed32(charge_complete_warning.entry_chrg_time) >= 1200) {
                    // show_chrging = true;
                    if (!is_in_charging_state) {
                        is_in_charging_state = true;
                        if (!charge_complete_warning.triggered) {
                            charge_complete_warning.triggered   = true;
                            charge_complete_warning.blink_count = 0;
                            charge_complete_warning.blink_time  = timer_read32();
                            charge_complete_warning.blink_state = false;
                            charge_complete_warning.completed   = false;
                        }
                    }
                }

                if (!charge_complete_warning.completed && charge_complete_warning.blink_count < 10) {
                    if (timer_elapsed32(charge_complete_warning.blink_time) >= 1000) {
                        charge_complete_warning.blink_time  = timer_read32();
                        charge_complete_warning.blink_state = !charge_complete_warning.blink_state;

                        if (charge_complete_warning.blink_state) {
                            charge_complete_warning.blink_count++;
                            if (charge_complete_warning.blink_count >= 10) {
                                charge_complete_warning.completed   = true;
                                charge_complete_warning.blink_state = false;
                            }
                        }
                    }

                    if (charge_complete_warning.blink_state) {
                        for (uint8_t i = 102; i <= 106; i++) {
                            rgb_matrix_set_color(i, RGB_GREEN);
                        }
                    } else {
                        for (uint8_t i = 102; i <= 106; i++) {
                            rgb_matrix_set_color(i, RGB_OFF);
                        }
                    }
                }
            }
        } else {
            // Charge pin indicates full - trigger only if not already displayed
            // charge_complete_warning.entry_chrg_time = timer_read32();
            // if (timer_elapsed32(charge_complete_warning.entry_full_time) >= 1200) {
            //     if (!charge_full_displayed) {
            //         show_chrg_full        = true;
            //         charge_full_displayed = true;
            //     }
            // }
        }

        // Check voltage >= 100% while cable connected - trigger only if not already displayed
        if ((bts_info.bt_info.pvol >= 100) && !charge_full_displayed) {
            show_chrg_full        = true;
            charge_full_displayed = true;
        }

        if (show_chrg_full) {
            if (!is_in_full_power_state) {
                is_in_full_power_state = true;
                if (!charge_complete_warning.full_triggered) {
                    charge_complete_warning.full_triggered = true;
                    charge_complete_warning.full_time      = timer_read32();
                }
            }

            if (!charge_complete_warning.full_completed) {
                if (timer_elapsed32(charge_complete_warning.full_time) <= 10000) {
                    for (uint8_t i = 102; i <= 106; i++) {
                        rgb_matrix_set_color(i, RGB_GREEN);
                    }
                } else {
                    charge_complete_warning.full_completed = true;
                    show_chrg_full                         = false;
                }
            }
        }
    } else {
        // Cable disconnected - reset charging state but NOT charge_full_displayed
        charge_full_displayed = false;

        is_in_charging_state   = false;
        is_in_full_power_state = false;
        // memset(&charge_complete_warning, 0, sizeof(charge_complete_warning_t));
        charge_complete_warning.full_completed = false;
        charge_complete_warning.completed      = false;
        charge_complete_warning.full_triggered = false;
        charge_complete_warning.triggered      = false;
        charge_complete_warning.blink_count    = 0;
        charge_complete_warning.blink_state    = false;
        charge_complete_warning.blink_time     = 0;
        // charge_complete_warning.full_time      = 0;
    }
}

static void bt_bat_low_level_shutdown(void) {
    if (bts_info.bt_info.low_vol_offed) {
        kb_sleep_flag       = true;
        low_vol_offed_sleep = true;
    }
}

// ===========================================
// Battery query display functions
// ===========================================
static void bt_bat_query_period(void) {
    static uint32_t query_vol_time = 0;
    if (!backlight_sleep_flag && !bt_init_time && !kb_sleep_flag && (bts_info.bt_info.paired) && (timer_elapsed32(query_vol_time) >= 4000)) {
        query_vol_time = timer_read32();
        bts_send_vendor(v_query_vol);
    }

    if (query_vol_flag) {
        uint8_t query_index[] = {18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
        uint8_t pvol          = bts_info.bt_info.pvol;
        uint8_t led_count     = 0;
        RGB     color;

        // if (pvol <= 20) {
        //     led_count = 1;
        // } else if (pvol <= 40) {
        //     led_count = 2;
        // } else if (pvol <= 60) {
        //     led_count = 3;
        // } else if (pvol <= 80) {
        //     led_count = 4;
        // } else {
        //     led_count = 5;
        // }

        led_count = (pvol < 10) ? 1 : ((pvol / 10) > 10 ? 10 : (pvol / 10));

        // for (uint8_t i = 0; i < (sizeof(query_index) / sizeof(query_index[0])); i++) {
        //     rgb_matrix_set_color(query_index[i], RGB_OFF);
        // }
        for (uint8_t i = 0; i < 102; i++) {
            rgb_matrix_set_color(i, RGB_OFF);
        }

        if (led_count <= 2) {
            // for (uint8_t i = 102; i <= 106; i++) {
            //     rgb_matrix_set_color(i, RGB_OFF);
            // }
            // for (uint8_t i = 102; i <= 103; i++) {
            //     rgb_matrix_set_color(i, RGB_RED);
            // }
            color = (RGB){100, 0, 0}; // 红色
        } else {
            color = (RGB){0, 100, 0}; // 绿色
        }

        for (uint8_t i = 0; i < led_count; i++) {
            rgb_matrix_set_color(query_index[i], color.r, color.g, color.b);
        }
    }
}

static void show_current_keyboard_state(void) {
    // Show current wireless device state when FN is pressed
    if ((get_highest_layer(default_layer_state | layer_state) == 1) || (get_highest_layer(default_layer_state | layer_state) == 3)) {
        switch (dev_info.devs) {
            case DEVS_HOST1: {
                rgb_matrix_set_color(MM_BLINK_HOST1_INDEX, RGB_WHITE);
            } break;
            case DEVS_HOST2: {
                rgb_matrix_set_color(MM_BLINK_HOST2_INDEX, RGB_WHITE);
            } break;
            case DEVS_HOST3: {
                rgb_matrix_set_color(MM_BLINK_HOST3_INDEX, RGB_WHITE);
            } break;
            case DEVS_2_4G: {
                rgb_matrix_set_color(MM_BLINK_2G4_INDEX, RGB_WHITE);
            } break;
            case DEVS_USB: {
                rgb_matrix_set_color(MM_BLINK_USB_INDEX, RGB_WHITE);
            } break;
            default:
                break;
        }
    }
}

static void factory_reset_indicate(void) {
    if (EE_CLR_flag) {
        for (uint8_t i = 0; i < 100; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
        if (timer_elapsed32(EE_CLR_press_time) > 300) {
            EE_CLR_press_time = timer_read32();
            EE_CLR_press_cnt++;
        }
        if (EE_CLR_press_cnt >= 7) {
            EE_CLR_press_cnt  = 0;
            EE_CLR_press_time = 0;
            EE_CLR_flag       = false;

            eeconfig_init();
            keymap_config.no_gui = false;

            mode = MODE_WORKING;
            dip_switch_read(true);

            if (dev_info.devs != DEVS_USB) {
                last_total_time  = 0;
                indicator_status = INDICATOR_CONNECTING;
            }
        }
        if (EE_CLR_press_cnt & 0x1) {
            for (uint8_t i = 0; i < 100; i++) {
                rgb_matrix_set_color(i, 0x64, 0x64, 0x64);
            }
        }
    }
}

// ===========================================
// Main RGB indicator functions
// ===========================================
bool bt_indicators_advanced(uint8_t led_min, uint8_t led_max) {
    if (rgb_test_en && rgb_matrix_get_flags()) {
        for (uint8_t i = 0; i < 100; i++) {
            rgb_matrix_set_color(i, rgb_test_color_table[3][0], rgb_test_color_table[3][1], rgb_test_color_table[3][2]);
        }
    }

    if ((dev_info.devs != DEVS_USB) && (readPin(MM_CABLE_PIN))) {
        bt_bat_low_level_warning();
    }

    // Show the current keyboard state
    show_current_keyboard_state();

    if (dev_info.devs == DEVS_USB) {
        usb_indicate();
    } else {
        bt_indicate();
        if (readPin(MM_CABLE_PIN)) {
            bt_bat_low_level_shutdown();
        }
    }

    bt_bat_query_period();

    // Charging status indicator
    bt_charging_indication();

    // Factory reset
    factory_reset_indicate();

    // rgb test
    // if (rgb_test_en && rgb_matrix_get_flags()) {
    // if (timer_elapsed32(rgb_test_time) >= 1000) {
    //     rgb_test_time = timer_read32();
    //     rgb_test_index++;
    //     if (rgb_test_index > 4) {
    //         rgb_test_index = 1;
    //     }
    // }

    // for (uint8_t i = led_min; i < led_max; i++) {
    //     rgb_matrix_set_color(i, rgb_test_color_table[rgb_test_index - 1][0], rgb_test_color_table[rgb_test_index - 1][1], rgb_test_color_table[rgb_test_index - 1][2]);
    // }
    //     for (uint8_t i = 0; i < 100; i++) {
    //         rgb_matrix_set_color(i, rgb_test_color_table[3][0], rgb_test_color_table[3][1], rgb_test_color_table[3][2]);
    //     }
    // }

    return true;
}

void bt_housekeeping_task(void) {
#ifdef MULTIMODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE) {
            // USB suspend state
            if (!usb_suspend_timer) {
                // Start timer
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                // Suspend for more than 10 seconds, turn off backlight
                if (!usb_suspend) {
                    // If not in suspend state before, execute suspend operation
                    usb_suspend = true;
                    led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            // USB active state, reset timer
            if (usb_suspend_timer) {
                usb_suspend_timer = 0;
                if (usb_suspend) {
                    // If previously in suspend state, restore backlight
                    usb_suspend = false;
                    led_config_all();
                }
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
            led_config_all();
        }
    }
#endif
}
