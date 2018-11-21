#include "./max6675/max6675.h"
 #include "./delaytime/delaytime.h"
 #include "stdio.h"

uint8_t thermocouple_flag = 0;
 
//max6675 gpio��ʼ������
void Max6675_Gpio_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd (MAX6675_SPI_SCK_GPIO_CLK | MAX6675_SPI_MISO_GPIO_CLK|MAX6675_CS_GPIO_CLK, ENABLE);
  	MAX6675_SPI_CLK_INIT (MAX6675_SPI_CLK, ENABLE);            //ʹ��SPIʱ��

 	//�������Ÿ���
 	GPIO_PinAFConfig(MAX6675_SPI_SCK_GPIO_PORT,MAX6675_SPI_SCK_PINSOURCE,MAX6675_SPI_SCK_AF); 
	GPIO_PinAFConfig(MAX6675_SPI_MISO_GPIO_PORT,MAX6675_SPI_MISO_PINSOURCE,MAX6675_SPI_MISO_AF); 
	
    /*!< ���� SPI_SI4432_SPI ����: SCK */
    GPIO_InitStructure.GPIO_Pin = MAX6675_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	  
	GPIO_Init(MAX6675_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_SI4432_SPI ����: MISO */
	GPIO_InitStructure.GPIO_Pin = MAX6675_SPI_MISO_PIN;
	GPIO_Init(MAX6675_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	  

	/*!< ���� SPI_SI4432_SPI ����: NSEL */
	GPIO_InitStructure.GPIO_Pin = MAX6675_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(MAX6675_CS_GPIO_PORT, &GPIO_InitStructure);

  	/* ֹͣ�ź�  NSEL���Ÿߵ�ƽ*/
    SPI_MAX6675_CS_HIGH();

	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                      //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                      //����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	
	SPI_Init(MAX6675_SPI, &SPI_InitStructure);                           //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(MAX6675_SPI, ENABLE);    //ʹ��SPI����
 
	
}




/****************************************************************************
* ��    ��: uint8_t SPI1_ReadWriteByte(uint8_t writeData)
* ��    �ܣ�spi2��д����
* ��ڲ�����writeData:Ҫд����ֽ�
* ���ز�������ȡ�����ֽ�
* ˵    ���� 		     
****************************************************************************/
uint8_t SPI2_ReadWriteByte(uint8_t writeData)
{		 			 
   	uint8_t waitnum=0;
  	while (SPI_I2S_GetFlagStatus(MAX6675_SPI, SPI_I2S_FLAG_TXE) == RESET) //�ȴ���������  
	{  
	      waitnum++;  
//	    if(waitnum>200) return 0;  
	}
	SPI_I2S_SendData(MAX6675_SPI, writeData);  //ͨ������SPI2����һ���ֽ�
		
  	while (SPI_I2S_GetFlagStatus(MAX6675_SPI, SPI_I2S_FLAG_RXNE) == RESET) //�ȴ�һ���ֽڽ����� 
    {    
			   waitnum++;  
//	    if(waitnum>200) return 0;  
	}
	return SPI_I2S_ReceiveData(MAX6675_SPI);  //����SPI2���յ�����	
 		    
}

//��дʮ��λ����
unsigned short SpiReadWriteWord(unsigned short spi_in)
{
  unsigned short temp16;
  				
  temp16 = (unsigned short) (SPI2_ReadWriteByte( (unsigned char)((spi_in & 0xFF00) >> 8) ));
  temp16 <<= 8;
  temp16 += (unsigned short) (SPI2_ReadWriteByte( (unsigned char)(spi_in & 0x00FF) ));
  return temp16;
}

#if 0

//���ȵ�ż��������(��λ)
uint16_t Max6675_Read_Tem(void)
{
	uint16_t temp = 0;
	uint8_t i;

	SPI_MAX6675_CS_LOW();

	 delay_us(50);
     for(i = 0; i< 16;i++)
	{
		GPIO_SetBits(MAX6675_SPI_SCK_GPIO_PORT, MAX6675_SPI_SCK_PIN);
		delay_us(50);
		 temp<<=1;
		if(READ_SO_DATA)
		{
		 temp |= 0x0001;
		}
		 GPIO_ResetBits(MAX6675_SPI_SCK_GPIO_PORT, MAX6675_SPI_SCK_PIN);
		 delay_us(50);	  	
	}
	
	SPI_MAX6675_CS_HIGH();

	temp = temp >> 3;
	return temp;
}
#else
//spi ��д
 uint16_t Max6675_Read_Tem(void)
{
	uint16_t temp = 0;
	uint8_t i;

	SPI_MAX6675_CS_LOW();

	temp =  SPI2_ReadWriteByte(0xff);
	temp <<= 8;
	temp |= SPI2_ReadWriteByte(0xff);
	
	SPI_MAX6675_CS_HIGH();
	thermocouple_flag = (temp>>2)&0x01;
	if(temp & 4)
	{
		temp = 4095;
	}
	else
	{
		
		temp = temp >> 3;
	}
	return temp;
}

#endif








