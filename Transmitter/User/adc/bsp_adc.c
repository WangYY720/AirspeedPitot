#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue1;
__IO uint16_t ADC_ConvertedValue2;

/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADC_PIN_1 | ADC_PIN_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}

/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// ��ADCʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2, ENABLE );
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Init(ADC2, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_CHANNEL1, 1, 
	                         ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_CHANNEL2, 1, 
	                         ADC_SampleTime_55Cycles5);

	// ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);
	
	// ����ADC1 ������ʼת��
	ADC_Cmd(ADC1, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC1);
	
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC1);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1));
	// ADC��ʼУ׼
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	// ����ADC2 ������ʼת��
	ADC_Cmd(ADC2, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC2);
	
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC2));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC2);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC2));
	// ADC��ʼУ׼
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}

static void ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	// ���ȼ�����
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // �����ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
	ADC_NVIC_Config();
}
void ADC_IRQHandler(void)
{	
	if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
	{
		// ��ȡADC��ת��ֵ
		ADC_ConvertedValue1 = ADC_GetConversionValue(ADC1);
		
	}
	if (ADC_GetITStatus(ADC2,ADC_IT_EOC)==SET) 
	{
		// ��ȡADC��ת��ֵ
		ADC_ConvertedValue2 = ADC_GetConversionValue(ADC2);
		
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
	ADC_ClearITPendingBit(ADC2,ADC_IT_EOC);
}
/*********************************************END OF FILE**********************/
