// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef WIRELESS_ENABLE
#    include "wireless.h"
#    include "lowpower.h"
#endif
#include "lib/lib8tion/lib8tion.h"
#include "usb_main.h"

typedef union {
    uint32_t raw;
    struct {
        uint8_t flag : 1;
        uint8_t devs : 3;
    };
} confinfo_t;
confinfo_t confinfo;

uint32_t post_init_timer = 0x00;

// Expose md_send_devinfo to support the Bridge75 Bluetooth naming quirk
// See the readme.md for more information about the quirk.
void md_send_devinfo(const char *name);

void eeconfig_confinfo_update(uint32_t raw) {
    eeconfig_update_kb(raw);
}

uint32_t eeconfig_confinfo_read(void) {
    return eeconfig_read_kb();
}

void eeconfig_confinfo_default(void) {
    confinfo.flag = true;
#ifdef WIRELESS_ENABLE
    confinfo.devs = DEVS_USB;
#endif

    eeconfig_confinfo_update(confinfo.raw);
}

void eeconfig_confinfo_init(void) {
    confinfo.raw = eeconfig_confinfo_read();
    if (!confinfo.raw) {
        eeconfig_confinfo_default();
    }
}

void keyboard_post_init_kb(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

    eeconfig_confinfo_init();

#ifdef LED_POWER_EN_PIN
    setPinOutput(LED_POWER_EN_PIN);
    writePinLow(LED_POWER_EN_PIN);
#endif

#ifdef USB_POWER_EN_PIN
    writePinLow(USB_POWER_EN_PIN);
    setPinOutput(USB_POWER_EN_PIN);
#endif

#ifdef WL_PWR_SW_PIN
    setPinInputHigh(WL_PWR_SW_PIN);
#endif

#ifdef BT_CABLE_PIN
    setPinInputHigh(BT_CABLE_PIN);
#endif

#ifdef BT_CHARGE_PIN
    setPinInput(BT_CHARGE_PIN);
#endif

#ifdef WIRELESS_ENABLE
    wireless_init();
    md_send_devinfo(MD_BT_NAME);
    wait_ms(10);
    wireless_devs_change(!confinfo.devs, confinfo.devs, false);
    post_init_timer = timer_read32();
#endif

    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
        eeconfig_update_keymap(&keymap_config);
    }

    keyboard_post_init_user();
}

#ifdef WIRELESS_ENABLE
void usb_power_connect(void) {
#    ifdef USB_POWER_EN_PIN
    writePinLow(USB_POWER_EN_PIN);
#    endif
}

void usb_power_disconnect(void) {
#    ifdef USB_POWER_EN_PIN
    writePinHigh(USB_POWER_EN_PIN);
#    endif
}

void suspend_power_down_kb(void) {
#    ifdef LED_POWER_EN_PIN
    writePinHigh(LED_POWER_EN_PIN);
#    endif
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
#    ifdef LED_POWER_EN_PIN
    writePinLow(LED_POWER_EN_PIN);
#    endif

    wireless_devs_change(wireless_get_current_devs(), wireless_get_current_devs(), false);
    suspend_wakeup_init_user();
}

void wireless_post_task(void) {
    // auto switching devs
    if (post_init_timer && timer_elapsed32(post_init_timer) >= 100) {
        md_send_devctrl(MD_SND_CMD_DEVCTRL_FW_VERSION);   // get the module fw version.
        md_send_devctrl(MD_SND_CMD_DEVCTRL_SLEEP_BT_EN);  // timeout 30min to sleep in bt mode, enable
        md_send_devctrl(MD_SND_CMD_DEVCTRL_SLEEP_2G4_EN); // timeout 30min to sleep in 2.4g mode, enable
        wireless_devs_change(!confinfo.devs, confinfo.devs, false);
        post_init_timer = 0x00;
    }
}

static bool low_vol_warning = false;
static bool low_vol_off     = false;

static bool     wls_factory_reset_flash_active    = false;
static uint8_t  wls_factory_reset_flash_remaining = 0;
static bool     wls_factory_reset_flash_on        = false;
static uint32_t wls_factory_reset_flash_timer     = 0x00;

void wls_factory_reset(void) {
    eeconfig_init();

    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
    }
}

