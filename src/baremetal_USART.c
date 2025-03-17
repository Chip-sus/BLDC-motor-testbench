/*
 * baremetal_USART.c
 *
 *  Created on: 3 Dec 2022
 *      Author: e1800961
 *
 *  This file accesses into necessary registers for enabling USART.
 */

/******************************************************************************
 *	Includes
 *****************************************************************************/
#include "baremetal_USART.h"

/******************************************************************************
 *	Variables
 *****************************************************************************/

/******************************************************************************
 *	Functions
 *****************************************************************************/

void USART2_Init(void)
{
	RCC->APB1ENR|= 0x00020000; 				// set bit 17 (USART2 EN)
	RCC->AHBENR|= 0x00000001; 				// enable GPIOA port clock bit 0 (GPIOA EN)
	GPIOA->AFR[0]= 0x00000700;				// GPIOx_AFRL p.188,AF7 p.177
	GPIOA->AFR[0]|= 0x00007000;				// GPIOx_AFRL p.188,AF7 p.177
	GPIOA->MODER|= 0x00000020; 				// MODER2=PA2(TX) to mode 10=alternate function mode. p184
	GPIOA->MODER|= 0x00000080; 				// MODER2=PA3(RX) to mode 10=alternate function mode. p184

	USART2->BRR = 0x00000116;				// 115200 BAUD and crystal 32MHz. p710, 116
	USART2->CR1 = (uint32_t)(1 << 3);		// TE bit. p739-740. Enable transmit
	USART2->CR1 |= (uint32_t)(1 << 2);		// RE bit. p739-740. Enable receiver
	USART2->CR1 |= (uint32_t)(1 << 13);		// UE bit. p739-740. Uart enable
}

void USART_write(char data)
{
	//wait while TX buffer is empty
	while(!(USART2->SR&0x0080)){} 	//TXE: Transmit data register empty. p736-737
		USART2->DR=(data);			//p739
}

char USART2_read()
{
	char data=0;
	//wait while RX buffer is data is ready to be read
	while(!(USART2->SR&0x0020)){} 	//Bit 5 RXNE: Read data register not empty
		data=USART2->DR;			//p739
		return data;
}

void USART2_interrupt_Init()
{
	USART2->CR1 |= 0x0020;					//enable RX interrupt
	NVIC_EnableIRQ(USART2_IRQn);			// Enable interrupt for USART2
}


