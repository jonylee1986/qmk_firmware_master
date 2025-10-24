/* Copyright 2021
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

enum LCD_command {
    LCD_RESET = 0,
    LCD_SLEEP,
    LCD_WEAKUP,
    LCD_SWITCH,
    LCD_LIGHT_SLEEP,
};

void LCD_IND_update(void);
void LCD_Page_update(uint8_t page_num);
void LCD_vol_update(bool vol_inc);
void LCD_charge_update(void);
void LCD_command_update(uint8_t LCD_command);