static void wls_factory_reset_feedback(void) {
#    ifdef RGB_MATRIX_ENABLE
    wls_factory_reset_flash_active    = true;
    wls_factory_reset_flash_remaining = 6;
    wls_factory_reset_flash_on        = true;
    wls_factory_reset_flash_timer     = timer_read32();
#    endif
}

uint32_t wls_factory_reset_long_press(uint32_t trigger_time, void *cb_arg) {
    (void)trigger_time;
    (void)cb_arg;

    wls_factory_reset();
    wls_factory_reset_feedback();
    return 0;
}

uint32_t wls_process_long_press(uint32_t trigger_time, void *cb_arg) {
    uint16_t keycode = *((uint16_t *)cb_arg);

    switch (keycode) {
        case KC_BT1: {
            wireless_devs_change(wireless_get_current_devs(), DEVS_BT1, true);
        } break;
        case KC_BT2: {
            wireless_devs_change(wireless_get_current_devs(), DEVS_BT2, true);
        } break;
        case KC_BT3: {
            wireless_devs_change(wireless_get_current_devs(), DEVS_BT3, true);
        } break;
        case KC_2G4: {
            wireless_devs_change(wireless_get_current_devs(), DEVS_2G4, true);
        } break;
        default:
            break;
    }

    return 0;
}

bool process_record_wls(uint16_t keycode, keyrecord_t *record) {
    static uint16_t       keycode_shadow               = 0x00;
    static deferred_token wls_process_long_press_token = INVALID_DEFERRED_TOKEN;
    static deferred_token wls_factory_reset_token      = INVALID_DEFERRED_TOKEN;

    keycode_shadow = keycode;

#    ifndef WLS_KEYCODE_PAIR_TIME
#        define WLS_KEYCODE_PAIR_TIME 3000
#    endif

#    define WLS_KEYCODE_EXEC(wls_dev)                                                                                          \
        do {                                                                                                                   \
            if (record->event.pressed) {                                                                                       \
                wireless_devs_change(wireless_get_current_devs(), wls_dev, false);                                             \
                if (wls_process_long_press_token == INVALID_DEFERRED_TOKEN) {                                                  \
                    wls_process_long_press_token = defer_exec(WLS_KEYCODE_PAIR_TIME, wls_process_long_press, &keycode_shadow); \
                }                                                                                                              \
            } else {                                                                                                           \
                cancel_deferred_exec(wls_process_long_press_token);                                                            \
                wls_process_long_press_token = INVALID_DEFERRED_TOKEN;                                                         \
            }                                                                                                                  \
        } while (false)

#    ifndef WLS_FACTORY_RESET_HOLD_TIME
#        define WLS_FACTORY_RESET_HOLD_TIME 3000
#    endif

#    define WLS_FACTORY_RESET_EXEC()                                                                                       \
        do {                                                                                                               \
            if (record->event.pressed) {                                                                                   \
                if (wls_factory_reset_token == INVALID_DEFERRED_TOKEN) {                                                   \
                    wls_factory_reset_token = defer_exec(WLS_FACTORY_RESET_HOLD_TIME, wls_factory_reset_long_press, NULL); \
                }                                                                                                          \
            } else {                                                                                                       \
                cancel_deferred_exec(wls_factory_reset_token);                                                             \
                wls_factory_reset_token = INVALID_DEFERRED_TOKEN;                                                          \
            }                                                                                                              \
        } while (false)

    switch (keycode) {
        case KC_BT1: {
            WLS_KEYCODE_EXEC(DEVS_BT1);
        } break;
        case KC_BT2: {
            WLS_KEYCODE_EXEC(DEVS_BT2);
        } break;
        case KC_BT3: {
            WLS_KEYCODE_EXEC(DEVS_BT3);
        } break;
        case KC_2G4: {
            WLS_KEYCODE_EXEC(DEVS_2G4);
        } break;
        case KC_USB: {
            if (record->event.pressed) {
                clear_keyboard();
                layer_clear();
                wireless_devs_change(wireless_get_current_devs(), DEVS_USB, false);
            }
        } break;
        case EE_CLR: {
            WLS_FACTORY_RESET_EXEC();
        } break;
        default:
            return true;
    }

    return false;
}
#endif

