#ifndef __IOI2C_H__
#define __IOI2C_H__
 
#include "stm32f10x.h"
#include "stdint.h"
 
#define CPU_FREQUENCY_MHZ   72
#define MYI2C_SCL_PIN	  	GPIO_Pin_6
#define MYI2C_SCL_PORT		GPIOB
#define MYI2C_SDA_PIN		GPIO_Pin_7
#define MYI2C_SDA_PORT		GPIOB
 
#define SDA_Dout_LOW()      GPIO_ResetBits(MYI2C_SDA_PORT,MYI2C_SDA_PIN) 
#define SDA_Dout_HIGH()     GPIO_SetBits(MYI2C_SDA_PORT,MYI2C_SDA_PIN) 
#define SDA_Data_IN()       PBin(7)
#define SCL_Dout_LOW()      GPIO_ResetBits(MYI2C_SCL_PORT,MYI2C_SCL_PIN)
#define SCL_Dout_HIGH()     GPIO_SetBits(MYI2C_SCL_PORT,MYI2C_SCL_PIN)
// #define SDA_Write(XX)   	

void I2C_Start(void);
void I2C_Stop(void);
void I2C_Send_Byte(uint8_t txd); 
uint8_t I2C_Read_Byte(uint8_t ack);
void I2C_NAck(void);
void I2C_Ack(void);
uint8_t I2C_Wait_Ack(void);
 
#endif
