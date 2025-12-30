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
#include "bled/bled.h"
#include "lib/lib8tion/lib8tion.h"

#ifdef USB_SUSPEND_CHECK_ENABLE
#    include "usb_main.h"
#endif

enum _layers {
    WIN_BASE,
    WIN_FN,
    AND_BASE,
    AND_FN,
    MAC_BASE,
    MAC_FN,
};

#define KC_TASK G(KC_TAB)
#define AN_HOME G(KC_H)
#define KC_SER G(KC_SPC)
// #define MAC_TSK LWIN(KC_TAB)
#define KEY_WIN PDF(0)
#define KEY_AND PDF(2)
#define KEY_MAC PDF(4)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             WORK_MOD,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             MO(1),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_MYCM,  KC_MAIL, KC_WHOM, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            _______,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, _______,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  KC_INS,
        RM_HUEU, KEY_AND,  _______,  KEY_MAC,  _______,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  RM_NEXT,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        BLED_MOD,          _______, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, GU_TOGG, _______,                              _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),

    [AND_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_BRID,  KC_BRIU,  KC_TASK,  AN_HOME,  RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            WORK_MOD,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             MO(3),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [AND_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             _______,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, _______,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  _______,
        RM_HUEU, _______,  KEY_WIN,  KEY_MAC,  _______,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  RM_NEXT,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        BLED_MOD,          _______, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, _______, _______,                              _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),

    [MAC_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_SER,   KC_F5,   KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            WORK_MOD,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             MO(5),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             _______,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  RM_TOGG,  _______,
        RM_HUEU, KEY_AND,  KEY_WIN,  _______,  _______,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  RM_NEXT,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        BLED_MOD,          _______, _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, _______, _______,                              _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = {ENCODER_CCW_CW(_______, _______) },
    [AND_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [AND_FN]   = {ENCODER_CCW_CW(_______, _______) },
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = {ENCODER_CCW_CW(_______, _______) },
};
#endif // ENCODER_MAP_ENABLE
// clang-format on

// Blink effect related
static uint8_t  single_blink_cnt     = 0;
static uint8_t  single_blink_index   = 0;
static RGB      single_blink_color   = {0};
static uint32_t single_blink_time    = 0;
static uint8_t  double_blink_cnt     = 0;
static uint8_t  double_blink_index[] = {0, 0};
static RGB      double_blink_color[] = {{0}, {0}};
static uint32_t double_blink_time    = 0;

enum blink_index {
    LWIN_INDEX,
    CAPS_INDEX,
};

static uint32_t GUI_pressed_time = 0;

#define MODE_ROW 1
#define MODE_COLUMN 14

static bool     mode_long_pressed_flag = false;
static uint32_t mode_long_pressed_time = 0;
static uint32_t caps_blink_time        = 0;
static uint8_t  caps_blink_cnt         = 0;

// bool process_rgb_matrix_user(uint16_t keycode, keyrecord_t *record) {
//     if (record->event.pressed) {
//         switch (keycode) {
//             // case RGB_TOG:
//             //     rgb_matrix_toggle();
//             //     return false;
//             case RGB_MOD:
//                 rgb_matrix_step();
//                 return false;
//             case RGB_HUI:
//                 rgb_matrix_increase_hue();
//                 return false;
//             case RGB_HUD:
//                 rgb_matrix_decrease_hue();
//                 return false;
//             case RGB_SAI:
//                 rgb_matrix_increase_sat();
//                 return false;
//             case RGB_SAD:
//                 rgb_matrix_decrease_sat();
//                 return false;
//             case RGB_VAI:
//                 rgb_matrix_increase_val();
//                 return false;
//             case RGB_VAD:
//                 rgb_matrix_decrease_val();
//             case RGB_SPI:
//                 rgb_matrix_increase_speed();
//                 return false;
//             case RGB_SPD:
//                 rgb_matrix_decrease_speed();
//                 return false;
//         }
//     }

//     return true;
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // if (!process_rgb_matrix_user(keycode, record)) return false;
    // extern bool process_record_kb_dg(uint16_t keycode, keyrecord_t *record);
    // if (!process_record_kb_dg(keycode, record)) return false;

#ifdef MULTIMODE_ENABLE
    if (!bt_process_record(keycode, record)) {
        return false;
    }
#endif

    switch (keycode) {
        case KEY_WIN:
            if (record->event.pressed) {
                single_blink_cnt   = 6;
                single_blink_index = LED_W_INDEX;
                single_blink_color = (RGB){RGB_RED};
                if (timer_elapsed32(single_blink_time) >= 500) {
                    single_blink_time = timer_read32();
                }
            }
            break;

        case KEY_AND:
            if (record->event.pressed) {
                if (keymap_config.no_gui) {
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config);
                }
                single_blink_cnt   = 6;
                single_blink_index = LED_Q_INDEX;
                single_blink_color = (RGB){RGB_RED};
                if (timer_elapsed32(single_blink_time) >= 500) {
                    single_blink_time = timer_read32();
                }
            }
            break;

        case KEY_MAC:
            if (record->event.pressed) {
                if (keymap_config.no_gui) {
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config);
                }
                single_blink_cnt   = 6;
                single_blink_index = LED_E_INDEX;
                single_blink_color = (RGB){RGB_RED};
                if (timer_elapsed32(single_blink_time) >= 500) {
                    single_blink_time = timer_read32();
                }
            }
            break;

        case RM_VALU:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    caps_blink_cnt  = 6;
                    caps_blink_time = timer_read32();
                }
            }
            break;
        case RM_VALD:
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    caps_blink_cnt  = 6;
                    caps_blink_time = timer_read32();
                }
            }
            break;
        case RM_SPDU:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    caps_blink_cnt  = 6;
                    caps_blink_time = timer_read32();
                }
            }
            break;
        case RM_SPDD:
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    caps_blink_cnt  = 6;
                    caps_blink_time = timer_read32();
                }
            }
            break;

        case MAC_TSK: {
            if (record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_TAB);
                GUI_pressed_time = 0; // Reset timer when pressed
            } else {
                unregister_code(KC_TAB);
                GUI_pressed_time = timer_read32(); // Start timer when released
            }
            return false;
        }

        case BLED_MOD: {
            if (record->event.pressed) {
                dev_info.bled_mode = (dev_info.bled_mode + 1) % BLED_MODE_COUNT;
                eeconfig_update_user(dev_info.raw);
            }
            return false;
        }

        case BLED_VAI: {
            if (record->event.pressed) {
                if (dev_info.bled_val == BLED_MAX_BRIGHTNESS) {
                    dev_info.bled_val = 0;
                } else {
                    dev_info.bled_val = qadd8(dev_info.bled_val, BLED_BRIGHTNESS_STEP);
                }
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }

        case BLED_SPI: {
            if (record->event.pressed) {
                if (dev_info.bled_speed == UINT8_MAX) {
                    dev_info.bled_speed = 0;
                } else {
                    dev_info.bled_speed = qadd8(dev_info.bled_speed, RGB_MATRIX_SPD_STEP);
                }
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }

        case BLED_HUI: {
            if (record->event.pressed) {
                dev_info.bled_color = (dev_info.bled_color == COLOR_WHITE) ? COLOR_RAINBOW : (dev_info.bled_color + 1);
                eeconfig_update_kb(dev_info.raw);
            }
            return false;
        }

        case WORK_MOD: {
            if (record->event.pressed) {
                mode_long_pressed_time = timer_read32();
            } else {
                mode_long_pressed_time = 0;
                if (mode_long_pressed_flag) {
                    mode_long_pressed_flag = false;
                } else {
                    if (dev_info.encoder_mode == MODE_WORKING) {
                        tap_code(KC_MUTE);
                    } else {
                        rgb_matrix_step();
                    }
                }
            }
            return false;
        }

        case KC_FLXP: {
            if (record->event.pressed) {
                if (dev_info.devs == DEVS_USB) {
                    register_code(KC_LWIN);
                    register_code(KC_E);
                } else {
                    bts_process_keys(KC_LWIN, 1, dev_info.devs, 0);
                    bts_process_keys(KC_E, 1, dev_info.devs, 0);
                }
            } else {
                if (dev_info.devs == DEVS_USB) {
                    unregister_code(KC_E);
                    unregister_code(KC_LWIN);
                } else {
                    bts_process_keys(KC_E, 0, dev_info.devs, 0);
                    bts_process_keys(KC_LWIN, 0, dev_info.devs, 0);
                }
            }
            return false;
        }

        case KC_LEFT:
        case KC_RIGHT:
        case KC_UP:
        case KC_DOWN:
        case KC_TAB:
            // Reset GUI timer if user is still navigating
            if (GUI_pressed_time) {
                GUI_pressed_time = timer_read32();
            }
            break;

        default:
            break;
    }

    return true;
}

