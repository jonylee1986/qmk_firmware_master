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

#include "cw2017.h"
#include "i2c_master.h"
#include "wait.h"
#include "debug.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/* QMK I2C wrapper functions */
static inline int cw_read(uint8_t reg, uint8_t *data) {
    i2c_status_t status = i2c_read_register(CW2017_ADDR, reg, data, 1, CW2017_I2C_TIMEOUT);
    return (status != I2C_STATUS_SUCCESS) ? 1 : 0;
}

static inline int cw_write(uint8_t reg, uint8_t *data) {
    i2c_status_t status = i2c_write_register(CW2017_ADDR, reg, data, 1, CW2017_I2C_TIMEOUT);
    return (status != I2C_STATUS_SUCCESS) ? 1 : 0;
}

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/
#define SIZE_BATINFO 80
#define NO_START_VERSION 160

STRUCT_CW_BATTERY cw_bat;
//**************************global**********************************/
static unsigned char cw_bat_config_info[SIZE_BATINFO] = {
    // 3200mA
    // 0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA3,0xBB,
    // 0xC0,0xE4,0x9C,0x95,0xDD,0xD5,0xC6,0xFF,0xFF,0xE0,
    // 0xBE,0xA4,0x80,0x51,0x43,0x38,0x29,0x9D,0x8C,0xD0,
    // 0xBD,0xBB,0xB8,0xB4,0xAE,0xAB,0xB4,0xCB,0xA9,0x95,
    // 0x8B,0x85,0x7E,0x77,0x6F,0x66,0x69,0x76,0x73,0x7E,
    // 0x81,0x8F,0x91,0x43,0x69,0x3C,0x00,0x00,0x90,0x02,
    // 0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x00,0x00,0x00,
    // 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA3,

    // 4000mA
    // 0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB2,0xC1,
    // 0xB5,0xA4,0x9B,0x96,0xF1,0xEA,0xEA,0xCE,0xBA,0x97,
    // 0x79,0x64,0x56,0x4C,0x48,0x48,0x47,0xA3,0xA4,0xD2,
    // 0x0A,0xFF,0xDE,0x8D,0x58,0x21,0x1F,0x2D,0x4A,0x82,
    // 0xBA,0xCE,0xAF,0x7A,0x53,0x5C,0x6C,0x77,0x89,0xA5,
    // 0xC6,0xE9,0xFF,0xEE,0xF0,0x11,0x00,0x00,0x90,0x02,
    // 0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x00,0x00,0x00,
    // 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,

    // 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xC0,

    // 0xB8, 0xB0, 0xA0, 0x9A, 0xEA, 0xDE, 0xD6, 0xD5, 0xC1, 0x9B,

    // 0x84, 0x66, 0x57, 0x4D, 0x46, 0x43, 0x3F, 0xA2, 0x99, 0xD1,

    // 0xFA, 0xFF, 0xEE, 0x87, 0x45, 0x23, 0x26, 0x2E, 0x5E, 0x9C,

    // 0xC7, 0xC0, 0x99, 0x81, 0x6B, 0x6F, 0x78, 0x85, 0x9A, 0xC7,

    // 0xF5, 0xF9, 0xE8, 0xD7, 0xD1, 0x15, 0x00, 0x00, 0xAB, 0x02,

    // 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00,

    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA1,

    0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0xBA,

    0xAA, 0x9F, 0x98, 0x94, 0xF8, 0xF5, 0xF5, 0xB9, 0xA5, 0x91,

    0x82, 0x65, 0x59, 0x50, 0x4B, 0x49, 0x47, 0xC7, 0xCC, 0xD3,

    0x21, 0xFF, 0xEE, 0x9E, 0x64, 0x32, 0x41, 0x60, 0x94, 0xBA,

    0xB4, 0x95, 0x8E, 0x96, 0x87, 0x82, 0x89, 0x9F, 0xC7, 0xE4,

    0xE5, 0xD9, 0xDC, 0xE5, 0xD6, 0x10, 0x00, 0x00, 0x99, 0x02,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB,
};

