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

enum _layers {
    WIN_BASE,
    WIN_FN,
    AND_BASE,
    AND_FN,
    MAC_BASE,
    MAC_FN,
    IOS_BASE,
    IOS_FN,
};

#define KC_TASK LALT(KC_TAB)
#define KC_FLXP LWIN(KC_E)
#define MAC_SWWD LWIN(KC_TAB)
#define MAC_DESK LWIN(KC_H)
#define KEY_WIN PDF(0)
#define KEY_AND PDF(2)
#define KEY_MAC PDF(4)
#define KEY_IOS PDF(6)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             SW_MODE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             MO(1),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_WHOM,  KC_MAIL,  KC_TASK,  KC_FLXP,  RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            RM_NEXT,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, _______,  _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  KC_INS,
        RM_HUEU, KEY_AND,  _______,  KEY_MAC,  KEY_IOS,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        _______,           _______,  _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, GU_TOGG,  _______,                               _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),

    [AND_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            SW_MODE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LWIN,  KC_LALT,                               KC_SPC,                             MO(3),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [AND_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             RM_NEXT,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, _______,  _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        RM_HUEU, _______,  KEY_WIN,  KEY_MAC,  KEY_IOS,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        _______,           _______,  _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, _______,  _______,                               _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),

    [MAC_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_BRID,  KC_BRIU,  MAC_SWWD, MAC_DESK, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            SW_MODE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             MO(5),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             RM_NEXT,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        RM_HUEU, KEY_AND,  KEY_WIN,  _______,  KEY_IOS,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        _______,           _______,  _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, _______,  _______,                               _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),

    [IOS_BASE] = LAYOUT_80_ansi(
        KC_ESC,  KC_BRID,  KC_BRIU,  KC_NO,    MAC_DESK, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,            SW_MODE,
        KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
        KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
        KC_CAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,   KC_PGDN,
        KC_LSFT,           KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
        KC_LCTL, KC_LOPT,  KC_LCMD,                               KC_SPC,                             MO(7),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [IOS_FN] = LAYOUT_80_ansi(
        EE_CLR,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,             RM_NEXT,
        BT_2_4G, BT_HOST1, BT_HOST2, BT_HOST3, BT_2_4G,  _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        RM_HUEU, KEY_AND,  KEY_WIN,  KEY_MAC,  _______,  _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______,            _______,  _______,
        _______,           _______,  _______,  _______,  _______, BT_VOL,  _______, _______, _______, _______, _______,  _______,  RM_VALU,  KC_HOME,
        _______, _______,  _______,                               _______,                            _______, _______,  RM_SPDD,  RM_VALD,  RM_SPDU),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = {ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [AND_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [AND_FN]   = {ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = {ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [IOS_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [IOS_FN]   = {ENCODER_CCW_CW(RM_VALD, RM_VALU) }
};
#endif // ENCODER_MAP_ENABLE
// clang-format on

// Blink effect related
static uint8_t  single_blink_cnt   = 0;
static uint8_t  single_blink_index = 0;
static RGB      single_blink_color = {0};
static uint32_t single_blink_time  = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef MULTIMODE_ENABLE
    if (!bt_process_record(keycode, record)) {
        return false;
    }
#endif
    switch (keycode) {
        case KEY_WIN:
            if (record->event.pressed) {
                single_blink_cnt   = 6;
                single_blink_time  = timer_read32();
                single_blink_index = LED_W_INDEX;
                single_blink_color = (RGB){RGB_RED};
            }
            break;

        case KEY_AND:
            if (record->event.pressed) {
                if (keymap_config.no_gui) {
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config);
                }
                single_blink_cnt   = 6;
                single_blink_time  = timer_read32();
                single_blink_index = LED_Q_INDEX;
                single_blink_color = (RGB){RGB_RED};
            }
            break;

        case KEY_MAC:
            if (record->event.pressed) {
                if (keymap_config.no_gui) {
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config);
                }
                single_blink_cnt   = 6;
                single_blink_time  = timer_read32();
                single_blink_index = LED_E_INDEX;
                single_blink_color = (RGB){RGB_RED};
            }
            break;

        case KEY_IOS:
            if (record->event.pressed) {
                if (keymap_config.no_gui) {
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(&keymap_config);
                }
                single_blink_cnt   = 6;
                single_blink_time  = timer_read32();
                single_blink_index = LED_R_INDEX;
                single_blink_color = (RGB){RGB_RED};
            }
            break;

        default:
            break;
    }

    return true;
}

void keyboard_pre_init_user(void) {
#ifdef RGB_MATRIX_SHUTDOWN_PIN
    setPinOutputPushPull(RGB_MATRIX_SHUTDOWN_PIN);
    writePinHigh(RGB_MATRIX_SHUTDOWN_PIN);
#endif
#ifdef VOL_INDICATE_LED_1_PIN
    setPinOutputPushPull(VOL_INDICATE_LED_1_PIN);
    writePin(VOL_INDICATE_LED_1_PIN, !VOL_INDICATE_LED_STATE_ON);
    setPinOutputPushPull(VOL_INDICATE_LED_2_PIN);
    writePin(VOL_INDICATE_LED_2_PIN, !VOL_INDICATE_LED_STATE_ON);
    setPinOutputPushPull(VOL_INDICATE_LED_3_PIN);
    writePin(VOL_INDICATE_LED_3_PIN, !VOL_INDICATE_LED_STATE_ON);
    setPinOutputPushPull(VOL_INDICATE_LED_4_PIN);
    writePin(VOL_INDICATE_LED_4_PIN, !VOL_INDICATE_LED_STATE_ON);
    setPinOutputPushPull(VOL_INDICATE_LED_5_PIN);
    writePin(VOL_INDICATE_LED_5_PIN, !VOL_INDICATE_LED_STATE_ON);
#endif
}

void keyboard_post_init_user(void) {
    if (keymap_config.no_gui) {
        keymap_config.no_gui = 0;
        eeconfig_update_keymap(&keymap_config);
    }
}

void suspend_power_down_user(void) {
    led_deconfig_all();
}

void suspend_wakeup_init_user(void) {
    led_config_all();
}

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_user(void) {
    if (keymap_config.no_gui) {
        rgb_matrix_set_color(72, 0xC8, 0xC8, 0xC8);
    }

    if (single_blink_cnt) {
        if (timer_elapsed32(single_blink_time) > 300) {
            single_blink_time = timer_read32();
            single_blink_cnt--;
        }
        if (single_blink_cnt % 2) {
            rgb_matrix_set_color(single_blink_index, single_blink_color.r, single_blink_color.g, single_blink_color.b);
        } else {
            rgb_matrix_set_color(single_blink_index, 0, 0, 0);
        }
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
#    ifdef MULTIMODE_ENABLE
    if (!bt_indicators_advanced(led_min, led_max)) {
        return false;
    }
#    endif
    return true;
}
#endif

void housekeeping_task_user(void) {
#ifdef MULTIMODE_ENABLE
    bt_housekeeping_task();
#endif
}

void matrix_scan_user(void) {
#ifdef MULTIMODE_ENABLE
    bt_task();
#endif
}

void matrix_init_user(void) {
#ifdef MULTIMODE_ENABLE
    bt_init();
    led_config_all();
#endif
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            if (mode == MODE_WORKING) {
                tap_code(KC_VOLU);
            } else {
                rgb_matrix_increase_val();
            }
        } else {
            if (mode == MODE_WORKING) {
                tap_code(KC_VOLD);
            } else {
                rgb_matrix_decrease_val();
            }
        }
    }
    return false;
}
