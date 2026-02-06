// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "common/bt_task.h"
#include "lib/lib8tion/lib8tion.h" //声明函数SIN8

enum layers {
    WIN_B,
    WIN_FN,
    MAC_B,
    MAC_FN,
};
// LOGO LED
typedef enum {
    ENC_LED_EFFECT_BREATHING, // 呼吸灯效
    ENC_LED_EFFECT_SOLID,     // 常亮灯效
    ENC_LED_OFF               // 关闭
} enc_led_effect_t;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_B] = LAYOUT_96_ansi(
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_END,  KC_HOME, KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_P4,   KC_P5,   KC_P6,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                                      MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_P0,   KC_PDOT
    ),

    [WIN_FN] = LAYOUT_96_ansi(
        EE_CLR,           KC_BRID, KC_BRIU,G(KC_TAB),KC_MYCM, KC_MAIL, KC_WHOM, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_INS,  KC_PSCR, KC_SCRL, BLED_MOD,
        _______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, _______, _______, _______, _______, _______, _______, _______, _______, RM_TOGG,          _______, _______, _______, _______,
        RGB_WHI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_NEXT,          _______, _______, _______, _______,
        _______, _______, SW_OS1,  _______, _______, _______, _______, _______, _______, _______, _______, _______,          RM_HUEU,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_PGUP, KC_PGDN, _______,                   _______, RM_VALU, _______, _______, _______, _______,
        _______, GU_TOGG, _______,                            _______,                                     _______, BT_VOL,  RM_SPDD, RM_VALD, RM_SPDU, _______, _______
    ),

    [MAC_B] = LAYOUT_96_ansi(
        KC_ESC,           KC_BRID, KC_BRIU, C(KC_UP),KC_WSCH, KC_F5,   KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,  KC_END,  KC_HOME, KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_P4,   KC_P5,   KC_P6,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                                      MO(3),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_P0,   KC_PDOT
    ),

    [MAC_FN] = LAYOUT_96_ansi(
        EE_CLR,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_PSCR, KC_SCRL, BLED_MOD,
        _______, BT_HOST1,BT_HOST2,BT_HOST3,BT_2_4G, _______, _______, _______, _______, _______, _______, _______, _______, RM_TOGG,          _______, _______, _______, _______,
        RGB_WHI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_NEXT,          _______, _______, _______, _______,
        _______, SW_OS1,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RM_HUEU,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_PGUP, KC_PGDN, _______,                   _______, RM_VALU, _______, _______, _______, _______,
        _______, _______, _______,                            _______,                                     _______, BT_VOL,  RM_SPDD, RM_VALD, RM_SPDU, _______, _______
    ),

};
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [WIN_B]  = {ENCODER_CCW_CW(ENC_CMSD, ENC_CMSI)},
    [WIN_FN] = {ENCODER_CCW_CW(ENC_CMSD, ENC_CMSI)},
    [MAC_B]  = {ENCODER_CCW_CW(ENC_CMSD, ENC_CMSI)},
    [MAC_FN] = {ENCODER_CCW_CW(ENC_CMSD, ENC_CMSI)},
};
#endif
// clang-format on

