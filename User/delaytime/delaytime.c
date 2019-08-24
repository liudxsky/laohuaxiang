#include "./delaytime/delaytime.h"
#include "stm32f4xx_rcc.h"

/*
 * ʹ��HSEʱ������ϵͳʱ�ӵĲ���
 * 1������HSE �����ȴ� HSE �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ
 *    ����VCO����ʱ�� ��Ƶ����        m
 *    ����VCO���ʱ�� ��Ƶ����        n
 *    ����PLLCLKʱ�ӷ�Ƶ����          p
 *    ����OTG FS,SDIO,RNGʱ�ӷ�Ƶ���� q
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 */

/*
 * m: VCO����ʱ�� ��Ƶ���ӣ�ȡֵ2~63
 * n: VCO���ʱ�� ��Ƶ���ӣ�ȡֵ192~432
 * p: PLLCLKʱ�ӷ�Ƶ����  ��ȡֵ2��4��6��8
 * q: OTG FS,SDIO,RNGʱ�ӷ�Ƶ���ӣ�ȡֵ4~15
 * �������þ�����ʹ��HSE����ʱ��
 * SYSCLK=HCLK=168M,PCLK2=HCLK/2=84M,PCLK1=HCLK/4=42M
 * HSE_SetSysClock(25, 336, 2, 7);
 * HSE��Ϊʱ����Դ������PLL��Ƶ��Ϊϵͳʱ�ӣ�����ͨ��������
 
 * ϵͳʱ�ӳ�Ƶ��216Mˬһ��
 * HSE_SetSysClock(25, 432, 2, 9);
 */
void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)	
{
  __IO uint32_t HSEStartUpStatus = 0;
  
  // ʹ��HSE�������ⲿ���񣬱���F407ʹ�� HSE=25M
  RCC_HSEConfig(RCC_HSE_ON);
	
  // �ȴ�HSE�����ȶ�
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    // ��ѹ����ѹ�����������Ϊ1���Ա�������Ϊ���Ƶ��
		// ����ʱʹ���ܺ͹���ʵ��ƽ��
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		
    // ���Ҫ��Ƶ�͵�������������
		// ����PLL��Դʱ�ӣ�����VCO��Ƶ����m������VCO��Ƶ����n��
		//  ����ϵͳʱ�ӷ�Ƶ����p������OTG FS,SDIO,RNG��Ƶ����q
		RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
		
		// ʹ��PLL
		RCC_PLLCmd(ENABLE);
  
	  // �ȴ� PLL�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }   

/*-----------------------------------------------------*/
    //���� OVER-RIDEģʽ�����ܴﵽ����Ƶ��(F429���иù���)
//    PWR->CR |= PWR_CR_ODEN;
//    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
//    {
//    }
//    PWR->CR |= PWR_CR_ODSWEN;
//    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
//    {
//    }      
    // ����FLASHԤȡָ,ָ���,���ݻ���͵ȴ�״̬
    FLASH->ACR = FLASH_ACR_PRFTEN 
		            | FLASH_ACR_ICEN 
		            | FLASH_ACR_DCEN 
		            | FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/
		
		// ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { // HSE����������

    while (1)
    {
    }
  }
}

/*
 * ʹ��HSIʱ������ϵͳʱ�ӵĲ���
 * 1������HSI �����ȴ� HSI �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ
 *    ����VCO����ʱ�� ��Ƶ����        m
 *    ����VCO���ʱ�� ��Ƶ����        n
 *    ����SYSCLKʱ�ӷ�Ƶ����          p
 *    ����OTG FS,SDIO,RNGʱ�ӷ�Ƶ���� q
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 */

/*
 * m: VCO����ʱ�� ��Ƶ���ӣ�ȡֵ2~63
 * n: VCO���ʱ�� ��Ƶ���ӣ�ȡֵ192~432
 * p: PLLCLKʱ�ӷ�Ƶ����  ��ȡֵ2��4��6��8
 * q: OTG FS,SDIO,RNGʱ�ӷ�Ƶ���ӣ�ȡֵ4~15
 * �������þ�����ʹ��HSI����ʱ��
 * SYSCLK=HCLK=168M,PCLK2=HCLK/2=84M,PCLK1=HCLK/4=42M
 * HSI_SetSysClock(16, 336, 2, 7);
 * HSE��Ϊʱ����Դ������PLL��Ƶ��Ϊϵͳʱ�ӣ�����ͨ��������
 
 * ϵͳʱ�ӳ�Ƶ��216Mˬһ��
 * HSI_SetSysClock(16, 432, 2, 9);
 */