bool     query_vol_flag = false;
uint32_t key_press_time = 0;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (confinfo.devs != DEVS_USB) {
        key_press_time = timer_read32();
        if (low_vol_off) {
            report_keyboard_t    temp_report_keyboard = {0};
            extern host_driver_t wireless_driver;
            wireless_driver.send_keyboard(&temp_report_keyboard);
            return false;
        }
    }

    if (process_record_user(keycode, record) != true) {
        return false;
    }

#ifdef WIRELESS_ENABLE
    if (process_record_wls(keycode, record) != true) {
        return false;
    }
#endif

    switch (keycode) {
        case RM_TOGG:
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

        case BT_VOL:
            if (record->event.pressed) {
                if (!query_vol_flag) query_vol_flag = true;
            } else {
                if (query_vol_flag) query_vol_flag = false;
            }
            return false;
        default:
            return true;
    }

    return false;
}

void matrix_scan_kb(void) {
    if (confinfo.devs != DEVS_USB) {
        if (!key_press_time) {
            key_press_time = timer_read32();
        } else if (timer_elapsed32(key_press_time) >= (5 * 60 * 1000)) {
            lpwr_set_state(LPWR_PRESLEEP);
        }
    }
}

#ifdef RGB_MATRIX_ENABLE

#    ifdef WIRELESS_ENABLE
bool     wls_rgb_indicator_reset    = false;
uint32_t wls_rgb_indicator_timer    = 0x00;
uint32_t wls_rgb_indicator_interval = 0;
uint32_t wls_rgb_indicator_times    = 0;
uint32_t wls_rgb_indicator_index    = 0;
RGB      wls_rgb_indicator_rgb      = {0};
#    endif // WIRELESS_ENABLE

#    ifdef WIRELESS_ENABLE
void rgb_matrix_wls_indicator_set(uint8_t index, RGB rgb, uint32_t interval, uint8_t times) {
    wls_rgb_indicator_timer = timer_read32();

    wls_rgb_indicator_index    = index;
    wls_rgb_indicator_interval = interval;
    wls_rgb_indicator_times    = times * 2;
    wls_rgb_indicator_rgb      = rgb;
}

void wireless_devs_change_kb(uint8_t old_devs, uint8_t new_devs, bool reset) {
    wls_rgb_indicator_reset = reset;

    if (confinfo.devs != wireless_get_current_devs()) {
        confinfo.devs = wireless_get_current_devs();
        eeconfig_confinfo_update(confinfo.raw);
    }

    switch (new_devs) {
        case DEVS_BT1: {
            if (reset) {
                rgb_matrix_wls_indicator_set(LED_HOST_BT1_INDEX, (RGB)LED_HOST_BT1_COLOR, WLS_BT_PAIR_INTERVAL, WLS_BT_PAIR_TIMEOUT);
            } else {
                rgb_matrix_wls_indicator_set(LED_HOST_BT1_INDEX, (RGB)LED_HOST_BT1_COLOR, WLS_BT_RECONNECT_INTERVAL, WLS_BT_RECONNECT_TIMEOUT);
            }
        } break;
        case DEVS_BT2: {
            if (reset) {
                rgb_matrix_wls_indicator_set(LED_HOST_BT2_INDEX, (RGB)LED_HOST_BT2_COLOR, WLS_BT_PAIR_INTERVAL, WLS_BT_PAIR_TIMEOUT);
            } else {
                rgb_matrix_wls_indicator_set(LED_HOST_BT2_INDEX, (RGB)LED_HOST_BT2_COLOR, WLS_BT_RECONNECT_INTERVAL, WLS_BT_RECONNECT_TIMEOUT);
            }
        } break;
        case DEVS_BT3: {
            if (reset) {
                rgb_matrix_wls_indicator_set(LED_HOST_BT3_INDEX, (RGB)LED_HOST_BT3_COLOR, WLS_BT_PAIR_INTERVAL, WLS_BT_PAIR_TIMEOUT);
            } else {
                rgb_matrix_wls_indicator_set(LED_HOST_BT3_INDEX, (RGB)LED_HOST_BT3_COLOR, WLS_BT_RECONNECT_INTERVAL, WLS_BT_RECONNECT_TIMEOUT);
            }
        } break;
        case DEVS_2G4: {
            if (reset) {
                rgb_matrix_wls_indicator_set(LED_HOST_2G4_INDEX, (RGB)LED_HOST_2G4_COLOR, WLS_2G4_PAIR_INTERVAL, WLS_2G4_PAIR_TIMEOUT);
            } else {
                rgb_matrix_wls_indicator_set(LED_HOST_2G4_INDEX, (RGB)LED_HOST_2G4_COLOR, WLS_2G4_RECONNECT_INTERVAL, WLS_2G4_RECONNECT_TIMEOUT);
            }
        } break;
        default:
            break;
    }
}

