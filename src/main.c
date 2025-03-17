/*
*  main.c
*
*  Created on: 30 Nov 2022
*  Author: e1800961
*/

/* Includes */
#include <stm32l1xx.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "SetSysClock.h"

#include "baremetal.h"
#include "TIM_PWM.h"
#include "nucleo_ihm07m1.h"
#include "test_hallsensors.h"
#include "test_driveroutput.h"


/* Private typedef */
/* Private define  */
#define dutyVal 2000

//#define EXTIH										// interrupt from hall signals, used for printing out on terminal
//#define PRINT_HVALUE								// uncomment to print out Hall sensors value on terminal

/* Private macro */

/* Global variables */

// pre-defined scaler for clock and pwm duty circle
uint32_t preScaler = 10;
uint32_t arr_Val = 10000;
uint32_t duty_Val = dutyVal;
int32_t temp = dutyVal;
int32_t* ptemp = &temp;

uint32_t stepChange = 500;

uint8_t ccw = 0;									// Counter-clockwise rotation or not?
uint8_t* pccw = &ccw;

char received_command = 0;

/* Private function prototypes */
/* Private functions */

void SetSysClock(void);

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{

	__disable_irq();								// global disable IRQs, M3_Generic_User_Guide p135.

	/* Configure the system clock to 32 MHz and update SystemCoreClock */
	SetSysClock();
	SystemCoreClockUpdate();

	/* TODO - Add your application code here */
	RCC->AHBENR |= (uint32_t)(1 << 0);				// Enable clock for GPIO A B C. p162
	RCC->AHBENR |= (uint32_t)(1 << 1);
	RCC->AHBENR |= (uint32_t)(1 << 2);
	RCC->APB2ENR |= 1;								// Bit 0 SYSCFGEN: System configuration controller clock enable. p157

	USART2_Init();									// for debugging and data display on terminal

	// Enabling on board PA5 LED using custom function in baremetal_GPIO
	// third parameter 0: input (reset state) | 1: GP output | 2: alternate function | 3: analog
	GPIO_PinInit(GPIOA, 5, 1, 0);

#ifdef EXTIH										// Hall sensor signal interrupt
	Hallsensor_Interrupt_Init();
#endif

	NVIC_EnableIRQ(EXTI15_10_IRQn);					// Enable EXTI15_10 IRQ
	NVIC_EnableIRQ(EXTI3_IRQn);						// Enable EXTI3 IRQ


	TIM_ClockEnable2_7(6);							// Timer 6 (32MHz) clock enable.
	TIM_Init(TIM6, preScaler - 1, arr_Val - 1, 0);	// Scale for PWM signal
	TIM_EnableCounter(TIM6);

	L6230_Init();									// Initialise 3-phase motor driver
	HallSensor_Init();								// Initialise Hall sensor for measuring position and speed.

	USART2_interrupt_Init();						// Initialise interrupt from terminal

	__enable_irq();									// Enable interrupt globally, after initialisations

	/* Infinite loop */
	while (1)
	{
		//** Use while loop for reading current step and generating PWM signal based on TIM6

		TIM_PWM(TIM6, duty_Val, ccw);				//** Try to use TIM6 for signal controlling reference only

//		GPIOA->ODR ^= (uint32_t)(1 << 5); 			// 0010 0000 xor bit 5 = PA5. p186
//		delay_ms(100);

	}
	return 0;
}

void  EXTI15_10_IRQHandler(void)					// Interrupt function for signal from PA15 and PB10
{
#ifdef PRINT_HVALUE
	// Printing current step of motor position
	HallSensor_PrintVal('A');
	HallSensor_PrintVal('B');
	HallSensor_PrintVal('Z');
	HallSensor_PrintCurrentStep();
#endif

#ifdef EXTIH										// Hall sensor interrupt
	EXTI->PR=0x8000;								// clearing bit for PR15
	EXTI->PR=0x0400;								// clearing bit for PR10
#endif
}

void  EXTI3_IRQHandler(void)						// Interrupt function for signal from PB3
{
#ifdef PRINT_HVALUE
	// Printing current motor step
	HallSensor_PrintVal('A');
	HallSensor_PrintVal('B');
	HallSensor_PrintVal('Z');
	HallSensor_PrintCurrentStep();
#endif

#ifdef EXTIH										// Hall sensor interrupt
	EXTI->PR=0x0008;								// clearing bit for PR3
#endif
}

void USART2_IRQHandler(void)						// Interrupt from terminal
{
	duty_Val = abs(PWM_ValChange(received_command, ptemp, pccw, stepChange));

	int percentage = temp / 100;					// changing to percentage
	char duty[4];
	sprintf(duty, "%d", percentage);				// convert integer to char

	int digitCount = 0;
	int count = percentage;
	while(count != 0)								// counting for printing size
	{
		digitCount++;
		count = count/10;
	}

	if(percentage > 0)
	{
		for(int i=0;i<digitCount;i++)				// Visualise changes to terminal
		{
			USART_write(duty[i]);
		}
		USART_write('%');
		USART_write('\n');
		USART_write('\r');
	}
	else if(percentage <= 0)
	{
		for(int i=0;i<digitCount+1;i++)
		{
			USART_write(duty[i]);
		}
		USART_write('%');
		USART_write('\n');
		USART_write('\r');
	}

}

