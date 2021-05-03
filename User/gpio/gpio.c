#include "./gpio/gpio.h"
#include "./delaytime/delaytime.h"
#include "stm32f4xx.h" 
#include "./screen/screenStatus.h"
#include "./flash/deviceinfo.h"
#include "./dac/dac.h"
int timer100;
int blinkcnt=0;
int ledstatus=0;
extern dev_info_t dev_info;
extern 
//LED
void digitalHi(GPIO_TypeDef * p,int i)
{
	p->BSRRL=i;
}
void digitalLo(GPIO_TypeDef * p,int i)
{
	p->BSRRH=i;
}
void digitalToggle(GPIO_TypeDef * p,int i)
{
	p->ODR ^=i;
}
void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LED_GPIO_CLK, ENABLE);

	  
	GPIO_InitStructure.GPIO_Pin = LED_PIN ;      		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;               
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;           
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 	
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_ResetBits(LED_GPIO_PORT,LED_PIN);				
	timer100=0;
}


void driver_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				 
	RCC_AHB1PeriphClockCmd(DRIVER_GPIO_CLK, ENABLE);	 
	RCC_AHB1PeriphClockCmd(BACK_GPIO_CLK, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = HEAT_CONTROL_PIN|SPINNER_RACK_CONTROL_PIN|
								  CIRCULATING_FUN_CONTROL_PIN|ALARM_CONTROL1_PIN|
								  ALARM_CONTROL2_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     
	GPIO_Init(DRIVER_GPIO_PORT, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = BACK_GPIO_PIN1|
								  BACK_GPIO_PIN2|BACK_GPIO_PIN3|
								  BACK_GPIO_PIN4|BACK_GPIO_PIN5;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        
	GPIO_Init(BACK_GPIO_PORT, &GPIO_InitStructure); 
}



//box_door gpio init
void door_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				
	RCC_AHB1PeriphClockCmd(BOX_DOOR_GPIO_CLK, ENABLE);	 
	
	GPIO_InitStructure.GPIO_Pin = BOX_DOOR_PIN;      	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     
	GPIO_Init(BOX_DOOR_GPIO_PORT, &GPIO_InitStructure);   
}
void Thermo_safe_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				
	RCC_AHB1PeriphClockCmd(THERMO_SAFE_CLK, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = THERMO_SAFE_PIN;      	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     
	GPIO_Init(THERMO_SAFE_BANK, &GPIO_InitStructure);  
}


void Gpio_Init(void)
{
	led_init();
	driver_gpio_init();
	door_gpio_init();
	Thermo_safe_init();
}
void ledBlinkThread()
{
	if(blinkcnt>0)
	{
		//blinkcnt--;
		if(ledstatus==0)
	{
		LED_ON;
		ledstatus=1;
	}
	else
	{
		LED_OFF;
		ledstatus=0;
	}
	
}
}
void test(void)
{
	GPIO_SetBits(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	delay_ms(500);
	GPIO_ResetBits(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	delay_ms(500);
	
	GPIO_SetBits(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	delay_ms(500);
	GPIO_ResetBits(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	delay_ms(500);
	
	GPIO_SetBits(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	delay_ms(500);
	GPIO_ResetBits(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	delay_ms(500);
	
	GPIO_SetBits(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	delay_ms(500);
	GPIO_ResetBits(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	delay_ms(500);
	
	GPIO_SetBits(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	delay_ms(500);
	GPIO_ResetBits(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	delay_ms(500);
}

void led_blink(uint8_t num)
{
	blinkcnt=num;
}


/*********************************************END OF FILE**********************/

			 
