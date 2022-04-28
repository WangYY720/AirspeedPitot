#ifndef _AIRSPEED_H
#define _AIRSPEED_H

#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"

#define LED_GPIO    GPIOB
#define LED_Pin     GPIO_Pin_15
#define LED_ON      GPIO_SetBits(LED_GPIO,LED_Pin)
#define LED_OFF     GPIO_ResetBits(LED_GPIO,LED_Pin)

void Airspeed_Init(void);
void Airspeed_readdata(u8* data);

#endif
