/* Copyright (C) 2025 @ HFD (https://www.hfdic.com/)
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
#include "os_detection.h"
#include "common/bt_task.h"

enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN,
};

#define BT1 BT_HOST1
#define BT2 BT_HOST2
#define BT3 BT_HOST3
#define BT4 BT_2_4G
#define SW_OS OS_SWITCH

#define KC_TASK G(KC_TAB)
#define KC_FILE G(KC_E)

#define KEY_FLASK_PRESSED 0x01
#define KEY_M_PRESSED 0x02
#define KEY_W_PRESSED 0x04
#define KEY_B_PRESSED 0x08
#define KEY_L_PRESSED 0x10
#define KEY_1_PRESSED 0x20
#define KEY_2_PRESSED 0x40
#define KEY_3_PRESSED 0x80
#define KEY_ESC_PRESSED 0x100

#define FUN_MAC_OS_MODE (KEY_FLASK_PRESSED | KEY_M_PRESSED)
#define FUN_WIN_OS_MODE (KEY_FLASK_PRESSED | KEY_W_PRESSED)
#define FUN_BAT_LEVEL_DETECT (KEY_FLASK_PRESSED | KEY_B_PRESSED)
#define FUN_LOCK_SCREEN (KEY_FLASK_PRESSED | KEY_L_PRESSED)
#define FUN_BT1_MODE (KEY_FLASK_PRESSED | KEY_1_PRESSED)
#define FUN_BT2_MODE (KEY_FLASK_PRESSED | KEY_2_PRESSED)
#define FUN_BT3_MODE (KEY_FLASK_PRESSED | KEY_3_PRESSED)
#define FUN_FACTORY_RESET (KEY_FLASK_PRESSED | KEY_ESC_PRESSED)

static uint32_t key_press_status = 0;

// static uint8_t  all_blink_cnt      = 0;
// static uint32_t all_blink_time     = 0;
// static RGB      all_blink_color    = {0};
extern uint8_t  single_blink_cnt;
extern uint8_t  single_blink_index;
extern RGB      single_blink_color;
extern uint32_t single_blink_time;
extern bool     query_vol_flag;
extern uint32_t EE_CLR_press_cnt;
extern uint32_t EE_CLR_press_time;
extern bool     EE_CLR_flag;

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_B] = LAYOUT_ansi_84( /* Base */
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,  KC_PSCR, KC_DEL,    KC_FLASK,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,           KC_BSPC,   KC_HOME,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC,          KC_BSLS,   KC_END,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,    KC_NUHS,          KC_ENT,    KC_PGUP,
        KC_LSFT, KC_NUBS,  KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, KC_UP,     KC_PGDN,
        KC_LCTL, KC_LWIN,  KC_LALT,                              KC_SPC,                             KC_RALT, MO(WIN_FN), KC_RCTL, KC_LEFT, KC_DOWN,   KC_RGHT ),

    [WIN_FN] = LAYOUT_ansi_84( /* FN */
        _______, KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,    KC_VOLU, _______, _______,  _______,
        _______, BT1,      BT2,      BT3,      _______, _______, _______, _______, _______, _______, _______, _______,    _______,          _______,  _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______,          _______,  _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______,          _______,  _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,  _______,
        _______, GU_TOGG,  _______,                              _______,                            _______, _______,    _______, _______, _______,  _______),

    [MAC_B] = LAYOUT_ansi_84( /* Base */
        KC_ESC,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,    KC_VOLU, KC_PSCR, KC_DEL,    KC_FLASK,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,           KC_BSPC,   KC_HOME,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC,          KC_BSLS,   KC_END,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,    KC_NUHS,          KC_ENT,    KC_PGUP,
        KC_LSFT, KC_NUBS,  KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, KC_UP,     KC_PGDN,
        KC_LCTL, KC_LOPT,  KC_LCMD,                              KC_SPC,                             KC_RCMD, MO(MAC_FN), KC_RCTL, KC_LEFT, KC_DOWN,   KC_RGHT ),

    [MAC_FN] = LAYOUT_ansi_84( /* FN */
        _______, KC_F1,    KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,  _______, _______,  _______,
        _______, BT1,      BT2,      BT3,      _______, _______, _______, _______, _______, _______, _______, _______,    _______,          _______,  _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______,          _______,  _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,    _______,          _______,  _______,
        _______, _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,  _______,
        _______, _______,  _______,                              _______,                            _______, _______,    _______, _______, _______,  _______),
};

