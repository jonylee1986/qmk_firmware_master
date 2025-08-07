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

#pragma once

#include <stdint.h>
#include <stdbool.h>


void uart3_init(uint32_t baud);

void uart3_write(uint8_t data);

uint8_t uart3_read(void);

void uart3_transmit(const uint8_t *data, uint16_t length);

void uart3_receive(uint8_t *data, uint16_t length);

bool uart3_available(void);

void uart3_stop(void);

void uart3_start(void);
