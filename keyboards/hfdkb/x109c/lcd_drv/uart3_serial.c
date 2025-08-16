/* Copyright 2021
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "uart3.h"

// #include "quantum.h"
#include "gpio.h"
#include "chibios_config.h"
#include <hal.h>

#ifndef UART3_DRIVER
#    define UART3_DRIVER SD3
#endif

#ifndef UART3_TX_PIN
#    define UART3_TX_PIN C10
#endif

#ifndef UART3_TX_PAL_MODE
#    ifdef USE_GPIOV1
#        define UART3_TX_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL
#    else
#        define UART3_TX_PAL_MODE 7
#    endif
#endif

#ifndef UART3_RX_PIN
#    define UART3_RX_PIN C11
#endif

#ifndef UART3_RX_PAL_MODE
#    ifdef USE_GPIOV1
#        define UART3_RX_PAL_MODE PAL_MODE_INPUT
#    else
#        define UART3_RX_PAL_MODE 7
#    endif
#endif

#ifndef UART3_CTS_PIN
#    define UART3_CTS_PIN B13
#endif

#ifndef UART3_CTS_PAL_MODE
#    ifdef USE_GPIOV1
#        define UART3_CTS_PAL_MODE PAL_MODE_INPUT
#    else
#        define UART3_CTS_PAL_MODE 7
#    endif
#endif

#ifndef UART3_RTS_PIN
#    define UART3_RTS_PIN B14
#endif

#ifndef UART3_RTS_PAL_MODE
#    ifdef USE_GPIOV1
#        define UART3_RTS_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL
#    else
#        define UART3_RTS_PAL_MODE 7
#    endif
#endif

#ifndef UART3_CR1
#    define UART3_CR1 0
#endif

#ifndef UART3_CR2
#    define UART3_CR2 0
#endif

#ifndef UART3_CR3
#    define UART3_CR3 0
#endif

#ifndef UART3_WRDLEN
#    define UART3_WRDLEN 3
#endif

#ifndef UART3_STPBIT
#    define UART3_STPBIT 0
#endif

#ifndef UART3_PARITY
#    define UART3_PARITY 0
#endif

#ifndef UART3_ATFLCT
#    define UART3_ATFLCT 0
#endif

#if defined(MCU_KINETIS)
static SerialConfig serialConfig = {SERIAL_DEFAULT_BITRATE};
#elif defined(WB32F3G71xx) || defined(WB32FQ95xx)
static SerialConfig serialConfig = {
    SERIAL_DEFAULT_BITRATE, UART3_WRDLEN, UART3_STPBIT, UART3_PARITY, UART3_ATFLCT,
};
#else
static SerialConfig serialConfig = {
    SERIAL_DEFAULT_BITRATE,
    UART3_CR1,
    UART3_CR2,
    UART3_CR3,
};
#endif

void uart3_init(uint32_t baud) {
    static bool is_initialised = false;

    if (is_initialised) {
        return;
    }
    is_initialised = true;

#if defined(MCU_KINETIS)
        serialConfig.sc_speed = baud;
#else
        serialConfig.speed = baud;
#endif

#if defined(USE_GPIOV1)
    palSetLineMode(UART3_TX_PIN, UART3_TX_PAL_MODE);
    palSetLineMode(UART3_RX_PIN, UART3_RX_PAL_MODE);
#else
    palSetLineMode(UART3_TX_PIN, PAL_MODE_ALTERNATE(UART3_TX_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
    palSetLineMode(UART3_RX_PIN, PAL_MODE_ALTERNATE(UART3_RX_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
#endif
    sdStart(&UART3_DRIVER, &serialConfig);
}

void uart3_write(uint8_t data) {
    sdPut(&UART3_DRIVER, data);
}

uint8_t uart3_read(void) {
    return (uint8_t)sdGet(&UART3_DRIVER);
}

void uart3_transmit(const uint8_t *data, uint16_t length) {
    extern bool LCD_DONT_SEND;
    if (LCD_DONT_SEND)
    {
        return;
    }
    sdWrite(&UART3_DRIVER, data, length);
}

void uart3_receive(uint8_t *data, uint16_t length) {
    sdRead(&UART3_DRIVER, data, length);
}

bool uart3_available(void) {
    return !sdGetWouldBlock(&UART3_DRIVER);
}

void uart3_stop(void) {
    sdStop(&UART3_DRIVER);
    setPinInput(UART3_TX_PIN);
    setPinInput(UART3_RX_PIN);
}

void uart3_start(void) {
    setPinOutputPushPull(UART3_TX_PIN);
    setPinOutputPushPull(UART3_RX_PIN);
    writePinHigh(UART3_RX_PIN);
    writePinHigh(UART3_TX_PIN);
    writePinLow(UART3_TX_PIN);
    writePinLow(UART3_RX_PIN);
    palSetLineMode(UART3_TX_PIN, PAL_MODE_ALTERNATE(UART3_TX_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
    palSetLineMode(UART3_RX_PIN, PAL_MODE_ALTERNATE(UART3_RX_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
    sdStart(&UART3_DRIVER, &serialConfig);
}
