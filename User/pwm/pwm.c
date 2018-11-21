#include "./pwm/pwm.h"
#include "./screen/screen.h"
#include "hmi_driver.h"
#include "./flash/deviceinfo.h"


extern BIG_SCREEN_ID_TAB biglanguage_screen;;
extern dev_info_t dev_info;






/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (PWM_GPIO_CLK, ENABLE); 
  /* 定时器通道引脚复用 */
	GPIO_PinAFConfig(PWM_GPIO_PORT,PWM_PINSOURCE,PWM_AF); 
  
	/* 定时器通道引脚配置 */															   
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
	
	// 开启TIMx_CLK 
  RCC_APB2PeriphClockCmd(PWM_TIMx_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  TIM_TimeBaseStructure.TIM_Period = dev_info.pwmscope-1;       
	
  // 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 
  // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式采用中心定时
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
   //重复寄存器，用于自动更新pwm占空比
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;     
	// 初始化定时器TIMx, x[2,3,4,5,12,13,14] 
  TIM_TimeBaseInit(PWM_TIMx, &TIM_TimeBaseStructure);



 // 配置为 PWM 模式 1，先输出高电平，达到比较值的时候再改变电平 
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 // 主输出使能 
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 

 // 互补输出使能 
 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
 // 配置比较值 
 TIM_OCInitStructure.TIM_Pulse = dev_info.pwmscope - dev_info.pwmvalue; 
 // 主输出高电平有效 
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 

 // 空闲输出高
 TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
  
 // 通道初始化 
 TIM_OC1Init(PWM_TIMx, &TIM_OCInitStructure); 

// TIM_OC2Init(PWM_TIMx, &TIM_OCInitStructure);
 // 使能通道重装载 
 TIM_OC1PreloadConfig(PWM_TIMx, TIM_OCPreload_Enable); 
 
 TIM_ARRPreloadConfig(PWM_TIMx,ENABLE);//ARPE使能 

		
 // 使能定时器
 TIM_Cmd(PWM_TIMx, ENABLE);	
	
 /* 主动输出使能 ，输出pwm必须打开*/
 TIM_CtrlPWMOutputs(PWM_TIMx, ENABLE);


}




/**
  * @brief  初始化控制通用定时器
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
  
  TIM_PWMOUTPUT_Config();
}

//PWM输出控制
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
