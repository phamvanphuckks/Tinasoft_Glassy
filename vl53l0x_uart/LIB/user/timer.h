/**
  ********************************************************************************************
  * @file    timer.h
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions prototypes for the timer, Buzzz
  *          firmware library functions (add-on to CMSIS functions).
  *********************************************************************************************
  * @attention
  * This file is used to declare function
  * prototypes for the timer peripheral as so as the Buzzz
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

/*!
 * @brief    function configuration PWM for the Buzz and called in main
 */
void PWM_Configuration(void);

/*!
 * @brief    function relation with Buzz
 */ 
void Update_status_VolumeBuzz(uint8_t VolumeBuzz_Levelx);
uint8_t Read_status_VolumeBuzz(void);
uint8_t  Read_status_Buzz(void);
uint16_t round_f(float x);
    
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
