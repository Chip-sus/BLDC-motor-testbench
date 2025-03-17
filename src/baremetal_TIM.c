/*
 * baremetal_TIM.c
 *
 *  Created on: 5 Dec 2022
 *      Author: e1800961
 *
 *  This file accesses into necessary registers for enabling the timers
 *  and related functions
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include "baremetal_TIM.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

void TIM_ClockEnable2_7(uint32_t num)
{
	RCC->APB1ENR|= (uint32_t)(1 << (num - 2));										// p164
}

void TIM_Init(TIM_TypeDef *TIMx, uint32_t pscVal, uint32_t arrVal, uint32_t CNT)
{
	TIMx->PSC = (uint16_t)(pscVal);
	TIMx->ARR = arrVal;
	TIMx->CNT = CNT;
}

void TIM_EnableCounter(TIM_TypeDef *TIMx)
{
	TIMx->CR1 |= (uint16_t)(1 << 0);
}

void TIM_DisableCounter(TIM_TypeDef *TIMx)
{
	TIMx->CR1 &= ~((uint16_t)(1 << 0));
}

void TIM_EnableInterrupt(TIM_TypeDef *TIMx)
{
	TIMx->DIER |= (uint16_t)(1 << 0);
}

void TIM_DisableInterrupt(TIM_TypeDef *TIMx)
{
	TIMx->DIER &= ~((uint16_t)(1 << 0));
}

// delay ms function using Systick timer
void delay_ms(uint32_t delay)									{
	uint32_t i = 0;
	SysTick->LOAD = 32000 - 1; 						// 32 000 000 = 1s so 32 000 = 1 ms
	SysTick->VAL = 0;
	SysTick->CTRL = 5; 								// enable counter, use processor clock, M3 Generic User Guide p. 159

	while(i < delay)
	{
		//CTRL register bit 16 returns 1 if timer counted to 0 since last time this was read.
		while(!((SysTick->CTRL) & 0x10000)){} 		// M3 Generic User Guide p. 159
		i++;
	}
}
