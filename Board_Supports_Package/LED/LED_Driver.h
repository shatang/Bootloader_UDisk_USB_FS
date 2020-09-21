#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H
#include "stm32f4xx.h" 

/****************************** GLOBAL FUNCTIONS ******************************/
#define LED0_ON         GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define LED0_OFF        GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LED1_ON         GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define LED1_OFF        GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LED2_ON         GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define LED2_OFF        GPIO_ResetBits(GPIOA,GPIO_Pin_7)
/****************************** GLOBAL FUNCTIONS ******************************/

#define LED_1(x)       ((x)?LED0_ON:LED0_OFF)
#define LED_2(x)       ((x)?LED1_ON:LED1_OFF)
#define LED_3(x)       ((x)?LED2_ON:LED2_OFF)

extern void LED_PortInit(void);

#endif
