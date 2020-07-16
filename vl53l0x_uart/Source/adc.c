/**
  *****************************************************************************************************
  * @file    adc.c
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions definition for Analog-to-Digital Converter(adc), Pin 
  *          firmware library (add-on to CMSIS functions).
  *****************************************************************************************************
  * @attention
  * This file is used to describe in detail
  * the functions that serve for the adc peripheral as so as Pin
  * All information related to copyright contact phamvanphuckks@gmail.com
  ******************************************************************************************************
  */
	
/*
	PA0 - ADC1, chanel_0 (ADC_IN0)
	AF
*/

#include "adc.h"

/**
  * @brief  Initial adc1 
  * @param  None
  * @return None
  * @retval None
  */
void ADC_Configuration(void)
{
    ADC_InitTypeDef     	ADC_InitStructure;
    GPIO_InitTypeDef    	GPIO_InitStructure;

    /* GPIOC Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Configure ADC Channel11 as analog input */

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADCs DeInit */  
    ADC_DeInit(ADC1);

    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);

    /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution 					= ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode 		    = ENABLE;        
    ADC_InitStructure.ADC_ExternalTrigConvEdge 			= ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign 					= ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection 				= ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure); 

    ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);

    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);

    /* Enable the ADC peripheral */
    ADC_Cmd(ADC1, ENABLE);     

    /* Wait the ADRDY flag */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 

    /* ADC1 regular Software Start Conv */ 
    ADC_StartOfConversion(ADC1);
}

/**
  * @brief  Get the battery percentage value 
  * @param  none
  * @return specifies level pin 0-100
  * @retval None
  */
uint8_t Read_Status_Pin(void)
{
	uint16_t  Pin_Levelx;
	Pin_Levelx = ADC_GetConversionValue(ADC1);
    
	return (uint8_t) ((Pin_Levelx*100)/4096);
}

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
