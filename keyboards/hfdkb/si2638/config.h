// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#define BT_MODE_ENABLE

#ifdef  BT_MODE_ENABLE
#  define NO_USB_STARTUP_CHECK
#  define ENTRY_STOP_MODE // 超时进入STOP Mode
#  define BT_CABLE_PIN B9 // 充电接入时为高
#  define BT_CHARGE_PIN B8 // 充电时为低，充满时为高
#  define RGB_DRIVER_SDB_PIN A15
#  define BT_MODE_SW_PIN C0 // 低电平时
#  define RF_MODE_SW_PIN B11 // 低电平时
# define INDLED_POWER_PIN C11
// #  define INDLED_BT_PIN D2
// #  define INDLED_2_4G_PIN C10
//#  define INDLED_WinLock_PIN A8
#endif

#define ENCODER_DEFAULT_POS 0x3

// /* DIP switch */
// #define DIP_SWITCH_PINS
//     { C11 }

/* I2C Config for LED Driver */
#define DRIVER_COUNT 2
#define DRIVER_ADDR_1 0b1110100
#define DRIVER_ADDR_2 0b1110111
#define I2C1_SDA_PIN B7
#define I2C1_SCL_PIN B6
#define I2C1_SCL_PAL_MODE 4
#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 400000

#define RGB_MATRIX_LED_COUNT 98
#define RGB_DISABLE_WHEN_USB_SUSPENDED 
// #define RGB_MATRIX_DEFAULT_VAL 200

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


#define n_rgb_matrix_set_color_all( r, g, b); for(uint8_t i = 16; i <= 17; i++){\
        rgb_matrix_set_color(i, r, g, b); \
    }


#define llv_rgb_matrix_set_color_all( r, g, b); for(uint8_t i = 0; i <= 97; i++){\
        if(i!=16 && i!=17){ \
            rgb_matrix_set_color(i, r, g, b); \
        }\
    } 