bool rgb_matrix_wls_indicator(void) {
    if (wls_rgb_indicator_timer) {
        if (timer_elapsed32(wls_rgb_indicator_timer) >= wls_rgb_indicator_interval) {
            wls_rgb_indicator_timer = timer_read32();

            if (wls_rgb_indicator_times) {
                wls_rgb_indicator_times--;
            }

            if (wls_rgb_indicator_times <= 0) {
                wls_rgb_indicator_timer = 0x00;
                if (*md_getp_state() != MD_STATE_CONNECTED) lpwr_set_state(LPWR_PRESLEEP);
            }

            if (*md_getp_state() == MD_STATE_CONNECTED) {
                if (wls_rgb_indicator_interval != 2000) {
                    wls_rgb_indicator_timer    = timer_read32();
                    wls_rgb_indicator_interval = 2000;
                    wls_rgb_indicator_times    = 1;
                } else {
                    wls_rgb_indicator_timer = 0x00;
                }
            }
        }

        if (wls_rgb_indicator_times % 2) {
            rgb_matrix_set_color(wls_rgb_indicator_index, wls_rgb_indicator_rgb.r, wls_rgb_indicator_rgb.g, wls_rgb_indicator_rgb.b);
        } else {
            rgb_matrix_set_color(wls_rgb_indicator_index, 0x00, 0x00, 0x00);
        }
    }

    return true;
}
#    endif // WIRELESS_ENABLE

static const uint8_t rgb_index_table[]          = {LED_HOST_USB_INDEX, LED_HOST_BT1_INDEX, LED_HOST_BT2_INDEX, LED_HOST_BT3_INDEX, 0xFF, 0xFF, LED_HOST_2G4_INDEX};
static const uint8_t rgb_index_color_table[][3] = {
    LED_HOST_USB_COLOR, LED_HOST_BT1_COLOR, LED_HOST_BT2_COLOR, LED_HOST_BT3_COLOR, {0, 0, 0}, {0, 0, 0}, LED_HOST_2G4_COLOR,
};

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    static uint8_t pvol = 94;
    pvol                = *md_getp_bat();

    if (wls_factory_reset_flash_active) {
        for (uint8_t i = 68; i <= 74; i++) {
            rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
        }

        if (timer_elapsed32(wls_factory_reset_flash_timer) >= 200) {
            wls_factory_reset_flash_timer = timer_read32();
            wls_factory_reset_flash_on    = !wls_factory_reset_flash_on;
            if (wls_factory_reset_flash_remaining > 0) {
                wls_factory_reset_flash_remaining--;
            }
            if (wls_factory_reset_flash_remaining == 0) {
                wls_factory_reset_flash_active = false;
            }
        }

        if (wls_factory_reset_flash_on) {
            for (uint8_t i = 0; i < 68; i++) {
                rgb_matrix_set_color(i, 0x77, 0, 0);
            }
        } else {
            for (uint8_t i = 0; i < 68; i++) {
                rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
            }
        }

        return true;
    }

    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
    } else {
        // Logo led effect
        uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_get_speed() / 4, 1));
        for (uint8_t i = 68; i <= 74; i++) {
            HSV hsv = {g_led_config.point[i].x - time, 255, rgb_matrix_get_val() / 3};
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }

    if (confinfo.devs != DEVS_USB) {
        if (query_vol_flag) {
            for (uint8_t i = 68; i <= 74; i++) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }

            uint8_t query_index[] = {74, 73, 72, 71, 70, 69, 68};
            uint8_t led_count     = 0;

            if (pvol >= 95)
                led_count = 7;
            else if (pvol >= 80)
                led_count = 6;
            else if (pvol >= 60)
                led_count = 5;
            else if (pvol >= 40)
                led_count = 4;
            else if (pvol >= 20)
                led_count = 3;
            else if (pvol > 10)
                led_count = 2;
            else if (pvol > 0)
                led_count = 1;
            else
                led_count = 0;

            RGB color = (RGB){14, 14, 14};
            for (uint8_t i = 0; i < led_count; i++) {
                rgb_matrix_set_color(query_index[i], color.r, color.g, color.b);
            }
        }

        extern bool low_vol_offed_sleep;

        if (readPin(BT_CABLE_PIN)) {
            // Critical battery - force sleep immediately
            if (pvol < 1 && !low_vol_off) {
                low_vol_off         = true;
                low_vol_offed_sleep = true;
                lpwr_set_state(LPWR_PRESLEEP);
            }

            // Don't show warning animation if already sleeping
            if (!low_vol_off) {
                if (pvol <= 10) {
                    if (!low_vol_warning) low_vol_warning = true;
                }

                if (low_vol_warning) {
                    rgb_matrix_set_color_all(0, 0, 0);

                    HSV     hsv        = {0, 255, 0};
                    uint8_t time       = scale16by8(g_rgb_timer, qadd8(128 / 4, 1));
                    uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, RGB_MATRIX_DEFAULT_VAL / 2);
                    hsv.v              = brightness;
                    RGB rgb            = hsv_to_rgb(hsv);

                    rgb_matrix_set_color(74, rgb.r, rgb.g, rgb.b);
                }
            }
        } else {
            low_vol_warning     = false;
            low_vol_off         = false;
            low_vol_offed_sleep = false;
        }
    }

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(LED_CAPS_LOCK_INDEX, 0x77, 0x77, 0x77);
    }

    if (keymap_config.no_gui) {
        rgb_matrix_set_color(LED_GUI_LOCK_INDEX, 0x77, 0x77, 0x77);
    }

    if (rgb_matrix_indicators_advanced_user(led_min, led_max) != true) {
        return false;
    }

    if ((get_highest_layer(default_layer_state | layer_state) == 1) || (get_highest_layer(default_layer_state | layer_state) == 4)) {
        rgb_matrix_set_color(rgb_index_table[confinfo.devs], rgb_index_color_table[confinfo.devs][0], rgb_index_color_table[confinfo.devs][1], rgb_index_color_table[confinfo.devs][2]);
    }

