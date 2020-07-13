/**
  ******************************************************************************
  * @file    UART.h
  * @author  GIAOSU
  * @date    2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to declare function
  * prototypes for communication with USART
  * All information related to copyright contact nguyendonbg@gmail.com
  ******************************************************************************
  */


#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

#define SERIAL_RX_BUFFER_SIZE 256

/** This array needs to be declared in main function */
extern char _rx_buffer[SERIAL_RX_BUFFER_SIZE] ;

/*!
 * @brief    function configuration USART and called in main
 */
int _write(int file, char *ptr, int len);
void UART1_config(void);

/*!
 * @brief    function interrup service UART
 */
void USART1_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

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
