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
#include "command.h"
#include "common/bt_task.h"
#include "lib/lib8tion/lib8tion.h"

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
        .bt_name        = {"Metafor Air - BT$", "Metafor Air - BT$", "Metafor Air - BT$"},
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

// clang-format off
long_pressed_keys_t long_pressed_keys[] = {
  {.keycode = BT_HOST1, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = BT_HOST2, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = BT_HOST3, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = BT_2_4G, .press_time = 0, .event_cb = long_pressed_keys_cb},
//   {.keycode = GU_TOGG, .press_time = 0, .event_cb = long_pressed_keys_cb},
//   {.keycode = SW_OS1, .press_time = 0, .event_cb = long_pressed_keys_cb},
//   {.keycode = RGB_TEST, .press_time = 0, .event_cb = long_pressed_keys_cb},
  {.keycode = EE_CLR, .press_time = 0, .event_cb = long_pressed_keys_cb},
};
// clang-format on

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

    bt_scan_mode();

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

        bts_send_vendor(v_en_sleep_wl);
        bts_send_vendor(v_en_sleep_bt);

        switch (dev_info.devs) {
            case DEVS_HOST1: {
                bts_send_vendor(v_host1);
                bts_send_name(DEVS_HOST1);
            } break;
            case DEVS_HOST2: {
                bts_send_vendor(v_host2);
                bts_send_name(DEVS_HOST2);
            } break;
            case DEVS_HOST3: {
                bts_send_vendor(v_host3);
                bts_send_name(DEVS_HOST3);
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

        bts_task(dev_info.devs);

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
    }
    retval = process_record_other(keycode, record);

    if (dev_info.devs != DEVS_USB) {
        if (retval != false) {
            retval = bts_process_keys(keycode, record->event.pressed, dev_info.devs, keymap_config.no_gui, KEY_NUM);
        }
    }

#ifdef RGB_MATRIX_ENABLE
    open_rgb();
#endif

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
                bts_send_name(DEVS_HOST1);
                bts_send_vendor(v_host1);
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
                bts_send_name(DEVS_HOST2);
                bts_send_vendor(v_host2);
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
                bts_send_name(DEVS_HOST3);
                bts_send_vendor(v_host3);
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
                bts_send_name(DEVS_2_4G);
                bts_send_vendor(v_2_4g);
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

static RGB      SW_OS1_ind_color     = {RGB_BLACK};
static uint32_t SW_OS1_ind_hold_time = 0;
static uint32_t EE_CLR_press_cnt     = 0;
static uint32_t EE_CLR_press_time    = 0;
static bool     EE_CLR_flag          = false;
bool            query_vol_flag       = false;

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
                if (dev_info.devs != DEVS_HOST1) {
                    if ((dev_info.devs != DEVS_HOST1) && (dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                        bt_switch_mode(dev_info.devs, DEVS_HOST1, false);
                    }
                }
            }
        } break;
        case BT_HOST2: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_HOST2) && (dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST2, false);
                }
            }
        } break;
        case BT_HOST3: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_HOST3) && (dev_info.devs != DEVS_USB) && (dev_info.devs != DEVS_2_4G)) {
                    bt_switch_mode(dev_info.devs, DEVS_HOST3, false);
                }
            }
        } break;
        case BT_2_4G: {
            if (record->event.pressed) {
                if ((dev_info.devs != DEVS_2_4G) && (dev_info.devs != DEVS_USB)) {
                    bt_switch_mode(dev_info.devs, DEVS_2_4G, false);
                }
            }
        } break;
        // case BT_VOL: {
        //     if (record->event.pressed) {
        //         bts_send_vendor(v_query_vol);
        //         query_vol_flag = true;
        //     } else {
        //         query_vol_flag = false;
        //     }
        // } break;
        case EE_CLR: {
        } break;
        case SW_OS1: { // OS switch key
            if (record->event.pressed) {
                if (get_highest_layer(default_layer_state) == 0) { // MAC_BASE
                    set_single_persistent_default_layer(3);
                    SW_OS1_ind_hold_time = timer_read32();
                    SW_OS1_ind_color     = (RGB){RGB_BLUE};
                    // extern void blink_rgb_set(uint8_t, uint8_t);
                    // blink_rgb_set(1, 0);
                } else if (get_highest_layer(default_layer_state) == 3) { // WIN_BASE
                    set_single_persistent_default_layer(0);
                    SW_OS1_ind_hold_time = timer_read32();
                    SW_OS1_ind_color     = (RGB){RGB_PURPLE};
                    // extern void blink_rgb_set(uint8_t, uint8_t);
                    // blink_rgb_set(0, 0);
                }
            }
        } break;

        default:
            return true;
    }

    return false;
}

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
        case SW_OS1: {
        } break;
        case EE_CLR: {
            if (!EE_CLR_flag) {
                EE_CLR_flag       = true;
                EE_CLR_press_time = timer_read32();
                EE_CLR_press_cnt  = 1;
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
    if (!readPin(BT_MODE_SW_PIN) && readPin(RF_MODE_SW_PIN)) {
        if ((dev_info.devs == DEVS_USB) || (dev_info.devs == DEVS_2_4G)) {
            bt_switch_mode(dev_info.devs, dev_info.last_devs, false); // BT mode
            // mcu_reset();
        }
    }
    if (!readPin(RF_MODE_SW_PIN) && readPin(BT_MODE_SW_PIN)) {
        if (dev_info.devs != DEVS_2_4G) {
            bt_switch_mode(dev_info.devs, DEVS_2_4G, false); // 2_4G mode
            // mcu_reset();
        }
    }
    if (readPin(BT_MODE_SW_PIN) && readPin(RF_MODE_SW_PIN)) {
        if (dev_info.devs != DEVS_USB) bt_switch_mode(dev_info.devs, DEVS_USB, false); // usb mode
    }
#endif
}

#ifdef RGB_MATRIX_ENABLE

static uint32_t key_press_time;
static uint32_t close_rgb_time;
static bool     bak_rgb_toggle;
static bool     sober         = true;
static bool     kb_sleep_flag = false;
extern bool     led_inited;
extern void     led_config_all(void);
extern void     led_deconfig_all(void);
static uint32_t sleep_time[3] = {3 * 60 * 1000, 30 * 60 * 1000, 1 * 60 * 60 * 1000};

static const uint8_t rgb_index_table[]          = {7, 3, 2, 1, 0};
static const uint8_t rgb_index_color_table[][3] = {
    {RGB_BLACK}, {RGB_BLUE}, {RGB_CYAN}, {RGB_PINK}, {RGB_WHITE},
};
static uint32_t last_total_time           = 0;
uint8_t         indicator_status          = 2;
uint8_t         indicator_reset_last_time = false;

static void close_rgb(void) {
    if (!key_press_time) {
        key_press_time = timer_read32();
        return;
    }

    if (sober) {
        if (kb_sleep_flag || ((timer_elapsed32(key_press_time) >= sleep_time[1]) && (indicator_status == 0))) {
            bak_rgb_toggle = rgb_matrix_config.enable;
            sober          = false;
            close_rgb_time = timer_read32();
            rgb_matrix_disable_noeeprom();
            writePinLow(RGB_DRIVER_SDB_PIN);
        }
    } else {
        if (!rgb_matrix_config.enable) {
            if (timer_elapsed32(close_rgb_time) >= ENTRY_STOP_TIMEOUT) {
                /* Turn off all indicators led */
                if (led_inited) {
                    led_deconfig_all();
                }

#    ifdef ENTRY_STOP_MODE
                lp_system_sleep();
#    endif
                extern void open_rgb(void);
                if (dev_info.devs != DEVS_2_4G)
                    bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
                else
                    bt_switch_mode(DEVS_USB, DEVS_2_4G, false);
                open_rgb();
            }
        }
    }
}

void open_rgb(void) {
    key_press_time = timer_read32();
    if (!sober) {
        writePinHigh(RGB_DRIVER_SDB_PIN);
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

const uint8_t hsv_table[][3] = {
    {HSV_RED},
    {HSV_ORANGE},
};

void pwr_led_breath(void) {}

uint8_t bt_indicator_rgb(uint8_t led_min, uint8_t led_max) {
    // FN 按下时显示当前设备状态
    if ((get_highest_layer(default_layer_state | layer_state) == 1) || (get_highest_layer(default_layer_state | layer_state))) {
        rgb_matrix_set_color(rgb_index_table[dev_info.devs], rgb_index_color_table[dev_info.devs][0], rgb_index_color_table[dev_info.devs][1], rgb_index_color_table[dev_info.devs][2]);
    } else {
        rgb_matrix_set_color(5, RGB_BLACK);
    }

    if (EE_CLR_flag) {
        if (timer_elapsed32(EE_CLR_press_time) >= 300) {
            EE_CLR_press_time = timer_read32();
            EE_CLR_press_cnt++;
        }
        if (EE_CLR_press_cnt >= 14) {
            EE_CLR_press_time = 0;
            EE_CLR_press_cnt  = 0;
            EE_CLR_flag       = false;
            eeconfig_init();
            // eeconfig_update_rgb_matrix_default();

            if (dev_info.devs != DEVS_USB) last_total_time = 0;
        }
    }

    if (EE_CLR_flag) {
        if (EE_CLR_press_cnt % 2) {
            rgb_matrix_set_color(5, RGB_RED);
        } else {
            rgb_matrix_set_color(5, 0, 0, 0);
        }
        return false;
    }

#    if defined(BT_CABLE_PIN) && defined(BT_CHARGE_PIN)
    // 充电接入
    static uint32_t charging_time;
    static uint32_t full_time;

    if (!readPin(BT_CABLE_PIN)) {
        if (!readPin(BT_CHARGE_PIN)) {
            // 正在充电
            full_time = timer_read32();
            if (timer_elapsed32(charging_time) >= 2000) {
                HSV hsv;
                hsv.h              = hsv_table[1][0];
                hsv.s              = hsv_table[1][1];
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(rgb_matrix_get_speed() / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, rgb_matrix_get_val());
                hsv.v              = brightness;
                RGB rgb            = hsv_to_rgb(hsv);
                rgb_matrix_set_color(5, rgb.r, rgb.g, rgb.b);
            }
        } else {
            // 充满
            charging_time = timer_read32();
            if (timer_elapsed32(full_time) <= 2000) {
                rgb_matrix_set_color(5, RGB_GREEN);
            }
        }
    }
#    endif

    if (dev_info.devs != DEVS_USB) {
        static uint32_t query_vol_time = 0;
        static bool     Low_power      = false;

        if (!bt_init_time && !kb_sleep_flag && (bts_info.bt_info.paired) && (timer_elapsed32(query_vol_time) > 4000)) {
            query_vol_time = timer_read32();
            bts_send_vendor(v_query_vol);
        }

        // if (query_vol_flag) {
        // }

        if ((bts_info.bt_info.pvol <= 10) && !Low_power) {
            Low_power = true;
        } else if ((bts_info.bt_info.pvol >= 40) && Low_power) {
            Low_power = false;
        }
        if (Low_power && readPin(BT_CABLE_PIN)) {
            HSV hsv;
            hsv.h              = hsv_table[0][0];
            hsv.s              = hsv_table[0][1];
            uint8_t time       = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 4, 1));
            uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, rgb_matrix_get_val());
            hsv.v              = brightness;
            RGB rgb            = hsv_to_rgb(hsv);
            rgb_matrix_set_color(5, rgb.r, rgb.g, rgb.b);
        }

        if (bts_info.bt_info.low_vol_offed) {
            extern bool low_vol_offed_sleep;
            kb_sleep_flag       = true;
            low_vol_offed_sleep = true;
        }
    }

    // SW_OS1 indicator
    if (SW_OS1_ind_hold_time && (timer_elapsed32(SW_OS1_ind_hold_time) < 3000)) {
        rgb_matrix_set_color(5, SW_OS1_ind_color.r, SW_OS1_ind_color.g, SW_OS1_ind_color.b);
    } else {
        SW_OS1_ind_hold_time = 0;
    }

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
                if (!bts_info.bt_info.pairing) {
                    if (dev_info.devs == DEVS_2_4G)
                        bt_switch_mode(DEVS_USB, DEVS_2_4G, true);
                    else
                        bt_switch_mode(DEVS_USB, dev_info.last_devs, true);
                }
                /* 超时60s退出 */
                if (timer_elapsed32(last_total_time) >= (60 * 1000)) {
                    // if (timer_elapsed32(last_total_time) >= (30 * 1000)) {
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

                /* 超时30s退出 */
                if (timer_elapsed32(last_total_time) >= (15 * 1000)) {
                    indicator_status = 0;
                    kb_sleep_flag    = true;
                }
            } break;
            case 3: { // 长亮模式
                if ((timer_elapsed32(last_long_time) < (2 * 1000))) {
                    rgb.r = rgb_index_color_table[dev_info.devs][0];
                    rgb.g = rgb_index_color_table[dev_info.devs][1];
                    rgb.b = rgb_index_color_table[dev_info.devs][2];
                    rgb_matrix_set_color(5, rgb.r, rgb.g, rgb.b);
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
                        // if (!bts_info.bt_info.pairing) {
                        //     indicator_status = 2;
                        //     break;
                        // }
                        indicator_status = 2;
                        if (dev_info.devs == DEVS_2_4G)
                            bt_switch_mode(DEVS_USB, DEVS_2_4G, false);
                        else
                            bt_switch_mode(DEVS_USB, dev_info.last_devs, false);
                        break;
                    }
                }
                return true;
        }

        rgb_matrix_set_color(rgb_index, rgb.r, rgb.g, rgb.b);
    }

    return true;
}
#endif
