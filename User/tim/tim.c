  #include "./tim/tim.h"

 

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
//��ʱ50us
 BOOL TIM_Mode_Config(uint16_t usTim1Timerout50us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	TIM_DeInit(BASIC_TIM); //���½�Timer����Ϊȱʡֵ

	// ����TIMx_CLK 
  	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		     
	TIM_TimeBaseStructure.TIM_Period = usTim1Timerout50us-1;  
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
  	//				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=84MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  	TIM_TimeBaseStructure.TIM_Prescaler = 4200-1;	//50US
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
	
	 // �����ʱ�������жϱ�־λ
	 TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);
	 
	 // ������ʱ�������ж�
	 TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	 // ���ܶ�ʱ��
	 TIM_Cmd(BASIC_TIM, DISABLE);


	 // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	return TRUE;
}






/**
  * @brief  ������ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ1
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = SECOND_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}




static void SECOND_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ����TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(SECOND_TIM_CLK, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������9999����Ϊ10000�Σ�Ϊһ����ʱ����
//  TIM_TimeBaseStructure.TIM_Period = 10000-1;       
	TIM_TimeBaseStructure.TIM_Period = 10-1;  
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=84MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	
	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(SECOND_TIM, &TIM_TimeBaseStructure);
	
	
	// �����ʱ�������жϱ�־λ
	TIM_ClearFlag(SECOND_TIM, TIM_FLAG_Update);
	
	// ������ʱ�������ж�
	TIM_ITConfig(SECOND_TIM,TIM_IT_Update,ENABLE);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(SECOND_TIM, ENABLE);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1s����һ���ж�
  * @param  ��
  * @retval ��
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
