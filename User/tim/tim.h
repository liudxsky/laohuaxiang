#ifndef __TIM_H
#define	__TIM_H
#include "stm32f4xx.h"
#include "port.h"




//modbus专用计时
#define BASIC_TIM           		TIM2
#define BASIC_TIM_CLK       		RCC_APB1Periph_TIM2

#define BASIC_TIM_IRQn				TIM2_IRQn
#define BASIC_TIM_IRQHandler    	TIM2_IRQHandler







//计时S
#define SECOND_TIM           		TIM6
#define SECOND_TIM_CLK       		RCC_APB1Periph_TIM6

#define SECOND_TIM_IRQn				TIM6_DAC_IRQn
#define SECOND_TIM_IRQHandler    	TIM6_DAC_IRQHandler


uint32_t getMsCounter(void);

void TIMx_Init(void);

BOOL TIM_Mode_Config(uint16_t usTim1Timerout50us);




#endif /* __BASIC_TIM_H */

