// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#ifdef WIRELESS_ENABLE
/* POWER */
#    define BT_CABLE_PIN B8
#    define BT_CHARGE_PIN B9
#    define USB_POWER_EN_PIN A14
#    define WL_PWR_SW_PIN C13

/* UART */
#    define UART_TX_PIN A9
#    define UART_TX_PAL_MODE 7
#    define UART_RX_PIN A10
#    define UART_RX_PAL_MODE 7

#    define LPWR_TIMEOUT RGB_MATRIX_TIMEOUT
#    define WLS_KEYBOARD_REPORT_KEYS 6
#endif

/* RGB Matrix */
#define LED_POWER_EN_PIN B7

/* RGB Indicator index */
#define LED_CAPS_LOCK_INDEX 30
#define LED_HOST_BT1_INDEX 1
#define LED_HOST_BT2_INDEX 2
#define LED_HOST_BT3_INDEX 3
#define LED_HOST_2G4_INDEX 0
// #define LED_HOST_USB_INDEX 29
#define LED_HOST_BT1_COLOR {0, 0, 77}
#define LED_HOST_BT2_COLOR {0, 0, 77}
#define LED_HOST_BT3_COLOR {0, 0, 77}
#define LED_HOST_2G4_COLOR {0, 77, 0}

/* Wireless indicator timeout (per mode, ms; 30 seconds by default) */
#ifndef WLS_BT_PAIR_TIMEOUT
#    define WLS_BT_PAIR_TIMEOUT 60000
#endif
#ifndef WLS_BT_RECONNECT_TIMEOUT
#    define WLS_BT_RECONNECT_TIMEOUT 10000
#endif
#ifndef WLS_2G4_PAIR_TIMEOUT
#    define WLS_2G4_PAIR_TIMEOUT 60000
#endif
#ifndef WLS_2G4_RECONNECT_TIMEOUT
#    define WLS_2G4_RECONNECT_TIMEOUT 10000
#endif

/* FLASH */
/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
