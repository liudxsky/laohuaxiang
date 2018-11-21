#ifndef __PWM_H
#define	__PWM_H

#include "stm32f4xx.h"



#define 			PWM_PIN             			GPIO_Pin_9              
#define				PWM_GPIO_PORT       			GPIOE                      
#define 			PWM_GPIO_CLK        			RCC_AHB1Periph_GPIOE
#define 			PWM_PINSOURCE					GPIO_PinSource9
#define 			PWM_AF							GPIO_AF_TIM1

#define 			PWM_TIMx           		    	TIM1
#define 			PWM_TIMx_CLK       		    	RCC_APB2Periph_TIM1


#define 			BACK_PWM_PIN             		GPIO_Pin_10 







void TIM_PWMOUTPUT_Config(void);

void TIMx_Configuration(void);


void Pwm_Output(FunctionalState NewState);



#endif /* __GENERAL_TIM_H */
