#include "tim.h"

void TIMx_Init(u16 arr,u16 psc){  														//TIM3 ��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx,ENABLE);				//ʹ��TIMx
    TIMx_NVIC_Init(); 																				//����TIMx�ж�����
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr-1; 									//�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc-1; 								//Ԥ��Ƶϵ��
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; 		//ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
    TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStrue); 						//TIMx��ʼ������
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);								//ʹ��TIMx�ж�    
    TIM_Cmd(TIMx,ENABLE); 																		//ʹ��TIMx
}

void TIMx_NVIC_Init (void){ 																	//����TIMx�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//������ռ�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

u8 float2char(float Xfloat)
{
//	u8 Xchar;
	if(Xfloat >= 128)
		return 255;
	else if(Xfloat <= -128)
		return 0;
	else return (int)Xfloat+128;
}

//void TIMx_IRQHandler(void){ 																	//TIMx�жϴ�����
//	
//	if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET){				//�ж��Ƿ���TIMx�ж�
//			TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
//			
//	}
//} 


