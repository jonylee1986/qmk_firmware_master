/* Copyright (C) 2022 Westberry Technology (ChangZhou) Corp., Ltd
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

#ifdef MULTIMODE_ENABLE
#    define MM_BT_HOST1_NAME "YUNZII B98 PRO BT"
#    define MM_BT_HOST2_NAME "YUNZII B98 PRO BT"
#    define MM_BT_HOST3_NAME "YUNZII B98 PRO BT"

// Indicate index of mm device
#    define MM_BLINK_HOST1_INDEX 18
#    define MM_BLINK_HOST2_INDEX 19
#    define MM_BLINK_HOST3_INDEX 20
#    define MM_BLINK_2G4_INDEX 21
#    define MM_BLINK_USB_INDEX 22

// Multi mode used pins
#    define MM_CABLE_PIN B9
#    define MM_CHARGE_PIN B8
#    define MM_BT_MODE_PIN C0
#    define MM_2G4_MODE_PIN B11

// Indicate color of mm device
#    define MM_BLINK_HOST1_COLOR RGB_BLUE // Host1 color
#    define MM_BLINK_HOST2_COLOR RGB_BLUE // Host2 color
#    define MM_BLINK_HOST3_COLOR RGB_BLUE // Host3 color
#    define MM_BLINK_2G4_COLOR RGB_CYAN   // 2.4G color
#    define MM_BLINK_USB_COLOR RGB_WHITE  // USB color

#    define BAT_LEVEL_DISPLAY_INDEX {102, 103, 104, 105, 106} // Battery level indicator

#    define USB_SUSPEND_CHECK_ENABLE
#endif

// Win lock indicator
#define LED_LWIN_INDEX 90
// Num lock indicator
#define LED_NUM_INDEX 32

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

/* I2C Config for LED Driver */
#define SNLED27351_I2C_ADDRESS_1 SNLED27351_I2C_ADDRESS_GND
#define SNLED27351_I2C_ADDRESS_2 SNLED27351_I2C_ADDRESS_VDDIO

#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 400000

// RGB Matrix Config
#define RGB_MATRIX_SHUTDOWN_PIN A15

// Encoder Config
#define ENCODER_DEFAULT_POS 0x0
// #define ENCODER_DIRECTION_FLIP
