#include "Adafruit_VL53L0X.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"

char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
uint16_t range;
uint8_t get_adc;
int main(void)
{
	SystemClock_Config();
	SystemCoreClockUpdate();
	SysTick_Init();
	
//	begin(VL53L0X_I2C_ADDR, 0);
//	
//	
//	VL53L0X_RangingMeasurementData_t measure;
	
	UART1_config();
	ADC_Configuration();
	PWM_Configuration();
	Set_PWM(35);
	
	while(1)
	{
//		rangingTest(&measure, false); 
//	
//		if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//				range = measure.RangeMilliMeter;			
//		} else {
//			range = 10000;
//		}

//		printf("value: %d\n", measure.RangeMilliMeter); 
		
//		GPIOA->ODR ^= (1 << 4);
//		delay_ms(1000);
		
	}
	
}

