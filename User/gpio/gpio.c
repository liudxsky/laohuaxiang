#include "./gpio/gpio.h"
#include "./delaytime/delaytime.h"


//LED�Ƴ�ʼ������
void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //����ṹ��

	RCC_AHB1PeriphClockCmd(LED_GPIO_CLK, ENABLE);//��������ʱ��

	  
	GPIO_InitStructure.GPIO_Pin = LED_PIN ;      				 //LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;               //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;           //Gpio�������Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 //��������Ϊ����ģʽ		
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_ResetBits(LED_GPIO_PORT,LED_PIN);						 //Ϩ��LED
}

//�������ش���ͷ������ų�ʼ������
void driver_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				 //����ṹ��
	RCC_AHB1PeriphClockCmd(DRIVER_GPIO_CLK, ENABLE);	 //��������ʱ��
	RCC_AHB1PeriphClockCmd(BACK_GPIO_CLK, ENABLE);	 //������������ʱ��
	GPIO_InitStructure.GPIO_Pin = HEAT_CONTROL_PIN|SPINNER_RACK_CONTROL_PIN|
								  CIRCULATING_FUN_CONTROL_PIN|ALARM_CONTROL1_PIN|
								  ALARM_CONTROL2_PIN;     //�������IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //Gpio�������Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //��������Ϊ������Ҳ������ģʽ
	GPIO_Init(DRIVER_GPIO_PORT, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = BACK_GPIO_PIN1|
								  BACK_GPIO_PIN2|BACK_GPIO_PIN3|
								  BACK_GPIO_PIN4|BACK_GPIO_PIN5;  //���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         //����ģʽ
	GPIO_Init(BACK_GPIO_PORT, &GPIO_InitStructure); 
}



//���ſ��س�ʼ������
void door_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 				 //����ṹ��
	RCC_AHB1PeriphClockCmd(BOX_DOOR_GPIO_CLK, ENABLE);	 //��������ʱ��
	
	GPIO_InitStructure.GPIO_Pin = BOX_DOOR_PIN;      	 //box_door
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //Gpio�������Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //��������Ϊ������Ҳ������ģʽ
	GPIO_Init(BOX_DOOR_GPIO_PORT, &GPIO_InitStructure);   
}



//����gpio��ʼ������
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

			 