#    ifdef WIRELESS_ENABLE
    if (wireless_get_current_devs() != DEVS_USB) {
        if (rgb_matrix_wls_indicator() != true) {
            return false;
        }
#    endif
    }

    return true;
}

void md_devs_change(uint8_t devs, bool reset) {
    switch (devs) {
        case DEVS_USB: {
            md_send_devctrl(MD_SND_CMD_DEVCTRL_USB);
        } break;
        case DEVS_2G4: {
            if (reset) {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_PAIR);
            } else {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_2G4);
            }
        } break;
        case DEVS_BT1: {
            if (reset) {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_PAIR);
            } else {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_BT1);
            }
        } break;
        case DEVS_BT2: {
            if (reset) {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_PAIR);
            } else {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_BT2);
            }
        } break;
        case DEVS_BT3: {
            if (reset) {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_PAIR);
            } else {
                md_send_devctrl(MD_SND_CMD_DEVCTRL_BT3);
            }
        } break;
        default:
            break;
    }
}

#endif // RGB_MATRIX_ENABLE

void wireless_send_nkro(report_nkro_t *report) {
    static report_keyboard_t temp_report_keyboard                 = {0};
    uint8_t                  wls_report_nkro[MD_SND_CMD_NKRO_LEN] = {0};

#ifdef NKRO_ENABLE

    if (report != NULL) {
        report_nkro_t temp_report_nkro = *report;
        uint8_t       key_count        = 0;

        temp_report_keyboard.mods = temp_report_nkro.mods;
        for (uint8_t i = 0; i < NKRO_REPORT_BITS; i++) {
            key_count += __builtin_popcount(temp_report_nkro.bits[i]);
        }

        /*
         * Use NKRO for sending when more than 6 keys are pressed
         * to solve the issue of the lack of a protocol flag in wireless mode.
         */

        for (uint8_t i = 0; i < key_count; i++) {
            uint8_t usageid;
            uint8_t idx, n = 0;

            for (n = 0; n < NKRO_REPORT_BITS && !temp_report_nkro.bits[n]; n++) {
            }
            usageid = (n << 3) | biton(temp_report_nkro.bits[n]);
            del_key_bit(&temp_report_nkro, usageid);

            for (idx = 0; idx < WLS_KEYBOARD_REPORT_KEYS; idx++) {
                if (temp_report_keyboard.keys[idx] == usageid) {
                    goto next;
                }
            }

            for (idx = 0; idx < WLS_KEYBOARD_REPORT_KEYS; idx++) {
                if (temp_report_keyboard.keys[idx] == 0x00) {
                    temp_report_keyboard.keys[idx] = usageid;
                    break;
                }
            }
        next:
            if (idx == WLS_KEYBOARD_REPORT_KEYS && (usageid < (MD_SND_CMD_NKRO_LEN * 8))) {
                wls_report_nkro[usageid / 8] |= 0x01 << (usageid % 8);
            }
        }

        temp_report_nkro = *report;

        // find key up and del it.
        uint8_t nkro_keys = key_count;
        for (uint8_t i = 0; i < WLS_KEYBOARD_REPORT_KEYS; i++) {
            report_nkro_t found_report_nkro;
            uint8_t       usageid = 0x00;
            uint8_t       n;

            found_report_nkro = temp_report_nkro;

            for (uint8_t c = 0; c < nkro_keys; c++) {
                for (n = 0; n < NKRO_REPORT_BITS && !found_report_nkro.bits[n]; n++) {
                }
                usageid = (n << 3) | biton(found_report_nkro.bits[n]);
                del_key_bit(&found_report_nkro, usageid);
                if (usageid == temp_report_keyboard.keys[i]) {
                    del_key_bit(&temp_report_nkro, usageid);
                    nkro_keys--;
                    break;
                }
            }

            if (usageid != temp_report_keyboard.keys[i]) {
                temp_report_keyboard.keys[i] = 0x00;
            }
        }
    } else {
        memset(&temp_report_keyboard, 0, sizeof(temp_report_keyboard));
    }
#endif // NKRO_ENABLE
    void wireless_task(void);
    bool smsg_is_busy(void);
    while (smsg_is_busy()) {
        wireless_task();
    }
    extern host_driver_t wireless_driver;
    wireless_driver.send_keyboard(&temp_report_keyboard);
    md_send_nkro(wls_report_nkro);
}

