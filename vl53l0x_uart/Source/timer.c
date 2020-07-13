/**
  ******************************************************************************
  * @file    timer.c
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to describe in detail
  * the functions that serve asynchronous serial communication uart
  * All information related to copyright contact phamvanphuckks@gmail.com
  ******************************************************************************
  */


#include  "timer.h"


/**
  * @brief  Initial timer3 pwm ?
  * @param  
  *         
  * @retval None
  */

void PWM_Configuration()
{
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBase_InitStructure;
	GPIO_InitTypeDef					GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*Config PA6 - TIM3_CH1*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);

	/*Config TIM_timeBase*/
	TIM_TimeBase_InitStructure.TIM_Period 						= 7999;
	TIM_TimeBase_InitStructure.TIM_Prescaler 					= 0x00;
	TIM_TimeBase_InitStructure.TIM_ClockDivision 			= TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode				=	TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_RepetitionCounter	= 0x00;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);
	

  /* Channel 1 Configuration in PWM2 mode */
  TIM_OCInitStructure.TIM_OCMode 					= TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState 		= TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState 		= TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse 					= 0x00;
  TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity 		= TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState 		= TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState 		= TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  /* TIM3 counter enable */
  TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  set pwm with duty_cycle and frequency
  * @param  pwm: specifies level sound
	*					@arg : 10% <=> Level_1
	*					@arg : 20% <=> Level_2
  * @param  
  *         
  * @retval None
  */
void Set_PWM(uint8_t pwm)
{
	uint16_t	TimerPeriod = TIM3->ARR;
	
	TIM3->CCR1  = (uint16_t) ((pwm*TimerPeriod)/100);
}


