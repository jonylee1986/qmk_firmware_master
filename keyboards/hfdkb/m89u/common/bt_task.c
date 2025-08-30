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

#include "bts_lib.h"
#include "bt_task.h"
#include "quantum.h"
#include "uart.h"
#include "usb_main.h"

#define NUM_LONG_PRESS_KEYS (sizeof(long_pressed_keys) / sizeof(long_pressed_keys_t))
#define LED_OFF_STANDBY_MS (10 * 60 * 1000)
#define KEY_NUM 8
// Wireless-specific settle time after toggling NumLock
#define BT_NUMLOCK_SETTLE_MS 40
// Battery full detection tuning
#define FULL_PVOL_THRESHOLD 100 // consider full at >= this value
#define FULL_HYSTERESIS_PVOL 96 // must drop to <= this to re-arm next cycle
#define FULL_DEBOUNCE_MS 1200   // must stay at full for this long to count

#ifdef BT_DEBUG_MODE
#    define BT_DEBUG_INFO(fmt, ...) dprintf(fmt, ##__VA_ARGS__)
#else
#    define BT_DEBUG_INFO(fmt, ...)
#endif

// Macro to check if a keycode is a numpad key
#define IS_NUMPAD_KEYCODE(code) ((code) >= KC_KP_1 && (code) <= KC_KP_DOT)

// ===========================================
// 函数声明
// ===========================================
static void long_pressed_keys_hook(void);
static void long_pressed_keys_cb(uint16_t keycode);
static bool bt_process_record_other(uint16_t keycode, keyrecord_t *record);
static void bt_scan_mode(void);
static void bt_used_pin_init(void);
static void bt_bat_low_level_state(void);
static void execute_factory_reset(void);
static void factory_reset_indeicate(void);
static void blink_effects(void);
static void show_device_state(void);
static void charging_indicate(void);
static void bt_bat_low_level_warning(void);
static void bt_bat_low_level_shutdown(void);
static void bt_bat_query_period(void);
static void bt_bat_level_display(void);
static void bt_indicate_led(void);
static void usb_indicate_led(void);
static bool is_switch_forcing_wired_mode(void);
static bool is_wireless_mode_keycode(uint16_t keycode);
static bool is_current_mode_wireless(void);
static bool is_current_mode_wireless_by_devs(uint8_t devs);
#ifdef RGB_MATRIX_ENABLE
static void led_config_all(void);
static void led_deconfig_all(void);
static void open_rgb(void);
static void close_rgb(void);
#endif

extern keymap_config_t keymap_config;

// ===========================================
// 结构体定义
// ===========================================
enum reset_type {
    _NONE = 0,
    _FACTORY,
    _KEYBOARD,
    _BLE,
};

// Simple factory reset state
typedef struct {
    uint8_t  type;     // 1=factory, 2=keyboard, 3=ble, 0=none
    uint8_t  progress; // 0-7 steps
    uint32_t start_time;
} factory_reset_t;
static factory_reset_t factory_reset = {0};

// 低电量警告状态结构体 (20%以下红色闪烁6次)
typedef struct {
    bool     triggered;
    uint8_t  blink_count;
    uint32_t blink_time;
    bool     blink_state;
    bool     completed;
} low_battery_warning_t;

// 充电完成指示状态结构体 (绿色闪烁5次)
typedef struct {
    bool     triggered;
    uint8_t  blink_count;
    uint32_t blink_time;
    uint32_t entry_full_time;
    bool     blink_state;
    bool     completed;
} charge_complete_warning_t;

// 低电量背光控制状态结构体 (5%以下关闭背光)

typedef struct {
    uint32_t press_time;
    uint16_t keycode;
    void (*event_cb)(uint16_t);
} long_pressed_keys_t;

// per_info_t per_info;

typedef enum {
    BATTERY_STATE_UNPLUGGED = 0, // No cable connected
    BATTERY_STATE_CHARGING,      // Cable connected, charging
    BATTERY_STATE_CHARGED_FULL   // Cable connected, fully charged
} battery_charge_state_t;
static battery_charge_state_t get_battery_charge_state(void);

// ===========================================
// 全局变量
// ===========================================
static bool led_inited = false;

static low_battery_warning_t     low_battery_warning     = {0};
static charge_complete_warning_t charge_complete_warning = {0};

static bool is_in_low_power_state  = false;
static bool is_in_full_power_state = false;
bool        low_vol_offed_sleep    = false;

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

// clang-format off
 // 长按键配置
 long_pressed_keys_t long_pressed_keys[] = {
     {.keycode = BT_HOST1, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = BT_HOST2, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = BT_HOST3, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = BT_2_4G, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = SW_OS, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = FACTORY_RESET, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = KEYBOARD_RESET, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = BLE_RESET, .press_time = 0, .event_cb = long_pressed_keys_cb},
     {.keycode = RGB_TEST, .press_time = 0, .event_cb = long_pressed_keys_cb},
 };
// clang-format on
// 指示器状态
uint8_t indicator_status          = 2;
uint8_t indicator_reset_last_time = false;
uint8_t layer_save;

// RGB控制
static uint32_t key_press_time;
static uint32_t close_rgb_time;
static bool     bak_rgb_toggle;
static bool     sober         = true;
bool            kb_sleep_flag = false;
extern bool     led_inited;
extern void     led_config_all(void);
extern void     led_deconfig_all(void);

// 设备指示配置
static const uint8_t rgb_index_table[]          = {MM_BLINK_USB_INDEX, MM_BLINK_HOST1_INDEX, MM_BLINK_HOST2_INDEX, MM_BLINK_HOST3_INDEX, MM_BLINK_2G4_INDEX};
static const uint8_t rgb_index_color_table[][3] = {
    {MM_BLINK_USB_COLOR}, {MM_BLINK_HOST1_COLOR}, {MM_BLINK_HOST2_COLOR}, {MM_BLINK_HOST3_COLOR}, {MM_BLINK_2G4_COLOR},
};