void keyboard_pre_init_user(void) {
    // #ifdef RGB_MATRIX_SHUTDOWN_PIN
    //     setPinOutputPushPull(RGB_MATRIX_SHUTDOWN_PIN);
    //     writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
    // #endif
}

void keyboard_post_init_user(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }

    bled_init();

    extern void snled27351_reset(void);
    snled27351_reset();
}

void eeconfig_init_user(void) {
    dev_info.encoder_mode = MODE_WORKING;
    eeconfig_update_user(dev_info.raw);

    bled_eeconfig_init();
}

void suspend_power_down_kb(void) {
    led_deconfig_all();
}

void suspend_wakeup_init_kb(void) {
    led_config_all();
}

// extern bool kb_light_sleep_flag;

bool rgb_matrix_indicators_user(void) {
    if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color_all(RGB_OFF);
    }

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    static bool f_caps_blink = false;

    if (rgb_matrix_get_flags()) {
        bled_task();
    }

#ifdef MULTIMODE_ENABLE
    if (!bt_indicators_advanced(led_min, led_max)) {
        return false;
    }
#endif

    if (keymap_config.no_gui) {
        rgb_matrix_set_color(72, 0xC8, 0xC8, 0xC8);
    }

    if (caps_blink_cnt) {
        if (timer_elapsed32(caps_blink_time) >= 300) {
            caps_blink_time = timer_read32();
            caps_blink_cnt--;
            if (caps_blink_cnt == 0) {
                f_caps_blink = true;
            }
        }
        if (caps_blink_cnt % 0x2) {
            gpio_write_pin(LED_CAPS_LOCK_PIN, 1);
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, 0);
        }
    } else {
        // Sync caps lock LED with host state after blinking finishes
        if (f_caps_blink) {
            f_caps_blink = false;
            gpio_write_pin(LED_CAPS_LOCK_PIN, host_keyboard_led_state().caps_lock);
        }
    }

    if (single_blink_cnt) {
        if (timer_elapsed32(single_blink_time) > 500) {
            single_blink_time = timer_read32();
            single_blink_cnt--;
        }
        if (single_blink_cnt % 2) {
            rgb_matrix_set_color(single_blink_index, single_blink_color.r, single_blink_color.g, single_blink_color.b);
        } else {
            rgb_matrix_set_color(single_blink_index, RGB_OFF);
        }
    }

    if (double_blink_cnt) {
        if (timer_elapsed32(double_blink_time) > 500) {
            double_blink_time = timer_read32();
            double_blink_cnt--;
        }
        if (double_blink_cnt % 2) {
            for (uint8_t i = 0; i < sizeof(double_blink_index) / sizeof(double_blink_index[0]); i++) {
                rgb_matrix_set_color(double_blink_index[i], double_blink_color[i].r, double_blink_color[i].g, double_blink_color[i].b);
            }
        } else {
            for (uint8_t i = 0; i < sizeof(double_blink_index) / sizeof(double_blink_index[0]); i++) {
                rgb_matrix_set_color(double_blink_index[i], RGB_OFF);
            }
        }
    }

    return true;
}

