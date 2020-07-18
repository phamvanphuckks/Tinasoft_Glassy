/**
  ******************************************************************************
  * @file    UART.h
  * @author  GIAOSU
  * @date    2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to describe in detail
  * the functions that serve asynchronous serial communication uart
  * All information related to copyright contact nguyendonbg@gmail.com
  ******************************************************************************
  */

#include "uart.h"
#include "stm32f0xx_tim.h"

struct __FILE {
    int dummy;
};

FILE __stdout;

int fputc(int ch, FILE *f){
	/* Send your custom byte */
	USART_SendData(USART1, ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
	/* If everything is OK, you have to return character written */
	return ch;
}

static volatile uint8_t rx_count = 0;

void UART1_config(void)
{
    //Configure USART1 pins: Tx (PA2) and Rx (PA3)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitTypeDef GPIO_InitStructure2;
    GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure2);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    //Configure USART1: ----------------------------
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

			
    USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);//
    NVIC_EnableIRQ(USART1_IRQn);//
}

int _write(int file, char *ptr, int len)
{
    for (int i = len; i != 0; i--)
    {

        while ((USART1->ISR & 0x80) == 0);
        USART1->TDR = *ptr++;
    }
    return len;
}

void USART1_IRQHandler()
{
    /** read the data from register DR  UART*/
    uint8_t chartoreceive = USART_ReceiveData(USART1);
    /** save the data in the array */
    _rx_buffer[rx_count] = chartoreceive;
    rx_count++;
    
//		while ((USART1->ISR & 0x80) == 0);
//		USART1->TDR = chartoreceive;
	
    // disable TIM16 : send messgae with the cycle 1s
//    if( TIM16->DIER  & (uint16_t) (TIM_IT_Update))
//        TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
 
    if((_rx_buffer[0]=='S')  && _rx_buffer[rx_count-1] == '\n')
    {
        flag_rx_connected = 1;
    }
    
    if(_rx_buffer[rx_count-1] == '\n')
    {
        rx_count = 0;
    }
    

}
/******************* (C) COPYRIGHT 2020 nguyendonbg@gmail.com *****END OF FILE****/
