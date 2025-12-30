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
#    define ENTRY_STOP_MODE  // 超时进入STOP Mode
#    define BT_CABLE_PIN B9  // 充电接入时为高
#    define BT_CHARGE_PIN B8 // 充电时为低，充满时为高
#    define RGB_DRIVER_SDB_PIN A15
// #    define BT_MODE_SW_PIN C0  // 低电平时
#    define RF_MODE_SW_PIN B11 // 低电平时
#endif

/* I2C Config for LED Driver */
/* I2C Config for LED Driver */
#define SNLED27351_I2C_ADDRESS_1 SNLED27351_I2C_ADDRESS_GND
#define SNLED27351_I2C_ADDRESS_2 SNLED27351_I2C_ADDRESS_VDDIO

#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 1000000

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

/* Encoder Config */
#define ENCODER_DEFAULT_POS 0x0

/* Indicator Config */
#define LED_CAPS_IND_INDEX 54
#define LED_WIN_IND_INDEX 91
