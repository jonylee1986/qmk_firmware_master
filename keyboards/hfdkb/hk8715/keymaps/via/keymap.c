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

#include "bled/bled.h"
#include "lib/lib8tion/lib8tion.h"

enum _layers {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

enum _custom_keycodes {
    SW_OS = BLED_Keycodes_End,
    F_KEY1,
    F_KEY2,
};

#define BL_NEXT BLED_Mode_Next
#define BL_VALU BLED_Brightness_Increase
#define BL_VALD BLED_Brightness_Decrease
#define BL_SPDU BLED_Speed_Increase
#define BL_SPDD BLED_Speed_Decrease
#define BL_HUEU BLED_Color_Increase

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_86_ansi(
        KC_ESC,            KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,    KC_F11,   KC_F12,   KC_PSCR,  KC_SCRL,  KC_PAUS,
        KC_GRV,  KC_1,     KC_2,    KC_3,     KC_4,     KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,  KC_Q,     KC_W,    KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS, KC_A,     KC_S,    KC_D,     KC_F,     KC_G,     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,
        KC_LSFT,           KC_Z,    KC_X,     KC_C,     KC_V,     KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,                      KC_UP,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             MO(WIN_FN), MO(WIN_FN),KC_RCTL,            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_86_ansi(
        F_KEY2,            KC_MYCM, KC_MAIL,  KC_WSCH,  KC_WHOM,  KC_CALC, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,   _______,  _______,  _______,
        _______, _______,  _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,  _______,  _______,   BL_NEXT,  BL_HUEU,  _______,
        _______, _______,  _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,  _______,  _______,   _______,  _______,  _______,
        _______, _______,  _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,            _______,
        _______,           _______, _______,  _______,  _______,  _______, _______, SW_OS,   _______, _______, _______,  _______,                       BL_VALU,
        _______, GU_TOGG,  _______,                               F_KEY1,                              _______, _______,  _______,             BL_SPDD,  BL_VALD,  BL_SPDU),

    [MAC_BASE] = LAYOUT_86_ansi(
        KC_ESC,            KC_BRID, KC_BRIU,  KC_MCTL,  G(KC_SPC),KC_NO,   KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,   KC_VOLD,  KC_VOLU,  KC_PSCR,  KC_SCRL,  KC_PAUS,
        KC_GRV,  KC_1,     KC_2,    KC_3,     KC_4,     KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,  KC_Q,     KC_W,    KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS, KC_A,     KC_S,    KC_D,     KC_F,     KC_G,     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,
        KC_LSFT,           KC_Z,    KC_X,     KC_C,     KC_V,     KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,                      KC_UP,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             MO(MAC_FN), MO(MAC_FN),KC_RCTL,            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_86_ansi(
        F_KEY2,            KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,    KC_F11,   KC_F12,   _______,  _______,  _______,
        _______, _______,  _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,   _______,  _______,  BL_NEXT,  BL_HUEU,  _______,
        _______, _______,  SW_OS,   _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,   _______,  _______,  _______,  _______,  _______,
        _______, _______,  _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,             _______,
        _______,           _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,   _______,                      BL_VALU,
        _______, _______,  _______,                               F_KEY1,                              _______, _______,   _______,            BL_SPDD,  BL_VALD,  BL_SPDU),
};
// clang-format on

/* Factory reset */
#define KEY_PRESS_FN (0x1 << 0)
#define KEY_PRESS_J (0x1 << 1)
#define KEY_PRESS_Z (0x1 << 2)
#define FN_KEY1 MO(WIN_FN)
#define FN_KEY2 MO(MAC_FN)
#define F_RESET_KEY1 F_KEY1
#define F_RESET_KEY2 F_KEY2
#define KEY_PRESS_FACTORY_RESET (KEY_PRESS_FN | KEY_PRESS_J | KEY_PRESS_Z)

#define COLOR_WHITE 0xC8, 0xC8, 0xC8

static uint16_t key_press_status     = 0;
static uint32_t timer_3s_buffer      = 0;
static uint32_t timer_300ms_buffer   = 0;
static uint8_t  factory_reset_count  = 0;
static RGB      fr_blink_color       = {0};
static uint8_t  all_blink_cnt        = 0;
static uint32_t all_blink_time       = 0;
static RGB      all_blink_color      = {0};
static uint32_t long_pressed_time    = 0;
static uint16_t long_pressed_keycode = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BL_NEXT: {
            if (record->event.pressed) {
                bled_info.bled_mode++;
                if (bled_info.bled_mode >= BLED_MODE_END) {
                    bled_info.bled_mode = BLED_MODE_CYCLE;
                }
                eeconfig_update_user(bled_info.raw);
            }
            return false;
        }

        case BL_VALU: {
            if (record->event.pressed) {
                if (bled_info.bled_Brightness >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){COLOR_WHITE};
                }
                bled_info.bled_Brightness = qadd8(bled_info.bled_Brightness, RGB_MATRIX_VAL_STEP);
                eeconfig_update_user(bled_info.raw);
            }
            return false;
        }

