/**
 * @file bt_task.h
 * @brief
 * @author Joy chang.li@westberrytech.com
 * @version 1.1.0
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022 Westberry Technology (ChangZhou) Corp., Ltd
 */

#pragma once

#include "common/bts_lib.h"
#include "host.h"
#include "keycode.h"
#include "keyboard.h"
#include "mousekey.h"
#include "programmable_button.h"
#include "command.h"
#include "led.h"
#include "action_layer.h"
#include "action_tapping.h"
#include "action_util.h"
#include "action.h"
#include "wait.h"
#include "keycode_config.h" // #define BT_DEBUG_MODE

// #define BT_DEBUG_MODE

// ===========================================
// Timing Constants
// ===========================================
#define ENTRY_STOP_TIMEOUT 100 // ms
// #define ENTRY_STOP_TIMEOUT (30 * 60000) // ms

#define LONG_PRESS_DURATION_MS (3 * 1000)
#define BT_INIT_DELAY_MS 2000
#define TASK_UPDATE_INTERVAL_MS 1
#define BLINK_INTERVAL_MS 500
#define CHARGING_DISPLAY_DURATION_MS 2000
#define BATTERY_QUERY_INTERVAL_MS 10000
#define RGB_TEST_CYCLE_DURATION_MS 1000

// ===========================================
// Hardware Constants
// ===========================================
#define LOW_BATTERY_THRESHOLD 20
#define BATTERY_LED_COUNT 10
#define RGB_TEST_COLOR_COUNT 4
#define USB_CONN_BLINK_COUNT 60

// ===========================================
// Device Type Enums
// ===========================================
typedef enum {
    INDICATOR_OFF        = 0,
    INDICATOR_PAIRING    = 1,
    INDICATOR_CONNECTING = 2,
    INDICATOR_CONNECTED  = 3,
    INDICATOR_DISABLED   = 4,
} indicator_state_t;

extern indicator_state_t indicator_status;

typedef union {
    uint32_t raw;
    struct {
        uint8_t devs;
        uint8_t last_devs;
        uint8_t after_sw_last_devs;
        uint8_t bled_color : 4;         // 颜色枚举（最多16种颜色，4位足够）
        uint8_t bled_mode : 3;          // 灯效模式（BLED_MODE_ON等，2位足够）
        bool    sleep_mode_enabled : 1; // 是否启用睡眠模式
    };
} dev_info_t;

extern dev_info_t dev_info;
extern bts_info_t bts_info;

/**
 * @brief bluetooth 初始化函数
 * @param None
 * @return None
 */
void bt_init(void);

/**
 * @brief bluetooth交互任务
 * @param None
 * @return None
 */
void bt_task(void);

/**
 * @brief 处理和BT相关的按键
 * @param keycode: 键值
 * @param record: 记录值
 * @return None
 */
bool process_record_bt(uint16_t keycode, keyrecord_t *record);

/**
 * @brief rgb指示灯任务
 * @param None
 * @return None
 */
bool bt_indicator_rgb(uint8_t led_min, uint8_t led_max);

/**
 * @brief 切换工作模式
 * @param None
 * @return None
 */
void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset);