// void delay_us(unsigned char us);
unsigned char cw2017_restart(void) {
    int           ret;
    unsigned char reg_val = MODE_SLEEP;

    ret = cw_write(REG_MODE_CONFIG, &reg_val);
    if (ret) {
        return 1;
    }
    cw_bat_init();
    return 0;
}

unsigned char cw2017_restart2(void) {
    int           ret;
    unsigned char reg_val = MODE_SLEEP;

    ret = cw_write(REG_MODE_CONFIG, &reg_val);
    if (ret) {
        return 1;
    }
    cw_bat_init();
    return 0;
}

int cw_get_version(void) {
    uint8_t       ret     = 0;
    unsigned char reg_val = 0;
    int           version = 0;
    ret                   = cw_read(REG_VERSION, &reg_val);
    if (ret) {
        return -1;
    }
    version = reg_val;
    /*printf("version = %d\n", version);*/
    return version;
}

long cw_get_vol(void) {
    uint8_t       ret        = 0;
    unsigned char reg_val[2] = {0, 0};
    unsigned int  ad_buff    = 0;

    ret = cw_read(REG_VCELL_H, &reg_val[0]);
    if (ret) {
        return -1;
    }
    ret = cw_read(REG_VCELL_L, &reg_val[1]);
    if (ret) {
        return -1;
    }
    ad_buff = (reg_val[0] << 8) + reg_val[1];
    ad_buff = ad_buff * 5 / 16;

    return (ad_buff);
}

#define UI_FULL 100
#define DECIMAL_MAX 80
#define DECIMAL_MIN 20

int cw_get_capacity(void) {
    uint8_t       ret         = 0;
    unsigned char reg_val     = 0;
    unsigned int  soc         = 0;
    unsigned int  soc_decimal = 0;
    static int    reset_loop  = 0;
    unsigned int  UI_SOC      = 0;
    unsigned int  remainder   = 0;

    ret = cw_read(REG_SOC_INT, &reg_val);
    if (ret) {
        return -1;
    }
    soc = reg_val;
    ret = cw_read(REG_SOC_DECIMAL, &reg_val);
    if (ret) return -1;
    soc_decimal = reg_val;

    if (soc > 100) {
        reset_loop++;
        if (reset_loop > 5) {
            reset_loop = 0;
            cw2017_restart();
        }
        return cw_bat.capacity;
    } else {
        reset_loop = 0;
    }

    // UI_SOC = ((soc * 256 + soc_decimal) * 100)/ (UI_FULL * 256);
    // remainder = (((soc * 256 + soc_decimal) * 100 * 100) / (UI_FULL * 256)) % 100;
    UI_SOC    = soc * 256 + soc_decimal;
    UI_SOC    = ((unsigned long)UI_SOC * 100) / (UI_FULL * 256);
    remainder = soc * 256 + soc_decimal;
    remainder = (((unsigned long)remainder * 100 * 100) / (UI_FULL * 256)) % 100;
    /* printf("soc = %d soc_decimal = %d ui_100 = %d UI_SOC = %d remainder = %d\n",
        soc, soc_decimal, UI_FULL, UI_SOC, remainder); */

    if (UI_SOC >= 100) {
        UI_SOC = 100;
    } else if ((0 == UI_SOC) && (10 >= remainder)) {
        UI_SOC = 0;
    } else {
        /* case 1 : aviod swing */
        if ((UI_SOC >= (cw_bat.capacity - 1)) && (UI_SOC <= (cw_bat.capacity + 1)) && ((remainder > DECIMAL_MAX) || (remainder < DECIMAL_MIN)) && (UI_SOC != 100)) {
            UI_SOC = cw_bat.capacity;
        }
    }

    return UI_SOC;
}

int cw_get_temp(void) {
    uint8_t       ret     = 0;
    unsigned char reg_val = 0;
    int           temp    = 0;

    ret = cw_read(REG_TEMP, &reg_val);
    if (ret) {
        return -1;
    }

    temp = (((int)reg_val * 10) / 2) - 400;

    return temp;
}