        case BL_VALD: {
            if (record->event.pressed) {
                if (bled_info.bled_Brightness <= RGB_MATRIX_VAL_STEP) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){COLOR_WHITE};
                }
                bled_info.bled_Brightness = qsub8(bled_info.bled_Brightness, RGB_MATRIX_VAL_STEP);
                eeconfig_update_user(bled_info.raw);
            }
            return false;
        }

        case BL_SPDU: {
            if (record->event.pressed) {
                if (bled_info.bled_speed >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){COLOR_WHITE};
                }
                bled_info.bled_speed = qadd8(bled_info.bled_speed, RGB_MATRIX_SPD_STEP);
                eeconfig_update_user(bled_info.raw);
            }
            return false;
        }

        case BL_SPDD: {
            if (record->event.pressed) {
                if (bled_info.bled_speed <= RGB_MATRIX_SPD_STEP) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){COLOR_WHITE};
                }
                bled_info.bled_speed = qsub8(bled_info.bled_speed, RGB_MATRIX_SPD_STEP);
                eeconfig_update_user(bled_info.raw);
            }
            return false;
        }

        case BL_HUEU: {
            if (record->event.pressed) {
                if (bled_info.bled_mode == BLED_MODE_SOLID || bled_info.bled_mode == BLED_MODE_BREATHING) {
                    bled_info.bled_color++;
                    if (bled_info.bled_color >= COLOR_COUNT) {
                        bled_info.bled_color = COLOR_RAINBOW;
                    }
                }
                eeconfig_update_user(bled_info.raw);
            }
            return false;
        }

        case SW_OS: {
            if (record->event.pressed) {
                // Do something when SW_OS1 is pressed
                long_pressed_time    = timer_read32();
                long_pressed_keycode = SW_OS;
            } else {
                long_pressed_time = 0;
            }
        }
            return false;

#if defined(FN_KEY1) || defined(FN_KEY2)
#    ifdef FN_KEY1
        case FN_KEY1: /* fall through */
#    endif
#    ifdef FN_KEY2
        case FN_KEY2:
#    endif
        {
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_FN;
            } else {
                key_press_status &= ~KEY_PRESS_FN;
                timer_3s_buffer = 0;
            }
        } break;

#endif
        case F_RESET_KEY1: {
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_J;
                // if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                //     timer_3s_buffer = timer_read32();
                // }
            } else {
                key_press_status &= ~KEY_PRESS_J;
                timer_3s_buffer = 0;
            }
        }
            return false;

        case F_RESET_KEY2: {
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_Z;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_Z;
                timer_3s_buffer = 0;
            }
        }
            return false;

        default:
            break;
    }

    return true;
}

bool rgb_matrix_indicators_user(void) {
    if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    bled_task();

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color_all(0x64, 0x64, 0x64);
    }

    if (all_blink_cnt) { // 全键闪烁
        rgb_matrix_set_color_all(0, 0, 0);
        if (timer_elapsed32(all_blink_time) > 500) {
            all_blink_time = timer_read32();
            all_blink_cnt--;
        }
        if (all_blink_cnt & 0x1) {
            rgb_matrix_set_color_all(all_blink_color.r, all_blink_color.g, all_blink_color.b);
        }
    }

    if (factory_reset_count) {
        rgb_matrix_set_color_all(0, 0, 0);
        if (timer_elapsed32(timer_300ms_buffer) > 500) {
            timer_300ms_buffer = timer_read32();
            factory_reset_count--;
        }
        if (factory_reset_count & 0x1) {
            rgb_matrix_set_color_all(fr_blink_color.r, fr_blink_color.g, fr_blink_color.b);
        }
    }

    return true;
}

static void execute_factory_reset(void) {
    key_press_status = 0;

    // Start animation
    factory_reset_count = 6;
    fr_blink_color      = (RGB){RGB_WHITE};
    timer_300ms_buffer  = timer_read32();

    // Reset all settings
    eeconfig_init();
    keymap_config.no_gui = 0;
}

void housekeeping_task_user(void) {
    if (timer_3s_buffer && (timer_elapsed32(timer_3s_buffer) > 1000)) {
        timer_3s_buffer = 0;
        if (key_press_status == KEY_PRESS_FACTORY_RESET) {
            execute_factory_reset();
        }
    }
    if (long_pressed_time && (timer_elapsed32(long_pressed_time) > 1000)) {
        long_pressed_time = 0;
        switch (long_pressed_keycode) {
            case SW_OS: {
                if (get_highest_layer(default_layer_state) == WIN_BASE) { // WIN_BASE
                    set_single_persistent_default_layer(MAC_BASE);
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config); // Write default value to EEPROM now
                    all_blink_time  = timer_read32();
                    all_blink_cnt   = 2;
                    all_blink_color = (RGB){COLOR_WHITE};
                } else if (get_highest_layer(default_layer_state) == MAC_BASE) { // Mac_BASE
                    set_single_persistent_default_layer(WIN_BASE);
                    all_blink_time  = timer_read32();
                    all_blink_cnt   = 2;
                    all_blink_color = (RGB){COLOR_WHITE};
                }
            } break;

            default:
                break;
        }
    }
}

void keyboard_post_init_user(void) {
    if (keymap_config.no_gui) {
        // Disable GUI key
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
    bled_info.raw = eeconfig_read_user();
}

void eeconfig_init_user(void) {
    bled_init();
}
