#include "./max6675/max6675.h"
 #include "./delaytime/delaytime.h"
 #include "stdio.h"

uint8_t thermocouple_flag = 0;
 
//max6675 gpio初始化设置
void Max6675_Gpio_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd (MAX6675_SPI_SCK_GPIO_CLK | MAX6675_SPI_MISO_GPIO_CLK|MAX6675_CS_GPIO_CLK, ENABLE);
  	MAX6675_SPI_CLK_INIT (MAX6675_SPI_CLK, ENABLE);            //使能SPI时钟

 	//设置引脚复用
 	GPIO_PinAFConfig(MAX6675_SPI_SCK_GPIO_PORT,MAX6675_SPI_SCK_PINSOURCE,MAX6675_SPI_SCK_AF); 
	GPIO_PinAFConfig(MAX6675_SPI_MISO_GPIO_PORT,MAX6675_SPI_MISO_PINSOURCE,MAX6675_SPI_MISO_AF); 
	
    /*!< 配置 SPI_SI4432_SPI 引脚: SCK */
    GPIO_InitStructure.GPIO_Pin = MAX6675_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	  
	GPIO_Init(MAX6675_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_SI4432_SPI 引脚: MISO */
	GPIO_InitStructure.GPIO_Pin = MAX6675_SPI_MISO_PIN;
	GPIO_Init(MAX6675_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	  

	/*!< 配置 SPI_SI4432_SPI 引脚: NSEL */
	GPIO_InitStructure.GPIO_Pin = MAX6675_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(MAX6675_CS_GPIO_PORT, &GPIO_InitStructure);

  	/* 停止信号  NSEL引脚高电平*/
    SPI_MAX6675_CS_HIGH();

	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                      //串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                      //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	
	SPI_Init(MAX6675_SPI, &SPI_InitStructure);                           //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(MAX6675_SPI, ENABLE);    //使能SPI外设
 
	
}




/****************************************************************************
* 名    称: uint8_t SPI1_ReadWriteByte(uint8_t writeData)
* 功    能：spi2读写函数
* 入口参数：writeData:要写入的字节
* 返回参数：读取到的字节
* 说    明： 		     
****************************************************************************/
uint8_t SPI2_ReadWriteByte(uint8_t writeData)
{		 			 
   	uint8_t waitnum=0;
  	while (SPI_I2S_GetFlagStatus(MAX6675_SPI, SPI_I2S_FLAG_TXE) == RESET) //等待发送区空  
	{  
	      waitnum++;  
//	    if(waitnum>200) return 0;  
	}
	SPI_I2S_SendData(MAX6675_SPI, writeData);  //通过外设SPI2发送一个字节
		
  	while (SPI_I2S_GetFlagStatus(MAX6675_SPI, SPI_I2S_FLAG_RXNE) == RESET) //等待一个字节接收完 
    {    
			   waitnum++;  
//	    if(waitnum>200) return 0;  
	}
	return SPI_I2S_ReceiveData(MAX6675_SPI);  //返回SPI2接收的数据	
 		    
}

//读写十六位数据
unsigned short SpiReadWriteWord(unsigned short spi_in)
{
  unsigned short temp16;
  				
  temp16 = (unsigned short) (SPI2_ReadWriteByte( (unsigned char)((spi_in & 0xFF00) >> 8) ));
  temp16 <<= 8;
  temp16 += (unsigned short) (SPI2_ReadWriteByte( (unsigned char)(spi_in & 0x00FF) ));
  return temp16;
}

#if 0

//读热电偶输入数据(移位)
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
//spi 读写
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








