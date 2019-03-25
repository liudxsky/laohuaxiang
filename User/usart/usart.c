#include "./usart/usart.h"
#include "stdio.h"
#include "./delaytime/delaytime.h"






//串口gpio和中断配置
void USART1_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  RCC_AHB1PeriphClockCmd(RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK,ENABLE);

  /* 使能USART时钟 */
  RCC_APB2PeriphClockCmd(RS232_USART_CLK, ENABLE);
                                         
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN;  
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE,RS232_USART_RX_AF);

  /* 连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);
  
 /* 配置串口模式 :USART_MODE*/
  /*波特率设置： USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  /* 字长(数据位+校验位) ：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 1个停止位*/
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 不使用校验位 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 不使用 硬件流*/
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* 同时能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 使用上面的结构体完成初始化配置 */
  USART_Init(RS232_USART, &USART_InitStructure); 
	
  /*  嵌套向量控制器组选择*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* 配置USART	为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
	/* 抢断优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 子优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);

  
	/* 使能串口接收中断*/
  USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(RS232_USART, ENABLE);
}

#if 1
/***************** 发送一个字符**********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字符数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空*/
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/***************** 发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***************** 发送一个16位数**********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位*/
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


//重定向C库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口*/
		USART_SendData(RS232_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//重定向C库函数scanf到串口，重定向后可使用scanf，getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据*/
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(RS232_USART);
}

uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(RS232_USART, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(RS232_USART));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\r Please enter valid number between 0 and 9 \n\r");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\r Please enter valid number between 0 and %d \n\r", value);
    return 0xFF;
  }
  return index;
}


#endif


void RS485_USART_Config(u32 RS485_USART_BAUDRATE)
{

	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RS485_USART_RX_GPIO_CLK,ENABLE); 			//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RS485_USART_CLK,ENABLE);					//使能USART时钟
	
  	//串口2引脚复用映射
	GPIO_PinAFConfig(RS485_USART_TX_GPIO_PORT,RS485_USART_TX_SOURCE,RS485_USART_TX_AF);
	GPIO_PinAFConfig(RS485_USART_RX_GPIO_PORT,RS485_USART_RX_SOURCE,RS485_USART_RX_AF); 
	
	//USART   TX
    GPIO_InitStructure.GPIO_Pin = RS485_USART_TX_PIN | RS485_USART_RX_PIN; 		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//上拉
	GPIO_Init(RS485_USART_RX_GPIO_PORT,&GPIO_InitStructure); 			//初始化PA2，PA3
	
	//推挽输出，485模式控制  
  	GPIO_InitStructure.GPIO_Pin = RS485_RE_PIN; 						
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//上拉
	GPIO_Init(RS485_RE_GPIO_PORT,&GPIO_InitStructure); 					
			
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = RS485_USART_BAUDRATE;			//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式
  	USART_Init(RS485_USART, &USART_InitStructure); 					//初始化串口
	
  	USART_Cmd(RS485_USART, ENABLE);  									//使能串口
	
	USART_ClearFlag(RS485_USART, USART_FLAG_TC);
	
	USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);				//开启接受中断

	//Usart NVIC 配置
	 /*  嵌套向量控制器组选择*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  	NVIC_InitStructure.NVIC_IRQChannel = RS485_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	RS485_RX_EN();				//初始化默认为接收模式	

}




/***************** 发送一个字符  **********************/ 
//使用单字节数据发送前要使能发送引脚，发送后要使能接收引脚。 
 void RS485_SendByte(  uint8_t ch ) 
 { 
//	RS485_TX_EN();//	使能发送数据
 	/* 发送一个字节数据到 USART */ 
     USART_SendData(RS485_USART,ch); 
     /* 等待发送完毕 */ 
     while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXE) == RESET); 
	 /*加短暂延时，保证485发送数据完毕*/
	delay_ms(1);
//	 RS485_RX_EN()	;//	使能接收数据
}

/*****************  发送指定长度的字符串 **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;

	RS485_TX_EN();//	使能发送数据	
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
		
	/*加短暂延时，保证485发送数据完毕*/
	delay_ms(1);
		
	RS485_RX_EN()	;//	使能接收数据
}


/*****************  发送字符串 **********************/
void RS485_SendString(  uint8_t *str)
{
	unsigned int k=0;
	
	RS485_TX_EN()	;//	使能发送数据
	
    do 
    {
        RS485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
	/*加短暂延时，保证485发送数据完毕*/
	delay_ms(1);
		
	RS485_RX_EN()	;//	使能接收数据
}





/*********************************************END OF FILE**********************/









