#ifndef __MS4525DO_H
#define __MS4525DO_H 												  	 

#include "stm32f10x.h"
 
//void MS4525DO_Init(void);
//void MS4525DO_SendData(uint8_t data);
uint16_t MS4525DO_ReadData(void);
uint8_t MS4525DO_Init(void);
float MS4525_GetDiffPressure(u16 Pressure_data);
float MS4525_GetSpeed(void);


#endif
