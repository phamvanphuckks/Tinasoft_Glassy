#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

void SystemClock_Config(void);
void SysTick_Init(void);
void SysTick_Handler(void);
uint64_t SysTick64(void);
uint32_t SysTick32(void);
uint32_t SysTick24(void);
uint64_t SysTick_Millis(void);
uint64_t SysTick_Micros(void);
void delay_us(unsigned long us);
void delay_ms(unsigned long ms);
uint64_t millis(void);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */
