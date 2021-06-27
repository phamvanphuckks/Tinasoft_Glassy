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
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN ;
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
    TIM_TimeBase_InitStructure.TIM_Period 					= 7999;//2s
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
  * @param  VolumeBuzz_Levelx : the levels of VolumeBuzz 0->100 <-> map 0->50 load to MCU because buzz 1/2 duty cycle
  *  Because this condition can make the buzzer film inside achieve the highest amplitude 
  * @return None       
  * @retval None
  */
void Update_status_volume_speaker(uint8_t VolumeBuzz_Levelx)
{
    TIM3->CCR1 = VolumeBuzz_Levelx*(415)/100;
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

uint32_t map_buzzer(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (uint32_t)(value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ngat se cap nhap co flag_tx_package, main se kiem tra no 
void TIM16_IRQHandler(void)
{
   flag_tx_package = 1; 
   TIM_ClearFlag(TIM16, TIM_FLAG_Update);
    
//   GPIOA->ODR ^= (1 << 4);
}
