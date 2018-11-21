#ifndef _USER_UART__
#define _USER_UART__

#include "stm32f4xx.h"     



//引脚定义
/***************************USART2*********************************/

#define SCREEN_USART                             USART2	
#define SCREEN_USART_CLK                         RCC_APB1Periph_USART2
#define SCREEN_USART_BAUDRATE                    115200  //波特率


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
* 名    称： UartInit()
* 功    能： 串口初始化
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void UartInit(void);

/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
 *****************************************************************/
void  SendChar(uint8_t t);




#endif
