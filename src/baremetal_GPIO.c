/*
 * baremetal_GPIO.c
 *
 *  Created on: 30 Nov 2022
 *      Author: e1800961
 *
 *  This section enables direct access into special function registers related to GPIO ports,
 *  based on the reference manual.
 *
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include "baremetal_GPIO.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

void GPIO_PinInit(GPIO_TypeDef *GPIOx, uint32_t pinNum, uint32_t modeVal, uint32_t altFuncVal)
{
	// Mode register p.184
	GPIOx->MODER &= ~((uint32_t)(0x3 << (pinNum * 2)));						// reset state
	GPIOx->MODER |= (uint32_t)(modeVal << (pinNum * 2));
}

uint32_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t pinNum)
{
	uint32_t bitVal;

	bitVal = (GPIOx->IDR & (uint16_t)(1 << pinNum)) >> pinNum;

	return bitVal;
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t pinNum, uint32_t bitVal)
{
	if (bitVal)
	{
		GPIOx->BSRR |= (uint16_t)(1 << pinNum);
	}
	else
	{
		GPIOx->BRR |= (uint16_t)(1 << pinNum);
	}
}
