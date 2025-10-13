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
#include "lib/lib8tion/lib8tion.h"

enum _layers {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

enum _custom_keycodes {
    SW_OS = QK_KB_0,
    BL_PREV,
    BL_NEXT,
    BL_HUEU,
    FK_KEY1,
    FK_KEY2,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_67_ansi(
        KC_ESC,  KC_1,     KC_2,    KC_3,     KC_4,     KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,    KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,    KC_D,     KC_F,     KC_G,     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,    KC_X,     KC_C,     KC_V,     KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             MO(WIN_FN), KC_RCTL,KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_67_ansi(
        KC_GRV,  KC_F1,    KC_F2,   KC_F3,    KC_F4,    KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   _______,   RM_TOGG,
        FK_KEY2, _______,  _______, _______,  _______,  KC_CALC,  KC_WHOM, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLU,  KC_VOLD,  _______,   RM_SPDU,
        _______, _______,  _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,            _______,   RM_SPDD,
        _______,           _______, _______,  _______,  _______,  _______, _______, SW_OS,   _______, _______, _______,  _______,  RM_VALU,   BL_HUEU,
        _______, GU_TOGG,  _______,                               FK_KEY1,                            _______, _______,  BL_PREV,  RM_VALD,   BL_NEXT),

    [MAC_BASE] = LAYOUT_67_ansi(
        KC_ESC,  KC_1,     KC_2,    KC_3,     KC_4,     KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,    KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,    KC_D,     KC_F,     KC_G,     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,    KC_X,     KC_C,     KC_V,     KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             MO(MAC_FN), KC_RCTL,KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_67_ansi(
        KC_GRV,  KC_F1,    KC_F2,   KC_F3,    KC_F4,    KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   _______,   RM_TOGG,
        FK_KEY2, KC_BRID,  KC_BRIU, KC_MCTL,  G(KC_SPC),_______,  _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLU,  KC_VOLD,  _______,   RM_SPDU,
        _______, _______,  SW_OS,   _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,            _______,   RM_SPDD,
        _______,           _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______,  _______,  RM_VALU,   BL_HUEU,
        _______, _______,  _______,                               FK_KEY1,                            _______, _______,  BL_PREV,  RM_VALD,   BL_NEXT),
};

// clang-format on

/* Factory reset */
#define KEY_PRESS_FN (0x1 << 0)
#define KEY_PRESS_K1 (0x1 << 1)
#define KEY_PRESS_K2 (0x1 << 2)
#define FN_KEY1 MO(WIN_FN)
#define FN_KEY2 MO(MAC_FN)
#define F_RESET_KEY1 FK_KEY1
#define F_RESET_KEY2 FK_KEY2
#define KEY_PRESS_FACTORY_RESET (KEY_PRESS_FN | KEY_PRESS_K1 | KEY_PRESS_K2)
#define IND_WHITE 0xA0, 0xA0, 0xA0

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

typedef union {
    uint32_t raw;
    struct {
        uint8_t RGB_effect; // Mode
        uint8_t RGB_color;  // Color
    };
} keyboard_config_t;

keyboard_config_t keyboard_config;

enum BL_Effect {
    FLOWING_EFFECT = 0,
    NEON_EFFECT,
    BREATH_EFFECT,
    SOLID_EFFECT,
    OFF_EFFECT,
};

enum BL_Color {
    COLOR_RAINBOW = 0,
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_WHITE,
};

uint8_t color_table[][3] = {
    {RGB_RED}, {RGB_YELLOW}, {RGB_GREEN}, {RGB_CYAN}, {RGB_BLUE}, {RGB_PURPLE}, {0xA0, 0xA0, 0xA0},
};

// HSV hue values for breath and other HSV-based effects
uint8_t hue_table[] = {
    0,   // Red
    43,  // Yellow
    85,  // Green
    128, // Cyan
    170, // Blue
    213, // Purple
    0,   // White (use red hue, low saturation will make it white-ish)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RM_VALU: {
            if (record->event.pressed) {
                if (rgb_matrix_get_val() >= (RGB_MATRIX_MAXIMUM_BRIGHTNESS - RGB_MATRIX_VAL_STEP)) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){IND_WHITE};
                }
            }
            break;
        }

