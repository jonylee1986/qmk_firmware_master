// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include "lcd_drv/uart3.h"
#include "common/bt_task.h"

enum bt_keycodes {
    BT_HOST1 = QK_KB_0,
    BT_HOST2,
    BT_HOST3,
    BT_2_4G,
    BT_USB,
    LCD_HOME,
    LCD_PAGE,
    LCD_SW,
    SLP_TG,
    BT_VOL,
    RGB_TEST,
    BT_SW,
    FN_FUN,
};

#define LCD_init() uart3_init(115200)

void LCD_IND_update(void);
void LCD_Page_update(uint8_t page_num);
void LCD_vol_update(bool vol_inc);
void LCD_charge_update(void);
void LCD_command_update(uint8_t LCD_command);

void set_led_state(void);

uint8_t get_battery_soc(void);

enum LCD_command {
    LCD_RESET = 0,
    LCD_SLEEP,
    LCD_WEAKUP,
    LCD_SWITCH,
    LCD_BACKLIGHT_SLEP,
    LCD_BACKLIGHT_WAKE,
};
