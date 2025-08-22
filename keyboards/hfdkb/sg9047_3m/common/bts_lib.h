/* Copyright (C) 2023 Westberry Technology (ChangZhou) Corp., Ltd
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

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define BTS_LIB_VERSION 1.1.1

typedef enum {
    DEVS_USB = 0,
    DEVS_HOST1,
    DEVS_HOST2,
    DEVS_HOST3,
    DEVS_2_4G,
    DEVS_HOST4,
    DEVS_HOST5,
} devs_t;

typedef enum {
    v_usb          = 0x11, // USB mode
    v_2_4g         = 0x30, // 2.4G mode
    v_host1        = 0x31, // Bluetooth 1
    v_host2        = 0x32, // Bluetooth 2
    v_host3        = 0x33, // Bluetooth 3
    v_host4        = 0x34, // Bluetooth 4
    v_host5        = 0x35, // Bluetooth 5
    v_pair         = 0x51, // Wireless pairing
    v_clear        = 0x52, // Clear pairing info
    v_query_vol    = 0x53, // Query battery level
    v_en_sleep_bt  = 0x55, // Allow Bluetooth sleep for 30 minutes
    v_dis_sleep_bt = 0x56, // Disable Bluetooth sleep
    v_en_sleep_wl  = 0x57, // Allow 2.4G sleep for 30 minutes
    v_dis_sleep_wl = 0x58, // Disable 2.4G sleep
    v_ota          = 0x81, // OTA update (wireless)
    v_fixed_freq   = 0x82, // Enter fixed-frequency mode
} vbs_t;

typedef struct {
    bool    sleeped;        // Sleeping
    bool    low_vol;        // Low voltage
    bool    low_vol_offed;  // Low-voltage shutdown
    bool    normal_vol;     // Normal voltage
    bool    pairing;        // Pairing in progress
    bool    paired;         // Paired
    bool    come_back;      // Reconnecting state
    bool    come_back_err;  // Reconnect failed
    bool    mode_switched;  // Mode switched
    uint8_t pvol;           // Battery percentage
    uint8_t indictor_rgb_s; // RGB indicator state
} bt_info_t;

/**
 * @brief  BTS Init structure definition
 */
typedef struct {
    const char *bt_name[5];
    bt_info_t   bt_info;
    void (*uart_init)(uint32_t);
    uint8_t (*uart_read)(void);
    void (*uart_transmit)(const uint8_t *, uint16_t);
    void (*uart_receive)(uint8_t *, uint16_t);
    bool (*uart_available)(void);
    uint32_t (*timer_read32)(void);
} bts_info_t;

void bts_init(bts_info_t *info);
void bts_task(devs_t dev_state); // Must be called every 1ms
bool bts_process_keys(uint16_t keycode, bool pressed, devs_t dev_state, bool no_gui);

bool    bts_send_fn(bool pressed);
bool    bts_send_name(devs_t host);
bool    bts_send_vendor(vbs_t cmd);
bool    bts_send_mouse_report(uint8_t *report);
void    bts_test_report_rate_task(void);
char   *bts_get_version(void);
uint8_t bts_is_busy(void);

void lp_system_sleep(void);

void bts_set_nkro(bool state);
bool bts_get_nkro(void);
