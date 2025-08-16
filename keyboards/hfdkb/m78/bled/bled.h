#ifndef BLED_H
#define BLED_H

#include "quantum.h"
#include "color.h"
#include "common/bt_task.h"
// 灯效模式枚举
typedef enum {
    BLED_MODE_CYCLE_LEFT_TO_RIGHT, // 流光
    BLED_MODE_BREATHING,           // 呼吸
    BLED_MODE_NEON,                // 霓虹
    BLED_MODE_ON_YELLOW,           // 单色黄
    BLED_MODE_ON_BLUE,             // 单色蓝
    BLED_MODE_ON_PINK,             // 单色粉
    BLED_MODE_ON_WHITE,            // 单色白
    BLED_MODE_OFF,                 // 关闭
    BLED_MODE_COUNT
} bled_mode_t;

typedef enum {
    COLOR_RAINBOW, // RGB彩色（默认首次显示）
    COLOR_RED,
    COLOR_ORANGE,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_COUNT
} bled_color_preset_t;

typedef struct {
    uint8_t mode;       // 当前模式
    uint8_t color;      // 预设色（仅单色/呼吸有效）
    uint8_t speed;      // 速度（流光/霓虹/呼吸有效）
    uint8_t brightness; // 亮度（所有模式有效）
} bled_param_t;

extern bled_param_t bled_param;

// 函数声明
void bled_task(void);
void bled_mode_next(void);
void bled_mode_prev(void);

// 速度控制函数（预留）
void bled_increase_speed(void);

void bled_decrease_speed(void);

// 亮度控制函数（预留）
void bled_increase_brightness(void);

void bled_decrease_brightness(void);

void bled_color_next(void);
void bled_color_prev(void);

#endif // BLED_H
