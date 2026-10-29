/* Copyright (C) 2022 jonylee@hfd
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
#ifdef MULTIMODE_ENABLE
#    include "bt_task.h"
#endif
#include "dynamic_keymap.h"
#include "bled.h"
#include "lib/lib8tion/lib8tion.h"
#include "usb_main.h"
#include "wwdg.h"
#include "signalrgb.h"

enum _layers {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

#define MAC_TSK C(KC_UP)
#define MAC_SEH G(KC_SPACE)
#define GOOGLE GOOGLE_WEB

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_107_ansi(
        KC_ESC,            KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_SCRL, KC_PAUS,  KC_CALC,  KC_WHOM,  VIA_WEB,  KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME, KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,  KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,                                KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,            KC_RSFT,            KC_UP,             KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             KC_RALT, MO(1),    KC_APP,   KC_RCTL,  KC_LEFT,  KC_DOWN, KC_RGHT,  KC_P0,              KC_PDOT),

    [WIN_FN] = LAYOUT_107_ansi(
        EE_CLR,            KC_BRID,  KC_BRIU,  WIN_TSK,  KC_MYCM, KC_MAIL, KC_WHOM, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______, _______,  _______,  _______,  _______,  _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  _______,  _______, _______,  _______,  _______,  _______,  _______,
        RGB_TEST,_______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, NK_TOGG, _______,  _______,  RM_NEXT,  _______,  _______, _______,  _______,  _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            RM_HUEU,                               _______,  _______,  _______,
        BLED_MOD,          BLED_HUI, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, SLED_HUI,           SLED_MOD,           RM_VALU,           _______,  _______,  _______,  _______,
        BLED_SPI,GU_TOGG,  BLED_VAI,                              SLED_SPI,                           SLED_VAI,_______,  _______,  _______,  RM_SPDD,  RM_VALD,  RM_SPDU, _______,            _______),

    [MAC_BASE] = LAYOUT_107_ansi(
        KC_ESC,            KC_BRID,  KC_BRIU,  MAC_TSK,  MAC_SEH, KC_F5,   KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_PSCR,  KC_SCRL, KC_PAUS,  M_CALC,   M_BROWSER,  VIA_WEB,  KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME, KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,  KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,                                KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,            KC_RSFT,            KC_UP,             KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             KC_RCMD, MO(3),    KC_APP,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT, KC_P0,              KC_PDOT),

    [MAC_FN] = LAYOUT_107_ansi(
        EE_CLR,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,  _______, _______,  _______,  _______,  _______,  _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  _______,  _______, _______,  _______,  _______,  _______,  _______,
        RGB_TEST,_______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, NK_TOGG, _______,  _______,  RM_NEXT,  _______,  _______, _______,  _______,  _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            RM_HUEU,                               _______,  _______,  _______,
        BLED_MOD,          BLED_HUI, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, SLED_HUI,           SLED_MOD,           RM_VALU,           _______,  _______,  _______,  _______,
        BLED_SPI,_______,  BLED_VAI,                              SLED_SPI,                           SLED_VAI,_______,  _______,  _______,  RM_SPDD,  RM_VALD,  RM_SPDU, _______,            _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = {ENCODER_CCW_CW(_______, _______) },
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = {ENCODER_CCW_CW(_______, _______) },
};
#endif // ENCODER_MAP_ENABLE
// clang-format on

enum via_web_states {
    VIA_WEB_IDLE,
    VIA_WEB_LAUNCH_RELEASE,
    VIA_WEB_LAUNCH_WAIT,
    VIA_WEB_CAPS_RELEASE,
    VIA_WEB_CAPS_WAIT,
    VIA_WEB_CHAR_PRESS,
    VIA_WEB_CHAR_RELEASE,
    VIA_WEB_SUBMIT_WAIT,
    VIA_WEB_ENTER_RELEASE,
    VIA_WEB_CAPS_RESTORE_WAIT,
    VIA_WEB_CAPS_RESTORE_RELEASE,
};

enum web_type {
    WEB_VIA,
    WEB_GOOGLE,
    WEB_CALC,
    WEB_BROWSER,
};

static const uint16_t PROGMEM via_web_keycodes[] = {
    KC_H, KC_T, KC_T, KC_P, KC_S, S(KC_SCLN), KC_SLSH, KC_SLSH, KC_U, KC_S, KC_E, KC_V, KC_I, KC_A, KC_DOT, KC_A, KC_P, KC_P, KC_SLSH,
};

static const uint16_t PROGMEM google_web_keycodes[] = {
    KC_H, KC_T, KC_T, KC_P, KC_S, S(KC_SCLN), KC_SLSH, KC_SLSH, KC_W, KC_W, KC_W, KC_DOT, KC_G, KC_O, KC_O, KC_G, KC_L, KC_E, KC_DOT, KC_C, KC_O, KC_M, KC_SLSH,
};

static const uint16_t PROGMEM calc_keycodes[] = {
    KC_C,
    KC_A,
    KC_L,
    KC_C,
};

static const uint16_t PROGMEM browser_keycodes[] = {
    KC_S, KC_A, KC_F, KC_A, KC_R, KC_I,
};

static uint8_t  via_web_state = VIA_WEB_IDLE;
static uint8_t  via_web_index = 0;
static uint16_t via_web_keycode;
static uint32_t via_web_timer;
static bool     via_web_gui_was_locked;
static bool     via_web_caps_was_on;
static bool     via_web_is_mac;
static enum web_type via_web_type;

static uint8_t web_keycode_count(void) {
    switch (via_web_type) {
        case WEB_VIA:
            return ARRAY_SIZE(via_web_keycodes);
        case WEB_GOOGLE:
            return ARRAY_SIZE(google_web_keycodes);
        case WEB_CALC:
            return ARRAY_SIZE(calc_keycodes);
        case WEB_BROWSER:
            return ARRAY_SIZE(browser_keycodes);
        default:
            return 0;
    }
}

static uint16_t web_keycode_at(uint8_t index) {
    switch (via_web_type) {
        case WEB_VIA:
            return pgm_read_word(&via_web_keycodes[index]);
        case WEB_GOOGLE:
            return pgm_read_word(&google_web_keycodes[index]);
        case WEB_CALC:
            return pgm_read_word(&calc_keycodes[index]);
        case WEB_BROWSER:
            return pgm_read_word(&browser_keycodes[index]);
        default:
            return KC_NO;
    }
}

static void start_web(enum web_type web_type) {
    if (via_web_state != VIA_WEB_IDLE) {
        return;
    }

    via_web_gui_was_locked = keymap_config.no_gui;
    via_web_caps_was_on    = host_keyboard_led_state().caps_lock;
    via_web_is_mac         = get_highest_layer(default_layer_state) == MAC_BASE;
    via_web_type           = web_type;
    via_web_index          = 0;

    keymap_config.no_gui = false;
    via_web_keycode      = via_web_is_mac ? G(KC_SPC) : G(KC_R);
    register_code16(via_web_keycode);
    via_web_timer = timer_read32();
    via_web_state = VIA_WEB_LAUNCH_RELEASE;
}

static void via_web_task(void) {
    switch (via_web_state) {
        case VIA_WEB_LAUNCH_RELEASE:
            if (timer_elapsed32(via_web_timer) >= 30) {
                unregister_code16(via_web_keycode);
                keymap_config.no_gui = via_web_gui_was_locked;
                via_web_timer        = timer_read32();
                via_web_state        = VIA_WEB_LAUNCH_WAIT;
            }
            break;

        case VIA_WEB_LAUNCH_WAIT:
            if (timer_elapsed32(via_web_timer) >= 600) {
                via_web_timer = timer_read32();
                if (via_web_caps_was_on) {
                    via_web_state = VIA_WEB_CHAR_PRESS;
                } else {
                    register_code(KC_CAPS);
                    via_web_state = VIA_WEB_CAPS_RELEASE;
                }
            }
            break;

        case VIA_WEB_CAPS_RELEASE:
            if (timer_elapsed32(via_web_timer) >= TAP_HOLD_CAPS_DELAY) {
                unregister_code(KC_CAPS);
                via_web_timer = timer_read32();
                via_web_state = VIA_WEB_CAPS_WAIT;
            }
            break;

        case VIA_WEB_CAPS_WAIT:
            if (timer_elapsed32(via_web_timer) >= 100) {
                via_web_timer = timer_read32();
                via_web_state = VIA_WEB_CHAR_PRESS;
            }
            break;

        case VIA_WEB_CHAR_PRESS:
            if (timer_elapsed32(via_web_timer) >= 10) {
                via_web_keycode = web_keycode_at(via_web_index);
                register_code16(via_web_keycode);
                via_web_timer = timer_read32();
                via_web_state = VIA_WEB_CHAR_RELEASE;
            }
            break;

        case VIA_WEB_CHAR_RELEASE:
            if (timer_elapsed32(via_web_timer) >= 20) {
                unregister_code16(via_web_keycode);
                via_web_timer = timer_read32();
                via_web_index++;
                via_web_state = via_web_index < web_keycode_count() ? VIA_WEB_CHAR_PRESS : VIA_WEB_SUBMIT_WAIT;
            }
            break;

        case VIA_WEB_SUBMIT_WAIT:
            if (timer_elapsed32(via_web_timer) >= (via_web_is_mac ? 500 : 100)) {
                register_code(KC_ENTER);
                via_web_timer = timer_read32();
                via_web_state = VIA_WEB_ENTER_RELEASE;
            }
            break;

        case VIA_WEB_ENTER_RELEASE:
            if (timer_elapsed32(via_web_timer) >= (via_web_is_mac ? 100 : 50)) {
                unregister_code(KC_ENTER);
                via_web_timer = timer_read32();
                via_web_state = via_web_caps_was_on ? VIA_WEB_IDLE : VIA_WEB_CAPS_RESTORE_WAIT;
            }
            break;

        case VIA_WEB_CAPS_RESTORE_WAIT:
            if (timer_elapsed32(via_web_timer) >= 100) {
                register_code(KC_CAPS);
                via_web_timer = timer_read32();
                via_web_state = VIA_WEB_CAPS_RESTORE_RELEASE;
            }
            break;

        case VIA_WEB_CAPS_RESTORE_RELEASE:
            if (timer_elapsed32(via_web_timer) >= TAP_HOLD_CAPS_DELAY) {
                unregister_code(KC_CAPS);
                via_web_state = VIA_WEB_IDLE;
            }
            break;

        default:
            break;
    }
}

bool no_indicator_under_srgb = false;
// uint8_t sled_mode_before_charge = SLED_MODE_VOL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SLED_MOD:
        case BLED_MOD: {
            if (record->event.pressed) {
                if (keycode == SLED_MOD) {
                    dev_info.sled_mode = (dev_info.sled_mode + 1) % SLED_MODE_COUNT;
                } else {
                    dev_info.bled_mode = (dev_info.bled_mode + 1) % BLED_MODE_COUNT;
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        }

        case SLED_VAI:
        case BLED_VAI: {
            if (record->event.pressed) {
                if (keycode == SLED_VAI) {
                    if (bled_info.sled_val == RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                        bled_info.sled_val = 0;
                    } else {
                        bled_info.sled_val = qadd8(bled_info.sled_val, RGB_MATRIX_VAL_STEP);
                    }
                } else {
                    if (bled_info.bled_val == RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                        bled_info.bled_val = 0;
                    } else {
                        bled_info.bled_val = qadd8(bled_info.bled_val, RGB_MATRIX_VAL_STEP);
                    }
                }
                eeconfig_update_kb(bled_info.raw);
            }
            return false;
        }

        case SLED_SPI:
        case BLED_SPI: {
            if (record->event.pressed) {
                if (keycode == SLED_SPI) {
                    if (bled_info.sled_speed == UINT8_MAX) {
                        bled_info.sled_speed = 0;
                    } else {
                        bled_info.sled_speed = qadd8(bled_info.sled_speed, RGB_MATRIX_SPD_STEP);
                    }
                } else {
                    if (bled_info.bled_speed == UINT8_MAX) {
                        bled_info.bled_speed = 0;
                    } else {
                        bled_info.bled_speed = qadd8(bled_info.bled_speed, RGB_MATRIX_SPD_STEP);
                    }
                }
                eeconfig_update_kb(bled_info.raw);
            }
            return false;
        }

        case SLED_HUI:
        case BLED_HUI: {
            if (record->event.pressed) {
                if (keycode == SLED_HUI) {
                    dev_info.sled_color = (dev_info.sled_color == COLOR_WHITE) ? COLOR_RAINBOW : (dev_info.sled_color + 1);
                } else {
                    dev_info.bled_color = (dev_info.bled_color == COLOR_WHITE) ? COLOR_RED : (dev_info.bled_color + 1);
                }
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        }

        case WIN_TSK: {
            if (record->event.pressed) {
                if (dev_info.devs == DEVS_USB) {
                    register_code(KC_LWIN);
                    register_code(KC_TAB);
                } else {
                    bts_process_keys(KC_LWIN, 1, dev_info.devs, 0, KEY_NUM);
                    bts_process_keys(KC_TAB, 1, dev_info.devs, 0, KEY_NUM);
                }
            } else {
                if (dev_info.devs == DEVS_USB) {
                    unregister_code(KC_TAB);
                    unregister_code(KC_LWIN);
                } else {
                    bts_process_keys(KC_TAB, 0, dev_info.devs, 0, KEY_NUM);
                    bts_process_keys(KC_LWIN, 0, dev_info.devs, 0, KEY_NUM);
                }
            }
            return false;
        }

        case RM_HUEU:
        case RM_HUED:
        case RM_VALU:
        case RM_VALD:
        case RM_SPDU:
        case RM_SPDD:
        case RM_SATU:
        case RM_SATD:
            if (no_indicator_under_srgb) {
                return false;
            }
            break;

        case KC_RWIN:
        case KC_APP:
            if (record->event.pressed) {
                if (keymap_config.no_gui) return false;
            }
            break;

        case VIA_WEB:
            if (record->event.pressed) {
                start_web(WEB_VIA);
            }
            return false;

        case GOOGLE_WEB:
            if (record->event.pressed) {
                start_web(WEB_GOOGLE);
            }
            return false;
        case M_CALC:
            if (record->event.pressed) {
                start_web(WEB_CALC);
            }
            return false;
        case M_BROWSER:
            if (record->event.pressed) {
                start_web(WEB_BROWSER);
            }
            return false;

        default:
            break;
    }

#ifdef MULTIMODE_ENABLE
    if (!bt_process_record(keycode, record)) {
        return false;
    }
#endif

    return true;
}

void keyboard_post_init_user(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
        // sled_mode_before_charge = SLED_MODE_VOL;
    }

    bled_init();
}

void eeconfig_init_user(void) {
    bled_eeconfig_init();

    keymap_config.nkro = 1;
    eeconfig_update_keymap(&keymap_config);
}

bool rgb_matrix_indicators_user(void) {
    if (!rgb_matrix_get_flags()) {
        // if (!rgb_matrix_get_flags() || backlight_sleep_flag) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    return true;
}

bool show_chrg      = false;
bool show_chrg_full = false;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // if (!backlight_sleep_flag && rgb_matrix_get_flags()) {
    // if (!backlight_sleep_flag) {
    extern bool Low_power;
    if (!Low_power) bled_task();
    if (!show_chrg && !show_chrg_full) sled_task();
    // }

#ifdef MULTIMODE_ENABLE
    if (!bt_indicators_advanced(led_min, led_max)) {
        return false;
    }
#endif

    return true;
}

#ifdef WWDG_ENABLE
extern void wb32_wwdg_start(void);
extern void wb32_wwdg_stop(void);

void suspend_power_down_user(void) {
    wb32_wwdg_stop();
}

void suspend_wakeup_init_user(void) {
    wb32_wwdg_start();
}
#endif

void housekeeping_task_user(void) {
    via_web_task();

#ifdef MULTIMODE_ENABLE
    extern void housekeeping_task_bt(void);
    housekeeping_task_bt();
#endif

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (usb_suspend) {
            bool wakeup = false;
            for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
                if (matrix_get_row(r)) {
                    wakeup = true;
                    break;
                }
            }
            if (wakeup) {
                // usbWakeupHost(&USB_DRIVER);
                // restart_usb_driver(&USB_DRIVER);
                usb_suspend       = false;
                usb_suspend_timer = 0;
#    ifdef RGB_MATRIX_SHUTDOWN_PIN
                gpio_write_pin_high(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
                rgb_matrix_init();
            }
        }

        if ((USB_DRIVER.state != USB_ACTIVE) || (USB_DRIVER.state == USB_SUSPENDED)) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
#    ifdef RGB_MATRIX_SHUTDOWN_PIN
                    gpio_write_pin_low(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
                }

                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;

#    ifdef RGB_MATRIX_SHUTDOWN_PIN
                gpio_write_pin_high(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
                rgb_matrix_init();
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
#    ifdef RGB_MATRIX_SHUTDOWN_PIN
            gpio_write_pin_high(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
            rgb_matrix_init();
        }
    }
#endif

    static uint32_t chrg_check_time = 0;
    extern void     Charge_Chat(void);
    if (timer_elapsed32(chrg_check_time) >= 2) {
        chrg_check_time = timer_read32();
        Charge_Chat();
    }
}

void matrix_scan_user(void) {
#ifdef MULTIMODE_ENABLE
    bt_task();
#endif
}

void matrix_init_user(void) {
#ifdef RGB_MATRIX_SHUTDOWN_PIN
    gpio_set_pin_output_push_pull(RGB_MATRIX_SHUTDOWN_PIN);
    gpio_write_pin_low(RGB_MATRIX_SHUTDOWN_PIN);
    wait_ms(10);
    gpio_write_pin_high(RGB_MATRIX_SHUTDOWN_PIN);
#endif

#ifdef MULTIMODE_ENABLE
    bt_init();
    led_config_all();
#endif
}

#if defined(MM_CABLE_PIN) && defined(MM_CHARGE_PIN)
static uint8_t  rChr_ChkBuf  = 0;
static uint8_t  rChr_OldBuf  = 0;
static uint16_t rChr_Cnt     = 0;
static uint8_t  f_ChargeOn   = 0;
static uint8_t  f_ChargeFull = 0;

#    define CHR_DEBOUNCE 25

void Charge_Chat(void) {
    uint8_t i = 0;

    if (USBLINK_Status == 0) i |= 0x01;
    if ((CHARGE_Status == 1) || ((dev_info.devs != DEVS_USB) && (bts_info.bt_info.pvol >= 100))) i |= 0x02;
    // if ((USBLINK_Status == 0) && ((CHARGE_Status == 0) || (CHARGE_Status == 1))) i |= 0x01;
    // if ((USBLINK_Status == 0) && (CHARGE_Status == 1 || bts_info.bt_info.pvol >= 100)) i |= 0x02;
    // if (USBLINK_Status == 0 && CHARGE_Status == 0) i |= 0x01;
    // if ((USBLINK_Status == 0) && (CHARGE_Status == 1 || bts_info.bt_info.pvol >= 100)) i |= 0x02;

    if (rChr_ChkBuf != i) {
        rChr_Cnt    = CHR_DEBOUNCE;
        rChr_ChkBuf = i;
    } else {
        if (rChr_Cnt != 0) {
            rChr_Cnt--;
            if (rChr_Cnt == 0) {
                i = rChr_ChkBuf ^ rChr_OldBuf;

                if (i != 0) {
                    rChr_OldBuf = rChr_ChkBuf;

                    if (i & 0x3) {
                        f_ChargeOn   = (rChr_ChkBuf & 0x01) ? 1 : 0;
                        f_ChargeFull = (rChr_ChkBuf & 0x02) ? 1 : 0;
                    }
                }
            }
        }
    }
}

bool is_charging(void) {
    return f_ChargeOn;
}

bool is_fully_charged(void) {
    return f_ChargeFull;
}
#endif
