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

#include "bled.h"
#include "../common/bt_task.h"
#include "lib/lib8tion/lib8tion.h"

#define BLED_VAL_STEP 64

bled_info_t bled_info = {0};

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
};
// clang-format on

#define BLED_CYCLE_GROUP_SIZE 1
#define BLED_CYCLE_LED_COUNT 2
// Marquee effect with configurable group size
#define SLED_CYCLE_GROUP_SIZE 1 // Number of LEDs in each group
#define SLED_CYCLE_LED_COUNT 5  // Total LEDs (115 - 83 = 32)

static uint8_t bled_leds[] = {100, 101};
#define BLED_LED_NUM (sizeof(bled_leds) / sizeof(bled_leds[0]))
static uint8_t sled_leds[] = {102, 103, 104, 105, 106};
#define SLED_LED_NUM (sizeof(sled_leds) / sizeof(sled_leds[0]))

void bled_task(void) {
    switch (dev_info.bled_mode) {
        case BLED_MODE_FLOW: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
            for (uint8_t i = 0; i < BLED_LED_NUM; i++) {
                HSV hsv = {g_led_config.point[i].x - time, 255, bled_info.bled_val};
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(bled_leds[i], rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case BLED_MODE_NEON: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 8, 1));
            HSV     hsv  = {time, 255, bled_info.bled_val};
            RGB     rgb  = hsv_to_rgb(hsv);
            for (uint8_t i = 0; i < BLED_LED_NUM; i++) {
                rgb_matrix_set_color(bled_leds[i], rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case BLED_MODE_SOLID: {
            // if (dev_info.bled_color == COLOR_RAINBOW) {
            //     // Rainbow solid color
            //     for (uint8_t i = 0; i < BLED_LED_NUM; i++) {
            //         HSV hsv = {((i + 1) * 100 + 40), 255, bled_info.bled_val};
            //         RGB rgb = hsv_to_rgb(hsv);
            //         rgb_matrix_set_color(bled_leds[i], rgb.r, rgb.g, rgb.b);
            //     }
            // } else {
            HSV hsv;
            hsv.h   = hsv_table[dev_info.bled_color - 1][0];
            hsv.s   = hsv_table[dev_info.bled_color - 1][1];
            hsv.v   = bled_info.bled_val;
            RGB rgb = hsv_to_rgb(hsv);
            for (uint8_t i = 0; i < BLED_LED_NUM; i++) {
                rgb_matrix_set_color(bled_leds[i], rgb.r, rgb.g, rgb.b);
            }
            // }

            break;
        }

        case BLED_MODE_BREATHING: {
            // if (dev_info.bled_color == COLOR_RAINBOW) {
            //     // Rainbow breathing effect
            //     uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
            //     uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.bled_val);
            //     for (uint8_t i = 0; i < BLED_LED_NUM; i++) {
            //         HSV hsv = {((i + 1) * 100 + 40), 255, brightness};
            //         RGB rgb = hsv_to_rgb(hsv);
            //         rgb_matrix_set_color(bled_leds[i], rgb.r, rgb.g, rgb.b);
            //     }
            // } else {
            HSV hsv;
            hsv.h              = hsv_table[dev_info.bled_color - 1][0];
            hsv.s              = hsv_table[dev_info.bled_color - 1][1];
            uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
            uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.bled_val);
            hsv.v              = brightness;
            RGB rgb            = hsv_to_rgb(hsv);
            for (uint8_t i = 0; i < BLED_LED_NUM; i++) {
                rgb_matrix_set_color(bled_leds[i], rgb.r, rgb.g, rgb.b);
            }
            // }
            break; // Added missing break statement!
        }

        default:
            break;
    }
}

void sled_task(void) {
    switch (dev_info.sled_mode) {
        case SLED_MODE_FLOW: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.sled_speed / 4, 1));
            for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                HSV hsv = {g_led_config.point[i].x - time, 255, bled_info.sled_val};
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(sled_leds[i], rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case SLED_MODE_CYCLE: {
            uint8_t time         = scale16by8(g_rgb_timer, qadd8(bled_info.sled_speed / 63, 1));
            uint8_t cycle_length = SLED_CYCLE_LED_COUNT * 2; // Simplified: just LED count * 2
            uint8_t position     = time % cycle_length;

            // Determine if we're going forward or backward
            bool    going_forward = position < SLED_CYCLE_LED_COUNT;
            uint8_t led_pos;

            if (going_forward) {
                led_pos = position;
            } else {
                led_pos = (SLED_CYCLE_LED_COUNT * 2 - 1) - position; // Proper reverse calculation
            }

            // Clear all LEDs first
            for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                rgb_matrix_set_color(sled_leds[i], RGB_OFF);
            }

            // Set the marquee group
            HSV base_hsv;
            if (dev_info.sled_color == COLOR_RAINBOW) {
                base_hsv = (HSV){time * 16, 255, bled_info.sled_val};
            } else {
                base_hsv.h = hsv_table[dev_info.sled_color - 1][0];
                base_hsv.s = hsv_table[dev_info.sled_color - 1][1];
                base_hsv.v = bled_info.sled_val;
            }

            // Light up the group of LEDs
            for (uint8_t j = 0; j < SLED_CYCLE_GROUP_SIZE; j++) {
                int16_t led_index = led_pos - j;
                if (led_index >= 0 && led_index < SLED_CYCLE_LED_COUNT) {
                    uint8_t actual_led = sled_leds[led_index];

                    // Create fading effect within the group
                    uint8_t brightness_scale = 255 - (j * 17); // Fade: 255, 210, 165, 120, 75
                    HSV     hsv              = base_hsv;
                    hsv.v                    = scale8(hsv.v, brightness_scale);

                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(actual_led, rgb.r, rgb.g, rgb.b);
                }
            }
            break;
        }

        case SLED_MODE_NEON: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.sled_speed / 8, 1));
            HSV     hsv  = {time, 255, bled_info.sled_val};
            RGB     rgb  = hsv_to_rgb(hsv);
            for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                rgb_matrix_set_color(sled_leds[i], rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case SLED_MODE_BREATHING: {
            if (dev_info.sled_color == COLOR_RAINBOW) {
                // Rainbow breathing effect
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.sled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.sled_val);
                for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                    HSV hsv = {i * 45, 255, brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(sled_leds[i], rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h              = hsv_table[dev_info.sled_color - 1][0];
                hsv.s              = hsv_table[dev_info.sled_color - 1][1];
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.sled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.sled_val);
                hsv.v              = brightness;
                RGB rgb            = hsv_to_rgb(hsv);
                for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                    rgb_matrix_set_color(sled_leds[i], rgb.r, rgb.g, rgb.b);
                }
            }
            break; // Added missing break statement!
        }

        case SLED_MODE_SOLID: {
            if (dev_info.sled_color == COLOR_RAINBOW) {
                // Rainbow
                for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                    HSV hsv = {i * 45, 255, bled_info.sled_val};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(sled_leds[i], rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h   = hsv_table[dev_info.sled_color - 1][0];
                hsv.s   = hsv_table[dev_info.sled_color - 1][1];
                hsv.v   = bled_info.sled_val;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
                    rgb_matrix_set_color(sled_leds[i], rgb.r, rgb.g, rgb.b);
                }
            }
            break;
        }

        case SLED_MODE_CHARGE: {
            bled_charging_indicate();
            break;
        }

        case SLED_MODE_VOL: {
            bled_vol_indicate();
            break;
        }

        case SLED_MODE_CHARGED: {
            bled_charged_indicate();
            break;
        }

        case SLED_MODE_LOW: {
            bled_low_indicate();
            break;
        }

        default:
            break;
    }
}