unsigned char cw2017_enable(void) {
    int           ret;
    unsigned char reg_val = MODE_DEFAULT;

    /*printf("cw2017_enable!!!\n");*/
    ret = cw_write(REG_MODE_CONFIG, &reg_val);
    if (ret) {
        return 1;
    }
    wait_ms(100);
    reg_val = MODE_SLEEP;
    ret     = cw_write(REG_MODE_CONFIG, &reg_val);
    if (ret) {
        return 1;
    }
    wait_ms(100);
    reg_val = MODE_NORMAL;
    ret     = cw_write(REG_MODE_CONFIG, &reg_val);
    if (ret) {
        return 1;
    }
    wait_ms(100);
    return 0;
}

unsigned char cw_update_config_info(void) {
    int           ret         = 0;
    unsigned char i           = 0;
    unsigned char reg_val     = 0;
    unsigned char reg_val_dig = 0;
    int           count       = 0;

    /* update new battery info */
    for (i = 0; i < SIZE_BATINFO; i++) {
        reg_val = cw_bat_config_info[i];
        ret     = cw_write(REG_BATINFO + i, &reg_val);
        if (ret) {
            return 1;
        }
    }

    ret = cw_read(REG_SOC_ALERT, &reg_val);
    if (ret) {
        return 1;
    }
    reg_val |= CONFIG_UPDATE_FLG; /* set UPDATE_FLAG */
    ret = cw_write(REG_SOC_ALERT, &reg_val);
    if (ret) {
        return 1;
    }

    ret = cw2017_enable();
    if (ret) {
        return 1;
    }

    while (cw_get_version() == NO_START_VERSION) {
        wait_ms(100); // sleep 100 ms
        count++;
        if (count > 10) break;
    }

    for (i = 0; i < 30; i++) {
        wait_ms(100); // sleep 100 ms
        ret = cw_read(REG_SOC_INT, &reg_val);
        ret = cw_read(REG_SOC_INT + 1, &reg_val_dig);
        if (ret)
            return 1;
        else if (reg_val <= 100)
            break;
    }

    return 0;
}

unsigned char cw_init(void) {
    int           ret;
    int           i;
    unsigned char reg_val    = MODE_NORMAL;
    unsigned char config_flg = 0;

    // i2c_init();

    ret = cw_read(REG_MODE_CONFIG, &reg_val);
    if (ret) {
        return 1;
    }

    ret = cw_read(REG_SOC_ALERT, &config_flg);
    if (ret) {
        return 1;
    }

    if ((reg_val != MODE_NORMAL) || ((config_flg & CONFIG_UPDATE_FLG) == 0x00)) {
        /*printf("update flag is 0, need to update config!");*/
        ret = cw_update_config_info();
        if (ret) {
            return ret;
        }
    } else {
        for (i = 0; i < SIZE_BATINFO; i++) {
            ret = cw_read(REG_BATINFO + i, &reg_val);
            if (ret) {
                return 1;
            }

            /*printf("r reg[%2X] = %2X", REG_BATINFO +i, reg_val);*/
            if (cw_bat_config_info[i] != reg_val) {
                break;
            }
        }
        if (i != SIZE_BATINFO) {
            //"update flag for new battery info need set"
            ret = cw_update_config_info();
            if (ret) {
                return ret;
            }
        }
    }
    return 0;
}

