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

#ifndef BLED_H
#define BLED_H

#include "quantum.h"

typedef enum {
    SLED_MODE_SOLID_RED,
    // SLED_MODE_FLOW,
    SLED_MODE_NEON,
    SLED_MODE_SOLID_RAINBOW,
    // SLED_MODE_SOLID_ORANGE,
    // SLED_MODE_SOLID_YELLO,
    // SLED_MODE_SOLID_GREEN,
    // SLED_MODE_SOLID_CYAN,
    // SLED_MODE_SOLID_BLUE,
    // SLED_MODE_SOLID_PURPLE,
    // SLED_MODE_SOLID_WHITE,
    SLED_MODE_OFF,
    SLED_MODE_COUNT,
} SLed_mode_t;

typedef enum {
    COLOR_RAINBOW,
    COLOR_RED,
    COLOR_ORIANGE,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_WHITE,
    COLOR_COUNT,
} SLed_color_preset_t;

// typedef struct PACKED {
//     uint8_t brightness;
//     uint8_t speed;
//     uint8_t color : 4;
//     uint8_t mode : 3;
//     bool    enable : 1;
// } SLed_info_t;

// SLed_info_t        SLed_info;
// extern SLed_info_t SLed_info;

void SLed_task(void);
void SLed_init(void);
// void SLed_eeconfig_init(void);

#endif // BLED_H