        case RM_VALD: {
            if (record->event.pressed) {
                if (rgb_matrix_get_val() <= RGB_MATRIX_VAL_STEP) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){IND_WHITE};
                }
            }
            break;
        }

        case RM_SPDU: {
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() >= (UINT8_MAX - RGB_MATRIX_SPD_STEP)) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){IND_WHITE};
                }
            }
            break;
        }

        case RM_SPDD: {
            if (record->event.pressed) {
                if (rgb_matrix_get_speed() <= RGB_MATRIX_SPD_STEP) {
                    all_blink_cnt   = 2;
                    all_blink_time  = timer_read32();
                    all_blink_color = (RGB){IND_WHITE};
                }
            }
            break;
        }

        case BL_HUEU: {
            if (record->event.pressed) {
                keyboard_config.RGB_color++;
                if (keyboard_config.RGB_color > COLOR_WHITE) {
                    keyboard_config.RGB_color = COLOR_RAINBOW;
                }
                eeconfig_update_user(keyboard_config.raw);
            }
            return false;
        }

        case BL_NEXT: {
            if (record->event.pressed) {
                keyboard_config.RGB_effect++;
                if (keyboard_config.RGB_effect > OFF_EFFECT) {
                    keyboard_config.RGB_effect = FLOWING_EFFECT;
                }
                eeconfig_update_user(keyboard_config.raw);
            }
            return false;
        }

        case BL_PREV: {
            if (record->event.pressed) {
                if (keyboard_config.RGB_effect == 0) {
                    keyboard_config.RGB_effect = OFF_EFFECT;
                } else {
                    keyboard_config.RGB_effect--;
                }
                eeconfig_update_user(keyboard_config.raw);
            }
            return false;
        }

        case RM_TOGG: {
            if (record->event.pressed) {
                if (record->event.pressed) {
                    switch (rgb_matrix_get_flags()) {
                        case LED_FLAG_ALL: {
                            rgb_matrix_set_flags(LED_FLAG_NONE);
                            // rgb_matrix_set_color_all(0, 0, 0);
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
            }
        }

        case SW_OS: {
            if (record->event.pressed) {
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
                key_press_status |= KEY_PRESS_K1;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_K1;
                timer_3s_buffer = 0;
            }
        }
            return false;

        case F_RESET_KEY2: {
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_K2;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_K2;
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
    if (rgb_matrix_get_flags()) {
        extern void RGB_effect_user(uint8_t effect, uint8_t color);
        RGB_effect_user(keyboard_config.RGB_effect, keyboard_config.RGB_color);
    } else {
        rgb_matrix_set_color_all(0x00, 0x00, 0x00);
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
    fr_blink_color      = (RGB){IND_WHITE};
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
                    all_blink_color = (RGB){IND_WHITE};
                } else if (get_highest_layer(default_layer_state) == MAC_BASE) { // Mac_BASE
                    set_single_persistent_default_layer(WIN_BASE);
                    all_blink_time  = timer_read32();
                    all_blink_cnt   = 2;
                    all_blink_color = (RGB){IND_WHITE};
                }
            } break;

            default:
                break;
        }
    }
}

void keyboard_post_init_user(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
    keyboard_config.raw = eeconfig_read_user();
}

void eeconfig_init_user(void) {
    keyboard_config.raw        = 0;
    keyboard_config.RGB_effect = FLOWING_EFFECT;
    keyboard_config.RGB_color  = COLOR_RAINBOW;
    eeconfig_update_user(keyboard_config.raw);
}

// Custom backlight effect

#define NEON_EFFCT_CONSTANT(time)       \
    (HSV) {                             \
        time, 255, rgb_matrix_get_val() \
    }

void RGB_neon_effect_user(void) {
    uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_get_speed() / 4, 1));
    for (uint8_t i = 0; i < 30; i++) {
        rgb_matrix_set_color(i, hsv_to_rgb(NEON_EFFCT_CONSTANT(time)).r, hsv_to_rgb(NEON_EFFCT_CONSTANT(time)).g, hsv_to_rgb(NEON_EFFCT_CONSTANT(time)).b);
    }
}

void RGB_off_effect_user(void) {
    for (uint8_t i = 0; i < 30; i++) {
        rgb_matrix_set_color(i, 0, 0, 0); /* code */
    }
}

