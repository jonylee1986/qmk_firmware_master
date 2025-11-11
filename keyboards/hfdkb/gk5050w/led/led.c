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

#include "led.h"
#include "lib/lib8tion/lib8tion.h"
#include "../common/bt_task.h"

// Color table for fixed color modes
// clang-format off
const uint8_t hsv_table[COLOR_COUNT][3] = {
    {HSV_RED},
    {HSV_ORANGE},
    {HSV_YELLOW},
    {HSV_GREEN},
    {HSV_CYAN},
    {HSV_BLUE},
    {HSV_PURPLE},
    {HSV_WHITE},
    {29, 38, 100},
};
// clang-format on

#define SLed_MIN 83
#define SLed_MAX 129

void SLed_task(void) {
    switch (dev_info.mode) {
            // case SLED_MODE_FLOW: {
            //     uint8_t time = scale16by8(g_rgb_timer, qadd8(dev_info.speed / 4, 1));
            //     for (uint8_t i = SLed_MIN; i < (SLed_MAX + 1); i++) {
            //         HSV hsv = {g_led_config.point[i].x - time, 255, dev_info.brightness};
            //         RGB rgb = hsv_to_rgb(hsv);
            //         rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            //     }
            //     break;
            // }

        case SLED_MODE_NEON: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(dev_info.speed / 8, 1));
            HSV     hsv  = {time, 255, dev_info.brightness};
            RGB     rgb  = hsv_to_rgb(hsv);
            for (uint8_t i = SLed_MIN; i < (SLed_MAX + 1); i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case SLED_MODE_SOLID_RAINBOW: {
            if (dev_info.color == COLOR_RAINBOW) {
                for (uint8_t i = SLed_MIN; i < (SLed_MAX + 1); i++) {
                    HSV hsv = {i * 5, 255, dev_info.brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h   = hsv_table[dev_info.color - 1][0];
                hsv.s   = hsv_table[dev_info.color - 1][1];
                hsv.v   = dev_info.brightness;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = SLed_MIN; i < (SLed_MAX + 1); i++) {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
            break;
        }

        case SLED_MODE_SOLID_RED: {
            HSV hsv;
            hsv.h   = hsv_table[8][0];
            hsv.s   = hsv_table[8][1];
            hsv.v   = dev_info.brightness;
            RGB rgb = hsv_to_rgb(hsv);
            for (uint8_t i = SLed_MIN; i < (SLed_MAX + 1); i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }

            // case SLED_MODE_SOLID_ORANGE: {
            //     break;
            // }

            // case SLED_MODE_SOLID_YELLO: {
            //     break;
            // }

            // case SLED_MODE_SOLID_GREEN: {
            //     break;
            // }

            // case SLED_MODE_SOLID_CYAN: {
            //     break;
            // }

            // case SLED_MODE_SOLID_BLUE: {
            //     break;
            // }

            // case SLED_MODE_SOLID_PURPLE: {
            //     break;
            // }

            // case SLED_MODE_SOLID_WHITE: {
            //     break;
            // }

        case SLED_MODE_OFF:
            for (uint8_t i = SLed_MIN; i < (SLed_MAX + 1); i++) {
                rgb_matrix_set_color(i, RGB_OFF);
            }
            break;

        default:
            break;
    }
}

void SLed_init(void) {}

void eeconfig_init_user(void) {
    dev_info.LCD_PAGE   = 0;
    dev_info.brightness = RGB_MATRIX_DEFAULT_VAL;
    dev_info.speed      = RGB_MATRIX_DEFAULT_SPD;
    dev_info.color      = COLOR_RAINBOW;
    dev_info.mode       = SLED_MODE_SOLID_RED;
    dev_info.enable     = true;
    eeconfig_update_user(dev_info.raw);
}
