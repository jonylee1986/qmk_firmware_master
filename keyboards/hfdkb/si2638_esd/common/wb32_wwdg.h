// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#ifndef rccEnableWWDG
#    define rccEnableWWDG() rccEnableAPB2(RCC_APB2ENR_BMX2EN | RCC_APB2ENR_WWDGEN)
#endif
#ifndef rccDisbleWWDG
#    define rccDisableWWDG() rccDisableAPB2(RCC_APB2ENR_WWDGEN)
#endif
#ifndef rccResetWWDG
#    define rccResetWWDG() rccResetAPB2(RCC_APB2ENR_WWDGEN)
#endif

/* ----------- WWDG registers bit address in the alias region ----------- */
#define WWDG_OFFSET (WWDG_BASE - PERIPH_BASE)

/* Alias word address of EWI bit */
#define CFR_OFFSET    (WWDG_OFFSET + 0x04)
#define EWI_BitNumber 0x09
#define CFR_EWI_BB    (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

/* --------------------- WWDG registers bit mask ------------------------ */

/* CR register bit mask */
#define CR_WDGA_Set ((uint32_t)0x00000080)

/* CFR register bit mask */
#define CFR_WDGTB_Mask ((uint32_t)0xFFFFFE7F)
#define CFR_W_Mask     ((uint32_t)0xFFFFFF80)
#define BIT_Mask       ((uint8_t)0x7F)

#define WWDG_Prescaler_1 ((uint32_t)0x00000000)
#define WWDG_Prescaler_2 ((uint32_t)0x00000080)
#define WWDG_Prescaler_4 ((uint32_t)0x00000100)
#define WWDG_Prescaler_8 ((uint32_t)0x00000180)

uint8_t wb32_wwdg_started(void);
void wb32_wwdg_start(void);
void wb32_wwdg_stop(void);
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler);
void WWDG_SetWindowValue(uint8_t WindowValue);
void WWDG_EnableIT(void);
void WWDG_SetCounter(uint8_t Counter);
void WWDG_Enable(uint8_t Counter);
uint32_t WWDG_GetFlagStatus(void);
void WWDG_ClearFlag(void);
