// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

enum bt_keycodes {
    BT_HOST1 = QK_KB_0,
    BT_HOST2,
    BT_HOST3,
    BT_2_4G,
    BT_USB,
    BT_VOL,
    SW_OS,
    INDICATOR_HUE,
    INDICATOR_BRIGHTNESS,
    ECO,
    FACTORY_RESET,
    KEYBOARD_RESET,
    BLE_RESET,
    FN_FUN,
    FN_MENU,
    WIN_LOCK,
    KC_MICT,

    RGB_TOG,
    RGB_MOD,
    RGB_HUD,
    RGB_HUI,
    RGB_SAD,
    RGB_SAI,
    RGB_VAD,
    RGB_VAI,
    RGB_SPD,
    RGB_SPI,
};

enum _keycodes {
    RGB_TEST = SAFE_RANGE,
};
