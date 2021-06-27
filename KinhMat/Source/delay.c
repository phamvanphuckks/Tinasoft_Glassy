#include "delay.h"
#include "stm32f0xx_rcc.h"


#define SYSCLK_MHZ 8


/*note: you must call fucntion , if you want to use delay */
void SysTick_Init()
{
  //SysTick->VAL  = 0;                                          // Load the SysTick Counter Value
  SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_TICKINT_Msk   |
                  SysTick_CTRL_ENABLE_Msk;                    // Enable SysTick IRQ and SysTick Timer
}
static uint32_t volatile SysTickCounter = 1;

void SysTick_Handler()
{
  ++SysTickCounter;
}

uint64_t SysTick64()
{
  return (((uint64_t)SysTickCounter) << 24) - SysTick->VAL;
}

uint32_t SysTick32()
{
	/*
	Khi tru di se ra : khoang cach toi nguong
	VD : SysTickCounter = 149000 bit[31:0] - (SysTickCounter co the > or < SysTick->VAL) , SysTick->VAL = 139000 bit[23:0]
	thi khi tru di van ra cung ket qua la 16 638 216  - phan bu cua 2^24 
	
	(uint32_t)(SysTickCounter << 24) = 	134,217,728
	(uint32_t)(SysTickCounter << 24) - SysTick->VAL									   =  134,078,728 [32:0]
	
	=> return 16 638 216 [23:0] => cai nay chinh la gia tri 24 bit cua kq + SysTick->VAL = 2^24
	
	<< 24bit la ro do phan giai 24 bit
	
	*/
  return (SysTickCounter << 24) - SysTick->VAL;
}

uint32_t SysTick24()
{
  return ~(SysTick->VAL);
}
// = millies() in arduino
uint64_t SysTick_Millis()
{
  return SysTick64() / (SYSCLK_MHZ *1000);
}

uint64_t SysTick_Micros()
{
  return SysTick64() / SYSCLK_MHZ;
}

void delay_us(unsigned long us)
{
  uint32_t finish = SysTick32() + (us * SYSCLK_MHZ);
  while (((int32_t)(finish - SysTick32())) > 0);
}

void delay_ms(unsigned long ms)
{
  for (; ms; ms--)  // while (ms--)
  {
    delay_us(1000);
  }
}

uint64_t millis(void)
{
  return SysTick64() / (SYSCLK_MHZ *1000);
}



