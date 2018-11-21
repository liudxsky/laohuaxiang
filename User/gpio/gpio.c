#include "./gpio/gpio.h"
#include "./delaytime/delaytime.h"


//LED灯初始化配置
void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义结构体

	RCC_AHB1PeriphClockCmd(LED_GPIO_CLK, ENABLE);//开启外设时钟

	  
	GPIO_InitStructure.GPIO_Pin = LED_PIN ;      				 //LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;               //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;           //Gpio输出速率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 //设置引脚为上拉模式		
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_ResetBits(LED_GPIO_PORT,LED_PIN);						 //熄灭LED
}

//驱动开关触点和反馈引脚初始化配置
void driver_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				 //定义结构体
	RCC_AHB1PeriphClockCmd(DRIVER_GPIO_CLK, ENABLE);	 //开启外设时钟
	RCC_AHB1PeriphClockCmd(BACK_GPIO_CLK, ENABLE);	 //开启反馈引脚时钟
	GPIO_InitStructure.GPIO_Pin = HEAT_CONTROL_PIN|SPINNER_RACK_CONTROL_PIN|
								  CIRCULATING_FUN_CONTROL_PIN|ALARM_CONTROL1_PIN|
								  ALARM_CONTROL2_PIN;     //五个驱动IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //Gpio输出速率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //设置引脚为不上拉也不下拉模式
	GPIO_Init(DRIVER_GPIO_PORT, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = BACK_GPIO_PIN1|
								  BACK_GPIO_PIN2|BACK_GPIO_PIN3|
								  BACK_GPIO_PIN4|BACK_GPIO_PIN5;  //五个反馈口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         //输入模式
	GPIO_Init(BACK_GPIO_PORT, &GPIO_InitStructure); 
}



//箱门开关初始化配置
void door_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				 //定义结构体
	RCC_AHB1PeriphClockCmd(BOX_DOOR_GPIO_CLK, ENABLE);	 //开启外设时钟
	
	GPIO_InitStructure.GPIO_Pin = BOX_DOOR_PIN;      	 //box_door
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         //输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //Gpio输出速率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //设置引脚为不上拉也不下拉模式
	GPIO_Init(BOX_DOOR_GPIO_PORT, &GPIO_InitStructure);   
}



//整体gpio初始化配置
void Gpio_Init(void)
{
	led_init();
	driver_gpio_init();
	door_gpio_init();
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
	uint8_t i;
	for(i=0;i<num;i++)
	{
		LED_ON;
		delay_ms(200);
		LED_OFF;
		delay_ms(200);
	}
}


/*********************************************END OF FILE**********************/

			 
