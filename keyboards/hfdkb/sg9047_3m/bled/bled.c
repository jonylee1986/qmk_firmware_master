#include "bled.h"
#ifdef MULTIMODE_ENABLE
#    include "bt_task.h"
#endif
#include "lib/lib8tion/lib8tion.h"

static void bled_task(void);

// #define BLED_VAL_STEP RGB_MATRIX_VAL_STEP
#define BLED_VAL_STEP RGB_MATRIX_VAL_STEP
#define BLED_MAXIMUM_BRIGHTNESS RGB_MATRIX_MAXIMUM_BRIGHTNESS
// #define BLED_MAXIMUM_BRIGHTNESS UINT8_MAX

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

bled_info_t bled_info = {0};

// clang-format on
bool bled_process_record(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BLED_Mode:
            if (record->event.pressed) {
                bled_info.bled_mode++;
                if (bled_info.bled_mode >= BLED_MODE_COUNT) {
                    bled_info.bled_mode = BLED_MODE_CYCLE;
                }
                eeconfig_update_user(bled_info.raw);
            }
            return false;

        case BLED_Brightness:
            if (record->event.pressed) {
                if (bled_info.bled_Brightness >= BLED_MAXIMUM_BRIGHTNESS) {
                    bled_info.bled_Brightness = 0;
                } else {
                    bled_info.bled_Brightness = qadd8(bled_info.bled_Brightness, BLED_VAL_STEP);
                }
                eeconfig_update_user(bled_info.raw);
            }
            return false;

        case BLED_Speed:
            if (record->event.pressed) {
                if (bled_info.bled_speed >= UINT8_MAX) { // UINT8_MAX = 255
                    bled_info.bled_speed = 0;
                } else {
                    bled_info.bled_speed = qadd8(bled_info.bled_speed, RGB_MATRIX_SPD_STEP);
                }
                eeconfig_update_kb(bled_info.raw);
            }
            return false;

        case BLED_Color:
            if (record->event.pressed) {
                if (bled_info.bled_mode == BLED_MODE_SOLID || bled_info.bled_mode == BLED_MODE_BREATHING) {
                    bled_info.bled_color++;
                    if (bled_info.bled_color >= COLOR_COUNT) {
                        bled_info.bled_color = COLOR_RAINBOW;
                    }
                }
                eeconfig_update_kb(bled_info.raw);
            }
            return false;

        default:
            break;
    }

    return true;
}

bool bled_indicators_advanced(uint8_t led_min, uint8_t led_max) {
    bled_task();

    return true;
}

static void bled_task(void) {
    switch (bled_info.bled_mode) {
        case BLED_MODE_CYCLE: {
            uint8_t time = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
            for (uint8_t i = 83; i < 115; i++) {
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
            for (uint8_t i = 83; i < 115; i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }

        case BLED_MODE_SOLID: {
            if (bled_info.bled_color == COLOR_RAINBOW) {
                // Rainbow
                for (uint8_t i = 83; i < 115; i++) {
                    HSV hsv = {(i - 83) * 8, 255, bled_info.bled_Brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h   = hsv_table[bled_info.bled_color - 1][0];
                hsv.s   = hsv_table[bled_info.bled_color - 1][1];
                hsv.v   = bled_info.bled_Brightness;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = 83; i < 115; i++) {
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
                for (uint8_t i = 83; i < 115; i++) {
                    HSV hsv = {(i - 83) * 8, 255, brightness};
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            } else {
                HSV hsv;
                hsv.h              = hsv_table[bled_info.bled_color - 1][0];
                hsv.s              = hsv_table[bled_info.bled_color - 1][1];
                uint8_t time       = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 4, 1));
                uint8_t brightness = scale8(abs8(sin8(time / 2) - 128) * 2, bled_info.bled_Brightness);
                // Set brightness based on breathing effect
                hsv.v   = brightness;
                RGB rgb = hsv_to_rgb(hsv);
                for (uint8_t i = 83; i < 115; i++) {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
            break; // Added missing break statement!
        }

        case BLED_MODE_OFF: {
            for (uint8_t i = 83; i < 115; i++) {
                rgb_matrix_set_color(i, RGB_OFF);
            }
            break;
        }

        case BLED_MODE_MARQUEE: {
// Marquee effect with configurable group size
#define MARQUEE_GROUP_SIZE 15 // Number of LEDs in each group
#define MARQUEE_LED_COUNT 32  // Total LEDs (115 - 83 = 32)

            uint8_t time         = scale16by8(g_rgb_timer, qadd8(bled_info.bled_speed / 8, 1));
            uint8_t cycle_length = MARQUEE_LED_COUNT * 2; // Simplified: just LED count * 2
            uint8_t position     = time % cycle_length;

            // Determine if we're going forward or backward
            bool    going_forward = position < MARQUEE_LED_COUNT;
            uint8_t led_pos;

            if (going_forward) {
                led_pos = position;
            } else {
                led_pos = (MARQUEE_LED_COUNT * 2 - 1) - position; // Proper reverse calculation
            }

            // Clear all LEDs first
            for (uint8_t i = 83; i < 115; i++) {
                rgb_matrix_set_color(i, RGB_OFF);
            }

            // Set the marquee group
            HSV base_hsv;
            if (bled_info.bled_color == COLOR_RAINBOW) {
                base_hsv = (HSV){time * 2, 255, bled_info.bled_Brightness};
            } else {
                base_hsv.h = hsv_table[bled_info.bled_color - 1][0];
                base_hsv.s = hsv_table[bled_info.bled_color - 1][1];
                base_hsv.v = bled_info.bled_Brightness;
            }

            // Light up the group of LEDs
            for (uint8_t j = 0; j < MARQUEE_GROUP_SIZE; j++) {
                int16_t led_index = led_pos - j;
                if (led_index >= 0 && led_index < MARQUEE_LED_COUNT) {
                    uint8_t actual_led = 83 + led_index;

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

        default:
            break;
    }
}

void bled_post_init(void) {
    bled_info.raw = eeconfig_read_kb();
}

void bled_init(void) {
    bled_info.bled_mode       = BLED_MODE_CYCLE;
    bled_info.bled_color      = COLOR_RAINBOW;
    bled_info.bled_Brightness = BLED_MAXIMUM_BRIGHTNESS;
    bled_info.bled_speed      = RGB_MATRIX_DEFAULT_SPD;
    eeconfig_update_kb(bled_info.raw);
}
