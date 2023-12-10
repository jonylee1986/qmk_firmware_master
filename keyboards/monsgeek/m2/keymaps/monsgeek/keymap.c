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

enum __via_custom_values {
    id_features_win_lock  = 1,
    id_features_mac_mode  = 2,
    id_toggle_wasd_button = 3
};

enum __layers {
    WIN_BASE,
    WIN_WASD,
    WIN_FN,
    MAC_BASE,
    MAC_WASD,
    MAC_FN
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define MG_MACM DF(MAC_BASE)
#define MG_WINM DF(WIN_BASE)
#define MG_W_FN MO(WIN_FN)
#define MG_M_FN MO(MAC_FN)
#define MG_WASD TG(WIN_WASD)
#define MG_MASD TG(MAC_WASD)

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_BASE] = LAYOUT(  /* Base */
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_HOME, KC_END,  KC_PGUP, KC_PGDN,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_P4,   KC_P5,   KC_P6,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MG_W_FN,          KC_LEFT, KC_DOWN, KC_RGHT,          KC_P0,   KC_PDOT
    ),

    [WIN_WASD] = LAYOUT(  /* Base */
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        _______, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______, KC_W,    _______, _______, _______, _______,
        _______, _______, _______,                   _______,                            _______, _______,          KC_A,    KC_S,    KC_D,             _______, _______
    ),


    [WIN_FN] = LAYOUT(  /* FN */
        _______, KC_MYCM, KC_MAIL, KC_WSCH, KC_WHOM, KC_MSEL, KC_MPLY, KC_MPRV, KC_MNXT, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SPD, RGB_SPI, _______,          _______, _______, _______, _______,
        _______, _______, MG_WASD, _______, _______, _______, _______, _______, KC_INS,  MG_MACM, KC_PSCR, RGB_HUD, RGB_HUI, RGB_MOD,          _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_TOG, _______, _______,          _______,          _______, _______, _______,
        _______, _______, _______, KC_CALC, _______, _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______,                   _______, RGB_VAI, _______, _______, _______, _______,
        _______, GUI_TOG, _______,                   _______,                            _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI,          _______, _______
    ),

    [MAC_BASE] = LAYOUT(  /* Base */
        KC_ESC,  KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,           KC_HOME, KC_END,  KC_PGUP, KC_PGDN,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_P4,   KC_P5,   KC_P6,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT, KC_UP,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_LCTL, KC_LALT, KC_LGUI,                   KC_SPC,                             KC_RGUI, MG_M_FN,          KC_LEFT, KC_DOWN, KC_RGHT,          KC_P0,   KC_PDOT
    ),

    [MAC_WASD] = LAYOUT(  /* Base */
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        _______, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______, KC_W,    _______, _______, _______, _______,
        _______, _______, _______,                   _______,                            _______, _______,          KC_A,    KC_S,    KC_D,             _______, _______
    ),

    [MAC_FN] = LAYOUT(  /* FN */
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,           _______, _______, _______, _______,
        QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SPD, RGB_SPI, _______,           _______, _______, _______, _______,
        _______, _______, MG_MASD, _______, _______, _______, _______, _______, KC_INS,  MG_WINM, KC_PSCR, RGB_HUD, RGB_HUI, RGB_MOD,           _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_TOG, _______, _______,          _______,           _______, _______, _______,
        _______, _______, _______, KC_CALC, _______, _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______,                   _______, RGB_VAI,  _______, _______, _______, _______,
        _______, _______, _______,                   _______,                            _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI,           _______, _______
    )
};

// clang-format on

void led_init_ports(void) {
    // Set our LED pings as output
    setPinOutput(LED_NUM_LOCK_PIN); // LED1 Num Lock
    writePinLow(LED_NUM_LOCK_PIN);
    setPinOutput(LED_CAPS_LOCK_PIN); // LED2 Caps Lock
    writePinLow(LED_CAPS_LOCK_PIN);
    setPinOutput(LED_WIN_LOCK_PIN); // LED3 Win Lock
    writePinLow(LED_WIN_LOCK_PIN);
}

bool led_update_user(led_t led_state) {
    writePin(LED_NUM_LOCK_PIN, led_state.num_lock);
    writePin(LED_CAPS_LOCK_PIN, led_state.caps_lock);
    writePin(LED_WIN_LOCK_PIN, keymap_config.no_gui);
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DF(WIN_BASE):
            if (record->event.pressed) {
                set_single_persistent_default_layer(WIN_BASE);
            }
            return false;

        case DF(MAC_BASE):
            if (record->event.pressed) {
                set_single_persistent_default_layer(MAC_BASE);
                // Setting Mac mode also re-enables Win-key
                if (keymap_config.no_gui == 1) {
                    keymap_config.no_gui = 0;
                    eeconfig_update_keymap(keymap_config.raw);
                }
            }
            return false;

        default:
            return true;
    }
}