void housekeeping_task_user(void) {
    if (GUI_pressed_time && (timer_elapsed32(GUI_pressed_time) >= 500)) {
        GUI_pressed_time = 0;
        unregister_code(KC_LGUI);
    }

#ifdef MULTIMODE_ENABLE
    bt_housekeeping_task();
#endif

    if (mode_long_pressed_time && (timer_elapsed32(mode_long_pressed_time) >= 3000)) {
        mode_long_pressed_time = 0;
        mode_long_pressed_flag = true;
        if (dev_info.encoder_mode == MODE_WORKING) {
            dev_info.encoder_mode = MODE_GAMING;
            eeconfig_update_user(dev_info.raw);

            for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer += 2) {
                //     if (dynamic_keymap_get_encoder(layer, 0, true) != RGB_VAI || dynamic_keymap_get_encoder(layer, 0, false) != RGB_VAD) {
                dynamic_keymap_set_encoder(layer, 0, true, RM_VALU);
                dynamic_keymap_set_encoder(layer, 0, false, RM_VALD);
                //     }
                //     if (dynamic_keymap_get_keycode(layer, MODE_ROW, MODE_COLUMN) != RGB_MOD) {
                //         dynamic_keymap_set_keycode(layer, MODE_ROW, MODE_COLUMN, RGB_MOD);
                //     }
            }
            double_blink_cnt               = 6;
            double_blink_time              = timer_read32();
            double_blink_index[LWIN_INDEX] = LED_LWIN_INDEX;
            double_blink_index[CAPS_INDEX] = LED_CAPS_INDEX;
            double_blink_color[LWIN_INDEX] = (RGB){RGB_GREEN};
            double_blink_color[CAPS_INDEX] = (RGB){RGB_GREEN};
        } else {
            dev_info.encoder_mode = MODE_WORKING;
            eeconfig_update_user(dev_info.raw);

            for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer += 2) {
                //     if (dynamic_keymap_get_encoder(layer, 0, true) != KC_VOLU || dynamic_keymap_get_encoder(layer, 0, false) != KC_VOLD) {
                dynamic_keymap_set_encoder(layer, 0, true, KC_VOLU);
                dynamic_keymap_set_encoder(layer, 0, false, KC_VOLD);
                //     }
                //     if (dynamic_keymap_get_keycode(layer, MODE_ROW, MODE_COLUMN) != KC_MUTE) {
                //         dynamic_keymap_set_keycode(layer, MODE_ROW, MODE_COLUMN, KC_MUTE);
                //     }
            }
            double_blink_cnt               = 6;
            double_blink_time              = timer_read32();
            double_blink_index[LWIN_INDEX] = LED_LWIN_INDEX;
            double_blink_index[CAPS_INDEX] = LED_CAPS_INDEX;
            double_blink_color[LWIN_INDEX] = (RGB){RGB_BLUE};
            double_blink_color[CAPS_INDEX] = (RGB){RGB_BLUE};
        }
    }

#ifdef USB_SUSPEND_CHECK_ENABLE
    static uint32_t usb_suspend_timer = 0;
    static uint32_t usb_suspend       = false;

    if (dev_info.devs == DEVS_USB) {
        if (USB_DRIVER.state != USB_ACTIVE || USB_DRIVER.state == USB_SUSPENDED) {
            if (!usb_suspend_timer) {
                usb_suspend_timer = timer_read32();
            } else if (timer_elapsed32(usb_suspend_timer) > 10000) {
                if (!usb_suspend) {
                    usb_suspend = true;
                    led_deconfig_all();
                }
                usb_suspend_timer = 0;
            }
        } else {
            if (usb_suspend_timer) {
                usb_suspend_timer = 0;
                if (usb_suspend) {
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

void matrix_scan_user(void) {
#ifdef MULTIMODE_ENABLE
    bt_task();
#endif
}

void matrix_init_user(void) {
    extern bool enable_dog;
    enable_dog = true;

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
