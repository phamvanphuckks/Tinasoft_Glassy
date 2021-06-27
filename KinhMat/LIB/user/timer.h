/**
  ********************************************************************************************
  * @file    timer.h
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions prototypes for the timer, Buzzz and send package
  *          firmware library functions (add-on to CMSIS functions).
  *********************************************************************************************
  * @attention
  * This file is used to declare function
  * prototypes for the timer peripheral, the Buzzz as so as send package
  * All information related to copyright contact phamvanphuckks@gmail.com
  *********************************************************************************************
  */
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx.h"     

     
extern volatile uint8_t flag_tx_package;     
     
/*!
 * @brief    function configuration PWM for the Buzz and called in main, update data with cycle 1s
 */
void PWM_Configuration(void);
void TIM16_Configuration(void);
/*!
 * @brief    function relation with Buzz
 */ 
void Update_status_volume_speaker(uint8_t VolumeBuzz_Levelx);
uint16_t round_f(float x);
uint32_t map_buzzer(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
#ifdef __cplusplus
}
#endif

#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2020 phamvanphuckks@gmail.com *****END OF FILE****/
