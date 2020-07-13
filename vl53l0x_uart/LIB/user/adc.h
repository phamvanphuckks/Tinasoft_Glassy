/**
  ******************************************************************************
  * @file    adc.h
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

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 

#include "stm32f0xx_adc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

/*!
 * @brief    function configuration USART and called in main
 */
void ADC_Configuration(void);
uint8_t Get_Pin(void);


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
