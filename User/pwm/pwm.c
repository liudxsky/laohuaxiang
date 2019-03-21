#include "./pwm/pwm.h"
#include "./screen/screen.h"
#include "hmi_driver.h"
#include "./flash/deviceinfo.h"


extern BIG_SCREEN_ID_TAB biglanguage_screen;;
extern dev_info_t dev_info;






/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
void SetPwmValue(int a)
{
	
	
if(a>dev_info.pwmscope)
	a=dev_info.pwmscope;
if(a<0)
	a=0;
dev_info.pwmvalue=a;
TIM_SetCompare1(TIM1,dev_info.pwmscope-dev_info.pwmvalue);
//TIM_PWMOUTPUT_Config();

}
void SetPwmScope(int a)
{
	dev_info.pwmscope=a;
	TIM_PWMOUTPUT_Config();
}
static void TIMx_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (PWM_GPIO_CLK, ENABLE); 
	GPIO_PinAFConfig(PWM_GPIO_PORT,PWM_PINSOURCE,PWM_AF); 
  
	GPIO_InitStructure.GPIO_Pin = PWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BACK_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStructure);
}



void TIM_PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	// TIMx_CLK 
  RCC_APB2PeriphClockCmd(PWM_TIMx_CLK, ENABLE); 

  TIM_TimeBaseStructure.TIM_Period = dev_info.pwmscope-1;       
	
  // TIMxCLK = HCLK/2=84MHz 
  // TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(84000-1);	
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;     
  TIM_TimeBaseInit(PWM_TIMx, &TIM_TimeBaseStructure);



 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 


 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

 TIM_OCInitStructure.TIM_Pulse = dev_info.pwmscope - dev_info.pwmvalue; 
 
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 


 TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
 
 TIM_OC1Init(PWM_TIMx, &TIM_OCInitStructure); 


 TIM_OC1PreloadConfig(PWM_TIMx, TIM_OCPreload_Enable); 
 
 TIM_ARRPreloadConfig(PWM_TIMx,ENABLE);//ARPEʹ�� 

		
 TIM_Cmd(PWM_TIMx, ENABLE);	
	
 TIM_CtrlPWMOutputs(PWM_TIMx, ENABLE);


}




/**
  * @brief  ��ʼ������ͨ�ö�ʱ��
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
  
  TIM_PWMOUTPUT_Config();
}

//PWM�������?
void Pwm_Output(FunctionalState NewState)
{
 	TIM_CtrlPWMOutputs(PWM_TIMx, NewState);
	if(NewState)
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,HIDE);
	}
	
}



/*********************************************END OF FILE**********************/
