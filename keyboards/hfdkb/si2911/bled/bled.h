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
    BLED_MODE_FLOW,
    // BLED_MODE_CYCLE,
    BLED_MODE_NEON,
    BLED_MODE_BREATHING,
    BLED_MODE_SOLID,
    BLED_MODE_COUNT,
} bled_mode_t;

typedef enum {
    SLED_MODE_FLOW,
    SLED_MODE_CYCLE,
    SLED_MODE_NEON,
    SLED_MODE_BREATHING,
    SLED_MODE_SOLID,
    SLED_MODE_COUNT,
    SLED_MODE_CHARGE,
    SLED_MODE_CHARGED,
    SLED_MODE_VOL,
    // SLED_MODE_CYCLE1,
    // SLED_MODE_CYCLE2,
} sled_mode_t;

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
} bled_color_preset_t;

typedef union {
    uint32_t raw;
    struct PACKED{
        uint8_t bled_val;   // Brightness
        uint8_t bled_speed; // Speed
        uint8_t sled_val;   // Brightness
        uint8_t sled_speed; // Speed
    };
} bled_info_t;

extern bled_info_t bled_info;

void bled_task(void);
void sled_task(void);
void bled_init(void);
void bled_eeconfig_init(void);
void bled_charging_indicate(void);
void bled_charged_indicate(void);
void bled_vol_indicate(void);

#endif // BLED_H
