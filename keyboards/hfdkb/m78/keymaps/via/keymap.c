/* Copyright (C) 2023 jonylee@hfd
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
#include "common/bt_task.h"
#include "bled/bled.h"

#define SW_SLEP SLEEP_TOGGLE
#define KC_SPOT MAC_Spotlight
#define KC_DICT MAC_Dictation
#define KC_DND MAC_Do_Not_Disturb
#define BL_NEXT BLED_MODE_NEXT

#define KC_FLEX LGUI(KC_TAB)
#define KC_DESK LGUI(KC_D)
#define KC_EXAK LCTL(KC_DOWN)

/* 恢复出厂设置相关 */
#define KEY_PRESS_FN (0x1 << 0)
#define KEY_PRESS_Y (0x1 << 1)
#define KEY_PRESS_J (0x1 << 2)
#define FN_KEY1 MO(1)
#define FN_KEY2 MO(3)
#define F_RESET_KEY1 KC_Y
#define F_RESET_KEY2 KC_J
#define KEY_PRESS_FACTORY_RESET (KEY_PRESS_FN | KEY_PRESS_Y | KEY_PRESS_J)

uint16_t key_press_status    = 0;
uint32_t timer_3s_buffer     = 0;
uint32_t timer_300ms_buffer  = 0;
uint8_t  factory_reset_count = 0;

enum __layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN,
};
// clang-format off

 const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_82_ansi( /* Base */
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,  KC_INS,   KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS,  KC_DEL,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,   KC_END,  KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT,  KC_UP,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             KC_RALT, MO(WIN_FN), KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT_82_ansi( /* FN */
        _______, KC_BRID,  KC_BRIU,  KC_FLEX,  KC_DESK,  KC_WBAK, KC_WSCH, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,    KC_VOLU, KEY_VIA, _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,    _______, _______,
        RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI, _______, _______, _______, _______, _______, _______,    _______, BL_NEXT, _______, _______,
        _______, RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD, _______, _______, _______, _______, _______, _______,             _______, _______, _______,
        _______,           _______,  _______,  SW_OS,    _______, BT_VOL,  _______, SW_SLEP, _______, _______, _______,             _______, _______,
        _______, GU_TOGG,  _______,                               _______,                            _______, _______,    _______, _______, _______, _______),

    [MAC_B] = LAYOUT_82_ansi( /* Base */
        KC_ESC,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  KC_EXAK, KC_WSCH, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,    KC_VOLU, KC_INS,   KC_MUTE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS,  KC_DEL,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,   KC_END,  KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT,  KC_UP,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             KC_RCMD, MO(MAC_FN), KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT_82_ansi( /* mac fn */
        _______, KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,  _______, _______,
        _______, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,    _______, _______,
        RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI, _______, _______, _______, _______, _______, _______,    _______, BL_NEXT, _______, _______,
        _______, RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD, _______, _______, _______, _______, _______, _______,             _______, _______, _______,
        _______,           _______,  SW_OS,    _______,  _______, BT_VOL,  _______, SW_SLEP, _______, _______, _______,             _______, _______,
        _______, _______,  _______,                               _______,                            _______, _______,    _______, _______, _______, _______),

 };

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_B]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN] = {ENCODER_CCW_CW(_______, _______) },
    [MAC_B]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN] = {ENCODER_CCW_CW(_______, _______) }
};
#endif // ENCODER_MAP_ENABLE

// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#if defined(FN_KEY1) || defined(FN_KEY2)
#    ifdef FN_KEY1
        case FN_KEY1: /* fall through */
#    endif
#    ifdef FN_KEY2
        case FN_KEY2:
#    endif
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_FN;
            } else {
                key_press_status &= ~KEY_PRESS_FN;
                timer_3s_buffer = 0;
            }
            return true;
#endif
        case F_RESET_KEY1:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_Y;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_Y;
                timer_3s_buffer = 0;
            }
            return true;
        case F_RESET_KEY2:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_J;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_J;
                timer_3s_buffer = 0;
            }
            return true;

        case BL_NEXT: {
            if (record->event.pressed) {
                bled_mode_next();
            }
        } break;
        case KEY_VIA: {
            if (record->event.pressed) {
                tap_code16_delay(G(KC_R), 50);
                send_string_with_delay("https://usevia.app", 10);
                tap_code(KC_ENTER);
            }
        } break;
        default:
            break;
    }
    return true;
}

static void execute_factory_reset(void) {
    // 开始复位动画
    factory_reset_count = 1;
    timer_300ms_buffer  = timer_read32();

    // 保存当前默认层
    // layer_state_t default_layer = default_layer_state;

    // 执行复位
    eeconfig_init();
    set_single_persistent_default_layer(WIN_B);

    // 重置键盘配置
    keymap_config.no_gui = 0;
    eeconfig_update_keymap(&keymap_config);

    dev_info.bled_mode = 0;
    eeconfig_update_user(dev_info.raw);

#ifdef RGB_MATRIX_ENABLE
    // 重新初始化 RGB
    if (!rgb_matrix_is_enabled()) {
        rgb_matrix_enable();
    }
    rgb_matrix_init();
#endif

    // 清除蓝牙配对
    if (dev_info.devs > DEVS_USB && dev_info.devs < DEVS_2_4G) {
        bts_send_vendor(v_clear);
        wait_ms(1000);
        // 重置状态指示器
        extern uint32_t          last_total_time;
        extern indicator_state_t indicator_status;
        last_total_time  = timer_read32();
        indicator_status = INDICATOR_CONNECTING;
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // 工厂复位动画
    if (factory_reset_count) {
        if (rgb_matrix_get_mode() == RGB_MATRIX_SOLID_COLOR) {
            if (factory_reset_count % 2) {
                rgb_matrix_sethsv_noeeprom(HSV_RED);
            } else {
                rgb_matrix_sethsv_noeeprom(HSV_OFF);
            }
        } else {
            RGB color = (factory_reset_count % 2) ? (RGB){255, 0, 0} : (RGB){0, 0, 0};
            for (uint8_t i = led_min; i <= led_max; i++) {
                rgb_matrix_set_color(i, color.r, color.g, color.b);
            }
        }
    }

    // Caps Lock 指示
    if (host_keyboard_led_state().caps_lock && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, 100, 100, 100);
    }

    // GUI Lock 指示
    if (keymap_config.no_gui && ((bts_info.bt_info.paired) || (dev_info.devs == DEVS_USB))) {
        rgb_matrix_set_color(GUI_LOCK_LED_INDEX, 100, 0, 0);
    }

    return true;
}

void matrix_scan_user(void) {
    // 工厂复位 3 秒检测
    if (timer_3s_buffer && timer_elapsed32(timer_3s_buffer) > 3000) {
        timer_3s_buffer = 0;
        if (key_press_status == KEY_PRESS_FACTORY_RESET) {
            execute_factory_reset();
        }
        key_press_status = 0; // 超时后清除状态
    }

    // 工厂复位动画
    if (factory_reset_count && timer_300ms_buffer) {
        if (timer_elapsed32(timer_300ms_buffer) > 300) {
            factory_reset_count++;
            if (factory_reset_count > 7) { // 闪烁 6 次后停止
                factory_reset_count = 0;
                timer_300ms_buffer  = 0;
            } else {
                timer_300ms_buffer = timer_read32();
            }
        }
    }
}
