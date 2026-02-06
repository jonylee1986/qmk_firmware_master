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

#define ENTRY_STOP_TIMEOUT 100 // ms
// #define ENTRY_STOP_TIMEOUT (30 * 60000) // ms

typedef union {
    uint32_t raw;
    struct {
        uint8_t devs;
        uint8_t last_devs;
    };
} dev_info_t;

extern dev_info_t dev_info;
extern bts_info_t bts_info;

bool get_low_vol_off(void);

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
uint8_t bt_indicator_rgb(uint8_t led_min, uint8_t led_max);

/**
 * @brief 切换工作模式
 * @param None
 * @return None
 */
void bt_switch_mode(uint8_t last_mode, uint8_t now_mode, uint8_t reset);
