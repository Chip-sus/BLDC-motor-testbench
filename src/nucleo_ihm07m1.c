/*
 * nucleo_ihm07m1.c
 *
 *  Created on: 30 Nov 2022
 *      Author: e1800961
 *
 *  This section initials the necessary pins for enabling the L6230 driver and
 *  commanding the UVW phases based on the read step from the hall sensors.
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include "nucleo_ihm07m1.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

void L6230_Init(void)
{
	/* These pins enable controlling signal (X-NUCLEO-IHM07M1 circuit schematic (4 of 4)) */
	GPIO_PinInit(GPIOC, 10, 1, 0);				// Output mode for PC10
	GPIO_PinInit(GPIOC, 11, 1, 0);				// Output mode for PC11
	GPIO_PinInit(GPIOC, 12, 1, 0);				// Output mode for PC12

	/* These pins send PWM controlling signal (X-NUCLEO-IHM07M1 circuit schematic (4 of 4))*/
	GPIO_PinInit(GPIOA, 8, 1, 0);				// Output mode for PA8
	GPIO_PinInit(GPIOA, 9, 1, 0);				// Output mode for PA9
	GPIO_PinInit(GPIOA, 10, 1, 0);				// Output mode for PA10
}

void HallSensor_Init(void)
{
	/* These pins receive Hall sensor signals (X-NUCLEO-IHM07M1 circuit schematic (4 of 4)) */
	GPIO_PinInit(GPIOA, 15, 0, 0);				// Input mode for PA15, A+ signal
	GPIO_PinInit(GPIOB, 3, 0, 0);				// Input mode for PB3, B+ signal
	GPIO_PinInit(GPIOB, 10, 0, 0);				// Input mode for PB10, Z+ signal
}

uint32_t HallSensor_Read(void)
{
	uint32_t h1h2h3 = 0;

	h1h2h3 |= GPIO_ReadPin(GPIOA, 15) << 2;		// Read PA15 as third bit
	h1h2h3 |= GPIO_ReadPin(GPIOB, 3) << 1;		// Read PB3 as second bit
	h1h2h3 |= GPIO_ReadPin(GPIOB, 10) << 0;		// Read PB10 as first bit

	return h1h2h3;
}

/* For DF45M024053-A2 BLDC motor
 * There are six steps in controlling the motor, as refer in the datasheet. (Wiring Diagram part)
 * And one low-state for PWM signal (step 0).
 */

void Step0(void)
{
//	GPIO_WritePin(GPIOC, 10, 0);
//	GPIO_WritePin(GPIOC, 11, 0);
//	GPIO_WritePin(GPIOC, 12, 0);

	GPIO_WritePin(GPIOA, 8, 0);
	GPIO_WritePin(GPIOA, 9, 0);
	GPIO_WritePin(GPIOA, 10, 0);
}

void Step1(uint8_t CC)
{
	GPIO_WritePin(GPIOC, 10, 0);
	GPIO_WritePin(GPIOC, 11, 1);
	GPIO_WritePin(GPIOC, 12, 1);

	GPIO_WritePin(GPIOA, 8, 0^CC);
	GPIO_WritePin(GPIOA, 9, 1^CC);
	GPIO_WritePin(GPIOA, 10, 0^CC);
}

void Step2(uint8_t CC)
{
	GPIO_WritePin(GPIOC, 10, 1);
	GPIO_WritePin(GPIOC, 11, 1);
	GPIO_WritePin(GPIOC, 12, 0);

	GPIO_WritePin(GPIOA, 8, 1^CC);
	GPIO_WritePin(GPIOA, 9, 0^CC);
	GPIO_WritePin(GPIOA, 10, 0^CC);
}

void Step3(uint8_t CC)
{
	GPIO_WritePin(GPIOC, 10, 1);
	GPIO_WritePin(GPIOC, 11, 0);
	GPIO_WritePin(GPIOC, 12, 1);

	GPIO_WritePin(GPIOA, 8, 1^CC);
	GPIO_WritePin(GPIOA, 9, 0^CC);
	GPIO_WritePin(GPIOA, 10, 0^CC);
}

void Step4(uint8_t CC)
{
	GPIO_WritePin(GPIOC, 10, 1);
	GPIO_WritePin(GPIOC, 11, 0);
	GPIO_WritePin(GPIOC, 12, 1);

	GPIO_WritePin(GPIOA, 8, 0^CC);
	GPIO_WritePin(GPIOA, 9, 0^CC);
	GPIO_WritePin(GPIOA, 10, 1^CC);
}

void Step5(uint8_t CC)
{
	GPIO_WritePin(GPIOC, 10, 1);
	GPIO_WritePin(GPIOC, 11, 1);
	GPIO_WritePin(GPIOC, 12, 0);

	GPIO_WritePin(GPIOA, 8, 0^CC);
	GPIO_WritePin(GPIOA, 9, 1^CC);
	GPIO_WritePin(GPIOA, 10, 0^CC);
}

void Step6(uint8_t CC)
{
	GPIO_WritePin(GPIOC, 10, 0);
	GPIO_WritePin(GPIOC, 11, 1);
	GPIO_WritePin(GPIOC, 12, 1);

	GPIO_WritePin(GPIOA, 8, 0^CC);
	GPIO_WritePin(GPIOA, 9, 0^CC);
	GPIO_WritePin(GPIOA, 10, 1^CC);
}
