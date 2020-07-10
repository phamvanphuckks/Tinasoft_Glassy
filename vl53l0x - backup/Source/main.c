#include "Adafruit_VL53L0X.h"
#include "delay.h"


uint16_t range;

int main(void)
{
	SystemClock_Config();
	SystemCoreClockUpdate();
	SysTick_Init();
	
	begin(VL53L0X_I2C_ADDR, 0);
	
	
	VL53L0X_RangingMeasurementData_t measure;
	


	while(1)
	{
		rangingTest(&measure, false); 
	
		if (measure.RangeStatus != 4) {  // phase failures have incorrect data
				range = measure.RangeMilliMeter;			
		} else {
			range = 10000;
		}
		
		GPIOA->ODR ^= (1 << 4);
		delay_ms(1000);
		
	}
	
}

