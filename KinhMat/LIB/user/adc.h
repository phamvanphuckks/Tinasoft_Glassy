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
#include "delay.h"

#define THRS_ADC_VALUE_UP       3965UL     
#define THRS_ADC_VALUE_DOWN     3446UL

// duoi ko quan tam     
#define THRS_ADC_DEVIDE_VALUE   3636UL // 60% ung voi 3636
#define THRS_PIN_PERCENT        80UL   // nguong sau khi map
#define THRS_PIN_PERCENT_MAX    100UL
#define THRS_PIN_PERCENT_MIN    0UL

     
/*!
 * @brief    function configuration ADC and called in main
 */
void ADC_Configuration(void);
 
/*!
 * @brief    function relation with Pin of Circuit
 */     
uint16_t Read_Status_Pin(void);

//uint32_t Read_VDDA(void);

uint32_t Map_Pin(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

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
