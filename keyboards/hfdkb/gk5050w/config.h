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

#define MULTI_MODE_ENABLE

/* Multimode config */
#ifdef MULTI_MODE_ENABLE
#    define BT_CABLE_PIN B8
#    define BT_CHARGE_PIN B9

// #    define BT_MODE_SW_PIN C13
#    define RF_MODE_SW_PIN C14
#endif

/* LED Indicate index */
#define LED_BT1 28
#define LED_BT2 27
#define LED_BT3 26
#define LED_2G 25
#define LED_USB 24
#define LED_CAPS_LOCK 58
#define LED_WIN_LOCK 81

/* LED Indicator pin */
#define LED_CHRG_PIN B13
#define LED_DOWN_PIN A8
#define LED_UNUSED_PIN B12

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

/* RGB matrix config */
#define RGB_MATRIX_SDB_PIN B7

/* EC11 encoder config */

#define USB_SUSPEND_CHECK_ENABLE
