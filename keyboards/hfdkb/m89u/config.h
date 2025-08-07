// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#ifdef MULTIMODE_ENABLE
#    define MM_BT_HOST1_NAME "FC220TP_1"
#    define MM_BT_HOST2_NAME "FC220TP_2"
#    define MM_BT_HOST3_NAME "FC220TP_3"

#    define MM_CABLE_PIN B8
#    define MM_CHARGE_PIN B9

#    define RGB_DRIVER_SDB_PIN A15

#    define MM_MODE_SW_PIN A1

// Indicate index of mm device
#    define MM_BLINK_HOST1_INDEX 16
#    define MM_BLINK_HOST2_INDEX 17
#    define MM_BLINK_HOST3_INDEX 18
#    define MM_BLINK_2G4_INDEX 12
#    define MM_BLINK_USB_INDEX 13

// Indicate color of mm device
#    define MM_BLINK_HOST1_COLOR RGB_WHITE // 蓝牙1指示灯颜色
#    define MM_BLINK_HOST2_COLOR RGB_WHITE // 蓝牙2指示灯颜色
#    define MM_BLINK_HOST3_COLOR RGB_WHITE // 蓝牙3指示灯颜色
#    define MM_BLINK_2G4_COLOR RGB_WHITE   // 2.4G指示灯颜色
#    define MM_BLINK_USB_COLOR RGB_WHITE   // USB指示灯颜色

#    define USB_SUSPEND_CHECK_ENABLE

#    define MM_BAT_LEVEL_INDICATE_INDEX {16, 17, 18, 12, 13, 14, 8, 9, 10, 20}
#endif

#define NUM_LOCK_INDICATE_INDEX 22

/* I2C Config for LED Driver */
#define SNLED27351_I2C_ADDRESS_1 SNLED27351_I2C_ADDRESS_GND
// #define SNLED27351_CURRENT_TUNE {0x70, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0x70, 0xFF, 0xFF}

#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 400000

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

#define USB_SUSPEND_CHECK_ENABLE
