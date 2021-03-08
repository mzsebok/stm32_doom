/*
 * systime.c
 */

#include "systime.h"
#include "tim.h"
#include "stm32f7xx_hal_tim.h"

static uint32_t mTimerOverflow = 0;

void systime_overflow(void)
{
    mTimerOverflow++;
}

void systime_init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

uint64_t systime_get(void)
{
    return (((uint64_t)mTimerOverflow << 32) + (uint64_t)__HAL_TIM_GET_COUNTER(&htim2));
}

