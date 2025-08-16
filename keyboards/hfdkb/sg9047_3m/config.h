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
#    define MM_BT_HOST1_NAME "R85Ultra-$"
#    define MM_BT_HOST2_NAME "R85Ultra-$"
#    define MM_BT_HOST3_NAME "R85Ultra-$"

// Indicate index of mm device
#    define MM_BLINK_HOST1_INDEX 26
#    define MM_BLINK_HOST2_INDEX 25
#    define MM_BLINK_HOST3_INDEX 24
#    define MM_BLINK_2G4_INDEX 23
#    define MM_BLINK_USB_INDEX 22

// Multi mode used pins
#    define MM_CABLE_PIN B9
#    define MM_CHARGE_PIN B8
#    define MM_BT_MODE_PIN C0
#    define MM_2G4_MODE_PIN B11

// Indicate color of mm device
#    define MM_BLINK_HOST1_COLOR RGB_BLUE   // 蓝牙1指示灯颜色
#    define MM_BLINK_HOST2_COLOR RGB_CYAN   // 蓝牙2指示灯颜色
#    define MM_BLINK_HOST3_COLOR RGB_PURPLE // 蓝牙3指示灯颜色
#    define MM_BLINK_2G4_COLOR RGB_GREEN    // 2.4G指示灯颜色
#    define MM_BLINK_USB_COLOR RGB_WHITE    // USB指示灯颜色

#    define BAT_LEVEL_DISPLAY_INDEX {26, 25, 24, 23, 22, 21, 20, 19, 18, 17} // 电量显示LED

// Other config
#    define USB_SUSPEND_CHECK_ENABLE
#    define CHRGE_LOW_LEVEL_INDICATOR_INDEX 74
#    define CHRGE_LOW_LEVEL_INDICATOR_COLOR RGB_RED

#    define N_RGB_MATRIX_OFF_ALL()                \
        do {                                      \
            for (uint8_t i = 0; i <= 82; i++) {   \
                rgb_matrix_set_color(i, 0, 0, 0); \
            }                                     \
        } while (0)

#    define N_RGB_MATRIX_SET_COLOR_ALL(r, g, b)   \
        do {                                      \
            for (uint8_t i = 0; i <= 82; i++) {   \
                rgb_matrix_set_color(i, r, g, b); \
            }                                     \
        } while (0)
#endif

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

// LED indicator config
#define CAPS_LOCK_LED_INDEX 56
#define GUI_LOCK_LED_INDEX 81

// RGB Matrix Config
#define RGB_MATRIX_SHUTDOWN_PIN D2

// Encoder Config
#define ENCODER_DEFAULT_POS 0x0