void HSI_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)	
{
  __IO uint32_t HSIStartUpStatus = 0;
	
	// ��RCC�����ʼ���ɸ�λ״̬
  RCC_DeInit();
  
  //ʹ��HSI, HSI=16M
	RCC_HSICmd(ENABLE);
	
  // �ȴ� HSI ����
	HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;
	
	// ֻ�� HSI����֮�����������ִ��
  if (HSIStartUpStatus == RCC_CR_HSIRDY)
  {
    // ��ѹ����ѹ�����������Ϊ1���Ա�������Ϊ���Ƶ��
		// ����ʱʹ���ܺ͹���ʵ��ƽ��
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		
    // ���Ҫ��Ƶ�͵�������������
		// ����PLL��Դʱ�ӣ�����VCO��Ƶ����m������VCO��Ƶ����n��
		//  ����ϵͳʱ�ӷ�Ƶ����p������OTG FS,SDIO,RNG��Ƶ����q
		RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);
		
		// ʹ��PLL
		RCC_PLLCmd(ENABLE);
  
	  // �ȴ� PLL�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }   

/*-----------------------------------------------------*/
    //���� OVER-RIDEģʽ�����ܴﵽ����Ƶ��(F429���иù���)
//    PWR->CR |= PWR_CR_ODEN;
//    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
//    {
//    }
//    PWR->CR |= PWR_CR_ODSWEN;
//    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
//    {
//    }      
    // ����FLASHԤȡָ,ָ���,���ݻ���͵ȴ�״̬
    FLASH->ACR = FLASH_ACR_PRFTEN 
		            | FLASH_ACR_ICEN 
		            |FLASH_ACR_DCEN 
		            |FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/
		
		// ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { // HSI����������
    while (1)
    {
    }
  }
}



void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}





//����ϵͳ�δ�ʱ����д����ʱ����

static u16  fac_us=0; //us��ʱ������			   
static u16 fac_ms=0; //ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��





void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;	 
	fac_ms=(u32)fac_us*1000; //ÿ��ms��Ҫ��systickʱ����   
}


//��ʱ1us��ʵ��1us
void  delay_us(u32 nus)    
{
	   u32 temp;
	   SysTick->LOAD=fac_us*nus;		   					//������װ��ֵ
	   SysTick->CTRL=0X01;		   					//ʹ�ܣ����������޶����������ⲿʱ��Դ
	   SysTick->VAL=0;			   					//���������
	   do
	   {
		   temp=SysTick->CTRL;					 	//��ȡ��ǰ������ֵ
	   }	
	   while((temp&0x01)&&(!(temp&(1<<16))));	  	//�ȴ�ʱ�䵽��
	   SysTick->CTRL=0;    							//�رռ�����
	   SysTick->VAL=0;		  						//��ռ�����

}



//��ʱ1ms��ʵ��1ms  (�����ʱ798ms)
void delay_ms(u32 nms)
{
     u32 temp;                                               
     SysTick->LOAD=(u32)fac_ms*nms;      			//������װ��ֵ
     SysTick->CTRL=0X01;        					//ʹ�ܣ����������޶����������ⲿʱ��Դ
     SysTick->VAL=0;            					//���������
     do
     {
         temp=SysTick->CTRL;      					 //��ȡ��ǰ������ֵ
     }
     while((temp&0x01)&&(!(temp&(1<<16))));    		 //�ȴ�ʱ�䵽��
     SysTick->CTRL=0;    							 //�رռ�����	
     SysTick->VAL=0;       							 //��ռ�����
}

void delay_s(u32 ns)
{
	u32 temp;
	for (temp = 0; temp < ns*2; ++temp)
	{
		delay_ms(500);
	}
}



			 
