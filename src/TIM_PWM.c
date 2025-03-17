/*
 * TIM_PWM.c
 *
 *  Created on: 9 Dec 2022
 *      Author: e1800961
 *
 *  This section uses timer to send PWM signal for controlling the motor.
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
//#include "baremetal_TIM.h"
#include "baremetal.h"
#include "nucleo_ihm07m1.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

// BLDC motor move function according to hall sensor status
void TIM_PWM(TIM_TypeDef *TIMx, uint32_t PWM_value, uint8_t CC)
{

	uint32_t motor_position = HallSensor_Read();

	uint32_t timer_value = 0;
	timer_value = TIMx->CNT;

	if(timer_value < PWM_value)		// PWM high state
	{
		GPIOA->BSRR |= 0x20;		// LED PA5 for debugging
		switch(motor_position)
		{
			case 1:
				Step1(CC);
				break;
			case 2:
				Step2(CC);
				break;
			case 3:
				Step3(CC);
				break;
			case 4:
				Step4(CC);
				break;
			case 5:
				Step5(CC);
				break;
			case 6:
				Step6(CC);
				break;
		}
	}
	else							// PWM low state
	{
		GPIOA->BSRR |= 0x200000;	// turn PA5 LED off
		Step0();					// disable all output signal
	}

}

uint32_t PWM_ValChange(char received_command, int32_t* ptemp, uint8_t* pccw, uint32_t stepChange)
{
	received_command = USART2_read();

	if(received_command == '+') *ptemp += stepChange;
	else if(received_command == '-') *ptemp -= stepChange;
	if(*ptemp < -10000) *ptemp = -10000;
	if(*ptemp > 10000) *ptemp = 10000;

	if(*ptemp <= 10000 && *ptemp >= 0) *pccw = 0;			// clockwise check
	else if(*ptemp >= -10000 && *ptemp < 0) *pccw = 1;

	return *ptemp;
}
