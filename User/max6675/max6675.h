#ifndef __MAX6675_H
#define __MAX6675_H 			   
#include "stm32f4xx.h" 




/*SPI接口定义-开头****************************/
#define MAX6675_SPI                           SPI2
#define MAX6675_SPI_CLK                       RCC_APB1Periph_SPI2
#define MAX6675_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd




//SCK引脚
#define MAX6675_SPI_SCK_PIN                   GPIO_Pin_13                  
#define MAX6675_SPI_SCK_GPIO_PORT             GPIOB                       
#define MAX6675_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define MAX6675_SPI_SCK_PINSOURCE             GPIO_PinSource13
#define MAX6675_SPI_SCK_AF                    GPIO_AF_SPI2

//MISO引脚
#define MAX6675_SPI_MISO_PIN                  GPIO_Pin_14                
#define MAX6675_SPI_MISO_GPIO_PORT            GPIOB                   
#define MAX6675_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define MAX6675_SPI_MISO_PINSOURCE            GPIO_PinSource14
#define MAX6675_SPI_MISO_AF                   GPIO_AF_SPI2


//CS引脚
#define MAX6675_CS_PIN                        GPIO_Pin_12              
#define MAX6675_CS_GPIO_PORT                  GPIOB                     
#define MAX6675_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOB

#define MAX6675_CLK							  RCC_AHB1Periph_GPIOB


//控制CS引脚输出低电平/高电平
#define SPI_MAX6675_CS_LOW()  do { GPIO_ResetBits(MAX6675_CS_GPIO_PORT, MAX6675_CS_PIN); } while(0)
#define SPI_MAX6675_CS_HIGH()  do { GPIO_SetBits(MAX6675_CS_GPIO_PORT, MAX6675_CS_PIN); } while(0)


//控制SCK引脚输出使能/禁能
#define SPI_MAX6675_SCK_LOW()   GPIO_ResetBits(MAX6675_SPI_SCK_GPIO_PORT, MAX6675_SPI_SCK_PIN) 
#define SPI_MAX6675_SCK_HIGH()  GPIO_SetBits(MAX6675_SPI_SCK_GPIO_PORT, MAX6675_SPI_SCK_PIN)

#define READ_SO_DATA 	GPIO_ReadInputDataBit(MAX6675_SPI_MISO_GPIO_PORT,MAX6675_SPI_MISO_PIN)









void Max6675_Gpio_Init(void);


uint16_t Max6675_Read_Tem(void);





#endif 
