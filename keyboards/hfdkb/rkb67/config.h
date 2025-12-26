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
#    define ENTRY_STOP_MODE
#    define BT_CABLE_PIN B8
#    define BT_CHARGE_PIN B9
// #    define BT_MODE_SW_PIN C13
// #    define RF_MODE_SW_PIN C14
#    define RGB_DRIVER_SDB_PIN B7
#    define USB_SUSPEND_STATE_CHECK
#endif

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

/* Enable CapsLock LED */
#define CAPS_LOCK_LED_INDEX 43

/* Enable GUI Lock LED */
#define GUI_LOCK_LED_INDEX 65

/* SLED define */
#define SLED_START_INDEX 67
#define SLED_END_INDEX 72
#define SLED_NUM 6

#define KEY_NUM 8
