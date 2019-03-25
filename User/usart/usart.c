#include "./usart/usart.h"
#include "stdio.h"
#include "./delaytime/delaytime.h"






//����gpio���ж�����
void USART1_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  RCC_AHB1PeriphClockCmd(RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK,ENABLE);

  /* ʹ��USARTʱ�� */
  RCC_APB2PeriphClockCmd(RS232_USART_CLK, ENABLE);
                                         
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN;  
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ����PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE,RS232_USART_RX_AF);

  /* ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);
  
 /* ���ô���ģʽ :USART_MODE*/
  /*���������ã� USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  /* �ֳ�(����λ+У��λ) ��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 1��ֹͣλ*/
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* ��ʹ��У��λ */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* ��ʹ�� Ӳ����*/
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* ͬʱ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ʹ������Ľṹ����ɳ�ʼ������ */
  USART_Init(RS232_USART, &USART_InitStructure); 
	
  /*  Ƕ��������������ѡ��*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* ����USART	Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
	/* �������ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);

  
	/* ʹ�ܴ��ڽ����ж�*/
  USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	
  /* ʹ�ܴ��� */
  USART_Cmd(RS232_USART, ENABLE);
}

#if 1
/***************** ����һ���ַ�**********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ַ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ��*/
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/***************** �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***************** ����һ��16λ��**********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ*/
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


//�ض���C�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����*/
		USART_SendData(RS232_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//�ض���C�⺯��scanf�����ڣ��ض�����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ�������������*/
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
	
	RCC_AHB1PeriphClockCmd(RS485_USART_RX_GPIO_CLK,ENABLE); 			//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RS485_USART_CLK,ENABLE);					//ʹ��USARTʱ��
	
  	//����2���Ÿ���ӳ��
	GPIO_PinAFConfig(RS485_USART_TX_GPIO_PORT,RS485_USART_TX_SOURCE,RS485_USART_TX_AF);
	GPIO_PinAFConfig(RS485_USART_RX_GPIO_PORT,RS485_USART_RX_SOURCE,RS485_USART_RX_AF); 
	
	//USART   TX
    GPIO_InitStructure.GPIO_Pin = RS485_USART_TX_PIN | RS485_USART_RX_PIN; 		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//����
	GPIO_Init(RS485_USART_RX_GPIO_PORT,&GPIO_InitStructure); 			//��ʼ��PA2��PA3
	
	//���������485ģʽ����  
  	GPIO_InitStructure.GPIO_Pin = RS485_RE_PIN; 						
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//����
	GPIO_Init(RS485_RE_GPIO_PORT,&GPIO_InitStructure); 					
			
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = RS485_USART_BAUDRATE;			//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;					//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
  	USART_Init(RS485_USART, &USART_InitStructure); 					//��ʼ������
	
  	USART_Cmd(RS485_USART, ENABLE);  									//ʹ�ܴ���
	
	USART_ClearFlag(RS485_USART, USART_FLAG_TC);
	
	USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);				//���������ж�

	//Usart NVIC ����
	 /*  Ƕ��������������ѡ��*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  	NVIC_InitStructure.NVIC_IRQChannel = RS485_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	RS485_RX_EN();				//��ʼ��Ĭ��Ϊ����ģʽ	

}




/***************** ����һ���ַ�  **********************/ 
//ʹ�õ��ֽ����ݷ���ǰҪʹ�ܷ������ţ����ͺ�Ҫʹ�ܽ������š� 
 void RS485_SendByte(  uint8_t ch ) 
 { 
//	RS485_TX_EN();//	ʹ�ܷ�������
 	/* ����һ���ֽ����ݵ� USART */ 
     USART_SendData(RS485_USART,ch); 
     /* �ȴ�������� */ 
     while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXE) == RESET); 
	 /*�Ӷ�����ʱ����֤485�����������*/
	delay_ms(1);
//	 RS485_RX_EN()	;//	ʹ�ܽ�������
}

/*****************  ����ָ�����ȵ��ַ��� **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;

	RS485_TX_EN();//	ʹ�ܷ�������	
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
		
	/*�Ӷ�����ʱ����֤485�����������*/
	delay_ms(1);
		
	RS485_RX_EN()	;//	ʹ�ܽ�������
}


/*****************  �����ַ��� **********************/
void RS485_SendString(  uint8_t *str)
{
	unsigned int k=0;
	
	RS485_TX_EN()	;//	ʹ�ܷ�������
	
    do 
    {
        RS485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
	/*�Ӷ�����ʱ����֤485�����������*/
	delay_ms(1);
		
	RS485_RX_EN()	;//	ʹ�ܽ�������
}





/*********************************************END OF FILE**********************/









