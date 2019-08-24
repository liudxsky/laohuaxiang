#include "./adc/adc.h"



__IO uint16_t ADC_ConvertedValue[ADC_NOFCHANEL] = {0};
__IO uint16_t VREFINT_CAL = 0;


__IO double Temp_Adc_Value = 0;

static void Analog_ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(BACK_ADC_GPIO_CLK, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = BACK_ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(BACK_ADC_GPIO_PORT, &GPIO_InitStructure);
		
}



static void Analog_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
// ------------------DMA Init 结构体参数 初始化--------------------------
 	// 开启DMA时钟
  	RCC_AHB1PeriphClockCmd(BACK_ANALOG_ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = BACK_ANALOG_ADC_DR_ADDR;	
  	// 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;  
  	// 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = ADC_NOFCHANEL;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	// 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
   	// 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  	//	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	// 禁止DMA FIFO	，使用直连模式
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  	// FIFO 大小，FIFO模式禁止时，这个不用配置	
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
 	DMA_InitStructure.DMA_Channel = BACK_ANALOG_ADC_DMA_CHANNEL; 
  	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(BACK_ANALOG_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  	DMA_Cmd(BACK_ANALOG_ADC_DMA_STREAM, ENABLE);
	
	// 开启ADC时钟
	RCC_APB2PeriphClockCmd(BACK_ANALOG_ADC_CLK , ENABLE);
  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  	// 时钟为fpclk x分频	
  	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  	// 禁止DMA直接访问模式	
  	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  	// 采样时间间隔	
  	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  	ADC_CommonInit(&ADC_CommonInitStructure);

//	//使能参考电压
//	ADC_TempSensorVrefintCmd(ENABLE);
//	VREFINT_CAL = *(__IO uint16_t *)(0x1FFF7A2A);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStructure);
  	// ADC 分辨率
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	// 扫描模式，多通道采集需要	
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  	// 连续转换	
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  	//禁止外部边沿触发
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	//外部触发通道，本例子使用软件触发，此值随便赋值即可
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  	//数据右对齐	
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	//转换通道 1个
  	ADC_InitStructure.ADC_NbrOfConversion = ADC_NOFCHANEL;                                    
  	ADC_Init(BACK_ANALOG_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  	// 配置 ADC 通道转换顺序 ，采样时间时钟周期
	ADC_RegularChannelConfig(BACK_ANALOG_ADC, BACK_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);
	
  	// 使能DMA请求 after last transfer (Single-ADC mode)
  	ADC_DMARequestAfterLastTransferCmd(BACK_ANALOG_ADC, ENABLE);

  	// 使能ADC DMA
  	ADC_DMACmd(BACK_ANALOG_ADC, ENABLE);

	// 使能ADC
  	ADC_Cmd(BACK_ANALOG_ADC, ENABLE); 

	 	
  	//开始adc转换，软件触发
  	ADC_SoftwareStartConv(BACK_ANALOG_ADC);
	
}




void  temp_adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// ------------------DMA Init 结构体参数 初始化--------------------------
 	// ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
 	// 开启DMA时钟
  	RCC_AHB1PeriphClockCmd(BACK_ANALOG_ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = TEMP_ADC_DR_ADDR;	
  	// 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Temp_Adc_Value;  
  	// 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = ADC_NOFCHANEL;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	// 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
   	// 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  	//	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	// 禁止DMA FIFO	，使用直连模式
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  	// FIFO 大小，FIFO模式禁止时，这个不用配置	
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
 	DMA_InitStructure.DMA_Channel = BACK_ANALOG_ADC_DMA_CHANNEL; 
  	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(BACK_ANALOG_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  	DMA_Cmd(BACK_ANALOG_ADC_DMA_STREAM, ENABLE);
	
	// 开启ADC时钟
	RCC_APB2PeriphClockCmd(TEMP_ADC_CLK , ENABLE);

	// -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  	// 时钟为fpclk x分频	
  	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  	// 禁止DMA直接访问模式	
  	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  	// 采样时间间隔	
  	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  	ADC_CommonInit(&ADC_CommonInitStructure);

	 // -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStructure);
  	// ADC 分辨率
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	// 扫描模式，多通道采集需要	
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  	// 连续转换	
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  	//禁止外部边沿触发
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	//外部触发通道，本例子使用软件触发，此值随便赋值即可
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  	//数据右对齐	
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	//转换通道 1个
  	ADC_InitStructure.ADC_NbrOfConversion = ADC_NOFCHANEL;                                    
  	ADC_Init(TEMP_ADC, &ADC_InitStructure);

	// 配置 ADC 通道转换顺序 ，采样时间时钟周期
	ADC_RegularChannelConfig(TEMP_ADC, TEMP_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);

	ADC_TempSensorVrefintCmd(ENABLE);//使能温度传感器的基准电源 

	// 使能DMA请求 after last transfer (Single-ADC mode)
  	ADC_DMARequestAfterLastTransferCmd(TEMP_ADC, ENABLE);

  	// 使能ADC DMA
  	ADC_DMACmd(TEMP_ADC, ENABLE);

	
	// 使能ADC
  	ADC_Cmd(TEMP_ADC, ENABLE); 
 	
  	//开始adc转换，软件触发
  	ADC_SoftwareStartConv(TEMP_ADC);

}

//ADC值转换成温度值
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







