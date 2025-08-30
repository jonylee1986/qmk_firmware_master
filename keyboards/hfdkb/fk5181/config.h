// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#define BT_MODE_ENABLE

#ifdef BT_MODE_ENABLE
#    define NO_USB_STARTUP_CHECK
#    define ENTRY_STOP_MODE // 超时进入STOP Mode
// #  define BT_MODE_SW_PIN C0 // 高电平时为无线模式
#    define BT_CABLE_PIN B9  // 充电接入时为高
#    define BT_CHARGE_PIN B8 // 充电时为低，充满时为高
#    define WS2812_EN_PIN B7 // WS2812电源

#    define INDLED_BT1_PIN A8
#    define INDLED_BT2_PIN A15
#    define INDLED_BT3_PIN C11
#    define INDLED_VOL_PIN D2
#endif

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
