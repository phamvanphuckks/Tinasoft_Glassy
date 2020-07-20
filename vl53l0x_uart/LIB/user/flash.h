/**
  ******************************************************************************
  * @file    flash.h
  * @author  Giaosu
  * @version
  * @date    2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  *
  * Any commercial product related to this source code is
  * required to specify the source
  * <h2><center>&copy; COPYRIGHT 2020 nguyendonbg@gmail.com</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_flash.h"
#include <string.h>
#include <stdio.h>
#include "delay.h"
     
#define FLASH_USER_START         ((uint32_t)0x08007800)  
#define FLASH_USER_END           ((uint32_t)0x08007C00)       
#define FLASH_RANGE_THRESHOLD    ((uint32_t)0x08007800)  
#define FLASH_SPEAKER_VOLUME     ((uint32_t)0x08007804) 
#define FLASH_SPEAKER_STATUS     ((uint32_t)0x08007808)   
     
/*!
 * @brief  function erase page flash
 * @param  [IN]uint32_t addr    address page erase
 */
void Flash_Erase(uint32_t addr);
/*!
 * @brief  function write data type int in the flash
 * @param  [IN]uint32_t addr    address page write
 * @param  [IN]int data         data type int will write in the flash
 */
void Flash_Write_Int(uint32_t addr, int data);
/*!
 * @brief  function read data type int in the flash
 * @param  [IN]uint32_t addr    address page read
 */
uint16_t Flash_Read_Int(uint32_t addr);
/*!
 * @brief  function write data type char in the flash
 * @param  [IN]uint32_t addr    address page write
 * @param  [IN]char* data       data type char will write in the flash
 */
void Flash_Write_Char(uint32_t addr, char* data);
/*!
 * @brief  function read data type char in the flash
 * @param  [IN]uint32_t addr    address page write
 * @param  [IN]char* dataOut       save data in the array data out
 */
void Flash_ReadChar(char* dataOut, uint32_t addr1, uint32_t size_read);
/*!
 * @brief  function rwrite Half Word in the flash
 * @param  [IN]uint32_t addr    address page write
 * @param  [IN]char* dataIn     data data to be written in the page memory
 */
void Flash_ProgramPage(char* dataIn, uint32_t addr1);
/*!
  * @brief  get the Volume status of Buzz!
  * @param  None
  * @return specifies level volume  : 0 ->100 
  * @retval None
  */
uint8_t Read_status_volume_speaker(void);
/*!
  * @brief  get the  status of Buzz!
  * @param  None
  * @return the status 0-OFF, >0 - ON  
  * @retval None
 */
uint8_t  Read_status_speaker(void);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2020 nguyendonbg@gmail.com *****END OF FILE****/
