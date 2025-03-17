/*
 * test_hallsensors.c
 *
 *  Created on: 3 Dec 2022
 *      Author: e1800961
 *
 *  This program prints out hall sensors data. which can be capture by any terminal application.
 *  USART2 is used.
 *  This test should be executed by manually rotating the motor only, for observation of Hall sensors behaviours
 *  Because of too many interrupts while motor rotating, the program will not adjust output signal in time.
 *
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include <test_hallsensors.h>
#include "baremetal.h"
#include "nucleo_ihm07m1.h"
#include "string.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

void Hallsensor_Interrupt_Init()
{
	SYSCFG->EXTICR[3] &= ~0xF000;
	SYSCFG->EXTICR[3] |= 0x0000;			// select port A (PA15) for EXTI15. p223
	EXTI->IMR |= 0x8000;					// Unmask EXTI15
	EXTI->RTSR |= 0x8000;					// Rising edge trigger
	EXTI->FTSR |= 0x8000;					// Falling edge trigger


	SYSCFG->EXTICR[2] &= ~0x0F00;
	SYSCFG->EXTICR[2] |= 0x0100;			// select port B (PB10) for EXTI10. p223
	EXTI->IMR |= 0x0400;					// Unmask EXTI10
	EXTI->RTSR |= 0x0400;					// Rising edge trigger
	EXTI->FTSR |= 0x0400;					// Falling edge trigger

	SYSCFG->EXTICR[0] &= ~0xF000;
	SYSCFG->EXTICR[0] |= 0x1000;			// select port B (PB3) for EXTI3. p223
	EXTI->IMR |= 0x0008;					// Unmask EXTI3
	EXTI->RTSR |= 0x0008;					// Rising edge trigger
	EXTI->FTSR |= 0x0008;					// Falling edge trigger
}

void HallSensor_PrintVal(char port)
{
	USART2_Init();
	if(port == 'A')
	{
		uint8_t tempA = GPIO_ReadPin(GPIOA, 15);			// NEED CORRECTION
		//char tempA = GPIO_ReadPin(GPIOA, 15);
		//uint8_t buf[]="  H1:";
		char buf[]="  H1:";
		for(int i=0;i<strlen(buf);i++)
		{
			USART_write(buf[i]);
		}
		USART_write(tempA + 48);
	}
	else if (port == 'B')
	{
		char tempB = GPIO_ReadPin(GPIOB, 3);
		char buf[]="  H2:";
		for(int i=0;i<strlen(buf);i++)
		{
			USART_write(buf[i]);
		}
		USART_write(tempB + 48);
	}
	else if(port == 'Z')
	{
		char tempZ = GPIO_ReadPin(GPIOB, 10);
		char buf[]="  H3:";
		for(int i=0;i<strlen(buf);i++)
		{
			USART_write(buf[i]);
		}
		USART_write(tempZ + 48);
	}

}

void HallSensor_PrintCurrentStep()
{
	uint32_t position = HallSensor_Read();
	char temp = (char)position + 48;
	char buf[]="  Step: ";
	for(int i=0;i<strlen(buf);i++)
		{
			USART_write(buf[i]);
		}
	USART_write(temp);
	USART_write('\n');
	USART_write('\r');
}

void UVW_PrintCurrentState()
{
	uint8_t u = GPIO_ReadPin(GPIOA, 8);
	char temp1 = (char)u + 48;
	char buf1[]="  u= ";
	for(int i=0;i<strlen(buf1);i++)
		{
			USART_write(buf1[i]);
		}
	USART_write(temp1);

	uint8_t v = GPIO_ReadPin(GPIOA, 9);
	char temp2 = (char)v + 48;
	char buf2[]="  v= ";
	for(int i=0;i<strlen(buf2);i++)
		{
			USART_write(buf2[i]);
		}
	USART_write(temp2);

	uint8_t w = GPIO_ReadPin(GPIOA, 10);
	char temp3 = (char)w + 48;
	char buf3[]="  w= ";
	for(int i=0;i<strlen(buf3);i++)
		{
			USART_write(buf3[i]);
		}
	USART_write(temp3);

	USART_write('\n');
	USART_write('\r');
}