#ifdef WIRELESS_ENABLE
void housekeeping_task_wls(void) {
    if (confinfo.devs == DEVS_USB) {
        if ((USB_DRIVER.state == USB_SUSPENDED) && (USB_DRIVER.saved_state == USB_ACTIVE)) {
            print("[s]");
            while (USB_DRIVER.state == USB_SUSPENDED) {
                /* Do this in the suspended state */
                suspend_power_down(); // on AVR this deep sleeps for 15ms
                /* Remote wakeup */
                if (suspend_wakeup_condition()) {
                    usbWakeupHost(&USB_DRIVER);
                    restart_usb_driver(&USB_DRIVER);
                }
            }
            /* Woken up */
            // variables has been already cleared by the wakeup hook
            send_keyboard_report();
#    ifdef MOUSEKEY_ENABLE
            mousekey_send();
#    endif /* MOUSEKEY_ENABLE */
        }
    }
}
#endif

void housekeeping_task_user(void) {
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

#ifdef WIRELESS_ENABLE
    housekeeping_task_wls();
#endif

    if (confinfo.devs == DEVS_USB) {
        if (usb_suspend) {
            if (suspend_wakeup_condition()) {
                // usbWakeupHost(&USB_DRIVER);
                // restart_usb_driver(&USB_DRIVER);
                usb_suspend       = false;
                usb_suspend_timer = 0;
#ifdef LED_POWER_EN_PIN
                writePinLow(LED_POWER_EN_PIN);
#endif
            }
        }

        if ((USB_DRIVER.state != USB_ACTIVE)) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                usb_suspend_timer = 0;
                if (!usb_suspend) {
                    usb_suspend = true;
#ifdef LED_POWER_EN_PIN
                    writePinHigh(LED_POWER_EN_PIN);
#endif
                    // lpwr_set_state(LPWR_PRESLEEP);
                }
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;

#ifdef LED_POWER_EN_PIN
                writePinLow(LED_POWER_EN_PIN);
#endif
            }
        }
    }
}
