#ifndef  __USART_H
#define  __USART_H
#include "stm32f4xx.h" 


#define RxBufferSize				20
#define UART_CMD_NUM				5

#define SHOWVALUE                   0
#define SET_PWMSCOPE				1
#define SET_PWMVLAUE				2
#define SET_ANGLE					3
#define HELP						4
#define RUN 						5
#define DEBUG 						6
#define SETPOINT 					7
#define SIMTEMP 					8
typedef struct
{
	char* cmd;
	uint8_t len;
}uart_cmd_t;



/************************usart1_rs232*******************************/
#define RS232_USART                             USART1   //usart1
#define RS232_USART_CLK                         RCC_APB2Periph_USART1
#define RS232_USART_BAUDRATE                    115200  //���Դ��ڲ�����

#define RS232_USART_RX_GPIO_PORT                GPIOA
#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RS232_USART_RX_PIN                      GPIO_Pin_10
#define RS232_USART_RX_AF                       GPIO_AF_USART1
#define RS232_USART_RX_SOURCE                   GPIO_PinSource10

#define RS232_USART_TX_GPIO_PORT                GPIOA
#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RS232_USART_TX_PIN                      GPIO_Pin_9
#define RS232_USART_TX_AF                       GPIO_AF_USART1
#define RS232_USART_TX_SOURCE                   GPIO_PinSource9

#define RS232_USART_IRQHandler                  USART1_IRQHandler
#define RS232_USART_IRQ                 		 USART1_IRQn
/***************************usart3_rs485*********************************/

#define RS485_USART                             USART3	
#define RS485_USART_CLK                         RCC_APB1Periph_USART3
//#define RS485_USART3_BAUDRATE                    115200  //������


#define RS485_USART_RX_GPIO_PORT                GPIOD
#define RS485_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define RS485_USART_RX_PIN                      GPIO_Pin_9
#define RS485_USART_RX_AF                       GPIO_AF_USART3
#define RS485_USART_RX_SOURCE                   GPIO_PinSource9

#define RS485_USART_TX_GPIO_PORT                GPIOD
#define RS485_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define RS485_USART_TX_PIN                      GPIO_Pin_8
#define RS485_USART_TX_AF                       GPIO_AF_USART3
#define RS485_USART_TX_SOURCE                   GPIO_PinSource8

//RS485�շ���������
#define RS485_RE_GPIO_PORT                      GPIOC 
#define RS485_RE_GPIO_CLK                       RCC_AHB1Periph_GPIOC 
#define RS485_RE_PIN                            GPIO_Pin_9


#define RS485_USART_IRQHandler 				 	USART3_IRQHandler
#define RS485_USART_IRQ 						 USART3_IRQn

/*************************************************************************************************************/


 /*�����շ�����*/ 
//�������ģʽ,����Ҫ����ʱ�ȴ� 485 ���������� 
#define RS485_RX_EN()  GPIO_ResetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);
//���뷢��ģʽ,����Ҫ����ʱ�ȴ� 485 ���������� 
#define RS485_TX_EN()  GPIO_SetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN); 

/*************************************************************************************************************/













void NVIC_Configuration(void);


void USART1_Config(void);



void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
uint8_t USART_Scanf(uint32_t value);




void RS485_USART_Config(u32 RS485_USART_BAUDRATE);



void RS485_SendByte(  uint8_t ch );

void RS485_SendStr_length( uint8_t *str,uint32_t strlen );

void RS485_SendString(  uint8_t *str);







#endif
