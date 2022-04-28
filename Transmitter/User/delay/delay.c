#include "delay.h"


#define AHB_INPUT  72  //请按RCC中设置的AHB时钟频率填写到这里（单位MHz�?


void Delay_us(u32 uS){ //uS微秒级延时程序（参考值即是延时数�?72MHz时最大�?233015�?	
	SysTick->LOAD=AHB_INPUT*uS;      //重装计数初值（当主频是72MHz�?72次为1微秒�?
	SysTick->VAL=0x00;        //清空定时器的计数�?
	SysTick->CTRL=0x00000005;//时钟源HCLK，打开定时�?
	while(!(SysTick->CTRL&0x00010000)); //等待计数�?0
	SysTick->CTRL=0x00000004;//关闭定时�?
}

void Delay_ms(u16 ms){ //mS毫秒级延时程序（参考值即是延时数，最大�?65535�?	 		  	  
	while( ms-- != 0){
		delay_us(1000);	//调用1000微秒的延�?
	}
}
 
void Delay_s(u16 s){ //S秒级延时程序（参考值即是延时数，最大�?65535�?	 		  	  
	while( s-- != 0){
		delay_ms(1000);	//调用1000毫秒的延�?
	}
} 

/*********************************************************************************************
 * 杜洋工作�? www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/


#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果需要使用OS,则包括下面的头文件即�?.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h"
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用�?
//ALIENTEK STM32开发板
//使用SysTick的普通计数模式对延迟进行管理(支持OS)
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论�?:www.openedv.com
//创建日期:2016/11/28
//版本：V1.8
//版权所有，盗版必究�?
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//修改说明
//////////////////////////////////////////////////////////////////////////////////  

static u8  fac_us=0;							//us延时倍乘�?			   
static u16 fac_ms=0;							//ms延时倍乘�?,在ucos�?,代表每个节拍的ms�?
	
	
extern void xPortSysTickHandler(void);

//systick中断服务函数,使用ucos时用�?
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}
		   
//初始化延迟函�?
//SYSTICK的时钟固定为AHB时钟，基础例程里面SYSTICK时钟频率为AHB/8
//这里为了兼容FreeRTOS，所以将SYSTICK的时钟频率改为AHB的频率！
//SYSCLK:系统时钟频率
void delay_init()
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//选择外部时钟  HCLK
	fac_us=SystemCoreClock/1000000;				//不论是否使用OS,fac_us都需要使�?
	reload=SystemCoreClock/1000000;				//每秒钟的计数次数 单位为M  
	reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间
												//reload�?24位寄存器,最大�?:16777216,�?72M�?,约合0.233s左右	
	fac_ms=1000/configTICK_RATE_HZ;				//代表OS可以延时的最少单�?	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//�?1/configTICK_RATE_HZ秒中断一�?	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
}								    


//延时nus
//nus:要延时的us�?.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=168)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的�?	    	 
	ticks=nus*fac_us; 						//需要的节拍�? 
	told=SysTick->VAL;        				//刚进入时的计数器�?
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退�?.
		}  
	};										    
}  
//延时nms
//nms:要延时的ms�?
//nms:0~65535
void delay_ms(u32 nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周�? 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时�?,采用普通方式延�?    
	}
	delay_us((u32)(nms*1000));				//普通方式延�?
}

//延时nms,不会引起任务调度
//nms:要延时的ms�?
void delay_xms(u32 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}




































































