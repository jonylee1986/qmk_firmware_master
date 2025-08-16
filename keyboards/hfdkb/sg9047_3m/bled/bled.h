#ifndef BLED_H
#define BLED_H

#include "quantum.h"

enum bled_keycodes {
    BLED_Mode = QK_KB_0,
    BLED_Brightness,
    BLED_Speed,
    BLED_Color,
    BLED_KEYCODE_END,
};

typedef enum {
    BLED_MODE_CYCLE,
    BLED_MODE_MARQUEE,
    BLED_MODE_SOLID,
    BLED_MODE_BREATHING,
    BLED_MODE_NEON,
    BLED_MODE_OFF,
    BLED_MODE_COUNT,
} bled_mode_t;

typedef enum {
    COLOR_RAINBOW,
    COLOR_RED,
    COLOR_ORANGE,
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
        uint8_t bled_Brightness; // 灯效亮度值
        uint8_t bled_speed;      // 呼吸速度
        uint8_t bled_color;      // 颜色
        uint8_t bled_mode;       // 灯效模式
    };
} bled_info_t;

bool bled_indicators_advanced(uint8_t led_min, uint8_t led_max);
bool bled_process_record(uint16_t keycode, keyrecord_t *record);
void bled_post_init(void);
void bled_init(void);

#endif // BLED_H
