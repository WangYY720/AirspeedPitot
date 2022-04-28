#include "airspeed.h"
#include "FreeRTOS.h"
#include "task.h"

#define AIRSPEED_I2C_ADDR    0x28
#define AIRSPEED_READ        0x51

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

#define GPIO_IIC        GPIOB
#define GPIO_SCL_Pin    GPIO_Pin_9
#define GPIO_SDA_Pin    GPIO_Pin_10

#define IIC_SCL    PBout(9) 		//SCL
#define IIC_SDA    PBout(10) 		//SDA	 
#define READ_SDA   PBin(10) 		//输入SDA 

#define WRITE   0x00
#define READ    0x01

void IIC_Delay(void)
{
	delay_us(2);
}
void _IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	IIC_Delay();
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void _IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	IIC_SCL=1;  
	IIC_SDA=1;//发送I2C总线结束信号
	IIC_Delay();							   	
}
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	IIC_Delay();
	IIC_SCL=1;
	IIC_Delay();
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	IIC_Delay();
	IIC_SCL=1;
	IIC_Delay();
	IIC_SCL=0;
}	
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 _IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;IIC_Delay();	   
	IIC_SCL=1;IIC_Delay();	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
u8 IIC_Read_Byte(u8 ack)
{
    unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        IIC_Delay();
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		IIC_Delay(); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		IIC_SCL=1;
		IIC_Delay(); 
		IIC_SCL=0;	
		IIC_Delay();
    }	 
} 
void Airspeed_readdata(u8* data)
{
    u8 temp,add=0;
    // _IIC_Start();
    // IIC_Send_Byte(AIRSPEED_I2C_ADDR<<1);            //Slave address,SA0=0
	// SDA_IN();
	// while(!READ_SDA){
	// }
	// IIC_Ack();
	for(add=0;add<255;add++){
		_IIC_Start();
   		 IIC_Send_Byte(add);            //Slave address,SA0=0
		temp = _IIC_Wait_Ack();	
    	if(temp){
        	LED_ON;
			_IIC_Stop();
		}
		else{
			LED_OFF;
			*(data+1) = add;
			return ;
		}
	}
    
	// _IIC_Start();
    // IIC_Send_Byte(0);
    // data[0] = IIC_Read_Byte(1);
    // data[1] = IIC_Read_Byte(1);
	// temp = _IIC_Wait_Ack();	
    // if(temp){
    //     LED_ON;
    // }
    // vTaskDelay(10);
    // Write_IIC_Byte(READ);			//write command
    
    // *data 	  = IIC_Read_Byte(1);
    // *(data+1) = IIC_Read_Byte(1);
	// *(data+2) = IIC_Read_Byte(1);
	// *(data+3) = IIC_Read_Byte(1);

	// *(data+4) = IIC_Read_Byte(1);
	// *(data+5) = IIC_Read_Byte(1);
	// *(data+6) = IIC_Read_Byte(1);
	// *(data+7) = IIC_Read_Byte(0);

    // _IIC_Stop();
}
void Airspeed_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //??A????
	GPIO_InitStructure.GPIO_Pin = GPIO_SCL_Pin|GPIO_SDA_Pin|LED_Pin|GPIO_Pin_0;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
 	GPIO_Init(GPIO_IIC, &GPIO_InitStructure);	  //???GPIO
 	GPIO_SetBits(GPIO_IIC,GPIO_SCL_Pin|GPIO_SDA_Pin);	
    LED_OFF;
}

