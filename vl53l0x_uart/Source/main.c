#include <string.h>
#include <stdlib.h>
#include "vl53l0x.h"
#include "delay.h"
#include "timer.h"
#include "flash.h"
#include "uart.h"
#include "adc.h"

char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
volatile uint8_t flag_rx_connected = 0, flag_tx_package = 0;
char id_message[9], range_min[4], speaker_volume[4], speaker_status[4], connected[12];

typedef struct{
    char *tx_id_message;
	/*!< Specifies the Pin levels*/
	uint16_t pin_status;	
	/*!< Specifies the range value, mm*/
	uint16_t range; 		
	/*!< Specifies the status of Buzz ON/OFF*/	    
	uint8_t  speaker_status;
	/*!< Specifies the Buzzz volume levels*/	
	uint16_t speaker_volume;
} tx_package;

tx_package tx_message_t;
tx_package *tx_message = &tx_message_t;

typedef struct
{
    char *rx_id_message;
    uint16_t range_threshold;
    uint8_t  speaker_volume;
    uint8_t  speaker_status;
} rx_package;

rx_package rx_message_t;
rx_package *rx_message = &rx_message_t;

void add_data_to_tx_message(uint8_t Pin_status_t, uint16_t range_t, uint8_t speaker_status_t, uint8_t speaker_volume_t)
{
    tx_message->pin_status = Pin_status_t;
    tx_message->range = range_t;
    tx_message->speaker_status = speaker_status_t;
    tx_message->speaker_volume = speaker_volume_t;
}

void add_data_to_rx_message(char *p_range_t, char *p_speaker_status_t, char *p_speaker_volume_t)
{
    rx_message->range_threshold = atoi(p_range_t);
    rx_message->speaker_status = atoi(p_speaker_status_t);
    rx_message->speaker_volume = atoi(p_speaker_volume_t);
}

void parse_package(char *str)
{
    char *p_rx_buffer;
    
    char *p_id_message, *p_range_min, *p_speaker_volume, *p_speaker_status;
    char *p_connected;
    
    p_rx_buffer = str;
    p_id_message = p_rx_buffer;
    p_rx_buffer = strchr(p_id_message, ',');

    if (p_rx_buffer != NULL)
    {
        *p_rx_buffer = 0; 
        strcpy(id_message, p_id_message);
    }
    p_connected = p_rx_buffer + 1; 

    // S, connected
    if(p_connected[0] == 'c')
    {
        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0; 
            strcpy(connected, p_connected);
        }
    }
    else
    { // S, a,b,c,d
        p_range_min = p_rx_buffer + 1; 
        p_rx_buffer = strchr(p_range_min, ',');

        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0;
            strcpy(range_min, p_range_min);
        }

        p_speaker_status = p_rx_buffer + 1; 
        p_rx_buffer = strchr(p_speaker_status, ',');

        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0;
            strcpy(speaker_status, p_speaker_status);
        }

        p_speaker_volume = p_rx_buffer + 1;
        p_rx_buffer = strchr(p_speaker_volume, ',');

        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0;
            strcpy(speaker_volume, p_speaker_volume);
        }
    }
}

void Transfer_Package(char ID, tx_package *tx_message)
{
    printf("%c,%d,%d,%d,%d,\n", ID, tx_message->pin_status, tx_message->range, tx_message->speaker_status, tx_message->speaker_volume);
}

void save_rx_message_to_flash()
{
    FLASH_Unlock();
    FLASH_ErasePage(FLASH_USER_START);
    FLASH_Lock();
    
    Flash_Write_Int(FLASH_RANGE_THRESHOLD, rx_message->range_threshold); 
    Flash_Write_Int(FLASH_SPEAKER_STATUS, rx_message->speaker_status);  
    Flash_Write_Int(FLASH_SPEAKER_VOLUME, rx_message->speaker_volume);      
}

void Process_request_from_app(){
    // parse() data recevie form App
    parse_package(_rx_buffer);
    add_data_to_rx_message(range_min, speaker_status, speaker_volume);
    
    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    
    if(!strcmp(id_message, "S"))
    {
        if(strstr(connected, "connected") != NULL)
        {
            add_data_to_tx_message(Read_Status_Pin(), readRangeSingleMillimeters(), Read_status_speaker(), Read_status_volume_speaker()); 
            Transfer_Package('S', tx_message);
            
            // Enable interrupt timer, uart
            TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);  
            TIM_Cmd(TIM16, ENABLE); 
            
            printf("Connected\n");
            memset(connected, '\0', strlen(connected));
       }
       else
       {    
            // save in flash - Disable interrupt timer, uart
            save_rx_message_to_flash();
            add_data_to_tx_message(Read_Status_Pin(), readRangeSingleMillimeters(), Read_status_speaker(), Read_status_volume_speaker()); 
            Transfer_Package('S', tx_message);   
            printf("Setup done\n");
           
            // Enable interrupt timer, uart
            TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);  
       }
    }
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   
}
void PrintSystemInfo()
{

  printf("\n");

  printf("RCC_CR    = %08X\n", RCC->CR);
  printf("RCC_CFGR  = %08X\n", RCC->CFGR);
  printf("FLASH_ACR = %08X\n", FLASH->ACR);
  printf("\n");

  RCC_ClocksTypeDef RCC_ClocksStatus;
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  printf("RCC_SYSCLK_Frequency = %ld\n", RCC_ClocksStatus.SYSCLK_Frequency);
  printf("RCC_HCLK_Frequency   = %ld\n", RCC_ClocksStatus.HCLK_Frequency);

  printf("RCC_ADCCLK_Frequency = %ld\n", RCC_ClocksStatus.ADCCLK_Frequency);
  printf("\n");
}

uint16_t range;
int main(void)
{
    SystemInit();
	SystemCoreClockUpdate();
	SysTick_Init();
	
    init_vl53l0x(true);
	startContinuous(0); 
    
	UART1_config();
	ADC_Configuration();
	PWM_Configuration();
    TIM16_Configuration(); // DISABLE interrupt/TIM6
    
	while(1)
	{
        if(flag_rx_connected)
        {
            Process_request_from_app();
            
            /*clear flag_rx_connected*/    
            flag_rx_connected = 0;
        }
        if(flag_tx_package)
        {
            add_data_to_tx_message(Read_Status_Pin(), readRangeSingleMillimeters(), Read_status_speaker(), Read_status_volume_speaker()); 
            Transfer_Package('S', tx_message);
                        
            // phat ra canh bao neu khoang cach < nguong
            if(tx_message->range < (uint16_t)Flash_Read_Int(FLASH_RANGE_THRESHOLD))
            {
                if((uint8_t)Flash_Read_Int(FLASH_SPEAKER_STATUS))  
                {
                    Update_status_volume_speaker((uint8_t)Flash_Read_Int(FLASH_SPEAKER_VOLUME)); 
                }
                else 
                {
                    Update_status_volume_speaker(0); 
                }
            }
            else
            {
                Update_status_volume_speaker(0);       
            }
            /*clear flag_tx_package*/          
            flag_tx_package = 0;
        }   

	}
}
