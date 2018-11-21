#ifndef __GPIO_H
#define __GPIO_H 			   
#include "stm32f4xx.h" 

//ledgpio�궨��
/*------------------------------------------------------------------------*/
#define  LED_PIN          				GPIO_Pin_3
#define  LED_GPIO_PORT    				GPIOA
#define  LED_GPIO_CLK     				RCC_AHB1Periph_GPIOA
/*------------------------------------------------------------------------*/


//������GPIO����
/*------------------------------------------------------------------------*/
#define  DRIVER_GPIO_PORT					GPIOB			
#define  DRIVER_GPIO_CLK					RCC_AHB1Periph_GPIOB

/*���ȿ�����GPIO*/
#define  HEAT_CONTROL_PIN           		GPIO_Pin_7

/*��ת��GPIO*/
#define  SPINNER_RACK_CONTROL_PIN  		 	GPIO_Pin_6

/*ѭ�����GPIO*/
#define  CIRCULATING_FUN_CONTROL_PIN        GPIO_Pin_5

/*���ⱨ����GPIO*/
#define  ALARM_CONTROL1_PIN        			GPIO_Pin_4
#define  ALARM_CONTROL2_PIN        			GPIO_Pin_3


/*����GPIO*/
#define  BACK_GPIO_PORT						GPIOE			
#define  BACK_GPIO_CLK					 	RCC_AHB1Periph_GPIOE

#define  BACK_GPIO_PIN1						GPIO_Pin_1
#define  BACK_GPIO_PIN2						GPIO_Pin_0
#define  BACK_GPIO_PIN3						GPIO_Pin_2
#define  BACK_GPIO_PIN4						GPIO_Pin_3
#define  BACK_GPIO_PIN5						GPIO_Pin_4
/*------------------------------------------------------------------------*/



//BOX DOOR
#define  BOX_DOOR_PIN          				GPIO_Pin_10
#define  BOX_DOOR_GPIO_PORT    				GPIOC
#define  BOX_DOOR_GPIO_CLK     				RCC_AHB1Periph_GPIOC


/*-----------------------------------------------------------------------*/

#define   ON     1
#define   OFF    0

//LED���ƺ꺯��
/*-----------------------------------------------------------------------*/
#define   LED(a)   if (a)\
					GPIO_SetBits(LED_GPIO_PORT,LED_PIN);\
					else\
           			GPIO_ResetBits(LED_GPIO_PORT,LED_PIN)	
/*-----------------------------------------------------------------------*/

/* ���ƼĴ���ֱ�ӿ���gpio */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//�͵�ƽ
#define digitalLo(p,i)			 {p->BSRRH=i;}		//�ߵ�ƽ
#define digitalToggle(p,i)	 	 {p->ODR ^=i;}		//��ת

/* LED���ƺ궨�� */
#define LED_TOGGLE			digitalToggle(LED_GPIO_PORT,LED_PIN)
#define LED_OFF				digitalHi(LED_GPIO_PORT,LED_PIN)
#define LED_ON				digitalLo(LED_GPIO_PORT,LED_PIN)


/* �������ش�����ƺ궨�� */
/*-----------------------------------------------------------------------*/
#define HEAT_ON					digitalHi(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN)		//������
#define HEAT_OFF				digitalLo(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN)

#define SPINNER_RACK_ON			digitalHi(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN)//��ת��
#define SPINNER_RACK_OFF		digitalLo(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN)

#define CIRCULATING_FUN_ON		digitalHi(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN)//���
#define CIRCULATING_FUN_OFF		digitalLo(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN)

#define ALARM1_ON				digitalHi(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN)	//������1
#define ALARM1_OFF				digitalLo(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN)


#define ALARM2_ON				digitalHi(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN)	//������2
#define ALARM2_OFF				digitalLo(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN)

/*-----------------------------------------------------------------------*/

void led_init(void);
void driver_gpio_init(void);
void door_gpio_init(void);
void Gpio_Init(void);


void led_blink(uint8_t num);



void test(void);







#endif





























