/* Copyright (C) 2023 jonylee@hfd
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

#include <stdint.h>
#include <stdbool.h>

/*_____ I N C L U D E S ____________________________________________________*/
/*_____ D E F I N I T I O N S ______________________________________________*/
#define CW2017_ADDR (0x63 << 1)

#ifndef CW2017_I2C_TIMEOUT
#    define CW2017_I2C_TIMEOUT 100
#endif
/*_____ M A C R O S ________________________________________________________*/

/*_____ D E C L A R A T I O N S ____________________________________________*/
#define REG_VERSION 0x00
#define REG_VCELL_H 0x02
#define REG_VCELL_L 0x03
#define REG_SOC_INT 0x04
#define REG_SOC_DECIMAL 0x05
#define REG_TEMP 0x06
#define REG_MODE_CONFIG 0x08
#define REG_GPIO_CONFIG 0x0A
#define REG_SOC_ALERT 0x0B
#define REG_TEMP_MAX 0x0C
#define REG_TEMP_MIN 0x0D
#define REG_VOLT_ID_H 0x0E
#define REG_VOLT_ID_L 0x0F

#define REG_BATINFO 0x10

#define MODE_SLEEP 0x30
#define MODE_NORMAL 0x00
#define MODE_DEFAULT 0xF0
#define CONFIG_UPDATE_FLG 0x80

#define GPIO_CONFIG_MIN_TEMP (0x00 << 4)
#define GPIO_CONFIG_MAX_TEMP (0x00 << 5)
#define GPIO_CONFIG_SOC_CHANGE (0x00 << 6)
#define GPIO_CONFIG_MIN_TEMP_MARK (0x01 << 4)
#define GPIO_CONFIG_MAX_TEMP_MARK (0x01 << 5)
#define GPIO_CONFIG_SOC_CHANGE_MARK (0x01 << 6)
#define ATHD 0x0             // 0x7F
#define DEFINED_MAX_TEMP 450 // 0xAB
#define DEFINED_MIN_TEMP 0   // 0x51

typedef struct tagSTRUCT_CW_BATTERY {
    /*IC value*/
    unsigned int version;
    unsigned int voltage;
    unsigned int capacity;
    int          temp;
    /*IC config*/
    unsigned char int_config;
    unsigned char soc_alert;
    uint16_t      temp_max;
    uint16_t      temp_min;
    /*Get before profile write*/
    unsigned int volt_id;

    /*Get from charger power supply*/
    unsigned int charger_mode;

    /*Mark for change cw_bat power_supply*/
    // unsigned int change;
} STRUCT_CW_BATTERY;
extern STRUCT_CW_BATTERY cw_bat;

/**
 * @brief Initialize CW2017 battery fuel gauge
 * @return 0 on success, non-zero on failure
 */
unsigned char cw_bat_init(void);

/**
 * @brief Update battery data (voltage, capacity, temp)
 * @return 0 on success
 */
unsigned int cw_update_data(void);

/**
 * @brief Get battery voltage in mV
 * @return Voltage in millivolts, or -1 on error
 */
long cw_get_vol(void);

/**
 * @brief Get battery capacity (SOC)
 * @return Capacity 0-100%, or -1 on error
 */
int cw_get_capacity(void);

/**
 * @brief Get battery temperature
 * @return Temperature in 0.1°C units (e.g., 250 = 25.0°C), or -1 on error
 */
int cw_get_temp(void);

/**
 * @brief Get chip version/ID
 * @return Version number, or -1 on error
 */
int cw_get_version(void);

/* Compatibility macros for existing code */
#define cw2017_get_soc() cw_get_capacity()
#define cw2017_get_voltage() cw_get_vol()
#define cw2017_get_temp() cw_get_temp()
#define cw2017_init() cw_bat_init()
