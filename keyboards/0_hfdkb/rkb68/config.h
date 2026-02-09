// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#define BT_MODE_ENABLE

#ifdef BT_MODE_ENABLE
#  define NO_USB_STARTUP_CHECK
#  define ENTRY_STOP_MODE // 超时进入STOP Mode
#  define BT_CABLE_PIN B8 // 充电接入时为高
#  define BT_CHARGE_PIN B9 // 充电时为低，充满时为高
#  define WS2812_EN_PIN B7 // WS2812电源
#  define BT_MODE_SW_PIN C13 // 低电平时
#  define RF_MODE_SW_PIN C14 // 低电平时

#endif

/* RGB Matrix */
#define RGB_DI_PIN C15
#define RGB_MATRIX_LED_COUNT 11
#define RGB_DISABLE_WHEN_USB_SUSPENDED

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
#define WEAR_LEVELING_BACKING_SIZE (4 * 1024)

#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES
