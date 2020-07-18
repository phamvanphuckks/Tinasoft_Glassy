#include <string.h>
#include <stdlib.h>
#include "vl53l0x.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"

char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
volatile uint8_t flag_rx_connected = 0, flag_tx_package = 0;
char id_message[9], range_min[4], speaker_volume[4], speaker_status[4], connected[12];

typedef struct{
    char *tx_id_message;
	/*!< Specifies the Pin levels*/
	uint16_t Pin_status;	
	/*!< Specifies the range value, mm*/
	uint16_t range; 		
	/*!< Specifies the status of Buzz ON/OFF*/	    
	uint8_t  speaker_status;
	/*!< Specifies the Buzzz volume levels*/	
	uint16_t speaker_volum;
} tx_package;

tx_package tx_message_t;
tx_package *tx_message = &tx_message_t;

typedef struct
{
    char *rx_id_message;
    uint16_t range;
    uint8_t  speaker_volum;
    uint8_t  speaker_status;
} rx_package;

rx_package rx_message_t;
rx_package *rx_message = &rx_message_t;

void add_data_to_tx_message(uint8_t status_Pin_t, uint16_t range_t, uint8_t status_Buzz_t, uint8_t volume_Buzz_t)
{
    tx_message->Pin_status = status_Pin_t;
    tx_message->range = range_t;
    tx_message->speaker_status = status_Buzz_t;
    tx_message->speaker_volum = volume_Buzz_t;
}

void add_data_to_rx_message(char *p_range_t, char *p_speaker_status_t, char *p_speaker_volume_t)
{
    rx_message->range = atoi(p_range_t);
    rx_message->speaker_status = atoi(p_speaker_status_t);
    rx_message->speaker_volum = atoi(p_speaker_volume_t);
}

void parse_package(char *str)
{
    char *p_rx_buffer;
    
    char *p_id, *p_range_min, *p_speaker_volume, *p_speaker_status;
    char *p_connected;
    
    p_rx_buffer = str;
    p_id = p_rx_buffer;
    p_rx_buffer = strchr(p_id, ',');

    if (p_rx_buffer != NULL)
    {
        *p_rx_buffer = 0; 
        strcpy(id_message, p_id);
    }
    p_connected = p_rx_buffer + 1; 

    // S, connected
    if(p_connected[0] == 'c'){
        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0; 
            strcpy(connected, p_connected);
        }
        connected[9] = '\0'; // ki tu 10/11 se la CR,CF
    }
    else{ // S, a,b,c,d
        p_range_min = p_rx_buffer + 1; 
        p_rx_buffer = strchr(p_range_min, ',');

        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0;
            strcpy(range_min, p_range_min);
        }
        //printf("range_min %s\n", range_min);        
        
        p_speaker_status = p_rx_buffer + 1; 
        p_rx_buffer = strchr(p_speaker_status, ',');

        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0;
            strcpy(speaker_status, p_speaker_status);
        }
        //printf("speaker_status %s\n", speaker_status);
        
        p_speaker_volume = p_rx_buffer + 1;
        p_rx_buffer = strchr(p_speaker_volume, ',');

        if (p_rx_buffer != NULL)
        {
            *p_rx_buffer = 0;
            strcpy(speaker_volume, p_speaker_volume);
        }
        //printf("speaker_volume %s\n", speaker_volume);
    }
}

void Transfer_Package(char ID, tx_package *tx_message)
{
    printf("%c,%d,%d,%d,%d,\n", ID, tx_message->Pin_status, tx_message->range, tx_message->speaker_status, tx_message->speaker_volum);
}

void Process_request_from_app(){
    
    // parse() data recevie form App
    parse_package(_rx_buffer);
    add_data_to_rx_message(range_min, speaker_status, speaker_volume);
    
    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
    USART_ITConfig(USART1,USART_IT_RXNE, DISABLE);
    // save in flash - Disable interrupt timer, uart
    
    // send package confirm - - Read_status_Buzz() : doc tu flash ra <chua sua> 
    if(!strcmp(id_message, "S")){
        if(!strcmp(connected, "connected")){
            add_data_to_tx_message(Read_Status_Pin(), readRangeContinuousMillimeters(), Read_status_speaker(), Read_status_volume_speaker()); 
            Transfer_Package('S', tx_message);
            
            // Enable interrupt timer, uart
            TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);  
            TIM_Cmd(TIM16, ENABLE); 
            
            printf("Connected\n");
            memset(connected,'\0', strlen(connected));
         }
       else
       {
            add_data_to_tx_message(Read_Status_Pin(), readRangeContinuousMillimeters(), Read_status_speaker(), Read_status_volume_speaker()); 
            Transfer_Package('S', tx_message);   
            printf("Setup done\n");
           
            // Enable interrupt timer, uart
            TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);  
       }
    }
    USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);   
}

int main(void)
{
	SystemClock_Config();
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
            flag_rx_connected = 0;
        }

        if(flag_tx_package)
        {
            add_data_to_tx_message(Read_Status_Pin(), readRangeContinuousMillimeters(), Read_status_speaker(), Read_status_volume_speaker()); 
            Transfer_Package('S', tx_message);
            // phat ra canh bao neu khoang cach < nguong
//            if(tx_message->range > 20)
//            {
//                // mode            
//                if()  Update_status_volume_speaker(50); 
//                else Update_status_volume_speaker(0); 
//            }
//            else
//            {
//                Update_status_volume_speaker(0);       
//            }
            /*clear flag_tx_package*/          
            flag_tx_package = 0;
        }    

	}
	
}


