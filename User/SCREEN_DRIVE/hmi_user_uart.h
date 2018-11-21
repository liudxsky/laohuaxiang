#ifndef _USER_UART__
#define _USER_UART__

#include "stm32f4xx.h"     



//���Ŷ���
/***************************USART2*********************************/

#define SCREEN_USART                             USART2	
#define SCREEN_USART_CLK                         RCC_APB1Periph_USART2
#define SCREEN_USART_BAUDRATE                    115200  //������


#define SCREEN_USART_RX_GPIO_PORT                GPIOD
#define SCREEN_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define SCREEN_USART_RX_PIN                      GPIO_Pin_6
#define SCREEN_USART_RX_AF                       GPIO_AF_USART2
#define SCREEN_USART_RX_SOURCE                   GPIO_PinSource6

#define SCREEN_USART_TX_GPIO_PORT                GPIOD
#define SCREEN_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define SCREEN_USART_TX_PIN                      GPIO_Pin_5
#define SCREEN_USART_TX_AF                       GPIO_AF_USART2
#define SCREEN_USART_TX_SOURCE                   GPIO_PinSource5

#define SCREEN_USART_IRQHandler 				   USART2_IRQHandler
#define SCREEN_USART_IRQ 						   USART2_IRQn

/*************************************************************************************************************/

/****************************************************************************
* ��    �ƣ� UartInit()
* ��    �ܣ� ���ڳ�ʼ��
* ��ڲ����� ��
* ���ڲ����� ��
****************************************************************************/
void UartInit(void);

/*****************************************************************
* ��    �ƣ� SendChar()
* ��    �ܣ� ����1���ֽ�
* ��ڲ����� t  ���͵��ֽ�
* ���ڲ����� ��
 *****************************************************************/
void  SendChar(uint8_t t);




#endif
