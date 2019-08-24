#ifndef __DAC_H
#define	__DAC_H


#include "stm32f4xx.h"

//#define DAC_DHR12RD_Address     (uint32_t)(DAC_BASE+0x20)   //DACͨ����������Ĵ�����ַ  0x40007420//


//dacʱ��
#define DAC_CLK                     RCC_APB1Periph_DAC
#define DAC_TIM                     TIM6
#define DAC_TIM_CLK                 RCC_APB1Periph_TIM6
#define DAC_TRIGGER                 DAC_Trigger_T6_TRGO


//dacDMAͨ������ʱ����
#define DAC_DMA_CLK                 RCC_AHB1Periph_DMA1
#define DAC_CHANNEL                 DMA_Channel_7
#define DAC_DMA_STREAM              DMA1_Stream5



//dacͨ��1
#define DAC_CH1_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define DAC_CH1_GPIO_PORT           GPIOA
#define DAC_CH1_GPIO_PIN            GPIO_Pin_4
#define DAC_CH1_CHANNEL             DAC_Channel_1



//dacͨ��2
#define DAC_CH2_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define DAC_CH2_GPIO_PORT           GPIOA
#define DAC_CH2_GPIO_PIN            GPIO_Pin_5
#define DAC_CH2_CHANNEL             DAC_Channel_2

//ѡ�񿪹�GPIO
#define DAC_SELECT_GPIO_CLK			RCC_AHB1Periph_GPIOG
#define DAC_SELECT_GPIO_PORT		GPIOG

#define DAC_SELECT1_GPIO_PIN		GPIO_Pin_0
#define DAC_SELECT2_GPIO_PIN		GPIO_Pin_1









void DAC1_Init(void);
void Dac1_Set_Vol(double vol);
void Dac_Select_Init(void);

#endif /* __DAC_H */

