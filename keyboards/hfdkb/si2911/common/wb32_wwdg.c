// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include <stdlib.h>
#include "wwdg.h"

static bool     feed_dog   = false;
static bool     enable_dog = false;
static uint16_t time       = 0;

void wwdg_disable(void) {
    if (feed_dog) {
        Disable_WWDG();
        feed_dog = false;
    }
}

void wwdg_enable(void) {
    enable_dog = true;
}

void housekeeping_task_kb(void) {
    if (enable_dog) {
        enable_dog = false;
        feed_dog   = true;
        Init_WWDG();
        WWDG_Enable(127);
        time = timer_read();
    }
    if (timer_elapsed(time) > 1) {
        time = timer_read();
        if (feed_dog) {
            WWDG_SetCounter(127);
        }
    }
}

void snled27351_reset(void) {
    setPinOutputOpenDrain(C11);
    writePinLow(C11);
    wait_ms(1);
    writePinHigh(C11);
    // wait_ms(20);

    WWDG_SetCounter(127);
    time = timer_read();

    rgb_matrix_init();
}

// void keyboard_pre_init_kb(void) {
//     feed_dog = true;
//     Init_WWDG();
//     WWDG_Enable(127);
//     time = timer_read();
// }