unsigned int cw_init_config(void) {
    int           ret             = 0;
    unsigned char reg_gpio_config = 0;
    unsigned char athd            = 0;
    unsigned char reg_val         = 0;

    /*IC config*/
    cw_bat.int_config = GPIO_CONFIG_MIN_TEMP | GPIO_CONFIG_MAX_TEMP | GPIO_CONFIG_SOC_CHANGE;
    cw_bat.soc_alert  = ATHD;
    cw_bat.temp_max   = DEFINED_MAX_TEMP;
    cw_bat.temp_min   = DEFINED_MIN_TEMP;

    reg_gpio_config = cw_bat.int_config;

    ret = cw_read(REG_SOC_ALERT, &reg_val);
    if (ret) {
        return ret;
    }
    athd = reg_val & CONFIG_UPDATE_FLG; // clear athd
    athd = athd | cw_bat.soc_alert;

    if (reg_gpio_config & GPIO_CONFIG_MAX_TEMP_MARK) {
        reg_val = (cw_bat.temp_max + 400) * 2 / 10;
        ret     = cw_write(REG_TEMP_MAX, &reg_val);
        if (ret) {
            return ret;
        }
    }
    if (reg_gpio_config & GPIO_CONFIG_MIN_TEMP_MARK) {
        reg_val = (cw_bat.temp_min + 400) * 2 / 10;
        ret     = cw_write(REG_TEMP_MIN, &reg_val);
        if (ret) {
            return ret;
        }
    }
    ret = cw_write(REG_GPIO_CONFIG, &reg_gpio_config);
    if (ret) {
        return ret;
    }
    ret = cw_write(REG_SOC_ALERT, &athd);
    if (ret) {
        return ret;
    }

    return 0;
}

unsigned int cw_update_data(void) {
#ifdef CW2017_EN
    /*IC value*/
    cw_bat.voltage  = cw_get_vol();
    cw_bat.capacity = cw_get_capacity();
    cw_bat.temp     = cw_get_temp();
#endif
    /* printf("vol = %d  cap = %d temp = %d\n",
        cw_bat.voltage, cw_bat.capacity, cw_bat.temp); */
    return 0;
}

unsigned int cw_init_data(void) {
    unsigned char reg_val   = 0;
    int           real_SOC  = 0;
    int           digit_SOC = 0;
    int           UI_SOC    = 0;
    int           ret       = 0;

    /*IC value*/
    cw_bat.version = cw_get_version();
    cw_bat.voltage = cw_get_vol();

    cw_bat.temp = cw_get_temp();

    ret = cw_read(REG_SOC_INT, &reg_val);
    if (ret) {
        return 1;
    }
    real_SOC = reg_val;

    ret = cw_read(REG_SOC_INT + 1, &reg_val);
    if (ret) {
        return 1;
    }
    digit_SOC = reg_val;

    // UI_SOC = ((real_SOC * 256 + digit_SOC) * 100)/ (UI_FULL*256);
    UI_SOC = real_SOC * 256 + digit_SOC;
    UI_SOC = ((unsigned long)UI_SOC * 100) / (UI_FULL * 256);

    /* printf("[cw_init_data]: real_SOC = %d  digit_SOC = %d\n", real_SOC, digit_SOC); */
    if (UI_SOC >= 100) {
        UI_SOC = 100;
    }

    cw_bat.capacity = UI_SOC;

    /* printf("ver = %d vol = %d  cap = %d temp = %d\n",
        cw_bat.version, cw_bat.voltage, cw_bat.capacity, cw_bat.temp); */

    // int volt_id;
    cw_bat.volt_id = 0;

    /*Get from charger power supply*/
    cw_bat.charger_mode = 0;

    /*Mark for change cw_bat power_supply*/
    // int change;
    return 0;
}

/*
static void update_alt(void)
{
    signed int alt;
    alt = cw_get_alt();
    if ((rrt >= 0) && (cw_bat.alt != alt))
    {
        cw_bat.alt = (unsigned int)alt;
    }
}
*/

/*
static void cw_bat_gpio_init(void)
{

     usb_det_pin -- init
     alt_pin  -- init

     return 0;
}
*/

///////////////////////////////////////MCU?a???3????????????????.//////////////////////////////////////
unsigned char cw_bat_init(void) {
    unsigned char ret;
    unsigned char loop = 0;
    // cw_bat_gpio_init();

    i2c_init();

    ret = cw_init();
    while ((loop++ < 3) && (ret != 0)) {
        ret = cw_init();
    }

    ret = cw_init_config();
    if (ret) {
        return ret;
    }

    ret = cw_init_data();
    if (ret) {
        return ret;
    }

    return ret;
}
