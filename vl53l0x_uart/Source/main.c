#include "vl53l0x.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"

char _rx_buffer[SERIAL_RX_BUFFER_SIZE];

typedef struct{
	/*!< Specifies the Distance value, mm*/
	uint16_t distance; 		
	/*!< Specifies the Pin levels*/
	uint16_t status_Pin;	
	/*!< Specifies the Buzzz volume levels*/	
	uint16_t status_volumeBuzz;
	/*!< Specifies the status of Buzz ON/OFF*/	    
	uint8_t  status_Buzz;
} TX_Message_t;

TX_Message_t    tx_message;

TX_Message_t *ptx_message= &tx_message;

//void add_value_tx_message(uint16_t distance_t, uint16_t status_Pin_t, uint16_t volume_Buzz_t, uint8_t status_Buzz_t)
//{
//    tx_message.distance = distance_t;
//	tx_message.status_Pin = status_Pin_t;
//    tx_message.status_volumeBuzz = volume_Buzz_t;
//    tx_message.status_Buzz = status_Buzz_t;
//}

void add_value_tx_message(uint16_t distance_t, uint16_t status_Pin_t, uint16_t volume_Buzz_t, uint8_t status_Buzz_t)
{
    ptx_message->distance = distance_t;
	ptx_message->status_Pin = status_Pin_t;
    ptx_message->status_volumeBuzz = volume_Buzz_t;
    ptx_message->status_Buzz = status_Buzz_t;
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

	while(1)
	{
        add_value_tx_message(readRangeContinuousMillimeters(), Read_Status_Pin(), Read_status_VolumeBuzz(), Read_status_Buzz());
        printf("%d,%d,%d,%d\n", tx_message.distance, tx_message.status_Pin, tx_message.status_volumeBuzz, tx_message.status_Buzz); 

        GPIOA->ODR ^= (1 << 4);
        delay_ms(1000);
	}
	
}