// clang-format on
bool process_detected_host_os_user(os_variant_t detected_os) {
    if (detected_os == OS_MACOS || detected_os == OS_IOS) {
        set_single_default_layer(MAC_B);
    } else {
        set_single_default_layer(WIN_B);
    }

    return true;
}

// static bool flask_held = false;
typedef struct {
    uint16_t physical_key;
    uint16_t task_key;
    uint32_t status_bit;
    uint8_t  host;
    uint8_t  led;
    RGB      color;
} flask_action_t;

static const flask_action_t flask_actions[] = {
    {
        .physical_key = KC_1,
        .task_key     = BT_HOST1,
        .status_bit   = KEY_1_PRESSED,
        .host         = DEVS_HOST1,
        .led          = BT1_LED_INDEX,
        .color        = BT1_LED_COLOR,
    },
    {
        .physical_key = KC_2,
        .task_key     = BT_HOST2,
        .status_bit   = KEY_2_PRESSED,
        .host         = DEVS_HOST2,
        .led          = BT2_LED_INDEX,
        .color        = BT2_LED_COLOR,
    },
    {
        .physical_key = KC_3,
        .task_key     = BT_HOST3,
        .status_bit   = KEY_3_PRESSED,
        .host         = DEVS_HOST3,
        .led          = BT3_LED_INDEX,
        .color        = BT3_LED_COLOR,
    },
    {
        .physical_key = KC_ESC,
        .task_key     = EE_CLR,
        .status_bit   = KEY_ESC_PRESSED,
    },
};

// 哪些按键的本次按下已被 FLASK 组合消费。
static uint32_t flask_consumed = 0;

// 最近一次 FLASK 组合所对应的任务。
static uint16_t flask_task = KC_NO;

static uint8_t brightness = 0;

static void flask_cancel_task(void) {
    if (flask_task != KC_NO) {
        long_press_cancel(flask_task, LONG_PRESS_SOURCE_FLASK);
        flask_task = KC_NO;
    }
}