// bool     ENC_FUN_flag   = 0;
uint16_t       ENC_press_time = 0;
bool           OS_blink       = 0;
uint16_t       OS_blink_time  = 0;
uint32_t       OS_press_time  = 0;
static uint8_t OS_blink_index = 100;
bool           process_record_user(uint16_t keycode, keyrecord_t *record) {
    // if (!process_record_keychron(keycode, record)) {
    //     return false;
    // }
    switch (keycode) {
        case ENC_CMSP: {
            if (record->event.pressed) {
                if (dynamic_keymap_get_keycode(0, 3, 18) == RM_NEXT) { // 获取某层键值
                    dynamic_keymap_set_keycode(0, 3, 18, KC_MUTE);
                    dynamic_keymap_set_keycode(2, 3, 18, KC_MUTE);
                } else {
                    dynamic_keymap_set_keycode(0, 3, 18, RM_NEXT);
                    dynamic_keymap_set_keycode(2, 3, 18, RM_NEXT);
                }
                dev_info.ENC_FUN_flag = !dev_info.ENC_FUN_flag;
            }
        }
        case ENC_CMSI: { // 顺时针
            if (!dev_info.ENC_FUN_flag) {
                // enc_led_effect = ENC_LED_EFFECT_SOLID;  // 切换到常亮
                if (dev_info.devs) {
                    bts_process_keys(KC_VOLU, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_VOLU);
                    } else {
                        unregister_code(KC_VOLU);
                    }
                }
            } else {
                // enc_led_effect = ENC_LED_EFFECT_BREATHING;  // 切换到呼吸
                // rgb_matrix_increase_val();          //亮度+
                if (record->event.pressed) {
                    if (rgb_matrix_get_val() < RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                        rgb_matrix_increase_val();
                    }
                }
            }
            break;
        }
        case ENC_CMSD: { // 逆时针
            if (!dev_info.ENC_FUN_flag) {
                // enc_led_effect = ENC_LED_EFFECT_SOLID;  // 切换到常亮
                if (dev_info.devs) {
                    bts_process_keys(KC_VOLD, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_VOLD);
                    } else {
                        unregister_code(KC_VOLD);
                    }
                }
            } else {
                //  enc_led_effect = ENC_LED_EFFECT_BREATHING;  // 切换到呼吸
                // rgb_matrix_decrease_val();
                if (record->event.pressed) {
                    if (rgb_matrix_get_val() > 0) {
                        rgb_matrix_decrease_val();
                    }
                }
            }
            break;
        }
        case SW_OS1:
            if (record->event.pressed) {
                if (get_highest_layer(default_layer_state) == 0) { // MAC_BASE
                    set_single_persistent_default_layer(2);
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config);
                    OS_press_time  = timer_read32();
                    OS_blink_index = 56;

                } else if (get_highest_layer(default_layer_state) == 2) { // WIN_BASE
                    set_single_persistent_default_layer(0);
                    OS_press_time  = timer_read32();
                    OS_blink_index = 55;
                }
            }
            break;
        case RGB_WHI:
            if (record->event.pressed) {
                dev_info.rgb_white_light = !dev_info.rgb_white_light;
                eeconfig_update_user(dev_info.raw);
            }
            break;
        case BLED_MOD:
            if (record->event.pressed) {
                dev_info.enc_led_effect = (dev_info.enc_led_effect + 1) % 3; // 0→1→2→0...
                // rgb_matrix_update_pwm_buffers();
                eeconfig_update_user(dev_info.raw);
                return false; // 跳过默认处理
            }

        case G(KC_TAB):
            if (keymap_config.no_gui) {
                if (dev_info.devs) {
                    bts_process_keys(KC_LGUI, record->event.pressed, dev_info.devs, false);
                    bts_process_keys(KC_TAB, record->event.pressed, dev_info.devs, keymap_config.no_gui);
                } else {
                    if (record->event.pressed) {
                        register_code(KC_LGUI);
                        register_code(KC_TAB);
                    } else {
                        unregister_code(KC_TAB);
                        unregister_code(KC_LGUI);
                    }
                }
                return false;
            }
            return true;

        case RM_NEXT:
            if (record->event.pressed) {
                dev_info.rgb_white_light = 0; // 切灯效就关闭白光
            }
            return true; // 继续处理其他
        default:
            return true;
    }
    return false;
}
uint16_t ENC_FUN_blink_time;
uint8_t  ENC_FUN_blink_cnt;

void housekeeping_task_user(void) {
    if (ENC_press_time && timer_elapsed(ENC_press_time) >= 3000) {
        dev_info.ENC_FUN_flag = !dev_info.ENC_FUN_flag;
        ENC_FUN_blink_time    = timer_read();
        ENC_FUN_blink_cnt     = 7;
    }
}

extern bool low_vol_off;
extern bool EE_CLR_flag;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    static uint8_t breath_step = 0;
    // 呼吸灯效
    if (!low_vol_off && !EE_CLR_flag) {
        if (dev_info.enc_led_effect == ENC_LED_EFFECT_BREATHING) {
            uint8_t brightness = (sin8(breath_step) + 1) / 2; // 将sin值映射到0-255
            n_rgb_matrix_set_color_all(brightness, brightness, brightness);
            breath_step += 1; // 调整步长以控制呼吸速度
        }
        // 常亮效果
        else if (dev_info.enc_led_effect == ENC_LED_EFFECT_SOLID) {
            n_rgb_matrix_set_color_all(100, 100, 100);
        } else {
            n_rgb_matrix_set_color_all(0, 0, 0);
        }
    }

    // 层切换闪烁
    if (timer_elapsed32(OS_press_time) < 1800) {  // 是否已经经过了1.8S
        if (timer_elapsed(OS_blink_time) > 300) { // 从 OS_blink_time 开始计时，是否已经经过了 300 毫秒（0.3 秒）
            OS_blink      = !OS_blink;
            OS_blink_time = timer_read(); // 更新 OS_blink_time 为当前时间，用于下一次计时
        }
        if (OS_blink) {
            rgb_matrix_set_color(OS_blink_index, 60, 0, 0);
        } else {
            rgb_matrix_set_color(OS_blink_index, 0, 0, 0);
        }
    }
    if (ENC_FUN_blink_cnt) {
        if (ENC_FUN_blink_cnt & 2) {
            // rgb_matrix_set_color(44, 0, ENC_FUN_flag?100:0, ENC_FUN_flag?0:100);
            // rgb_matrix_set_color(73, 0, ENC_FUN_flag?100:0, ENC_FUN_flag?0:100);
        } else {
            // rgb_matrix_set_color(44, 0, 0, 0);
            // rgb_matrix_set_color(73, 0, 0, 0);
        }
    }
    if (keymap_config.no_gui) {
        rgb_matrix_set_color(88, 100, 100, 100);
    }

    // if(host_keyboard_led_state().scroll_lock){
    //     rgb_matrix_set_color(15, 100, 100, 100);
    // }

    return true;
}
