/**
  ******************************************************************************************************
  * @file    adc.h
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions prototypes for Analog-to-Digital Converter(adc), Pin
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************************************
  * @attention
  * This file is used to declare function
  * prototypes for the adc peripheral as so as the Pin
  * All information related to copyright contact phamvanphuckks@gmail.com
  ******************************************************************************************************
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
 * @brief    function configuration ADC and called in main
 */
void ADC_Configuration(void);
 
/*!
 * @brief    function relation with Pin of Circuit
 */     
uint8_t Read_Status_Pin(void);

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
