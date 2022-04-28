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

//����IIC��ʼ�ź�

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


//����IICֹͣ�ź�

void I2C_Stop(void)
{
    // ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź�
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
    SDA_Dout_HIGH();	/* CPU�ͷ�SDA���� */
    SDA_Input();
	delay();
    SCL_Dout_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    delay();
    for(re=0;re<100;re++){
        if (SDA_Data_IN())	//1Ϊ��Ӧ��0ΪӦ��
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

//����ACKӦ��

void I2C_Ack(void)
{
    SDA_Output();
    SDA_Dout_LOW();	/* CPU����SDA = 0 */
    delay();
    SCL_Dout_HIGH();	/* CPU����1��ʱ�� */
    delay();
    SCL_Dout_LOW();
    delay();
    SDA_Dout_HIGH();	/* CPU�ͷ�SDA���� */
}

//������ACKӦ��

void I2C_NAck(void)
{
    SDA_Output();
    SDA_Dout_HIGH();	/* CPU����SDA = 1 */
    delay();
    SCL_Dout_HIGH();	/* CPU����1��ʱ�� */
    delay();
    SCL_Dout_LOW();
    delay();
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void I2C_Send_Byte(uint8_t txd)
{
    uint8_t i;
    SDA_Output();

    // �ȷ����ֽڵĸ�λbit7
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
            SDA_Dout_HIGH(); 	// �ͷ�����
        }

        txd <<= 1;		// ����һ��bit
        delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t I2C_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;
    SDA_Input();
    /* ������1��bitΪ���ݵ�bit7 */
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
