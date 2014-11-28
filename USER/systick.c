/****************************************************************************
 *
 *   Copyright (c) 2014-2014 PlayUAV Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PlayUAV nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlayUAV  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include "stm32f10x_tim.h"

static volatile uint32_t sysTickMillis = 0;
static uint32_t sysTickPerUs = 72;

static inline int systick_check_underflow(void)
{
    return SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk;
}

///////////////////////////////////////////////////////////////////////////////
// System Time in Microseconds
///////////////////////////////////////////////////////////////////////////////

unsigned int micros(void)
{
    uint32_t cycle, timeMs;

    do
    {
        timeMs = sysTickMillis;
        cycle = SysTick->VAL;
        __asm volatile("nop");
        __asm volatile("nop");
    }
    while (timeMs != sysTickMillis);

    if (systick_check_underflow())
    {
        timeMs++;
        cycle = SysTick->VAL;
    }

    return (timeMs * 1000) + (SysTick->LOAD + 1 - cycle) / sysTickPerUs;
}

///////////////////////////////////////////////////////////////////////////////
// System Time in Milliseconds
///////////////////////////////////////////////////////////////////////////////

unsigned int millis(void)
{
    return sysTickMillis;
}

///////////////////////////////////////////////////////////////////////////////
// SysTick
///////////////////////////////////////////////////////////////////////////////
static void (*systickUserCallback)(void);

void SysTick_Handler(void)
{
    __disable_irq();
    systick_check_underflow();
    sysTickMillis++;
    __enable_irq();

    if (systickUserCallback)
    {
        systickUserCallback();
    }
}

void SysTickAttachCallback(void (*callback)(void))
{
    systickUserCallback = callback;
}

void InitSysTick(void)
{
    sysTickPerUs = SystemCoreClock / 1000000;
    SysTick_Config(SystemCoreClock / 1000);
    //NVIC_SetPriority(SysTick_IRQn, 0);//set systick interrupt priority, 0 is the highest for all
}
