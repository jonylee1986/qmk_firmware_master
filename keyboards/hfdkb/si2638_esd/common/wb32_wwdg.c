#include "hal.h"
#include "wb32_wwdg.h"

static volatile bool is_initialised = false;

volatile uint8_t wb32_wwdg_started(void) {

    return is_initialised;
}

void wb32_wwdg_start(void) {

    if (!is_initialised) {
        is_initialised = true;
        rccEnableWWDG();
        rccResetWWDG();

        WWDG_SetPrescaler(WWDG_Prescaler_8);
        WWDG_SetWindowValue(0x7F); // maximum
        WWDG_Enable(127);
    }
}

void wb32_wwdg_stop(void) {

    if (is_initialised) {
        is_initialised = false;
        rccResetWWDG();
        rccDisableWWDG();
    }
}

void shutdown_user(void) {
    wb32_wwdg_stop();
}

void mcu_reset(void) {
    wb32_wwdg_stop();
    NVIC_SystemReset();
}

/**
 * @brief  Sets the WWDG Prescaler.
 * @param  WWDG_Prescaler: specifies the WWDG Prescaler.
 *   This parameter can be one of the following values:
 *     @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK2/4096)/1
 *     @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK2/4096)/2
 *     @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK2/4096)/4
 *     @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK2/4096)/8
 * @retval None
 */
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler) {
    uint32_t tmpreg = 0;

    /* Clear WDGTB[1:0] bits */
    tmpreg = WWDG->CFR & CFR_WDGTB_Mask;
    /* Set WDGTB[1:0] bits according to WWDG_Prescaler value */
    tmpreg |= WWDG_Prescaler;
    /* Store the new value */
    WWDG->CFR = tmpreg;
}

/**
 * @brief  Sets the WWDG window value.
 * @param  WindowValue: specifies the window value to be compared to the downcounter.
 *   This parameter value must be lower than 0x80.
 * @retval None
 */
void WWDG_SetWindowValue(uint8_t WindowValue) {
    volatile uint32_t tmpreg = 0;

    /* Clear W[6:0] bits */

    tmpreg = WWDG->CFR & CFR_W_Mask;

    /* Set W[6:0] bits according to WindowValue value */
    tmpreg |= WindowValue & (uint32_t)BIT_Mask;

    /* Store the new value */
    WWDG->CFR = tmpreg;
}

/**
 * @brief  Enables the WWDG Early Wakeup interrupt(EWI).
 * @param  None
 * @retval None
 */
void WWDG_EnableIT(void) {
    *(volatile uint32_t *)CFR_EWI_BB = (uint32_t)1;
}

/**
 * @brief  Sets the WWDG counter value.
 * @param  Counter: specifies the watchdog counter value.
 *   This parameter must be a number between 0x40 and 0x7F.
 * @retval None
 */
void WWDG_SetCounter(uint8_t Counter) {
    /* Write to T[6:0] bits to configure the counter value, no need to do
       a read-modify-write; writing a 0 to WDGA bit does nothing */
    WWDG->CR = Counter & BIT_Mask;
}

/**
 * @brief  Enables WWDG and load the counter value.
 * @param  Counter: specifies the watchdog counter value.
 *   This parameter must be a number between 0x40 and 0x7F.
 * @retval None
 */
void WWDG_Enable(uint8_t Counter) {
    WWDG->CR = CR_WDGA_Set | Counter;
}

/**
 * @brief  Checks whether the Early Wakeup interrupt flag is set or not.
 * @param  None
 * @retval The new state of the Early Wakeup interrupt flag (SET or RESET)
 */
uint32_t WWDG_GetFlagStatus(void) {
    return (uint32_t)(WWDG->SR);
}

/**
 * @brief  Clears Early Wakeup interrupt flag.
 * @param  None
 * @retval None
 */
void WWDG_ClearFlag(void) {
    WWDG->SR = (uint32_t)0;
}
