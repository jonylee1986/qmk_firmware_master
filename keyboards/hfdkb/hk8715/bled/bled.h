#ifndef BLED_H
#define BLED_H

#include "quantum.h"

enum bled_keycodes {
    BLED_Mode_Next = QK_KB_0,
    BLED_Brightness_Increase,
    BLED_Brightness_Decrease,
    BLED_Speed_Increase,
    BLED_Speed_Decrease,
    BLED_Color_Increase,
    BLED_Keycodes_End,
};

typedef enum {
    BLED_MODE_CYCLE,
    BLED_MODE_NEON,
    BLED_MODE_BREATHING,
    BLED_MODE_SOLID,
    BLED_MODE_OFF,
    BLED_MODE_END,
} bled_mode_t;

typedef enum {
    COLOR_RAINBOW,
    COLOR_RED,
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
    struct {
        uint8_t     bled_Brightness; // Brightness
        uint8_t     bled_speed;      // Speed
        uint8_t     bled_color;      // Color
        bled_mode_t bled_mode;       // Mode
    };
} bled_info_t;

extern bled_info_t bled_info;

void bled_task(void);
void bled_init(void);

#endif // BLED_H
