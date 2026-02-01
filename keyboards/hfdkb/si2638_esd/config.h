// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#ifdef BT_MODE_ENABLE
#    define BT_CABLE_PIN B9  // 充电接入时为高
#    define BT_CHARGE_PIN B8 // 充电时为低，充满时为高
#    define RGB_DRIVER_SDB_PIN A15
#    define BT_MODE_SW_PIN C0  // 低电平时
#    define RF_MODE_SW_PIN B11 // 低电平时
#    define INDLED_POWER_PIN C11

#    define USB_SUSPEND_CHECK_ENABLE
#endif

#define ENCODER_DEFAULT_POS 0x3

/* I2C Config for LED Driver */
#define IS31FL3733_DRIVER_COUNT 2
#define IS31FL3733_I2C_ADDRESS_1 0b1110100
#define IS31FL3733_I2C_ADDRESS_2 0b1110111
#define I2C1_SDA_PIN B7
#define I2C1_SCL_PIN B6
#define I2C1_SCL_PAL_MODE 4
#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 400000

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

#define n_rgb_matrix_set_color_all(r, g, b)   \
    do {                                      \
        for (uint8_t i = 16; i <= 17; i++) {  \
            rgb_matrix_set_color(i, r, g, b); \
        }                                     \
    } while (0)

#define llv_rgb_matrix_set_color_all(r, g, b)     \
    do {                                          \
        for (uint8_t i = 0; i <= 97; i++) {       \
            if (i != 16 && i != 17) {             \
                rgb_matrix_set_color(i, r, g, b); \
            }                                     \
        }                                         \
    } while (0)

#define MATRIX_UNSELECT_DRIVE_HIGH
