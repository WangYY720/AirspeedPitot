#include "ms4525do.h"
#include "ioi2c.h"
#include "math.h"

#define   	MS4525DO_ADDR1	0x28
#define 	MS4525DO_READ	0x51
#define     PRESSURE_STA    8166
#define     PSI2Pa          6894.75
#define 	DENSITY			1.225

// 滤波数量
#define 	MS4525_FilterSize	300
#define 	Airspeed_FilterSize	300

// 空速矫正系数
#define 	Ratio	0.5

// MS4525滤波
int MS4525_DataSum=0;
u16 MS4525_DataNO=0,MS4525_IsFilterFull=0;
u16 MS4525_DataFilter[MS4525_FilterSize];

// 空速滤波
double Airspeed_DataSum=0;
u16 Airspeed_DataNO=0,Airspeed_IsFilterFull=0;
float Airspeed_DataFilter[Airspeed_FilterSize];

// 从MS4525DO读取初始数据
uint16_t MS4525DO_ReadData(void)
{
	uint16_t pressure_dat;
	
	I2C_Start();
	I2C_Send_Byte(MS4525DO_READ);
	I2C_Wait_Ack();

	pressure_dat=I2C_Read_Byte(1);
	pressure_dat<<=8;
	pressure_dat|=I2C_Read_Byte(0);
	
	I2C_Stop();

	pressure_dat<<=2;
	pressure_dat>>=2;
	
	// 对MS4525初始读数进行滤波
	if(!MS4525_IsFilterFull){
		MS4525_DataFilter[MS4525_DataNO] = pressure_dat;
		MS4525_DataSum += pressure_dat;
		MS4525_DataNO++;
		if(MS4525_DataNO == MS4525_FilterSize){
			MS4525_IsFilterFull = 1;
			MS4525_DataNO = 0;
		}
		pressure_dat = MS4525_DataSum/MS4525_DataNO;
	}else{
		MS4525_DataSum -= MS4525_DataFilter[MS4525_DataNO];
		MS4525_DataSum += pressure_dat;
		MS4525_DataFilter[MS4525_DataNO] = pressure_dat;
		MS4525_DataNO++;
		if(MS4525_DataNO == MS4525_FilterSize){
			MS4525_DataNO = 0;
		}
		pressure_dat = MS4525_DataSum/MS4525_FilterSize;
	}

	return pressure_dat;
}
// 获得气压差（静压-动压）
float MS4525_GetDiffPressure(u16 Pressure_data)
{
	float temp;

	temp = (float)Pressure_data - (float)PRESSURE_STA;

	return temp*2.0*PSI2Pa/(0.8*16383);
}
// 获取空速
float MS4525_GetSpeed(void)
{
	u16 temp;
	float Diff_Pressure,Airspeed;
	temp = MS4525DO_ReadData();
	Diff_Pressure = MS4525_GetDiffPressure(temp);
	Airspeed = sqrt(2*Diff_Pressure/DENSITY);

	Airspeed = Airspeed*Ratio;

	// 空速滤波
	if(!Airspeed_IsFilterFull){
		Airspeed_DataFilter[Airspeed_DataNO] = Airspeed;
		Airspeed_DataSum += Airspeed;
		Airspeed_DataNO++;
		if(Airspeed_DataNO == Airspeed_FilterSize){
			Airspeed_IsFilterFull = 1;
			Airspeed_DataNO = 0;
		}
		Airspeed = Airspeed_DataSum/Airspeed_DataNO;
	}else{
		Airspeed_DataSum -= Airspeed_DataFilter[Airspeed_DataNO];
		Airspeed_DataSum += Airspeed;
		Airspeed_DataFilter[Airspeed_DataNO] = Airspeed;
		Airspeed_DataNO++;
		if(Airspeed_DataNO == Airspeed_FilterSize){
			Airspeed_DataNO = 0;
		}
		Airspeed = Airspeed_DataSum/Airspeed_FilterSize;
	}
	
	return Airspeed;
}
// MS4525DO初始化
uint8_t MS4525DO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = MYI2C_SCL_PIN | MYI2C_SDA_PIN; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MYI2C_SCL_PORT,&GPIO_InitStruct);
	GPIO_ResetBits(MYI2C_SCL_PORT,MYI2C_SCL_PIN|MYI2C_SDA_PIN);

	I2C_Start();
	I2C_Send_Byte(MS4525DO_READ);
	if(I2C_Wait_Ack())
	{
		I2C_Stop();
		return 1;
	}
	else
	{
		I2C_Stop();
		return 0;
	}
}
