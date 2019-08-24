#include "./adc/adc.h"



__IO uint16_t ADC_ConvertedValue[ADC_NOFCHANEL] = {0};
__IO uint16_t VREFINT_CAL = 0;


__IO double Temp_Adc_Value = 0;

static void Analog_ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(BACK_ADC_GPIO_CLK, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = BACK_ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(BACK_ADC_GPIO_PORT, &GPIO_InitStructure);
		
}



static void Analog_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
// ------------------DMA Init �ṹ����� ��ʼ��--------------------------
 	// ����DMAʱ��
  	RCC_AHB1PeriphClockCmd(BACK_ANALOG_ADC_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = BACK_ANALOG_ADC_DR_ADDR;	
  	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;  
  	// ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ��ָһ�δ����������
	DMA_InitStructure.DMA_BufferSize = ADC_NOFCHANEL;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	// �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
   	// �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  	//	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	// ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  	// FIFO ��С��FIFOģʽ��ֹʱ�������������	
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
 	DMA_InitStructure.DMA_Channel = BACK_ANALOG_ADC_DMA_CHANNEL; 
  	//��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(BACK_ANALOG_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  	DMA_Cmd(BACK_ANALOG_ADC_DMA_STREAM, ENABLE);
	
	// ����ADCʱ��
	RCC_APB2PeriphClockCmd(BACK_ANALOG_ADC_CLK , ENABLE);
  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  	// ʱ��Ϊfpclk x��Ƶ	
  	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  	// ��ֹDMAֱ�ӷ���ģʽ	
  	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  	// ����ʱ����	
  	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  	ADC_CommonInit(&ADC_CommonInitStructure);

//	//ʹ�ܲο���ѹ
//	ADC_TempSensorVrefintCmd(ENABLE);
//	VREFINT_CAL = *(__IO uint16_t *)(0x1FFF7A2A);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStructure);
  	// ADC �ֱ���
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	// ɨ��ģʽ����ͨ���ɼ���Ҫ	
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  	// ����ת��	
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  	//��ֹ�ⲿ���ش���
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	//�ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  	//�����Ҷ���	
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	//ת��ͨ�� 1��
  	ADC_InitStructure.ADC_NbrOfConversion = ADC_NOFCHANEL;                                    
  	ADC_Init(BACK_ANALOG_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  	// ���� ADC ͨ��ת��˳�� ������ʱ��ʱ������
	ADC_RegularChannelConfig(BACK_ANALOG_ADC, BACK_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);
	
  	// ʹ��DMA���� after last transfer (Single-ADC mode)
  	ADC_DMARequestAfterLastTransferCmd(BACK_ANALOG_ADC, ENABLE);

  	// ʹ��ADC DMA
  	ADC_DMACmd(BACK_ANALOG_ADC, ENABLE);

	// ʹ��ADC
  	ADC_Cmd(BACK_ANALOG_ADC, ENABLE); 

	 	
  	//��ʼadcת�����������
  	ADC_SoftwareStartConv(BACK_ANALOG_ADC);
	
}




void  temp_adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// ------------------DMA Init �ṹ����� ��ʼ��--------------------------
 	// ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
 	// ����DMAʱ��
  	RCC_AHB1PeriphClockCmd(BACK_ANALOG_ADC_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = TEMP_ADC_DR_ADDR;	
  	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Temp_Adc_Value;  
  	// ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ��ָһ�δ����������
	DMA_InitStructure.DMA_BufferSize = ADC_NOFCHANEL;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	// �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
   	// �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  	//	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	// ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  	// FIFO ��С��FIFOģʽ��ֹʱ�������������	
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
 	DMA_InitStructure.DMA_Channel = BACK_ANALOG_ADC_DMA_CHANNEL; 
  	//��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(BACK_ANALOG_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  	DMA_Cmd(BACK_ANALOG_ADC_DMA_STREAM, ENABLE);
	
	// ����ADCʱ��
	RCC_APB2PeriphClockCmd(TEMP_ADC_CLK , ENABLE);

	// -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  	// ʱ��Ϊfpclk x��Ƶ	
  	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  	// ��ֹDMAֱ�ӷ���ģʽ	
  	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  	// ����ʱ����	
  	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  	ADC_CommonInit(&ADC_CommonInitStructure);

	 // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStructure);
  	// ADC �ֱ���
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	// ɨ��ģʽ����ͨ���ɼ���Ҫ	
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  	// ����ת��	
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  	//��ֹ�ⲿ���ش���
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	//�ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  	//�����Ҷ���	
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	//ת��ͨ�� 1��
  	ADC_InitStructure.ADC_NbrOfConversion = ADC_NOFCHANEL;                                    
  	ADC_Init(TEMP_ADC, &ADC_InitStructure);

	// ���� ADC ͨ��ת��˳�� ������ʱ��ʱ������
	ADC_RegularChannelConfig(TEMP_ADC, TEMP_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);

	ADC_TempSensorVrefintCmd(ENABLE);//ʹ���¶ȴ������Ļ�׼��Դ 

	// ʹ��DMA���� after last transfer (Single-ADC mode)
  	ADC_DMARequestAfterLastTransferCmd(TEMP_ADC, ENABLE);

  	// ʹ��ADC DMA
  	ADC_DMACmd(TEMP_ADC, ENABLE);

	
	// ʹ��ADC
  	ADC_Cmd(TEMP_ADC, ENABLE); 
 	
  	//��ʼadcת�����������
  	ADC_SoftwareStartConv(TEMP_ADC);

}

//ADCֵת�����¶�ֵ
void adc_temp_value(void)
{
	ADC_ConvertedValue[1] = (ADC_ConvertedValue[1]*(3.3/4096) - 0.76)/2.5 + 25;
}




void Analog_Init(void)
{
	Analog_ADC_Config();
	Analog_ADC_Mode_Config();
//	temp_adc_config();
}







