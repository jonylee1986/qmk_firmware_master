// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#ifdef BT_MODE_ENABLE
#    define BT_HOST1_NAME "Metafor Air - BT$"
#    define BT_HOST2_NAME "Metafor Air - BT$"
#    define BT_HOST3_NAME "Metafor Air - BT$"

/* Multi mode used pins */
#    define BT_CABLE_PIN B8
#    define BT_CHARGE_PIN B9
#    define BT_MODE_SW_PIN C13
#    define RF_MODE_SW_PIN C14
#    define RGB_DRIVER_SDB_PIN B1

/* Indicator index of mm device */
#    define BT_HOST1_INDEX 3
#    define BT_HOST2_INDEX 2
#    define BT_HOST3_INDEX 1
#    define BT_HOST4_INDEX 0xFF
#    define BT_HOST5_INDEX 0xFF
#    define BT_2G4_INDEX 0
#    define BT_USB_INDEX 5

/* Indicator color of mm device */
#    define BT_HOST1_COLOR RGB_BLUE    // Host1 color
#    define BT_HOST2_COLOR RGB_CYAN    // Host2 color
#    define BT_HOST3_COLOR RGB_MAGENTA // Host3 color
#    define BT_HOST4_COLOR RGB_BLACK   // Host4 color
#    define BT_HOST5_COLOR RGB_BLACK   // Host5 color
#    define BT_2G4_COLOR RGB_WHITE     // 2.4G color
#    define BT_USB_COLOR RGB_WHITE     // USB color

/* Others */
#    define KEY_NUM 6
#    define CW2017_EN
#endif

/* I2C Config for LED Driver */
#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 1000000

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_effect_off

// clang-format off
#if 0
#define CW2017_BATTERY_PROFILE { \
    0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0xBA, \
    0xAA, 0x9F, 0x98, 0x94, 0xF8, 0xF5, 0xF5, 0xB9, 0xA5, 0x91, \
    0x82, 0x65, 0x59, 0x50, 0x4B, 0x49, 0x47, 0xC7, 0xCC, 0xD3, \
    0x21, 0xFF, 0xEE, 0x9E, 0x64, 0x32, 0x41, 0x60, 0x94, 0xBA, \
    0xB4, 0x95, 0x8E, 0x96, 0x87, 0x82, 0x89, 0x9F, 0xC7, 0xE4, \
    0xE5, 0xD9, 0xDC, 0xE5, 0xD6, 0x10, 0x00, 0x00, 0x99, 0x02, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB  \
}
#endif