static const uint8_t rgb_test_color_table[][3] = {
    {200, 0, 0},
    {0, 200, 0},
    {0, 0, 200},
    {100, 100, 100},
};
static uint8_t  rgb_test_index = 0;
static bool     rgb_test_en    = false;
static uint32_t rgb_test_time  = 0;

static bool rgb_status_save = 1;

const uint32_t sleep_time_table[4] = {0, 10 * 60 * 1000, 30 * 60 * 1000, 60 * 60 * 1000};

// 闪烁效果相关
static uint8_t  all_blink_cnt;
static uint32_t all_blink_time;
static RGB      all_blink_color;
static uint8_t  single_blink_cnt;
static uint8_t  single_blink_index;
static RGB      single_blink_color;
static uint32_t single_blink_time;

// 电量查询
bool          query_vol_flag  = false;
const uint8_t query_index[10] = MM_BAT_LEVEL_INDICATE_INDEX;

// USB相关
uint32_t USB_switch_time;
uint8_t  USB_blink_cnt;

uint32_t last_total_time = 0;

#include "command.h"
#include "action.h"

static bool    original_num_lock_state   = false; // Track original NumLock state
static uint8_t numpad_keys_pressed_count = 0;     // Count of currently pressed numpad keys

void register_mouse(uint8_t mouse_keycode, bool pressed);
/** \brief Utilities for actions. (FIXME: Needs better description)
 *
 * FIXME: Needs documentation.
 */
extern bool key_eql_pressed;

