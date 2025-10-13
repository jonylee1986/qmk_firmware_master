// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

enum bt_keycodes {
    // Device switching keys
    BT_HOST1 = QK_KB_0,
    BT_HOST2,
    BT_HOST3,
    BT_2_4G,
    BT_USB,

    // Utility keys
    BT_VOL,       // Battery voltage query
    SW_OS,        // Switch OS compatibility
    SLEEP_TOGGLE, // Toggle sleep mode

    // LED control
    BLED_MODE_NEXT, // Backlight mode next
    // Other keys
    KEY_VIA,
    // RGB test mode
    RGB_TEST,

    // macOS specific keys
    MAC_Spotlight,
    MAC_Dictation,
    MAC_Do_Not_Disturb,
};
