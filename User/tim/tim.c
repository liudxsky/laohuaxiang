  #include "./tim/tim.h"

 

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
//定时50us
 BOOL TIM_Mode_Config(uint16_t usTim1Timerout50us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	TIM_DeInit(BASIC_TIM); //重新将Timer设置为缺省值

	// 开启TIMx_CLK 
  	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		     
	TIM_TimeBaseStructure.TIM_Period = usTim1Timerout50us-1;  
	//定时器时钟源TIMxCLK = 2 * PCLK1  
  	//				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=84MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  	TIM_TimeBaseStructure.TIM_Prescaler = 4200-1;	//50US
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
	
	 // 清除定时器更新中断标志位
	 TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);
	 
	 // 开启定时器更新中断
	 TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	 // 禁能定时器
	 TIM_Cmd(BASIC_TIM, DISABLE);


	 // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	return TRUE;
}






/**
  * @brief  基本定时器 TIMx,x[6,7]中断优先级配置
  * @param  无
  * @retval 无
  */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为1
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = SECOND_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}




static void SECOND_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(SECOND_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到9999，即为10000次，为一个定时周期
//  TIM_TimeBaseStructure.TIM_Period = 10000-1;       
	TIM_TimeBaseStructure.TIM_Period = 10-1;  
	//定时器时钟源TIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=84MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	
	
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(SECOND_TIM, &TIM_TimeBaseStructure);
	
	
	// 清除定时器更新中断标志位
	TIM_ClearFlag(SECOND_TIM, TIM_FLAG_Update);
	
	// 开启定时器更新中断
	TIM_ITConfig(SECOND_TIM,TIM_IT_Update,ENABLE);
	
	// 使能定时器
	TIM_Cmd(SECOND_TIM, ENABLE);	
}

/**
  * @brief  初始化基本定时器定时，1s产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Init(void)
{
	TIMx_NVIC_Configuration();	
  	SECOND_TIM_Config();
}



uint32_t getMsCounter(void)
{
	extern uint32_t _ms_counter;
	return _ms_counter;
}





/*********************************************END OF FILE**********************/