__attribute__((weak)) void register_code(uint8_t code) {
    if (dev_info.devs) {
        // Handle numpad keys with custom behavior when unsync is enabled (BT mode)
        if (!key_eql_pressed && dev_info.unsync && IS_NUMPAD_KEYCODE(code)) {
            if (numpad_keys_pressed_count == 0) {
                {
                    uint8_t keyboard_led_state = 0;
                    led_t  *kb_leds            = (led_t *)&keyboard_led_state;
                    kb_leds->raw               = bts_info.bt_info.indictor_rgb_s;
                    usb_device_state_set_leds(keyboard_led_state);
                    original_num_lock_state = host_keyboard_led_state().num_lock;
                }
                if (dev_info.num_unsync) {
                    if (original_num_lock_state != dev_info.num_unsync) {
                        // Host NumLock is OFF - turn it ON temporarily
                        bts_process_keys(KC_NUM_LOCK, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                        bts_task(dev_info.devs);
                        while (bts_is_busy())
                            wait_ms(1);
                        bts_process_keys(KC_NUM_LOCK, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                        bts_task(dev_info.devs);
                        while (bts_is_busy())
                            wait_ms(1);
                    }
                }
            }
            numpad_keys_pressed_count++;
            // Always just send the key (do not re-run NumLock logic for >1 key held)
            bts_process_keys(code, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        } else {
            // Normal BT behavior for non-numpad keys
            bts_process_keys(code, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }

        // Always call bts_task for BT mode
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
            if (host_keyboard_led_state().caps_lock) return;
#    endif
            add_key(KC_CAPS_LOCK);
            send_keyboard_report();
            wait_ms(TAP_HOLD_CAPS_DELAY);
            del_key(KC_CAPS_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_NUM_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (host_keyboard_led_state().num_lock) return;
#    endif
            add_key(KC_NUM_LOCK);
            send_keyboard_report();
            wait_ms(100);
            del_key(KC_NUM_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_SCROLL_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (host_keyboard_led_state().scroll_lock) return;
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

            // Handle numpad keys with custom behavior when unsync is enabled
            if (!key_eql_pressed && dev_info.unsync && IS_NUMPAD_KEYCODE(code)) {
                if (numpad_keys_pressed_count == 0) {
                    if (dev_info.num_unsync) {
                        // First numpad key pressed: apply NumLock sync logic
                        wait_ms(10);
                        original_num_lock_state = host_keyboard_led_state().num_lock;
                        if (!original_num_lock_state) {
                            add_key(KC_NUM_LOCK);
                            send_keyboard_report();
                            del_key(KC_NUM_LOCK);
                            send_keyboard_report();
                        }
                    }
                }
                numpad_keys_pressed_count++;
                // Always just send the key (do not re-run NumLock logic for >1 key held)
                add_key(code);
                send_keyboard_report();
            } else {
                // Normal behavior: follow host state or non-numpad keys
                add_key(code);
                send_keyboard_report();
            }
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
        // Handle numpad keys with custom behavior when unsync is enabled (BT mode)
        if (!key_eql_pressed && dev_info.unsync && IS_NUMPAD_KEYCODE(code)) {
            if (dev_info.num_unsync) {
                // Force numpad to produce numbers (NumLock ON behavior)
                bts_process_keys(code, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_task(dev_info.devs);
                while (bts_is_busy())
                    wait_ms(1);

                // Decrement counter and restore state only when last numpad key is released
                if (numpad_keys_pressed_count > 0) {
                    numpad_keys_pressed_count--;
                    if (numpad_keys_pressed_count == 0) {
                        // Last numpad key released - restore original NumLock state if needed
                        if (!original_num_lock_state && host_keyboard_led_state().num_lock) {
                            // Original was OFF, current is ON, restore to OFF
                            bts_process_keys(KC_NUM_LOCK, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                            bts_task(dev_info.devs);
                            while (bts_is_busy())
                                wait_ms(1);
                            bts_process_keys(KC_NUM_LOCK, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                            bts_task(dev_info.devs);
                            while (bts_is_busy())
                                wait_ms(1);
                        }
                    }
                }
            } else {
                bts_process_keys(code, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                bts_task(dev_info.devs);
                while (bts_is_busy()) {
                    wait_ms(1);
                }
            }
        } else {
            // Normal BT behavior for non-numpad keys
            bts_process_keys(code, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            bts_task(dev_info.devs);
            while (bts_is_busy()) {
                wait_ms(1);
            }
        }
    } else {
        if (code == KC_NO) {
            return;

#ifdef LOCKING_SUPPORT_ENABLE
        } else if (KC_LOCKING_CAPS_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            // Resync: ignore if caps lock already is off
            if (!host_keyboard_led_state().caps_lock) return;
#    endif
            add_key(KC_CAPS_LOCK);
            send_keyboard_report();
            del_key(KC_CAPS_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_NUM_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (!host_keyboard_led_state().num_lock) return;
#    endif
            add_key(KC_NUM_LOCK);
            send_keyboard_report();
            del_key(KC_NUM_LOCK);
            send_keyboard_report();

        } else if (KC_LOCKING_SCROLL_LOCK == code) {
#    ifdef LOCKING_RESYNC_ENABLE
            if (!host_keyboard_led_state().scroll_lock) return;
#    endif
            add_key(KC_SCROLL_LOCK);
            send_keyboard_report();
            del_key(KC_SCROLL_LOCK);
            send_keyboard_report();
#endif

        } else if (IS_BASIC_KEYCODE(code)) {
            // Handle numpad keys with custom behavior when unsync is enabled
            if (!key_eql_pressed && dev_info.unsync && IS_NUMPAD_KEYCODE(code)) {
                if (dev_info.num_unsync) {
                    // Force numpad to produce numbers (NumLock ON behavior)
                    del_key(code);
                    send_keyboard_report();

                    // Decrement counter and restore state only when last numpad key is released
                    if (numpad_keys_pressed_count > 0) {
                        numpad_keys_pressed_count--;
                        if (numpad_keys_pressed_count == 0) {
                            wait_ms(10);
                            // Last numpad key released - restore original NumLock state if needed
                            if (!original_num_lock_state) {
                                // Original was OFF, current is ON, restore to OFF
                                add_key(KC_NUM_LOCK);
                                send_keyboard_report();
                                del_key(KC_NUM_LOCK);
                                send_keyboard_report();
                            }
                        }
                    }
                } else {
                    del_key(code);
                    send_keyboard_report();
                }
            } else {
                // Normal behavior: follow host state or non-numpad keys
                del_key(code);
                send_keyboard_report();
            }
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
                bts_process_keys(KC_RCTL, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LCTL, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x2) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RSFT, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LSFT, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x4) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RALT, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LALT, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x8) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RGUI, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LGUI, true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(code), true, dev_info.devs, keymap_config.no_gui, KEY_NUM);
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
                bts_process_keys(KC_RCTL, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LCTL, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x2) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RSFT, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LSFT, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x4) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RALT, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LALT, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        if (QK_MODS_GET_MODS(code) & 0x8) {
            if (QK_MODS_GET_MODS(code) & 0x10)
                bts_process_keys(KC_RGUI, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
            else
                bts_process_keys(KC_LGUI, false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
        bts_process_keys(QK_MODS_GET_BASIC_KEYCODE(code), false, dev_info.devs, keymap_config.no_gui, KEY_NUM);
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
// 线程定义
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
// 初始化函数
// ===========================================
void bt_init(void) {
    bts_init(&bts_info);
    bt_used_pin_init();

    // 读取用户配置
    dev_info.raw = eeconfig_read_user();
    if (!dev_info.raw) {
        dev_info.devs      = DEVS_USB;
        dev_info.last_devs = DEVS_USB;
        eeconfig_update_user(dev_info.raw);
    }

    bt_init_time = timer_read32();
    chThdCreateStatic(waThread1, sizeof(waThread1), HIGHPRIO, Thread1, NULL);
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

    rgb_status_save = rgb_matrix_config.enable;
}

// ===========================================
// 蓝牙任务函数
// ===========================================
void bt_task(void) {
    static uint32_t last_time = 0;

    if ((bt_init_time != 0) && (timer_elapsed32(bt_init_time) >= 2000)) {
        bt_init_time = 0;

        bts_send_name(DEVS_HOST1);
        switch (dev_info.devs) {
            case DEVS_HOST1:
                bts_send_vendor(v_host1);
                break;
            case DEVS_HOST2:
                bts_send_vendor(v_host2);
                break;
            case DEVS_HOST3:
                bts_send_vendor(v_host3);
                break;
            case DEVS_2_4G:
                bts_send_vendor(v_2_4g);
                break;
            default:
                bts_send_vendor(v_usb);
                dev_info.devs = DEVS_USB;
                eeconfig_update_user(dev_info.raw);
                break;
        }

        bts_send_vendor(v_en_sleep_bt);
        bts_send_vendor(v_en_sleep_wl);
    }

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
    if (!bt_init_time) bt_scan_mode();
}

// ===========================================
// 按键处理函数
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

        if (!rgb_matrix_config.enable) {
            if (rgb_status_save) {
                rgb_matrix_enable_noeeprom();
            }
        }
    }

    retval = bt_process_record_other(keycode, record);

    if (dev_info.devs != DEVS_USB) {
        if (retval != false) {
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
            } else {
                if (dev_info.unsync) {
                    if (keycode == KC_NUM_LOCK) {
                        if (dev_info.unsync) {
                            if (record->event.pressed) {
                                dev_info.num_unsync = !dev_info.num_unsync;
                                eeconfig_update_user(dev_info.raw);
                            }
                            return false;
                        } else {
                            return false;
                        }
                    }
                    if (IS_NUMPAD_KEYCODE(keycode)) {
                        // Use register_code/unregister_code for consistent numpad behavior
                        if (record->event.pressed) {
                            register_code(keycode);
                        } else {
                            unregister_code(keycode);
                        }
                        retval = false; // We handled the key
                    }
                } else {
                    retval = bts_process_keys(keycode, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
                }
            }
        }
    }

#ifdef RGB_MATRIX_ENABLE
    open_rgb();
#endif

    return retval;
}

// ===========================================
// 设备切换函数
// ===========================================
void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset) {
    if (!rgb_matrix_config.enable) {
        if (rgb_status_save) {
            rgb_matrix_enable_noeeprom();
        }
    }

    bool usb_sws = !!last_mode ? !now_mode : !!now_mode;

    extern uint8_t  indicator_status;
    extern uint8_t  indicator_reset_last_time;
    extern uint32_t last_total_time;

    if (usb_sws) {
        if (!!now_mode) {
            usbDisconnectBus(&USB_DRIVER);
            usbStop(&USB_DRIVER);
        } else {
            init_usb_driver(&USB_DRIVER);
        }
    }

    if (dev_info.devs != dev_info.last_devs) {
        last_total_time = timer_read32();
    }

    dev_info.devs = now_mode;
    if (dev_info.devs != DEVS_USB) {
        dev_info.last_devs = dev_info.devs;
    } else if (dev_info.devs == DEVS_USB) {
        USB_switch_time = timer_read32();
        USB_blink_cnt   = 0;
    }

    if (dev_info.devs == DEVS_USB) {
        writePinLow(A14);
    } else {
        writePinHigh(A14);
    }

    // 重置蓝牙状态
    bts_info.bt_info.pairing        = false;
    bts_info.bt_info.paired         = false;
    bts_info.bt_info.come_back      = false;
    bts_info.bt_info.come_back_err  = false;
    bts_info.bt_info.mode_switched  = false;
    bts_info.bt_info.indictor_rgb_s = 0;

    if (readPin(MM_MODE_SW_PIN)) eeconfig_update_user(dev_info.raw);

    // 发送相应的蓝牙命令
    switch (dev_info.devs) {
        case DEVS_HOST1:
        case DEVS_HOST2:
        case DEVS_HOST3:
        case DEVS_2_4G:
            if (reset != false) {
                indicator_status          = 1;
                indicator_reset_last_time = true;
                // uint8_t vendor_cmds[]     = {v_host1, v_host2, v_host3, v_2_4g};
                uint8_t vendor_names[] = {DEVS_HOST1, DEVS_HOST2, DEVS_HOST3, DEVS_2_4G};
                bts_send_name(vendor_names[dev_info.devs - 1]);
                // bts_send_vendor(vendor_cmds[dev_info.devs - 1]);
                bts_send_vendor(v_pair);
            } else {
                indicator_status          = 2;
                indicator_reset_last_time = true;
                uint8_t vendor_cmds[]     = {v_host1, v_host2, v_host3, v_2_4g};
                bts_send_vendor(vendor_cmds[dev_info.devs - 1]);
            }
            break;
        case DEVS_USB:
            indicator_status          = 2;
            indicator_reset_last_time = true;
            bts_send_vendor(v_usb);
            break;
        default:
            break;
    }
}

// ===========================================
// 其他按键处理
// ===========================================
static bool bt_process_record_other(uint16_t keycode, keyrecord_t *record) {
    // 更新长按键时间
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

    // Hardware switch check - prevent wireless mode selection when switch forces wired
    if (is_switch_forcing_wired_mode() && is_wireless_mode_keycode(keycode)) {
        return false; // Block wireless mode keys when hardware switch forces wired mode
    }

    switch (keycode) {
        case BT_HOST1:
        case BT_HOST2:
        case BT_HOST3:
        case BT_2_4G:
        case BT_USB: {
            if (record->event.pressed) {
                uint8_t target_devs = (keycode == BT_HOST1) ? DEVS_HOST1 : (keycode == BT_HOST2) ? DEVS_HOST2 : (keycode == BT_HOST3) ? DEVS_HOST3 : (keycode == BT_2_4G) ? DEVS_2_4G : DEVS_USB;
                if (dev_info.devs != target_devs) {
                    // Track if user manually selected USB mode
                    if (target_devs == DEVS_USB) {
                        dev_info.manual_usb_mode = true;
                    } else {
                        dev_info.manual_usb_mode = false; // User selected wireless mode
                    }
                    eeconfig_update_user(dev_info.raw);
                    bt_switch_mode(dev_info.devs, target_devs, false);
                }
            }
        } break;

        case BT_VOL: {
            if (record->event.pressed) {
                // bts_send_vendor(v_query_vol);
                // switch (get_battery_charge_state()) {
                //     case BATTERY_STATE_CHARGING:
                //         bts_send_vendor(v_query_vol_chrg);
                //         break;

                //     case BATTERY_STATE_CHARGED_FULL:
                //         bts_send_vendor(v_query_vol_full);
                //         break;

                //     case BATTERY_STATE_UNPLUGGED:
                //     default:
                //         bts_send_vendor(v_query_vol);
                //         break;
                // }
                query_vol_flag = true;
            } else {
                query_vol_flag = false;
            }
        } break;

        case SW_OS: {
        } break;

        case NK_TOGG: {
            if (record->event.pressed) {
                keymap_config.nkro = !keymap_config.nkro;
                if (keymap_config.nkro) {
                    single_blink_cnt   = 6;
                    single_blink_index = 0;
                    single_blink_color = (RGB){0, 0, 200};
                } else {
                    single_blink_cnt   = 6;
                    single_blink_index = 0;
                    single_blink_color = (RGB){100, 100, 100};
                }
                single_blink_time = timer_read32();
            }
        } break;

        case RGB_TEST: {
            if (record->event.pressed) {
                if (rgb_test_en) {
                    rgb_test_en    = false;
                    rgb_test_index = 0;
                }
            }
        } break;

        case FACTORY_RESET:
        case KEYBOARD_RESET:
        case BLE_RESET:
            break;
        default:
            return true;
    }

    return false;
}

// ===========================================
// 长按键处理
// ===========================================
static void long_pressed_keys_cb(uint16_t keycode) {
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

        case SW_OS: {
            if (get_highest_layer(default_layer_state) == 0) {
                // If in win layer, switch to mac layer
                set_single_persistent_default_layer(3);
                dev_info.unsync = false;
                eeconfig_update_user(dev_info.raw);
                single_blink_cnt   = 6;
                single_blink_index = 20;
                single_blink_color = (RGB){RGB_BLUE};
                single_blink_time  = timer_read32();
            } else if (get_highest_layer(default_layer_state) == 3) {
                // If in mac layer, switch back to win layer
                set_single_persistent_default_layer(0);
                dip_switch_read(true);
                single_blink_cnt   = 6;
                single_blink_index = 20;
                single_blink_color = (RGB){RGB_GREEN};
                single_blink_time  = timer_read32();
            }
        } break;

        case FACTORY_RESET:
        case KEYBOARD_RESET:
        case BLE_RESET: {
            if (!factory_reset.type) {
                factory_reset.type       = (keycode == FACTORY_RESET) ? _FACTORY : (keycode == KEYBOARD_RESET) ? _KEYBOARD : _BLE;
                factory_reset.progress   = 0;
                factory_reset.start_time = timer_read32();

                // Enable RGB for indication
                if (!rgb_matrix_config.enable) {
                    rgb_matrix_enable_noeeprom();
                }
            }
            break;
        }

        case RGB_TEST: {
            if (rgb_test_en != true) {
                rgb_test_en    = true;
                rgb_test_index = 1;
                rgb_test_time  = timer_read32();
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

// ===========================================
// Helper functions for hardware switch
// ===========================================

/**
 * @brief Check if hardware switch is forcing wired mode
 * @return true if switch is ON (forcing wired mode), false if switch is OFF (allowing all modes)
 */
static bool is_switch_forcing_wired_mode(void) {
#ifdef MM_MODE_SW_PIN
    return !readPin(MM_MODE_SW_PIN); // Switch ON = false reading = force wired
#else
    return false; // No switch = allow all modes
#endif
}

/**
 * @brief Check if a keycode is for a wireless mode
 * @param keycode The keycode to check
 * @return true if keycode is for wireless mode (BT1, BT2, BT3, 2.4G)
 */
static bool is_wireless_mode_keycode(uint16_t keycode) {
    return (keycode == BT_HOST1 || keycode == BT_HOST2 || keycode == BT_HOST3 || keycode == BT_2_4G);
}

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

// ===========================================
// 硬件管理函数
// ===========================================
static void bt_used_pin_init(void) {
#ifdef MM_MODE_SW_PIN
    setPinInputHigh(MM_MODE_SW_PIN);
#endif

#if defined(MM_CABLE_PIN) && defined(MM_CHARGE_PIN)
    setPinInputHigh(MM_CABLE_PIN);
    setPinInput(MM_CHARGE_PIN);
#endif

#ifdef RGB_DRIVER_SDB_PIN
    setPinOutputPushPull(RGB_DRIVER_SDB_PIN);
    writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
}

static void bt_scan_mode(void) {
#ifdef MM_MODE_SW_PIN
    static bool last_switch_state = false; // Track previous switch state

    bool switch_state = readPin(MM_MODE_SW_PIN);

    // Handle switch state changes
    if (last_switch_state != switch_state) {
        last_switch_state = switch_state;

        if (is_switch_forcing_wired_mode()) {
            // Switch turned ON -> Force wired mode immediately
            if (dev_info.devs != DEVS_USB) {
                // Save current wireless mode for restoration when switch turns OFF
                if (is_current_mode_wireless()) {
                    dev_info.last_devs = dev_info.devs;
                }
                bt_switch_mode(dev_info.devs, DEVS_USB, false);
                dev_info.manual_usb_mode = false; // This is a forced switch, not manual
                eeconfig_update_user(dev_info.raw);
            }
        } else {
            // Switch turned OFF -> Restore last wireless mode ONLY if it was wireless AND USB wasn't manually selected
            if (dev_info.devs == DEVS_USB && is_current_mode_wireless_by_devs(dev_info.last_devs) && !dev_info.manual_usb_mode) {
                // Only auto-switch back if:
                // 1. Currently in USB mode
                // 2. Last mode was wireless
                // 3. USB mode wasn't manually selected by user
                bt_switch_mode(dev_info.devs, dev_info.last_devs, false);
            }
            // If USB was manually selected or last mode was USB, stay in USB mode
        }
    }

    // Continuously enforce hardware switch restriction when ON
    if (is_switch_forcing_wired_mode() && dev_info.devs != DEVS_USB) {
        // Hardware switch is ON -> force USB mode (handles any edge cases)
        bt_switch_mode(dev_info.devs, DEVS_USB, false);
        dev_info.manual_usb_mode = false; // This is a forced switch, not manual
        eeconfig_update_user(dev_info.raw);
    }
#endif
}

// ===========================================
// 低电量管理函数
// ===========================================
static void bt_bat_low_level_state(void) {
    // 进入低电压状态：电量≤5%且未充电
    if (bts_info.bt_info.low_vol) {
        rgb_matrix_set_color_all(RGB_OFF);
    }
}

// ===========================================
// RGB控制函数
// ===========================================
void led_config_all(void) {
    if (!led_inited) {
#ifdef RGB_DRIVER_SDB_PIN
        // setPinOutputPushPull(RGB_DRIVER_SDB_PIN);
        writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
        led_inited = true;
    }
}

void led_deconfig_all(void) {
    if (led_inited) {
#ifdef RGB_DRIVER_SDB_PIN
        // setPinOutputPushPull(RGB_DRIVER_SDB_PIN);
        writePinLow(RGB_DRIVER_SDB_PIN);
#endif
        led_inited = false;
    }
}

static void close_rgb(void) {
    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }

    if (sober) {
        if (kb_sleep_flag || ((timer_elapsed32(key_press_time) >= sleep_time_table[dev_info.sleep_mode]) && (sleep_time_table[dev_info.sleep_mode] != 0))) {
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();
#ifdef RGB_DRIVER_SDB_PIN
            writePinLow(RGB_DRIVER_SDB_PIN);
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

static void open_rgb(void) {
    key_press_time = timer_read32();
    if (!sober) {
#ifdef RGB_DRIVER_SDB_PIN
        writePinHigh(RGB_DRIVER_SDB_PIN);
#endif
        if (bak_rgb_toggle) {
            extern bool low_vol_offed_sleep;
            kb_sleep_flag       = false;
            low_vol_offed_sleep = false;
            rgb_matrix_enable_noeeprom();
        }
        if (!led_inited) {
            led_config_all();
        }
        sober = true;
    }
}

// ===========================================
// 指示灯函数
// ===========================================
static void bt_indicate_led(void) {
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
        case 1: { // 闪烁模式 5Hz 重置
            if ((last_time == 0) || (timer_elapsed32(last_time) >= 200)) {
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

            if (timer_elapsed32(last_total_time) >= (60 * 1000)) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;

        case 2: { // 闪烁模式 2Hz 回连
            if ((last_time == 0) || (timer_elapsed32(last_time) >= 500)) {
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

            if (timer_elapsed32(last_total_time) >= (60 * 1000)) {
                indicator_status = 0;
                kb_sleep_flag    = true;
            }
        } break;

        case 3: { // 长亮模式
            if ((timer_elapsed32(last_long_time) < (2 * 1000))) {
                rgb.r = rgb_index_color_table[dev_info.devs][0];
                rgb.g = rgb_index_color_table[dev_info.devs][1];
                rgb.b = rgb_index_color_table[dev_info.devs][2];
            } else {
                low_battery_warning.triggered   = true;
                low_battery_warning.blink_count = 0;
                low_battery_warning.blink_time  = timer_read32();
                low_battery_warning.blink_state = false;
                low_battery_warning.completed   = false;
                indicator_status                = 0;
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
                    if ((dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                        bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
                    }
                    if (dev_info.devs == DEVS_2_4G) {
                        bt_switch_mode(DEVS_USB, DEVS_2_4G, false);
                    }
                }
            }
            break;
    }

    if (indicator_status) {
        rgb_matrix_set_color(rgb_index, rgb.r, rgb.g, rgb.b);
    }
}

static void usb_indicate_led(void) {
    static uint16_t USB_blink_time;
    static bool     USB_blink;

    if ((USB_DRIVER.state != USB_ACTIVE)) {
        if (USB_blink_cnt <= 20) {
            if (timer_elapsed(USB_blink_time) >= 500) {
                USB_blink_cnt++;
                USB_blink      = !USB_blink;
                USB_blink_time = timer_read();
            }
            if (USB_blink) {
                rgb_matrix_set_color(rgb_index_table[DEVS_USB], rgb_index_color_table[DEVS_USB][0], rgb_index_color_table[DEVS_USB][1], rgb_index_color_table[DEVS_USB][2]);
            } else {
                rgb_matrix_set_color(rgb_index_table[DEVS_USB], 0, 0, 0);
            }
        }
        USB_switch_time = timer_read32();
    } else {
        if (timer_elapsed32(USB_switch_time) < 3000) {
            rgb_matrix_set_color(rgb_index_table[DEVS_USB], rgb_index_color_table[DEVS_USB][0], rgb_index_color_table[DEVS_USB][1], rgb_index_color_table[DEVS_USB][2]);
        }
    }
}

// ===========================================
// Factory Reset Functions start
// ===========================================

static void execute_factory_reset(void) {
    switch (factory_reset.type) {
        case _FACTORY: // Factory reset
            eeconfig_init();
            keymap_config.nkro = false;
            dip_switch_read(true);
            if (readPin(MM_MODE_SW_PIN) && (dev_info.devs != DEVS_USB)) {
                bts_send_vendor(v_clear);
                wait_ms(1000);
                bt_switch_mode(DEVS_HOST1, DEVS_USB, false);
                last_total_time = timer_read32();
            }
            break;

        case _KEYBOARD: // Keyboard reset
            eeconfig_init();
            keymap_config.nkro = false;
            dip_switch_read(true);
            break;

        case _BLE: // BLE reset
            if (readPin(MM_MODE_SW_PIN) && (dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                bts_send_vendor(v_clear);
                wait_ms(1000);
                bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                last_total_time = timer_read32();
            }
            break;
    }
}

static void factory_reset_indeicate(void) {
    if (!factory_reset.type) return;

    // Update progress based on time
    uint32_t elapsed       = timer_elapsed32(factory_reset.start_time);
    factory_reset.progress = (elapsed / 500) + 1; // 500ms per step

    // Execute reset at step 7
    if (factory_reset.progress >= 7) {
        execute_factory_reset();
        factory_reset.type = _NONE; // Reset state
        return;
    }

    // Show progress: turn off all LEDs, light up the reset type LED
    rgb_matrix_set_color_all(0, 0, 0);

    uint8_t led_index = (factory_reset.type == _FACTORY) ? 7 : // F1 for factory
                            (factory_reset.type == _KEYBOARD) ? 6
                                                              : // F2 for keyboard
                            5;                                  // F3 for BLE

    rgb_matrix_set_color(led_index, RGB_RED); // White progress indicator
}

static void blink_effects(void) {
    // 全键闪烁
    if (all_blink_cnt) {
        rgb_matrix_set_color_all(0, 0, 0);
        if (timer_elapsed32(all_blink_time) > 500) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt % 2) {
            rgb_matrix_set_color_all(all_blink_color.r, all_blink_color.g, all_blink_color.b);
        } else {
            rgb_matrix_set_color_all(0, 0, 0);
        }
    }

    // 单键闪烁
    if (single_blink_cnt) {
        if (timer_elapsed32(single_blink_time) > 500) {
            single_blink_time = timer_read32();
            single_blink_cnt--;
        }
        if (single_blink_cnt % 2) {
            rgb_matrix_set_color(single_blink_index, single_blink_color.r, single_blink_color.g, single_blink_color.b);
        } else {
            rgb_matrix_set_color(single_blink_index, 0, 0, 0);
        }
    }
}

static void show_device_state(void) {
    // FN 按下时显示当前设备状态
    if (get_highest_layer(layer_state) == 2 || get_highest_layer(layer_state) == 4) {
        rgb_matrix_set_color(rgb_index_table[dev_info.devs], RGB_BLUE);
    }
}

static void charging_indicate(void) {
    extern bool charge_full;

    // Debounced, hysteretic "first full" detector
    // static uint8_t  prev_pvol               = 0;
    static bool     first_reach_full        = false; // drives the blink once per cycle
    static uint32_t full_candidate_since    = 0;
    static bool     full_latched_this_cycle = false;

    uint8_t  pv      = bts_info.bt_info.pvol;
    uint32_t now     = timer_read32();
    bool     plugged = (get_battery_charge_state() != BATTERY_STATE_UNPLUGGED);

    if (dev_info.devs != DEVS_USB) {
        // Detect and debounce first reach to FULL_PVOL_THRESHOLD
        if (!full_latched_this_cycle) {
            if (pv >= FULL_PVOL_THRESHOLD && plugged) {
                if (full_candidate_since == 0) {
                    full_candidate_since = now;
                } else if (timer_elapsed32(full_candidate_since) >= FULL_DEBOUNCE_MS) {
                    first_reach_full        = true; // trigger this cycle's indication
                    full_latched_this_cycle = true; // prevent re-trigger until hysteresis clears
                    full_candidate_since    = 0;    // reset candidate window
                }
            } else {
                full_candidate_since = 0; // reset if we dip below threshold
            }
        } else {
            // Re-arm for next cycle only after dropping below hysteresis or unplugging
            if (pv <= FULL_HYSTERESIS_PVOL || !plugged) {
                full_latched_this_cycle = false;
                // Don't force first_reach_full here; it'll be set when we debounce next time
            }
        }
    }

    // Track charge state timing window for steady full detection
    if (get_battery_charge_state() == BATTERY_STATE_CHARGING) {
        charge_complete_warning.entry_full_time = now;
    }

    if ((get_battery_charge_state() == BATTERY_STATE_CHARGED_FULL) || first_reach_full) {
        if (timer_elapsed32(charge_complete_warning.entry_full_time) > 2000 || first_reach_full) {
            // 充满状态
            if (!is_in_full_power_state) {
                is_in_full_power_state = true;
                if (!charge_complete_warning.triggered) {
                    charge_complete_warning.triggered   = true;
                    charge_complete_warning.blink_count = 0;
                    charge_complete_warning.blink_time  = now;
                    charge_complete_warning.blink_state = false;
                    charge_complete_warning.completed   = false;
                    charge_full                         = true;
                }
            }

            // 只有在未完成闪烁且闪烁次数未达到5次时才显示充电指示
            if (!charge_complete_warning.completed && charge_complete_warning.blink_count < 6) {
                if (timer_elapsed32(charge_complete_warning.blink_time) >= 1000) {
                    charge_complete_warning.blink_time  = now;
                    charge_complete_warning.blink_state = !charge_complete_warning.blink_state;

                    if (charge_complete_warning.blink_state) {
                        charge_complete_warning.blink_count++;
                        if (charge_complete_warning.blink_count >= 6) {
                            charge_full                         = false;
                            charge_complete_warning.completed   = true;
                            charge_complete_warning.blink_state = false;
                            first_reach_full                    = false; // allow future cycles
                        }
                    }
                }

                // 显示充电完成闪烁
                if (charge_complete_warning.blink_state) {
                    rgb_matrix_set_color(22, 0, 200, 0);
                } else {
                    rgb_matrix_set_color(22, 0, 0, 0);
                }
            }
        }
    } else {
        // 充电线未接入，重置充电状态
        if (is_in_full_power_state) {
            charge_full            = false;
            is_in_full_power_state = false;
            memset(&charge_complete_warning, 0, sizeof(charge_complete_warning_t));
        }
    }

    // prev_pvol = pv;
}

static void bt_bat_low_level_warning(void) {
    // update_low_voltage_state();

    // 低电量警告（电量≤20%）
    if (bts_info.bt_info.pvol <= 20) {
        if (!is_in_low_power_state) {
            is_in_low_power_state = true;

            if (!low_battery_warning.triggered) {
                low_battery_warning.triggered   = true;
                low_battery_warning.blink_count = 0;
                low_battery_warning.blink_time  = timer_read32();
                low_battery_warning.blink_state = false;
                low_battery_warning.completed   = false;
            }
        }

        // 处理闪烁逻辑
        if (!low_battery_warning.completed && !kb_sleep_flag && (indicator_status == 0)) {
            if (timer_elapsed32(low_battery_warning.blink_time) >= 1000) {
                low_battery_warning.blink_time  = timer_read32();
                low_battery_warning.blink_state = !low_battery_warning.blink_state;

                if (low_battery_warning.blink_state) {
                    low_battery_warning.blink_count++;
                    if (low_battery_warning.blink_count >= 6) {
                        low_battery_warning.completed   = true;
                        low_battery_warning.blink_state = false;
                    }
                }
            }

            // 显示闪烁效果
            if (low_battery_warning.blink_state) {
                rgb_matrix_set_color(22, 200, 0, 0);
            } else {
                rgb_matrix_set_color(22, 0, 0, 0);
            }
        }
    }

    if (bts_info.bt_info.pvol > 20) {
        if (is_in_low_power_state) {
            is_in_low_power_state = false;
            memset(&low_battery_warning, 0, sizeof(low_battery_warning_t));
        }
    }
}

static void bt_bat_low_level_shutdown(void) {
    extern bool low_vol_offed_sleep;
    if (bts_info.bt_info.low_vol_offed) {
        kb_sleep_flag       = true;
        low_vol_offed_sleep = true;
    }
}

static battery_charge_state_t get_battery_charge_state(void) {
#if defined(MM_CABLE_PIN) && defined(MM_CHARGE_PIN)
    static battery_charge_state_t stable_state = BATTERY_STATE_UNPLUGGED;

    if (readPin(MM_CABLE_PIN)) {
        stable_state = BATTERY_STATE_UNPLUGGED;
    } else {
        if (!readPin(MM_CHARGE_PIN)) {
            stable_state = BATTERY_STATE_CHARGING;
        } else {
            stable_state = BATTERY_STATE_CHARGED_FULL;
        }
    }

    return stable_state;
#else
    return BATTERY_STATE_UNPLUGGED;
#endif
}

static void bt_bat_query_period(void) {
    // 定期查询电量
    static uint32_t query_vol_time = 0;

    // Check if we should query battery (avoid querying too frequently)
    if (!bt_init_time && (bts_info.bt_info.paired) && !kb_sleep_flag && timer_elapsed32(query_vol_time) > 4000) {
        query_vol_time = timer_read32();

        // Send appropriate query command based on charge state
        switch (get_battery_charge_state()) {
            case BATTERY_STATE_CHARGING:
                bts_send_vendor(v_query_vol_chrg);
                break;

            case BATTERY_STATE_CHARGED_FULL:
                bts_send_vendor(v_query_vol_full);
                break;

            case BATTERY_STATE_UNPLUGGED:
                bts_send_vendor(v_query_vol);
                break;
            default:
                bts_send_vendor(v_query_vol);
                break;
        }
    }
}

static void bt_bat_level_display(void) {
    if (query_vol_flag) {
        // 清空显示区域
        for (uint8_t i = 0; i < 22; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }

        // 电量显示LED
        uint8_t pvol = bts_info.bt_info.pvol;

        // 计算LED数量（至少2个，最多10个）
        uint8_t led_count = (pvol < 30) ? 2 : ((pvol / 10) >= 10 ? 10 : (pvol / 10));

        // 根据电量确定颜色
        RGB color;
        if (pvol < 30) {
            color = (RGB){100, 0, 0}; // 红色
        } else if (pvol < 60) {
            color = (RGB){100, 50, 0}; // 橙色
        } else {
            color = (RGB){0, 100, 0}; // 绿色
        }

        // 点亮LED
        for (uint8_t i = 0; i < led_count; i++) {
            rgb_matrix_set_color(query_index[i], color.r, color.g, color.b);
        }
    }
}

// ===========================================
// 主RGB指示器函数
// ===========================================
bool bt_indicators_advanced(uint8_t led_min, uint8_t led_max) {
    if (get_battery_charge_state() != BATTERY_STATE_UNPLUGGED) {
        if (dev_info.devs != DEVS_USB) {
            // trurn off backlight when the voltage is low
            bt_bat_low_level_state();
        }
    }

    bt_bat_query_period();

    // 充电状态指示: only call when pvol first reaches 100 in each cycle
    charging_indicate();

    // Show the current device state
    show_device_state();

    // 设备状态指示
    if (dev_info.devs == DEVS_USB) {
        usb_indicate_led();
    } else {
        bt_indicate_led();
        if (get_battery_charge_state() == BATTERY_STATE_UNPLUGGED) {
            // 非充电状态下的其他指示
            bt_bat_low_level_warning();
            bt_bat_low_level_shutdown();
        }
    }

    // Show current device state
    bt_bat_level_display();

    // 闪烁效果处理
    blink_effects();

    // 工厂重置显示（最高优先级）
    if (factory_reset.type) {
        factory_reset_indeicate();
    }

    // rgb test
    if (rgb_test_en) {
        if (timer_elapsed32(rgb_test_time) >= 1000) {
            rgb_test_time = timer_read32();
            rgb_test_index++;
            if (rgb_test_index > 4) {
                rgb_test_index = 1;
            }
        }

        for (uint8_t i = 0; i < 23; i++) {
            rgb_matrix_set_color(i, rgb_test_color_table[rgb_test_index - 1][0], rgb_test_color_table[rgb_test_index - 1][1], rgb_test_color_table[rgb_test_index - 1][2]);
        }
    }

    return true;
}

void matrix_scan_user(void) {
#ifdef MULTIMODE_ENABLE
    bt_task();
#endif
}

void housekeeping_task_kb(void) {
#ifdef MULTIMODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE) {
            // USB挂起状态
            if (!usb_suspend_timer) {
                // 开始计时
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                // 挂起超过10秒，关闭背光
                if (!usb_suspend) {
                    // 如果之前没有进入挂起状态，执行挂起操作
                    usb_suspend = true;
                    led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            // USB活跃状态，重置计时器
            if (usb_suspend_timer) {
                usb_suspend_timer = 0;
                if (usb_suspend) {
                    // 如果之前处于挂起状态，恢复背光
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

    housekeeping_task_user();
}

void matrix_init_user(void) {
#ifdef MULTIMODE_ENABLE
    // 初始化蓝牙
    bt_init();
    led_config_all();
#endif
}

void suspend_power_down_user(void) {
    // rgb_matrix_disable_noeeprom();
    led_deconfig_all();
}

void suspend_wakeup_init_user(void) {
    // rgb_matrix_enable_noeeprom();
    led_config_all();
}
