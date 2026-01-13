#pragma once

#include "wb32fq95xx.h"

#include <stdbool.h>
#include "quantum.h"

#define CR_WDGA_Set ((uint32_t)0x00000080)
#define CFR_WDGTB_Mask ((uint32_t)0xFFFFFE7F)
#define CFR_W_Mask ((uint32_t)0xFFFFFF80)
#define BIT_Mask ((uint8_t)0x7F)
#define WWDG_Prescaler_1 ((uint32_t)0x00000000)
#define WWDG_Prescaler_2 ((uint32_t)0x00000080)
#define WWDG_Prescaler_4 ((uint32_t)0x00000100)
#define WWDG_Prescaler_8 ((uint32_t)0x00000180)

void WWDG_SetCounter(uint8_t Counter);
void WWDG_Enable(uint8_t Counter);
void WWDG_SetWindowValue(uint8_t WindowValue);
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler);
void WWDG_Init(void);
void WWDG_Deinit(void);
void Init_WWDG(void);
void Disable_WWDG(void);
