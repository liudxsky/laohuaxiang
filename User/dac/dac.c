#include "./dac/dac.h"

uint8_t dac_config_flag = 0;

//DACͨ��1�����ʼ��
void DAC1_Init(void)
{  
  	GPIO_InitTypeDef  GPIO_InitStructure;
  	DAC_InitTypeDef   DAC_InitType;
	
  	RCC_AHB1PeriphClockCmd(DAC_CH1_GPIO_CLK|DAC_CH2_GPIO_CLK, ENABLE);	//ʹ��GPIOAʱ��
  	RCC_APB1PeriphClockCmd(DAC_CLK, ENABLE);							//ʹ��DACʱ��

	if (dac_config_flag == 0)
	{
	  	GPIO_InitStructure.GPIO_Pin = DAC_CH1_GPIO_PIN;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;						//ģ������
	  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;						//����
	  	GPIO_Init(DAC_CH1_GPIO_PORT, &GPIO_InitStructure);					//��ʼ��

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;							//��ʹ�ô������� TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;			//��ʹ�ò��η���
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//���Ρ���ֵ����
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;			//DAC1�������ر� BOFF1=1
	  	DAC_Init(DAC_CH1_CHANNEL,&DAC_InitType);	 						//��ʼ��DACͨ��1

		DAC_Cmd(DAC_CH1_CHANNEL, ENABLE);   								//ʹ��DACͨ��1
		DAC_SetChannel1Data(DAC_Align_12b_R, 0);  							//12λ�Ҷ������ݸ�ʽ����DACֵ
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = DAC_CH2_GPIO_PIN;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;						//ģ������
	  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;						//����
	  	GPIO_Init(DAC_CH2_GPIO_PORT, &GPIO_InitStructure);					//��ʼ��

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;							//��ʹ�ô������� TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;			//��ʹ�ò��η���
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//���Ρ���ֵ����
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;			//DAC1�������ر� BOFF1=1
	  	DAC_Init(DAC_CH2_CHANNEL,&DAC_InitType);	 						//��ʼ��DACͨ��2

		DAC_Cmd(DAC_CH2_CHANNEL, ENABLE);   								//ʹ��DACͨ��2
		DAC_SetChannel2Data(DAC_Align_12b_R, 0);  							//12λ�Ҷ������ݸ�ʽ����DACֵ
	}
  	
}
void Dac_Select_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure; //����ṹ��

	RCC_AHB1PeriphClockCmd(DAC_SELECT_GPIO_CLK, ENABLE);//��������ʱ��

	  
	GPIO_InitStructure.GPIO_Pin = DAC_SELECT1_GPIO_PIN|DAC_SELECT2_GPIO_PIN ;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                     //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;           	 //Gpio�������Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 //�������Ų�����Ҳ������ģʽ		
	GPIO_Init(DAC_SELECT_GPIO_PORT, &GPIO_InitStructure); 
}




/****************************************************************************
* ��    ��: void Dac1_Set_Vol(double vol)
* ��    �ܣ�����ͨ��1�����ѹ
* ��ڲ�����vol:0~3300,����0~3.3V
* ���ز�������
* ˵    ����       
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
		DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
	}
	else
	{
		DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
	}
}
