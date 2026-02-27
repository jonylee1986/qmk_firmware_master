// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* RGB Matrix */

/* SPI Config for spi flash*/

/* Bootmagic Lite key configuration */

#ifdef BT_MODE_ENABLE
#    define BT_HOST1_NAME "Meaw-R BT$"
#    define BT_HOST2_NAME "Meaw-R BT$"
#    define BT_HOST3_NAME "Meaw-R BT$"

// #    define USB_CHECK_SUSPEND_STATE
#endif
