// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#ifdef BT_MODE_ENABLE
#    define BT_CABLE_PIN B8  // 充电接入时为高
#    define BT_CHARGE_PIN B9 // 充电时为低，充满时为高
#endif

/* RGB Matrix */
#define WS2812_EN_PIN B7

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
