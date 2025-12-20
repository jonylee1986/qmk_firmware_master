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
#define BLED_CYCLE_LED_COUNT 5

static uint8_t leds[] = {80, 81, 82, 83, 84};

void bled_task(void) {
    switch (dev_info.bled_mode) {
        case BLED_MODE_CYCLE: {
            uint8_t time = scale16by8(g_rgb_timer / 2, qadd8(dev_info.bled_speed / 63, 1));
            // uint8_t cycle_length = BLED_CYCLE_LED_COUNT * 1;
            uint8_t position = time % BLED_CYCLE_LED_COUNT;

            // uint8_t led_pos = (BLED_CYCLE_LED_COUNT * 2 - 1) - position;

            // Clear all LEDs first
            for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
                rgb_matrix_set_color(leds[i], RGB_OFF);
            }

            // Set the marquee group
            HSV base_hsv;
            if (dev_info.bled_color == COLOR_RAINBOW) {
                base_hsv = (HSV){time * 16, 255, dev_info.bled_val};
            } else {
                base_hsv.h = hsv_table[dev_info.bled_color - 1][0];
                base_hsv.s = hsv_table[dev_info.bled_color - 1][1];
                base_hsv.v = dev_info.bled_val;
            }

            // Light up the group of LEDs
            for (uint8_t j = 0; j < BLED_CYCLE_GROUP_SIZE; j++) {
                int16_t led_index = position - j;
                if ((led_index >= 0) && (led_index < (sizeof(leds) / sizeof(leds[0])))) {
                    uint8_t actual_led = leds[led_index];

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

        case BLED_MODE_BREATHING: {
            if (dev_info.bled_color == COLOR_RAINBOW) {
                // Rainbow breathing effect
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(dev_info.bled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, dev_info.bled_val);
                for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
                    HSV hsv = {i * 45, 255, brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h              = hsv_table[dev_info.bled_color - 1][0];
                hsv.s              = hsv_table[dev_info.bled_color - 1][1];
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(dev_info.bled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, dev_info.bled_val);
                hsv.v              = brightness;
                RGB rgb            = hsv_to_rgb(hsv);
                for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
                    rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
                }
            }
            break; // Added missing break statement!
        }

        case BLED_MODE_SOLID: {
            if (dev_info.bled_color == COLOR_RAINBOW) {
                // Rainbow
                for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
                    HSV hsv = {i * 45, 255, dev_info.bled_val};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h   = hsv_table[dev_info.bled_color - 1][0];
                hsv.s   = hsv_table[dev_info.bled_color - 1][1];
                hsv.v   = dev_info.bled_val;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
                    rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
                }
            }
            break;
        }

        case BLED_MODE_OFF:
            for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
                rgb_matrix_set_color(leds[i], RGB_OFF);
            }
            break;

        default:
            break;
    }
}

void bled_charging_indicate(void) {
    // uint8_t        time      = scale16by8(g_rgb_timer / 4, qadd8(dev_info.bled_speed / 63, 1));
    // uint8_t        led_index = time % 5; // Cycle through 0-4
    // static uint8_t leds[]    = {84, 83, 82, 81, 80};

    // Clear all LEDs first
    // for (uint8_t i = led_index; i < (sizeof(leds) / sizeof(leds[0])); i++) {
    //     rgb_matrix_set_color(leds[i], RGB_OFF);
    // }

    // Set the color
    // HSV base_hsv;
    // if (dev_info.bled_color == COLOR_RAINBOW) {
    //     base_hsv = (HSV){time * 8, 255, dev_info.bled_val};
    // } else {
    //     base_hsv.h = hsv_table[dev_info.bled_color - 1][0];
    //     base_hsv.s = hsv_table[dev_info.bled_color - 1][1];
    //     base_hsv.v = dev_info.bled_val;
    // }

    // Light up only the current LED in the cycle
    // RGB rgb = hsv_to_rgb(base_hsv);
    // for (uint8_t i = 0; i <= led_index; i++) {
    //     rgb_matrix_set_color(leds[led_index], rgb.r, rgb.g, rgb.b);
    // }
    uint8_t time = scale16by8(g_rgb_timer / 2, qadd8(dev_info.bled_speed / 63, 1));
    // uint8_t cycle_length = BLED_CYCLE_LED_COUNT * 1;
    uint8_t position = time % BLED_CYCLE_LED_COUNT;

    uint8_t led_pos = (BLED_CYCLE_LED_COUNT - 1) - position;

    // Clear all LEDs first
    for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
        rgb_matrix_set_color(leds[i], RGB_OFF);
    }

    // Set the marquee group
    HSV base_hsv;
    if (dev_info.bled_color == COLOR_RAINBOW) {
        base_hsv = (HSV){time * 16, 255, dev_info.bled_val};
    } else {
        base_hsv.h = hsv_table[dev_info.bled_color - 1][0];
        base_hsv.s = hsv_table[dev_info.bled_color - 1][1];
        base_hsv.v = dev_info.bled_val;
    }

    // Light up the group of LEDs
    for (uint8_t j = 0; j < BLED_CYCLE_GROUP_SIZE; j++) {
        int16_t led_index = led_pos - j;
        if ((led_index >= 0) && (led_index < (sizeof(leds) / sizeof(leds[0])))) {
            uint8_t actual_led = leds[led_index];

            // Create fading effect within the group
            uint8_t brightness_scale = 255 - (j * 17); // Fade: 255, 210, 165, 120, 75
            HSV     hsv              = base_hsv;
            hsv.v                    = scale8(hsv.v, brightness_scale);

            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(actual_led, rgb.r, rgb.g, rgb.b);
        }
    }
}

void bled_charged_indicate(void) {
    if (dev_info.bled_color == COLOR_RAINBOW) {
        // Rainbow
        for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
            HSV hsv = {i * 45, 255, dev_info.bled_val};
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
        }
    } else {
        HSV hsv;
        hsv.h   = hsv_table[dev_info.bled_color - 1][0];
        hsv.s   = hsv_table[dev_info.bled_color - 1][1];
        hsv.v   = dev_info.bled_val;
        RGB rgb = hsv_to_rgb(hsv);
        for (uint8_t i = 0; i < (sizeof(leds) / sizeof(leds[0])); i++) {
            rgb_matrix_set_color(leds[i], rgb.r, rgb.g, rgb.b);
        }
    }
}

void bled_init(void) {}

void bled_eeconfig_init(void) {
    dev_info.bled_mode  = BLED_MODE_CYCLE;
    dev_info.bled_val   = BLED_MAX_BRIGHTNESS;
    dev_info.bled_speed = RGB_MATRIX_DEFAULT_SPD;
    eeconfig_update_user(dev_info.raw);
}
