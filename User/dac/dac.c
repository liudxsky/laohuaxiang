#include "./dac/dac.h"

uint8_t dac_config_flag = 0;

//DAC通道1输出初始化
void DAC1_Init(void)
{  
  	GPIO_InitTypeDef  GPIO_InitStructure;
  	DAC_InitTypeDef   DAC_InitType;
	
  	RCC_AHB1PeriphClockCmd(DAC_CH1_GPIO_CLK|DAC_CH2_GPIO_CLK, ENABLE);	//使能GPIOA时钟
  	RCC_APB1PeriphClockCmd(DAC_CLK, ENABLE);							//使能DAC时钟

	if (dac_config_flag == 0)
	{
	  	GPIO_InitStructure.GPIO_Pin = DAC_CH1_GPIO_PIN;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;						//模拟输入
	  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;						//下拉
	  	GPIO_Init(DAC_CH1_GPIO_PORT, &GPIO_InitStructure);					//初始化

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;							//不使用触发功能 TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;			//不使用波形发生
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//屏蔽、幅值设置
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;			//DAC1输出缓存关闭 BOFF1=1
	  	DAC_Init(DAC_CH1_CHANNEL,&DAC_InitType);	 						//初始化DAC通道1

		DAC_Cmd(DAC_CH1_CHANNEL, ENABLE);   								//使能DAC通道1
		DAC_SetChannel1Data(DAC_Align_12b_R, 0);  							//12位右对齐数据格式设置DAC值
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = DAC_CH2_GPIO_PIN;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;						//模拟输入
	  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;						//下拉
	  	GPIO_Init(DAC_CH2_GPIO_PORT, &GPIO_InitStructure);					//初始化

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;							//不使用触发功能 TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;			//不使用波形发生
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//屏蔽、幅值设置
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;			//DAC1输出缓存关闭 BOFF1=1
	  	DAC_Init(DAC_CH2_CHANNEL,&DAC_InitType);	 						//初始化DAC通道2

		DAC_Cmd(DAC_CH2_CHANNEL, ENABLE);   								//使能DAC通道2
		DAC_SetChannel2Data(DAC_Align_12b_R, 0);  							//12位右对齐数据格式设置DAC值
	}
  	
}
void Dac_Select_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure; //定义结构体

	RCC_AHB1PeriphClockCmd(DAC_SELECT_GPIO_CLK, ENABLE);//开启外设时钟

	  
	GPIO_InitStructure.GPIO_Pin = DAC_SELECT1_GPIO_PIN|DAC_SELECT2_GPIO_PIN ;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                     //输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;           	 //Gpio输出速率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 //设置引脚不上拉也不下拉模式		
	GPIO_Init(DAC_SELECT_GPIO_PORT, &GPIO_InitStructure); 
}




/****************************************************************************
* 名    称: void Dac1_Set_Vol(double vol)
* 功    能：设置通道1输出电压
* 入口参数：vol:0~3300,代表0~3.3V
* 返回参数：无
* 说    明：       
****************************************************************************/

void Dac1_Set_Vol(double vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	if(temp >= 4096)
	{
		temp = 4095;
	}
	if(dac_config_flag == 0)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
	}
	else
	{
		DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
	}
}
