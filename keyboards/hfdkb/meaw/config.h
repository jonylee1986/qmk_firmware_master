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
#    define BT_CABLE_PIN B8  // 充电接入时为高
#    define BT_CHARGE_PIN B9 // 充电时为低，充满时为高
// #    define WS2812_EN_PIN B6 // WS2812电源

#    define BT_BLINK_HOST1_INDEX 0
#    define BT_BLINK_HOST2_INDEX 0
#    define BT_BLINK_HOST3_INDEX 0
#    define BT_BLINK_2G4_INDEX 0
#    define BT_BLINK_USB_INDEX 0

#    define BT_BLINK_HOST1_COLOR {0, 0, 100}
#    define BT_BLINK_HOST2_COLOR {0, 0, 100}
#    define BT_BLINK_HOST3_COLOR {0, 0, 100}
#    define BT_BLINK_2G4_COLOR {0, 100, 0}
#    define BT_BLINK_USB_COLOR {5, 55, 100}

#    define USB_CHECK_SUSPEND_STATE
#endif

/* RGB Matrix */

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