static bool process_flask_long_press(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_FLASK) {
        if (record->event.pressed) {
            key_press_status |= KEY_FLASK_PRESSED;

            if (!brightness) brightness = rgb_matrix_get_val();

            rgb_matrix_config.hsv.v = brightness / 5;
        } else {
            key_press_status &= ~KEY_FLASK_PRESSED;
            flask_cancel_task();

            rgb_matrix_config.hsv.v = brightness;
            brightness              = 0;
        }
        return false;
    }

    for (uint8_t i = 0; i < sizeof(flask_actions) / sizeof(flask_actions[0]); i++) {
        const flask_action_t *action = &flask_actions[i];

        if (keycode != action->physical_key) {
            continue;
        }

        if (record->event.pressed) {
            key_press_status |= action->status_bit;

            // 普通数字键/ESC 保持原来的行为。
            if (!(key_press_status & KEY_FLASK_PRESSED)) {
                return true;
            }

            flask_consumed |= action->status_bit;

            // 切换组合目标时，取消此前的 FLASK 长按任务。
            flask_cancel_task();

            if (action->task_key != EE_CLR) {
                // 保留原来的硬件模式限制：
                // 开关不允许时不切换，也不启动对码。
                if (gpio_read_pin(BT_MODE_SW_PIN)) {
                    return false;
                }

                if (dev_info.devs != action->host) {
                    bt_switch_mode(dev_info.devs, action->host, false);

                    single_blink_cnt   = 6;
                    single_blink_index = action->led;
                    single_blink_color = action->color;
                    single_blink_time  = timer_read32();
                }
            }

            flask_task = action->task_key;
            long_press_start(flask_task, LONG_PRESS_SOURCE_FLASK);
            return false;
        }

        key_press_status &= ~action->status_bit;

        if (flask_consumed & action->status_bit) {
            flask_consumed &= ~action->status_bit;

            // 旧目标释放，不能取消新目标的计时。
            if (flask_task == action->task_key) {
                flask_cancel_task();
            }

            return false;
        }

        return true;
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // if (keycode == KC_FLASK) {
    //     flask_held = record->event.pressed;
    //     return false; // Flask alone sends nothing
    // }

    // if (flask_held && record->event.pressed) {
    //     switch (keycode) {
    //         case KC_0:
    //             layer_move(WIN_B);
    //             return false;
    //         case KC_1:
    //             layer_move(WIN_FN);
    //             return false;
    //         case KC_2:
    //             layer_move(MAC_B);
    //             return false;
    //         case KC_3:
    //             layer_move(MAC_FN);
    //             return false;
    //     }
    // }
    if (!process_flask_long_press(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_M:
            if (record->event.pressed) {
                key_press_status |= KEY_M_PRESSED;
                if (key_press_status == FUN_MAC_OS_MODE) {
                    if (get_highest_layer(default_layer_state) == 0) { // WIN_BASE
                        set_single_persistent_default_layer(2);
                        if (keymap_config.no_gui) {
                            keymap_config.no_gui = false;
                        }
                        eeconfig_update_keymap(&keymap_config);
                        // single_blink_cnt   = 6;
                        // single_blink_color = (RGB){0xFF / 3, 0xFF, 0xFF};
                        // if (timer_elapsed32(single_blink_time) >= 300) {
                        //     single_blink_time = timer_read32();
                        // }
                        // single_blink_index = GUI_LOCK_LED_INDEX;
                        return false;
                    }
                }
            } else {
                key_press_status &= ~KEY_M_PRESSED;
            }
            return true;

        case KC_W:
            if (record->event.pressed) {
                key_press_status |= KEY_W_PRESSED;
                if (key_press_status == FUN_WIN_OS_MODE) {
                    set_single_persistent_default_layer(0);
                    return false;
                }
            } else {
                key_press_status &= ~KEY_W_PRESSED;
            }
            return true;

        case KC_B:
            if (record->event.pressed) {
                key_press_status |= KEY_B_PRESSED;
                if (key_press_status == FUN_BAT_LEVEL_DETECT) {
                    bts_send_vendor(v_query_vol);
                    query_vol_flag = true;
                    return false;
                }
            } else {
                key_press_status &= ~KEY_B_PRESSED;
                query_vol_flag = false;
            }
            return true;

        case KC_L:
            if (record->event.pressed) {
                key_press_status |= KEY_L_PRESSED;
                if (key_press_status == FUN_LOCK_SCREEN) {
                    if (get_highest_layer(default_layer_state) == 0) {
                        tap_code16_delay(G(KC_L), 10);
                    } else {
                        tap_code16_delay(G(C(KC_Q)), 10);
                    }
                    return false;
                }
            } else {
                key_press_status &= ~KEY_L_PRESSED;
            }
            return true;

        default:
            break;
    }

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    static bool     key_highlight_on   = true;
    static bool     highlight_active   = false;
    static uint32_t key_highlight_time = 0;

    bool active = (key_press_status & KEY_FLASK_PRESSED) != 0;
    if (!active) {
        highlight_active = false;
        return true;
    }

    if (!highlight_active) {
        highlight_active   = true;
        key_highlight_on   = true;
        key_highlight_time = timer_read32();
    } else if (timer_elapsed32(key_highlight_time) >= 500) {
        key_highlight_time = timer_read32();
        key_highlight_on   = !key_highlight_on;
    }

    uint8_t leds[2];
    uint8_t led_count = 0;

    switch (dev_info.devs) {
        case DEVS_HOST1:
            leds[led_count++] = BT1_LED_INDEX;
            break;
        case DEVS_HOST2:
            leds[led_count++] = BT2_LED_INDEX;
            break;
        case DEVS_HOST3:
            leds[led_count++] = BT3_LED_INDEX;
            break;
        case DEVS_USB:
            break;
        default:
            return true;
    }

    leds[led_count++] = get_highest_layer(default_layer_state) == MAC_B ? 69 : 33;

    static const uint8_t indicator_leds[] = {17, 18, 19, 33, 55, 67, 69};
    for (uint8_t i = 0; i < sizeof(indicator_leds) / sizeof(indicator_leds[0]); i++) {
        uint8_t led = indicator_leds[i];
        if (dev_info.devs == DEVS_USB && led == 67) {
            continue;
        }
        if (led >= led_min && led < led_max) {
            bool should_blink = false;
            for (uint8_t j = 0; j < led_count; j++) {
                if (led == leds[j]) {
                    should_blink = true;
                    break;
                }
            }

            if (should_blink && !key_highlight_on) {
                rgb_matrix_set_color(led, 0, 0, 0);
            } else {
                rgb_matrix_set_color(led, 0xFF / 3, 0xFF, 0xFF);
            }
        }
    }

    return true;
}
