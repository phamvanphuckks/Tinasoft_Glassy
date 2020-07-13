/**
  ******************************************************************************
  * @file    timer.h
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
#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


/*!
 * @brief    function configuration PWM and called in main
 */
void PWM_Configuration(void);
void Set_PWM(uint8_t pwm );

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