void bled_low_indicate(void) {
    static uint32_t Low_power_time = 0;
    static bool     Low_power_bink = false;

    for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
        rgb_matrix_set_color(sled_leds[i], RGB_OFF);
    }

    if (timer_elapsed32(Low_power_time) >= 500) {
        Low_power_bink = !Low_power_bink;
        Low_power_time = timer_read32();
    }
    if (Low_power_bink) {
        rgb_matrix_set_color(102, RGB_RED);
    } else {
        rgb_matrix_set_color(102, RGB_OFF);
    }
}

void bled_charging_indicate(void) {
    uint8_t time      = scale16by8(g_rgb_timer / 4, qadd8(bled_info.sled_speed / 63, 1));
    uint8_t led_index = time % 5; // Cycle through 0-4

    // Clear all LEDs first
    for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
        rgb_matrix_set_color(sled_leds[i], RGB_OFF);
    }

    // Set the color
    // HSV base_hsv;
    // if (dev_info.sled_color == COLOR_RAINBOW) {
    //     base_hsv = (HSV){time * 16, 255, bled_info.sled_val};
    // } else {
    //     base_hsv.h = hsv_table[dev_info.sled_color - 1][0];
    //     base_hsv.s = hsv_table[dev_info.sled_color - 1][1];
    //     base_hsv.v = bled_info.sled_val;
    // }

    // Light up only the current LED in the cycle
    // RGB rgb = hsv_to_rgb(base_hsv);
    for (uint8_t i = 0; i <= led_index && i < SLED_LED_NUM; i++) {
        rgb_matrix_set_color(sled_leds[i], 0, 200, 0);
    }
}

void bled_charged_indicate(void) {
    // Show solid green to indicate fully charged
    for (uint8_t i = 0; i < SLED_LED_NUM; i++) {
        rgb_matrix_set_color(sled_leds[i], RGB_GREEN);
    }
}

void bled_vol_indicate(void) {
    uint8_t query_index[] = {102, 103, 104, 105, 106};
    uint8_t pvol          = bts_info.bt_info.pvol;
    uint8_t led_count     = 0;
    RGB     color;

    if (pvol <= 20) {
        led_count = 1;
    } else if (pvol <= 40) {
        led_count = 2;
    } else if (pvol <= 60) {
        led_count = 3;
    } else if (pvol <= 80) {
        led_count = 4;
    } else {
        led_count = 5;
    }

    for (uint8_t i = 0; i < (sizeof(query_index) / sizeof(query_index[0])); i++) {
        rgb_matrix_set_color(query_index[i], RGB_OFF);
    }

    if (pvol <= 20) {
        color = (RGB){100, 0, 0}; // 红色
    } else {
        color = (RGB){0, 100, 0}; // 绿色
    }

    for (uint8_t i = 0; i < led_count; i++) {
        rgb_matrix_set_color(query_index[i], color.r, color.g, color.b);
    }
}

void bled_init(void) {
    dev_info.raw  = eeconfig_read_user();
    bled_info.raw = eeconfig_read_kb();
}

void bled_eeconfig_init(void) {
    dev_info.sled_mode  = SLED_MODE_FLOW;
    dev_info.bled_mode  = BLED_MODE_FLOW;
    dev_info.bled_color = COLOR_RED;
    eeconfig_update_user(dev_info.raw);

    bled_info.bled_val   = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
    bled_info.bled_speed = RGB_MATRIX_DEFAULT_SPD;

    bled_info.sled_val   = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
    bled_info.sled_speed = RGB_MATRIX_DEFAULT_SPD;
    eeconfig_update_kb(bled_info.raw);
}
