/**
  ******************************************************************************
  * @file    timer.c
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions definition for timer, Buzzz and send package
  *          firmware library (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to describe in detail
  * the functions that serve for the timer peripheral, the Buzzz as so as send package
  * All information related to copyright contact phamvanphuckks@gmail.com
  ******************************************************************************
  */

#include "timer.h"
#include "flash.h"

/**
  * @brief  Initial TIM3 PWM 
  * @param  None
  * @return None        
  * @retval None
  */
void PWM_Configuration()
{
    TIM_OCInitTypeDef  				TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef		    TIM_TimeBase_InitStructure;
    GPIO_InitTypeDef				GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /*Config PA6 - TIM3_CH1*/
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);

    /*Config TIM_timeBase*/
    TIM_TimeBase_InitStructure.TIM_Period 					= 2962;
    TIM_TimeBase_InitStructure.TIM_Prescaler 				= 0x00;
    TIM_TimeBase_InitStructure.TIM_ClockDivision 			= TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode				= TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_RepetitionCounter	    = 0x00;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);


    /* Channel 1 Configuration in PWM1 mode */
    TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState 		= TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState 		= TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse 				= 0x00;
    TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity 		= TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState 		= TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState 		= TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    /* TIM3 counter enable */
    TIM_Cmd(TIM3, ENABLE);
}


/**
  * @brief  Initial TIM15, The cycle is 1s 
  * @param  None
  * @return None        
  * @retval None
  */

void TIM16_Configuration(void)
{

    TIM_TimeBaseInitTypeDef		    TIM_TimeBase_InitStructure;
    NVIC_InitTypeDef				NVIC_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    /*Config TIM_timeBase*/
    TIM_TimeBase_InitStructure.TIM_Period 					= 7999;
    TIM_TimeBase_InitStructure.TIM_Prescaler 				= 0x3E7;
    TIM_TimeBase_InitStructure.TIM_ClockDivision 			= TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode				= TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_RepetitionCounter	    = 0x00;
    TIM_TimeBaseInit(TIM16, &TIM_TimeBase_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_Init(&NVIC_InitStructure);
   
    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
    
    /* TIM16 counter enable */
    TIM_Cmd(TIM16, DISABLE);   
}


/**
  * @brief  update the new level of VolumeBuzz
  * @param  VolumeBuzz_Levelx : the levels of VolumeBuzz 0->100
  * @return None       
  * @retval None
  */
void Update_status_volume_speaker(uint8_t VolumeBuzz_Levelx)
{
	uint16_t TimerPeriod, TIM3_PWM_Value;
    
    TimerPeriod = TIM3->ARR;
    TIM3_PWM_Value = (uint16_t)(VolumeBuzz_Levelx*TimerPeriod)/100;    
	TIM3->CCR1 = (uint16_t) TIM3_PWM_Value;
}

/**
  * @brief  get the Volume status of Buzz!
  * @param  None
  * @return specifies level volume  : 0 ->100 
  * @retval None
  */

uint8_t Read_status_volume_speaker(void)
{

    uint8_t volume_speaker_Levelx;
    volume_speaker_Levelx = (uint8_t)Flash_Read_Int(FLASH_SPEAKER_VOLUME);
	return volume_speaker_Levelx;
}

/**
  * @brief  get the  status of Buzz!
  * @param  None
  * @return the status 0-OFF, >0 - ON  
  * @retval None
  */

uint8_t  Read_status_speaker(void)
{
    uint8_t speaker_status;
    speaker_status = (uint8_t)Flash_Read_Int(FLASH_SPEAKER_STATUS);
    return speaker_status;
}

/**
  * @brief  round the number type float
  * @param  the number type float
  * @return Returns the number type  uint16_t
  * @retval None
  */
uint16_t round_f(float x)
{
    uint16_t right_decimal_point;
    float left_decimal_point;
    
    right_decimal_point = (uint16_t) x;
    left_decimal_point = x - right_decimal_point;
    
    return ((left_decimal_point >= 0.5) ? (right_decimal_point+1) : right_decimal_point);
}

// ngat se cap nhap co flag_tx_package, main se kiem tra no 
void TIM16_IRQHandler(void)
{
   flag_tx_package = 1; 
   TIM_ClearFlag(TIM16, TIM_FLAG_Update);
    
   GPIOA->ODR ^= (1 << 4);
}
