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

#    define BT_CABLE_PIN B8  // 充电接入时为高
#    define BT_CHARGE_PIN B9 // 充电时为低，充满时为高

// #  define RGB_DRIVER_SDB_PIN A15

#    define BT_MODE_SW_PIN C13 // 低电平时
#    define RF_MODE_SW_PIN C14 // 低电平时

#    define WS2812_EN_PIN B7 // WS2812 Enable pin

// #  define LED_CAPS_LOCK_PIN C10
// #define LED_MAC_OS_PIN B11
// #define LED_WIN_LOCK_PIN B12
#endif

#define ENCODER_DEFAULT_POS 0x0 // Default position of the encoder
#define ENCODER_RESOLUTION 2    // Resolution of the encoder (2 for 2 steps per detent)

// #define WIN_MAC_SWITCH_PIN C0

// #define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB

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
#define RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE

#define LED_BT1 29
#define LED_BT2 30
#define LED_BT3 31
#define LED_2G 32
#define LED_USB 33
