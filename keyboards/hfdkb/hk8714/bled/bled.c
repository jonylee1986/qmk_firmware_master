#include "bled.h"
#include "lib/lib8tion/lib8tion.h"

#define BLED_VAL_STEP 64

// Color table for fixed color modes
// clang-format off
const uint8_t hsv_table[COLOR_COUNT][3] = {
    {HSV_RED},
    {HSV_YELLOW},
    {HSV_GREEN},
    {HSV_CYAN},
    {HSV_BLUE},
    {HSV_PURPLE},
    {HSV_WHITE},
};
// clang-format on

bled_info_t bled_info = {0};

#define LED_MIN 0
#define LED_MAX 6

void bled_task(void) {
    switch (bled_info.bled_mode) {
        case BLED_MODE_CYCLE: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
            for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                HSV hsv = {g_led_config.point[i].x - time, 255, bled_info.bled_Brightness};
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case BLED_MODE_NEON: {
            // Option 1: Slow rainbow cycling (classic neon)
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 8, 1));
            HSV     hsv  = {time, 255, bled_info.bled_Brightness};
            RGB     rgb  = hsv_to_rgb(hsv);
            for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case BLED_MODE_SOLID: {
            if (bled_info.bled_color == COLOR_RAINBOW) {
                // Rainbow
                for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                    HSV hsv = {i * 42, 255, bled_info.bled_Brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h   = hsv_table[bled_info.bled_color - 1][0];
                hsv.s   = hsv_table[bled_info.bled_color - 1][1];
                hsv.v   = bled_info.bled_Brightness;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
            break;
        }

        case BLED_MODE_BREATHING: {
            if (bled_info.bled_color == COLOR_RAINBOW) {
                // Rainbow breathing effect
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.bled_Brightness);
                // uint16_t time = scale16by8(g_rgb_timer, bled_info.bled_speed / 8);
                // uint8_t brightness = scale8(abs8(sin8(time) - 128) * 2, bled_info.bled_Brightness);
                for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                    HSV hsv = {i * 42, 255, brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h              = hsv_table[bled_info.bled_color - 1][0];
                hsv.s              = hsv_table[bled_info.bled_color - 1][1];
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.bled_Brightness);
                // uint16_t time = scale16by8(g_rgb_timer, bled_info.bled_speed / 8);
                // uint8_t brightness = scale8(abs8(sin8(time) - 128) * 2, bled_info.bled_Brightness);
                // Set brightness based on breathing effect
                hsv.v   = brightness;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
            break; // Added missing break statement!
        }

        case BLED_MODE_OFF: {
            for (uint8_t i = LED_MIN; i < LED_MAX; i++) {
                rgb_matrix_set_color(i, RGB_OFF);
            }
            break;
        }

        default:
            break;
    }
}

void bled_init(void) {
    bled_info.raw             = 0;
    bled_info.bled_mode       = BLED_MODE_CYCLE;
    bled_info.bled_Brightness = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
    bled_info.bled_speed      = RGB_MATRIX_DEFAULT_SPD;
    bled_info.bled_color      = COLOR_RAINBOW;
    eeconfig_update_user(bled_info.raw);
}
