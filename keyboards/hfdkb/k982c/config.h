/* Copyright 2023 @ Keychron(https://www.keychron.com)
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

#ifdef MM_MODE_ENABLE
#    define MM_HOST1_NAME "EWEADN-X75-BT$"
#    define MM_HOST2_NAME "EWEADN-X75-BT$"
#    define MM_HOST3_NAME "EWEADN-X75-BT$"

/* Multi mode used pins */
#    define MM_CABLE_PIN B9
#    define MM_CHARGE_PIN B8
#    define MM_BT_MODE_PIN C0
#    define MM_2G4_MODE_PIN B11

/* Indicator index of mm device */
#    define MM_HOST1_INDEX 30
#    define MM_HOST2_INDEX 31
#    define MM_HOST3_INDEX 32
#    define MM_HOST4_INDEX 0xFF
#    define MM_HOST5_INDEX 0xFF
#    define MM_2G4_INDEX 34
#    define MM_USB_INDEX 0xFF

/* Indicator color of mm device */
#    define MM_HOST1_COLOR RGB_CYAN    // Host1 color
#    define MM_HOST2_COLOR RGB_MAGENTA // Host2 color
#    define MM_HOST3_COLOR RGB_YELLOW  // Host3 color
#    define MM_HOST4_COLOR RGB_BLACK   // Host4 color
#    define MM_HOST5_COLOR RGB_BLACK   // Host5 color
#    define MM_2G4_COLOR RGB_BLUE      // 2.4G color
#    define MM_USB_COLOR RGB_BLACK     // USB color

// #    define BAT_LEVEL_DISPLAY_INDEX {102, 103, 104, 105, 106} // Battery level indicator

#    ifdef NO_USB_STARTUP_CHECK
#        define USB_SUSPEND_CHECK_ENABLE
#    endif
#endif

/* RGB Matrix Driver Configuration */
#define SNLED27351_I2C_ADDRESS_1 SNLED27351_I2C_ADDRESS_GND
#define SNLED27351_I2C_ADDRESS_2 SNLED27351_I2C_ADDRESS_VDDIO

/* Increase I2C speed to 1000 KHz */
// #define I2C1_TIMINGR_PRESC 0U
// #define I2C1_TIMINGR_SCLDEL 3U
// #define I2C1_TIMINGR_SDADEL 0U
// #define I2C1_TIMINGR_SCLH 15U
// #define I2C1_TIMINGR_SCLL 51U
#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 1000000

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
