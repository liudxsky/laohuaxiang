#include "./delaytime/delaytime.h"
#include "stm32f4xx_rcc.h"

/*
 * 使用HSE时，设置系统时钟的步骤
 * 1、开启HSE ，并等待 HSE 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源
 *    设置VCO输入时钟 分频因子        m
 *    设置VCO输出时钟 倍频因子        n
 *    设置PLLCLK时钟分频因子          p
 *    设置OTG FS,SDIO,RNG时钟分频因子 q
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/*
 * m: VCO输入时钟 分频因子，取值2~63
 * n: VCO输出时钟 倍频因子，取值192~432
 * p: PLLCLK时钟分频因子  ，取值2，4，6，8
 * q: OTG FS,SDIO,RNG时钟分频因子，取值4~15
 * 函数调用举例，使用HSE设置时钟
 * SYSCLK=HCLK=168M,PCLK2=HCLK/2=84M,PCLK1=HCLK/4=42M
 * HSE_SetSysClock(25, 336, 2, 7);
 * HSE作为时钟来源，经过PLL倍频作为系统时钟，这是通常的做法
 
 * 系统时钟超频到216M爽一下
 * HSE_SetSysClock(25, 432, 2, 9);
 */
void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)	
{
  __IO uint32_t HSEStartUpStatus = 0;
  
  // 使能HSE，开启外部晶振，秉火F407使用 HSE=25M
  RCC_HSEConfig(RCC_HSE_ON);
	
  // 等待HSE启动稳定
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    // 调压器电压输出级别配置为1，以便在器件为最大频率
		// 工作时使性能和功耗实现平衡
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		
    // 如果要超频就得在这里下手啦
		// 设置PLL来源时钟，设置VCO分频因子m，设置VCO倍频因子n，
		//  设置系统时钟分频因子p，设置OTG FS,SDIO,RNG分频因子q
		RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
		
		// 使能PLL
		RCC_PLLCmd(ENABLE);
  
	  // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }   

/*-----------------------------------------------------*/
    //开启 OVER-RIDE模式，以能达到更高频率(F429才有该功能)
//    PWR->CR |= PWR_CR_ODEN;
//    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
//    {
//    }
//    PWR->CR |= PWR_CR_ODSWEN;
//    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
//    {
//    }      
    // 配置FLASH预取指,指令缓存,数据缓存和等待状态
    FLASH->ACR = FLASH_ACR_PRFTEN 
		            | FLASH_ACR_ICEN 
		            | FLASH_ACR_DCEN 
		            | FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/
		
		// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // 读取时钟切换状态位，确保PLLCLK被选为系统时钟
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { // HSE启动出错处理

    while (1)
    {
    }
  }
}

/*
 * 使用HSI时，设置系统时钟的步骤
 * 1、开启HSI ，并等待 HSI 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源
 *    设置VCO输入时钟 分频因子        m
 *    设置VCO输出时钟 倍频因子        n
 *    设置SYSCLK时钟分频因子          p
 *    设置OTG FS,SDIO,RNG时钟分频因子 q
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/*
 * m: VCO输入时钟 分频因子，取值2~63
 * n: VCO输出时钟 倍频因子，取值192~432
 * p: PLLCLK时钟分频因子  ，取值2，4，6，8
 * q: OTG FS,SDIO,RNG时钟分频因子，取值4~15
 * 函数调用举例，使用HSI设置时钟
 * SYSCLK=HCLK=168M,PCLK2=HCLK/2=84M,PCLK1=HCLK/4=42M
 * HSI_SetSysClock(16, 336, 2, 7);
 * HSE作为时钟来源，经过PLL倍频作为系统时钟，这是通常的做法
 
 * 系统时钟超频到216M爽一下
 * HSI_SetSysClock(16, 432, 2, 9);
 */

void HSI_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)	
{
  __IO uint32_t HSIStartUpStatus = 0;
	
	// 把RCC外设初始化成复位状态
  RCC_DeInit();
  
  //使能HSI, HSI=16M
	RCC_HSICmd(ENABLE);
	
  // 等待 HSI 就绪
	HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;
	
	// 只有 HSI就绪之后则继续往下执行
  if (HSIStartUpStatus == RCC_CR_HSIRDY)
  {
    // 调压器电压输出级别配置为1，以便在器件为最大频率
		// 工作时使性能和功耗实现平衡
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		
    // 如果要超频就得在这里下手啦
		// 设置PLL来源时钟，设置VCO分频因子m，设置VCO倍频因子n，
		//  设置系统时钟分频因子p，设置OTG FS,SDIO,RNG分频因子q
		RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);
		
		// 使能PLL
		RCC_PLLCmd(ENABLE);
  
	  // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }   

/*-----------------------------------------------------*/
    //开启 OVER-RIDE模式，以能达到更高频率(F429才有该功能)
//    PWR->CR |= PWR_CR_ODEN;
//    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
//    {
//    }
//    PWR->CR |= PWR_CR_ODSWEN;
//    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
//    {
//    }      
    // 配置FLASH预取指,指令缓存,数据缓存和等待状态
    FLASH->ACR = FLASH_ACR_PRFTEN 
		            | FLASH_ACR_ICEN 
		            |FLASH_ACR_DCEN 
		            |FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/
		
		// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // 读取时钟切换状态位，确保PLLCLK被选为系统时钟
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { // HSI启动出错处理
    while (1)
    {
    }
  }
}



void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}





//利用系统滴答定时，编写的延时函数

static u16  fac_us=0; //us延时倍乘数			   
static u16 fac_ms=0; //ms延时倍乘数,在ucos下,代表每个节拍的ms数





void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;	 
	fac_ms=(u32)fac_us*1000; //每个ms需要的systick时钟数   
}


//延时1us，实测1us
void  delay_us(u32 nus)    
{
	   u32 temp;
	   SysTick->LOAD=fac_us*nus;		   					//设置重装数值
	   SysTick->CTRL=0X01;		   					//使能，减到零是无动作，采用外部时钟源
	   SysTick->VAL=0;			   					//清零计数器
	   do
	   {
		   temp=SysTick->CTRL;					 	//读取当前倒计数值
	   }	
	   while((temp&0x01)&&(!(temp&(1<<16))));	  	//等待时间到达
	   SysTick->CTRL=0;    							//关闭计数器
	   SysTick->VAL=0;		  						//清空计数器

}



//延时1ms，实测1ms  (最大延时798ms)
void delay_ms(u32 nms)
{
     u32 temp;                                               
     SysTick->LOAD=(u32)fac_ms*nms;      			//设置重装数值
     SysTick->CTRL=0X01;        					//使能，减到零是无动作，采用外部时钟源
     SysTick->VAL=0;            					//清零计数器
     do
     {
         temp=SysTick->CTRL;      					 //读取当前倒计数值
     }
     while((temp&0x01)&&(!(temp&(1<<16))));    		 //等待时间到达
     SysTick->CTRL=0;    							 //关闭计数器	
     SysTick->VAL=0;       							 //清空计数器
}

void delay_s(u32 ns)
{
	u32 temp;
	for (temp = 0; temp < ns*2; ++temp)
	{
		delay_ms(500);
	}
}



			 
