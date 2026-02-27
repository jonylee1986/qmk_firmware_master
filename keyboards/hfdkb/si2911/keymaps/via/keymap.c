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

enum _layers {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

#define MAC_TSK C(KC_UP)
#define MAC_SEH G(KC_SPACE)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_100_ansi(
        KC_ESC,            KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   KC_DEL,   KC_HOME, KC_END,   KC_PAUS,  KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_NUM,  KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,  KC_P7,   KC_P8,    KC_P9,    KC_PPLS,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,  KC_P4,   KC_P5,    KC_P6,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,              KC_P1,   KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             KC_RALT, MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,             KC_PDOT),

    [WIN_FN] = LAYOUT_100_ansi(
        EE_CLR,            KC_BRID,  KC_BRIU,  WIN_TSK,  KC_MYCM, KC_MAIL, KC_WHOM, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  KC_PSCR, KC_SCRL,  _______,  _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  _______,  _______, _______,  _______,  _______,
        RGB_TEST,_______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_NEXT,  _______,  _______, _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            RM_HUEU,  _______,  _______, _______,  _______,
        BLED_MOD,          BLED_HUI, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, SLED_HUI, SLED_MOD, RM_VALU,            _______, _______,  _______,  _______,
        BLED_SPI,GU_TOGG,  BLED_VAI,                              SLED_SPI,                           SLED_VAI,_______,  RM_SPDD,  RM_VALD,  RM_SPDU,  _______,           _______),

    [MAC_BASE] = LAYOUT_100_ansi(
        KC_ESC,            KC_BRID,  KC_BRIU,  MAC_TSK,  MAC_SEH, KC_F5,   KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_DEL,   KC_HOME, KC_END,   KC_PAUS,  KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_NUM,  KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,  KC_P7,   KC_P8,    KC_P9,    KC_PPLS,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,  KC_P4,   KC_P5,    KC_P6,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,              KC_P1,   KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             KC_RCMD, MO(3),    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,             KC_PDOT),

    [MAC_FN] = LAYOUT_100_ansi(
        EE_CLR,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,  KC_PSCR, KC_SCRL,  _______,  _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  _______,  _______, _______,  _______,  _______,
        RGB_TEST,_______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_NEXT,  _______,  _______, _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            RM_HUEU,  _______,  _______, _______,  _______,
        BLED_MOD,          BLED_HUI, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, SLED_HUI, SLED_MOD, RM_VALU,            _______, _______,  _______,  _______,
        BLED_SPI,_______,  BLED_VAI,                              SLED_SPI,                           SLED_VAI,_______,  RM_SPDD,  RM_VALD,  RM_SPDU,  _______,           _______),
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

#define MODE_ROW 3
#define MODE_COLUMN 18

// uint8_t sled_mode_before_charge = SLED_MODE_VOL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef MULTIMODE_ENABLE
    if (!bt_process_record(keycode, record)) {
        return false;
    }
#endif

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
                    bts_process_keys(KC_LWIN, 1, dev_info.devs, 0);
                    bts_process_keys(KC_TAB, 1, dev_info.devs, 0);
                }
            } else {
                if (dev_info.devs == DEVS_USB) {
                    unregister_code(KC_TAB);
                    unregister_code(KC_LWIN);
                } else {
                    bts_process_keys(KC_TAB, 0, dev_info.devs, 0);
                    bts_process_keys(KC_LWIN, 0, dev_info.devs, 0);
                }
            }
            return false;
        }

        default:
            break;
    }

    return true;
}

void keyboard_post_init_user(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
        // sled_mode_before_charge = SLED_MODE_VOL;
    }

    bled_init();

    // extern void snled27351_reset(void);
    // snled27351_reset();
}

void eeconfig_init_user(void) {
    bled_eeconfig_init();
}

bool rgb_matrix_indicators_user(void) {
    if (!rgb_matrix_get_flags() || backlight_sleep_flag) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    return true;
}

bool show_chrg      = false;
bool show_chrg_full = false;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (!backlight_sleep_flag && rgb_matrix_get_flags()) {
        bled_task();
        if (!show_chrg && !show_chrg_full) sled_task();
    }

#ifdef MULTIMODE_ENABLE
    if (!bt_indicators_advanced(led_min, led_max)) {
        return false;
    }
#endif

    return true;
}

// void suspend_power_down_user(void) {
//     wwdg_disable();
// }
// void suspend_wakeup_init_user(void) {
// wwdg_enable();
// }

void housekeeping_task_user(void) {
    static uint32_t chrg_check_time = 0;

#ifdef MULTIMODE_ENABLE
    bt_housekeeping_task();
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
                writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
            }
        }

        if ((USB_DRIVER.state != USB_ACTIVE) || (USB_DRIVER.state == USB_SUSPENDED)) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
#    ifdef RGB_MATRIX_SHUTDOWN_PIN
                    writePinLow(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
                }

                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend) {
                usb_suspend_timer = 0;
                usb_suspend       = false;

#    ifdef RGB_MATRIX_SHUTDOWN_PIN
                writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
            }
        }
    } else {
        if (usb_suspend) {
            usb_suspend_timer = 0;
            usb_suspend       = false;
#    ifdef RGB_MATRIX_SHUTDOWN_PIN
            writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
#    endif
        }
    }
#endif

    extern void Charge_Chat(void);
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
    // wwdg_enable();

#ifdef RGB_MATRIX_SHUTDOWN_PIN
    setPinOutputPushPull(RGB_MATRIX_SHUTDOWN_PIN);
    writePinLow(RGB_MATRIX_SHUTDOWN_PIN);
    wait_ms(10);
    writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
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

#    define CHR_DEBOUNCE 100

void Charge_Chat(void) {
    uint8_t i = 0;

    if (USBLINK_Status == 0 && CHARGE_Status == 0) i |= 0x01;
    if ((USBLINK_Status == 0) && (CHARGE_Status == 1 || bts_info.bt_info.pvol >= 100)) i |= 0x02;

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

                    if (i) {
                        f_ChargeOn = (rChr_ChkBuf & 0x01) ? 1 : 0;

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
