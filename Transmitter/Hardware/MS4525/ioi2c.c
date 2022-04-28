#include "ioi2c.h"
#include "delay.h"

#define delay() delay_us(2)

//--------------------------------------------
void SDA_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = MYI2C_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MYI2C_SDA_PORT, &GPIO_InitStruct);
}


void SDA_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = MYI2C_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MYI2C_SDA_PORT, &GPIO_InitStruct);
}

//产生IIC起始信号

void I2C_Start(void)
{
    SDA_Output();
    SDA_Dout_HIGH();
    SCL_Dout_HIGH();
    delay();
    SDA_Dout_LOW();
    delay();
    SCL_Dout_LOW();
    delay();
}


//产生IIC停止信号

void I2C_Stop(void)
{
    // 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号
    SDA_Output();
    SDA_Dout_LOW();
    SCL_Dout_HIGH();
    delay();
    SDA_Dout_HIGH();
    delay();
}

uint8_t I2C_Wait_Ack(void)
{
    uint8_t re;
    SDA_Output();
    SDA_Dout_HIGH();	/* CPU释放SDA总线 */
    SDA_Input();
	delay();
    SCL_Dout_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    delay();
    for(re=0;re<100;re++){
        if (SDA_Data_IN())	//1为非应答，0为应答
        {
            delay_us(1);
        }
        else
        {
            SCL_Dout_LOW();
            delay();
            return 0;
        }
    }
    SCL_Dout_LOW();
    delay();
    return 1;
}

//产生ACK应答

void I2C_Ack(void)
{
    SDA_Output();
    SDA_Dout_LOW();	/* CPU驱动SDA = 0 */
    delay();
    SCL_Dout_HIGH();	/* CPU产生1个时钟 */
    delay();
    SCL_Dout_LOW();
    delay();
    SDA_Dout_HIGH();	/* CPU释放SDA总线 */
}

//不产生ACK应答

void I2C_NAck(void)
{
    SDA_Output();
    SDA_Dout_HIGH();	/* CPU驱动SDA = 1 */
    delay();
    SCL_Dout_HIGH();	/* CPU产生1个时钟 */
    delay();
    SCL_Dout_LOW();
    delay();
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void I2C_Send_Byte(uint8_t txd)
{
    uint8_t i;
    SDA_Output();

    // 先发送字节的高位bit7
    for (i = 0; i < 8; i++)
    {
        if (txd & 0x80)
        {
            SDA_Dout_HIGH();
        }
        else
        {
            SDA_Dout_LOW();
        }

        delay();
        SCL_Dout_HIGH();
        delay();
        SCL_Dout_LOW();

        if (i == 7)
        {
            SDA_Dout_HIGH(); 	// 释放总线
        }

        txd <<= 1;		// 左移一个bit
        delay();
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t I2C_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;
    SDA_Input();
    /* 读到第1个bit为数据的bit7 */
    value = 0;

    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        SCL_Dout_HIGH();
        delay();

        if (SDA_Data_IN())
        {
            value++;
        }

        SCL_Dout_LOW();
        delay();
    }

    if(ack) I2C_Ack();
    else I2C_NAck();

    return value;
}
