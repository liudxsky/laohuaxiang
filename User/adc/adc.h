#ifndef __ADC_H
#define	__ADC_H

#include "stm32f4xx.h"


#define  ADC_NOFCHANEL   2

#define BACK1_ADC

#if defined BACK1_ADC
/*-----------------------------------反馈1-------------------------------------*/
//BACK1 ADC3 GPIO 宏定义
#define BACK_ADC_GPIO_PORT     			GPIOF
#define BACK_ADC_GPIO_PIN     			GPIO_Pin_5
#define BACK_ADC_CHANNEL      			ADC_Channel_15
#define BACK_ADC_GPIO_CLK     			RCC_AHB1Periph_GPIOF


// ADC 序号宏定义
/*****************************************************************/
#define BACK_ANALOG_ADC              		ADC3
#define BACK_ANALOG_ADC_CLK          		RCC_APB2Periph_ADC3

/*****************************************************************/
// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define BACK_ANALOG_ADC_DR_ADDR    			((uint32_t)ADC3+0x4c)


// ADC DMA 通道宏定义，这里我们使用DMA传输
#define BACK_ANALOG_ADC_DMA_CLK      		RCC_AHB1Periph_DMA2
#define BACK_ANALOG_ADC_DMA_CHANNEL  		DMA_Channel_2
#define BACK_ANALOG_ADC_DMA_STREAM   		DMA2_Stream0


/*----------------------------------反馈2-----------------------------------------*/


#elif defined BACK2_ADC


//BACK2 ADC3 GPIO 宏定义
#define BACK_ADC_GPIO_PORT     			GPIOC
#define BACK_ADC_GPIO_PIN     			GPIO_Pin_4
#define BACK_ADC_CHANNEL      			ADC_Channel_14
#define BACK_ADC_GPIO_CLK     			RCC_AHB1Periph_GPIOC


// ADC 序号宏定义
/*****************************************************************/
#define BACK_ANALOG_ADC              		ADC1
#define BACK_ANALOG_ADC_CLK          		RCC_APB2Periph_ADC1

/*****************************************************************/
// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define BACK_ANALOG_ADC_DR_ADDR    			((uint32_t)ADC1+0x4c)


// ADC DMA 通道宏定义，这里我们使用DMA传输
#define BACK_ANALOG_ADC_DMA_CLK      		RCC_AHB1Periph_DMA2
#define BACK_ANALOG_ADC_DMA_CHANNEL  		DMA_Channel_0
#define BACK_ANALOG_ADC_DMA_STREAM   		DMA2_Stream0
/*------------------------------------------------------------------------------*/
#endif 




#define   TEMP_ADC							ADC1
#define   TEMP_ADC_CLK						RCC_APB2Periph_ADC1
#define   TEMP_ADC_CHANNEL					ADC_Channel_16

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define   TEMP_ADC_DR_ADDR    				((uint32_t)ADC1+0x4c)















void Temp_Adc_Init(void);


void Analog_Init(void);
void Analog_Gpio_Config(void);




#endif /* __BSP_ADC_H */



