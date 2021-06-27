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
#include "math.h"
#include "stm32f0xx_dma.h"


extern volatile uint16_t  ADC1ConvertedValue , ADC1ConvertedVoltage;
extern  volatile uint16_t RegularConvData_Tab[2];

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
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
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

    //initial channel_0
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
uint16_t Read_Status_Pin(void)
{
	int  Pin_Levelx = 0, sum_adc = 0;
    uint16_t i;
        
    for(i = 0; i < 100; i++)
    {
        sum_adc += ADC_GetConversionValue(ADC1);
    }     
    sum_adc /= 100;
    if(sum_adc <= THRS_ADC_VALUE_DOWN)  return Pin_Levelx = 0;
    else if(sum_adc >= THRS_ADC_VALUE_UP)  return Pin_Levelx = 100;
    else
        Pin_Levelx = (uint32_t)((sum_adc - THRS_ADC_VALUE_DOWN)*100)/(THRS_ADC_VALUE_UP - THRS_ADC_VALUE_DOWN);
    
    return  (uint16_t)Pin_Levelx;
}  

uint32_t Map_Pin(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (uint32_t)(value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t Read_Status_Pin_test(void)
{
	int  Pin_Levelx=0, sum_adc = 0;
    uint16_t i;
        
    for(i = 0; i < 100; i++)
    {
        sum_adc += ADC_GetConversionValue(ADC1);
    }     
    sum_adc /= 100;
    
    if(sum_adc <= THRS_ADC_VALUE_DOWN) Pin_Levelx = 0;
    else if(sum_adc >= THRS_ADC_VALUE_UP)  Pin_Levelx = 100;
    else if(sum_adc >= THRS_ADC_DEVIDE_VALUE){
            Pin_Levelx = Map_Pin(sum_adc, THRS_ADC_DEVIDE_VALUE, THRS_ADC_VALUE_UP + 1, THRS_PIN_PERCENT, THRS_PIN_PERCENT_MAX + 1);       
    }
    else if(sum_adc < THRS_ADC_DEVIDE_VALUE){
            Pin_Levelx = Map_Pin(sum_adc, THRS_ADC_VALUE_DOWN, THRS_ADC_DEVIDE_VALUE -1, THRS_PIN_PERCENT_MIN, THRS_PIN_PERCENT);        
    }
       
//    if(Pin_Levelx >= 95) Pin_Levelx = 100;
    
    return  (uint16_t)Pin_Levelx;
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
