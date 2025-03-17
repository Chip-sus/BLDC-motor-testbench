/*
 * baremetal_TIM.h
 *
 *  Created on: 5 Dec 2022
 *      Author: hgiah
 */

#ifndef BAREMETAL_TIM_H_
#define BAREMETAL_TIM_H_

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include <stddef.h>
#include "stm32l1xx.h"

/******************************************************************************
 *	Typedef
 *****************************************************************************/

/******************************************************************************
 *	Define
 *****************************************************************************/

/******************************************************************************
 *	Macro
 *****************************************************************************/

/******************************************************************************
 *	Function prototypes
 *****************************************************************************/

void TIM_ClockEnable2_7(uint32_t num);
void TIM_Init(TIM_TypeDef *TIMx, uint32_t pscVal, uint32_t arrVal, uint32_t CNT);
void TIM_EnableCounter(TIM_TypeDef *TIMx);
void TIM_DisableCounter(TIM_TypeDef *TIMx);
void TIM_EnableInterrupt(TIM_TypeDef *TIMx);
void TIM_DisableInterrupt(TIM_TypeDef *TIMx);
void delay_ms(unsigned long delay);

#endif /* BAREMETAL_TIM_H_ */
