
/*--------------------------------------------------------------------------------------
                                  ʹ�ñض�
   hmi_user_uart.c�еĴ��ڷ��ͽ��պ�����3�����������ڳ�ʼ��Uartinti()������1���ֽ�SendChar()��
   �����ַ���SendStrings().����ֲ������ƽ̨����Ҫ�޸ĵײ��
   ��������,����ֹ�޸ĺ������ƣ������޷���HMI������(hmi_driver.c)ƥ�䡣
--------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------
                          1. ����STM32ƽ̨��������
----------------------------------------------------------------------------------------*/
#include "hmi_user_uart.h"
#include "stdlib.h"  
#include "./gpio/gpio.h" 


/* Ƕ�������жϿ����� NVIC ���� */ 

 static void NVIC_Configuration(void)
 {
		 NVIC_InitTypeDef NVIC_InitStructure;

		 /* Ƕ�������жϿ�������ѡ�� */

		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		 /* ���� USART Ϊ�ж�Դ */
		 NVIC_InitStructure.NVIC_IRQChannel = SCREEN_USART_IRQ;
		 /* �������ȼ�Ϊ 1 */
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		 /* �����ȼ�Ϊ 1 */
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		 /* ʹ���ж� */
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		 /* ��ʼ������ NVIC */
		 NVIC_Init(&NVIC_InitStructure);
 }


/****************************************************************************
* ��    �ƣ� UartInit()
* ��    �ܣ� ���ڳ�ʼ��
* ��ڲ����� ��
* ���ڲ����� ��
****************************************************************************/


void UartInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( SCREEN_USART_RX_GPIO_CLK|SCREEN_USART_TX_GPIO_CLK, ENABLE);

  /* ʹ�� USART ʱ�� */
  RCC_APB1PeriphClockCmd(SCREEN_USART_CLK, ENABLE);

   /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(SCREEN_USART_RX_GPIO_PORT,SCREEN_USART_RX_SOURCE,SCREEN_USART_RX_AF);
   
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(SCREEN_USART_TX_GPIO_PORT,SCREEN_USART_TX_SOURCE, SCREEN_USART_TX_AF);


  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(SCREEN_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_RX_PIN;
  GPIO_Init(SCREEN_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô�DEBUG_USART ģʽ */
  USART_InitStructure.USART_BaudRate = SCREEN_USART_BAUDRATE; //����������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�������λ+У��λ����8
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No ;//��ʹ��У��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ����
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ͬʱʹ�ܽ��պͷ���
  USART_Init(SCREEN_USART, &USART_InitStructure); //���USart��ʼ������

	/* Ƕ�������жϿ����� NVIC ���� */ 
	NVIC_Configuration();

	/* ʹ�ܴ��ڽ����ж� */
  USART_ITConfig(SCREEN_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(SCREEN_USART, ENABLE);
}



/*****************************************************************
* ��    �ƣ� SendChar()
* ��    �ܣ� ����1���ֽ�
* ��ڲ����� t  ���͵��ֽ�
* ���ڲ����� ��
 *****************************************************************/
void  SendChar(uint8_t t)
{
    USART_SendData(SCREEN_USART,t);
    while(USART_GetFlagStatus(SCREEN_USART, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(SCREEN_USART, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
}


