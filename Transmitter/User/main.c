/**************************************************
							空速测量项目 发射端程序
**************************************************/

#include "main.h"

u16 MS4525_Data;
u8 count=0, Data[8]={100}; 
u8 str[6]={0},temp1,temp2,temp3;
float Airspeed;

#define START_TASK_PRIO		1
#define START_STK_SIZE		128
TaskHandle_t	StartTask_Handle;
void start_task(void *pvParameters);

#define TASK1_TASK_PRIO		1
#define TASK1_STK_SIZE		128
TaskHandle_t	Task1Task_Handle;
void task1_task(void *pvParameters);

#define TASK2_TASK_PRIO		2
#define TASK2_STK_SIZE		128
TaskHandle_t	Task2Task_Handle;
void task2_task(void *pvParameters); 

#define TASK3_TASK_PRIO		1
#define TASK3_STK_SIZE		128
TaskHandle_t	Task3Task_Handle;
void task3_task(void *pvParameters);

void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	
	xTaskCreate((TaskFunction_t	)task1_task,
							(const char*		)"task1_task",
							(u16				)TASK1_STK_SIZE,
							(void*				)NULL,
							(UBaseType_t		)TASK1_TASK_PRIO,
							(TaskHandle_t		)&Task1Task_Handle);
							
	xTaskCreate((TaskFunction_t	)task2_task,
							(const char*		)"task2_task",
							(u16				)TASK2_STK_SIZE,
							(void*				)NULL,
							(UBaseType_t		)TASK2_TASK_PRIO,
							(TaskHandle_t		)&Task2Task_Handle);
							
	xTaskCreate((TaskFunction_t	)task3_task,
							(const char*		)"task3_task",
							(u16				)TASK3_STK_SIZE,
							(void*				)NULL,
							(UBaseType_t		)TASK3_TASK_PRIO,
							(TaskHandle_t		)&Task3Task_Handle);
							
	vTaskDelete(StartTask_Handle);
	taskEXIT_CRITICAL();
							
}
// 任务1：
void task1_task(void *pvparameters)
{
	while(1){
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		vTaskDelay(500);
	}
}
// 任务2：更新空速
void task2_task(void *pvparameters)
{
	while(1){

		MS4525_Data = MS4525DO_ReadData();
		Airspeed = MS4525_GetSpeed();

		vTaskDelay(5);
		temp2++;

		if(temp2 == 20){

			sprintf((char*)str, " %.1f m/s ",Airspeed);

			OLED_ShowNum(48,2,MS4525_Data,5,16);
			OLED_ShowString(30,4,str,16);

			temp2 = 0;
		}
	}
}
// 任务3：
void task3_task(void *pvParameters)
{
	while(1){


		vTaskDelay(100);
	}
}

int main(void)
{	
	//系统初始化
	SystemInit();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	// 配置串口
	USART1_Init(9600);
	
	//OLED初始化
	OLED_Init();
	OLED_Clear();  

	//空速管初始化
	if(MS4525DO_Init()){
		printf("初始化失败\n\r");
		OLED_ShowString(8,0,"MS4525 Failed",16);
	}
	
	Delay_ms(1000);

	xTaskCreate((TaskFunction_t	)start_task,
							(const char*		)"start_task",
							(u16				)START_STK_SIZE,
							(void*				)NULL,
							(UBaseType_t		)START_TASK_PRIO,
							(TaskHandle_t		)&StartTask_Handle);
	vTaskStartScheduler();
}
/*********************************************END OF FILE**********************/
