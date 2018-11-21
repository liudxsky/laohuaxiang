
/*--------------------------------------------------------------------------------------
                                  使用必读
   hmi_user_uart.c中的串口发送接收函数共3个函数：串口初始化Uartinti()、发送1个字节SendChar()、
   发送字符串SendStrings().若移植到其他平台，需要修改底层寄
   存器设置,但禁止修改函数名称，否则无法与HMI驱动库(hmi_driver.c)匹配。
--------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------
                          1. 基于STM32平台串口驱动
----------------------------------------------------------------------------------------*/
#include "hmi_user_uart.h"
#include "stdlib.h"  
#include "./gpio/gpio.h" 


/* 嵌套向量中断控制器 NVIC 配置 */ 

 static void NVIC_Configuration(void)
 {
		 NVIC_InitTypeDef NVIC_InitStructure;

		 /* 嵌套向量中断控制器组选择 */

		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		 /* 配置 USART 为中断源 */
		 NVIC_InitStructure.NVIC_IRQChannel = SCREEN_USART_IRQ;
		 /* 抢断优先级为 1 */
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		 /* 子优先级为 1 */
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		 /* 使能中断 */
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		 /* 初始化配置 NVIC */
		 NVIC_Init(&NVIC_InitStructure);
 }


/****************************************************************************
* 名    称： UartInit()
* 功    能： 串口初始化
* 入口参数： 无
* 出口参数： 无
****************************************************************************/


void UartInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( SCREEN_USART_RX_GPIO_CLK|SCREEN_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(SCREEN_USART_CLK, ENABLE);

   /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(SCREEN_USART_RX_GPIO_PORT,SCREEN_USART_RX_SOURCE,SCREEN_USART_RX_AF);
   
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(SCREEN_USART_TX_GPIO_PORT,SCREEN_USART_TX_SOURCE, SCREEN_USART_TX_AF);


  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(SCREEN_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_RX_PIN;
  GPIO_Init(SCREEN_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串DEBUG_USART 模式 */
  USART_InitStructure.USART_BaudRate = SCREEN_USART_BAUDRATE; //波特率设置
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长（数据位+校验位）：8
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No ;//不使用校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//同时使能接收和发送
  USART_Init(SCREEN_USART, &USART_InitStructure); //完成USart初始化配置

	/* 嵌套向量中断控制器 NVIC 配置 */ 
	NVIC_Configuration();

	/* 使能串口接收中断 */
  USART_ITConfig(SCREEN_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(SCREEN_USART, ENABLE);
}



/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
 *****************************************************************/
void  SendChar(uint8_t t)
{
    USART_SendData(SCREEN_USART,t);
    while(USART_GetFlagStatus(SCREEN_USART, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(SCREEN_USART, USART_FLAG_TC) == RESET));//等待串口发送完毕
}


