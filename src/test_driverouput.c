/*
 * test_driverouput.c
 *
 *  Created on: 20 Nov 2023
 *      Author: e1800961
 *
*   Two out of three solenoids in a combination should have a
*   specific directional  electric current at precise time to drive the motor.
*   This section verify the functionality of the output pins for controlling
*   3-phase motor.
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include <test_driveroutput.h>
#include "baremetal.h"
#include "nucleo_ihm07m1.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

void OUT1combination()
{
	GPIO_WritePin(GPIOC, 10, 0);		// First combination of IN1 and EN1 for OUT1
	GPIO_WritePin(GPIOA, 8, 0);			//						LOW		LOW
	GPIO_WritePin(GPIOA, 5, 1);			// PA5 debugging LED on state
	delay_ms(3000);

	GPIO_WritePin(GPIOC, 10, 1);		// Second combination of IN1 and EN1 for OUT1
	GPIO_WritePin(GPIOA, 8, 0);			//						 LOW	 HIGH
	GPIO_WritePin(GPIOA, 5, 0);			// PA5 off state
	delay_ms(3000);

	GPIO_WritePin(GPIOC, 10, 0);
	GPIO_WritePin(GPIOA, 8, 1);
	GPIO_WritePin(GPIOA, 5, 0);
	delay_ms(3000);

	GPIO_WritePin(GPIOC, 10, 1);
	GPIO_WritePin(GPIOA, 8, 1);
	GPIO_WritePin(GPIOA, 5, 0);
	delay_ms(3000);

}

void HtoSwitch(uint8_t CC)
{
	uint32_t motor_position = HallSensor_Read();
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
