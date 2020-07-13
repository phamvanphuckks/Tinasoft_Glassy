/**
  ******************************************************************************
  * @file    i2c.h
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

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_rcc.h"


/****************************************
*					User Defition                 *
*****************************************/

/*
	PA4 - Led D2
	I2C1_SCL : PA9
	I2C1_SDA : PA10
	AF4
*/

// pin-mode
#define I2Cx                          I2C1
#define I2Cx_CLK                      RCC_APB1Periph_I2C1

#define I2Cx_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define I2Cx_SDA_PIN                  GPIO_Pin_10                
#define I2Cx_SDA_GPIO_PORT            GPIOA                       
#define I2Cx_SDA_SOURCE               GPIO_PinSource10
#define I2Cx_SDA_AF                   GPIO_AF_4
  
#define I2Cx_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define I2Cx_SCL_PIN                  GPIO_Pin_9                
#define I2Cx_SCL_GPIO_PORT            GPIOA                    
#define I2Cx_SCL_SOURCE               GPIO_PinSource9
#define I2Cx_SCL_AF                   GPIO_AF_4

/* STANDARD_I2C_MODE*/
#define I2C_SPEED 		 100000
#define I2C_DUTYCYCLE  I2C_DutyCycle_2 


/****************************************
*					User function                 *
*****************************************/
void i2c_configuration(void);
void GPIO_configuration(void);

void I2C_Write_Reg(uint8_t Addr, uint8_t Reg, uint8_t Data);
void I2C_Write_Reg16bit(uint8_t Addr, uint8_t Reg, uint16_t Data);
void I2C_Write_Reg32bit(uint8_t Addr, uint8_t Reg, uint32_t Data);
void I2C_Write_MultiReg(uint8_t Addr, uint8_t Reg, uint8_t NumOfRegister,uint8_t *Data);


uint8_t  I2C_Read_Reg(uint8_t Addr, uint8_t Reg);
uint16_t I2C_Read_Reg16bit(uint8_t Addr, uint8_t Reg);
uint32_t I2C_Read_Reg32bit(uint8_t Addr, uint8_t Reg);
void I2C_Read_MultiReg(uint8_t Addr,uint8_t Reg, uint8_t NumOfRegister, uint8_t *Data);


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