#define FLOWING_EFFCT_CONSTANT(i, time)                           \
    (HSV) {                                                       \
        g_led_config.point[i].x - time, 255, rgb_matrix_get_val() \
    }
void RGB_flowing_effect_user(void) {
    uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_get_speed() / 4, 1));
    for (uint8_t i = 0; i < 30; i++) {
        rgb_matrix_set_color(i, hsv_to_rgb(FLOWING_EFFCT_CONSTANT(i, time)).r, hsv_to_rgb(FLOWING_EFFCT_CONSTANT(i, time)).g, hsv_to_rgb(FLOWING_EFFCT_CONSTANT(i, time)).b);
    }
}

#define BREATH_EFFCT_CONSTANT(time) scale8(abs8(sin8(time / 2) - 128) * 2, rgb_matrix_get_val())

void RGB_breath_effect_user(uint8_t color) {
    uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_get_speed() / 4, 1));
    for (uint8_t i = 0; i < 30; i++) {
        if (keyboard_config.RGB_color == COLOR_RAINBOW) {
            // Calculate hue for smooth rainbow distribution across all 30 LEDs
            uint8_t hue = (i * 161) % 255; // 161 ≈ 255 * golden_ratio_fraction
            rgb_matrix_set_color(i, hsv_to_rgb((HSV){hue, 255, BREATH_EFFCT_CONSTANT(time)}).r, hsv_to_rgb((HSV){hue, 255, BREATH_EFFCT_CONSTANT(time)}).g, hsv_to_rgb((HSV){hue, 255, BREATH_EFFCT_CONSTANT(time)}).b);
        } else {
            // Use the hue table for solid colors
            uint8_t hue = hue_table[color - 1];
            uint8_t sat = (color == COLOR_WHITE) ? 0 : 255; // White needs low saturation
            rgb_matrix_set_color(i, hsv_to_rgb((HSV){hue, sat, BREATH_EFFCT_CONSTANT(time)}).r, hsv_to_rgb((HSV){hue, sat, BREATH_EFFCT_CONSTANT(time)}).g, hsv_to_rgb((HSV){hue, sat, BREATH_EFFCT_CONSTANT(time)}).b);
        }
    }
}

#define SOLID_EFFCT_CONSTANT rgb_matrix_get_val() / RGB_MATRIX_MAXIMUM_BRIGHTNESS

void RGB_solid_effect_user(uint8_t color) {
    for (uint8_t i = 0; i < 30; i++) {
        if (keyboard_config.RGB_color == COLOR_RAINBOW) {
            // Calculate hue for smooth rainbow distribution across all 30 LEDs
            // 255 / 30 = 8.5, so we use (i * 256) / 30 for better precision
            // uint8_t hue = g_led_config.point[i].x;
            // uint8_t hue = (i * 256) / 30;
            // uint8_t hue = (i * 255) / 29; // Uses 29 as denominator since i goes 0-29
            // uint8_t hue = (i * 17) % 255; // Creates about 1.5 full rainbows across 30 LEDs
            uint8_t hue = (i * 161) % 255; // 161 ≈ 255 * golden_ratio_fraction
            rgb_matrix_set_color(i, hsv_to_rgb((HSV){hue, 255, rgb_matrix_get_val()}).r, hsv_to_rgb((HSV){hue, 255, rgb_matrix_get_val()}).g, hsv_to_rgb((HSV){hue, 255, rgb_matrix_get_val()}).b);
        } else {
            rgb_matrix_set_color(i, color_table[color - 1][0] * SOLID_EFFCT_CONSTANT, color_table[color - 1][1] * SOLID_EFFCT_CONSTANT, color_table[color - 1][2] * SOLID_EFFCT_CONSTANT); /* code */
        }
    }
}

void RGB_effect_user(uint8_t effect, uint8_t color) {
    switch (effect) {
        case FLOWING_EFFECT:
            RGB_flowing_effect_user();
            break;
        case BREATH_EFFECT:
            RGB_breath_effect_user(color);
            break;
        case SOLID_EFFECT:
            RGB_solid_effect_user(color);
            break;
        case NEON_EFFECT:
            RGB_neon_effect_user();
            break;
        case OFF_EFFECT:
            RGB_off_effect_user();
            break;
        default:
            break;
    }
}
