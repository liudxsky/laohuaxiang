/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./delaytime/delaytime.h"
#include "./gpio/gpio.h"
#include "stdio.h"
#include "./usart/usart.h"
#include "port.h"
#include "./tim/tim.h"
#include "./pwm/pwm.h"
#include "mb.h"
#include "mbport.h"
#include "./screen/screen.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "string.h"
#include "stdlib.h"
#include "./flash/deviceinfo.h"
#include "./flash/flash.h" 
#include "./adc/adc.h"
#include "./dac/dac.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//extern volatile uint32_t timer_tick_count;

void SysTick_Handler(void)
{
//	 ++timer_tick_count;
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/



uint8_t RxBuffer[RxBufferSize];

__IO uint8_t RxCounter = 0x00;
extern dev_info_t dev_info;
extern MainShowTextValue showtextvalue;	//主页面文本控件缓存值
extern int runstatus;//0 stop, 1 start, 2, autotune
extern int debuginfo;

const uart_cmd_t uart_cmd[] = 
{
	{"show_value", 10},
	{"set_pwmscope",12},
	{"set_pwmvalue",12},
	{"set_angle",9},
	{"help", 4},
	{"run",3},
	{"debug",5},
	{"setpoint",8}
};

//串口1中断函数
void RS232_USART_IRQHandler(void)
{
	char dst_vale[5];
	uint8_t i = 0;
	uint8_t index = 0;
	if(USART_GetITStatus(RS232_USART, USART_IT_RXNE) != RESET)  //接收中断
	{
		RxBuffer[RxCounter] =USART_ReceiveData(RS232_USART);	 //读取收到的数据
		//回显
		printf("%c",RxBuffer[RxCounter]);
		//0x0d 回车
		if(RxBuffer[RxCounter]==0x0d||RxBuffer[RxCounter]==0x0a)
		{
			if(strstr((char *)RxBuffer,uart_cmd[SET_PWMSCOPE].cmd)) 	
			{					
				index = uart_cmd[SET_PWMSCOPE].len;
				memset((void *)dst_vale,0,5);
				strncpy(dst_vale,(char *)&RxBuffer[index],RxCounter-index);
				dev_info.valid_flag = true;
				SetPwmScope(atoi(dst_vale));
				//dev_info.pwmscope = atoi(dst_vale);
//				FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
				//TIM_PWMOUTPUT_Config();
				printf("\r\ncurrent PWM Scope is  0 -- %d \r\n",dev_info.pwmscope);
			}
			else if(strstr((char *)RxBuffer,uart_cmd[SET_PWMVLAUE].cmd)) 	
			{					
				index = uart_cmd[SET_PWMVLAUE].len;
				memset((void *)dst_vale,0,5);
				strncpy(dst_vale,(char *)&RxBuffer[index],RxCounter-index);
				dev_info.valid_flag = true;
				SetPwmValue(atoi(dst_vale));
				//dev_info.pwmvalue = atoi(dst_vale);
//				FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));			
				//TIM_PWMOUTPUT_Config();
				printf("\r\n\r\npwmvalue is set: \r\n------dec: %d\r\n------hex: 0x%x\r\n",dev_info.pwmvalue,dev_info.pwmvalue);
				printf("\r\n\r\ncurrent temperature is : %.2lf\r\n",showtextvalue.current_temp_vlaue);
			}
			else if(strstr((char *)RxBuffer,uart_cmd[SET_ANGLE].cmd)) 	
			{					
				index = uart_cmd[SET_ANGLE].len;
				memset((void *)dst_vale,0,5);
				strncpy(dst_vale,(char *)&RxBuffer[index],RxCounter-index);
				dev_info.valid_flag = true;
				dev_info.airvalveangle = atoi(dst_vale);
//				FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));		
				printf("\r\n\r\nair valve angle value is set: \r\n------dec: %d\r\n------hex: 0x%x\r\n",dev_info.airvalveangle,dev_info.airvalveangle);
				printf("\r\n\r\nback value  is : %04x  %d\r\n",dev_info.airvalveangle,dev_info.airvalveangle);
				Dac1_Set_Vol(dev_info.airvalveangle);
			}
			else if(strstr((char *)RxBuffer,uart_cmd[SHOWVALUE].cmd))			//显示当前设备存储值
			{		
//				FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
				printf("\r\n***********device info*********\r\n");
				printf("\r\ncurrent PWM Scope is  0 -- %d \r\n",dev_info.pwmscope);
				printf("\r\ncurrent PWM Value is %d \r\n",dev_info.pwmvalue);	
				printf("\r\n\r\ncurrent temperature is :%.2lf\r\n",showtextvalue.current_temp_vlaue);
				printf("\r\n\r\ncurrent air valve angle value is set: \r\n------dec: %d\r\n------hex: 0x%x\r\n",dev_info.airvalveangle,dev_info.airvalveangle);
				printf("\r\n\r\ncurrent back value  is : %04x  %d\r\n",dev_info.airvalveangle,dev_info.airvalveangle);
			}
			else if(strstr((char *)RxBuffer,uart_cmd[HELP].cmd))				//帮助说明
			{
				printf("\r\n=========cmd prompt============\r\n");
				printf("1  ->show_value(show All param)\r\n");
				printf("2  ->set_pwmscope (set pwm parameter scope)\r\n");
				printf("3  ->set_pwmvalue (set pwm/temperature parameter value)\r\n");
				printf("2  ->set_angle (set air valve angle value)\r\n");
				printf("\r\n");
			}
			else if(strstr((char *)RxBuffer,uart_cmd[RUN].cmd))
			{
				index = uart_cmd[RUN].len;
				memset((void *)dst_vale,0,5);
				strncpy(dst_vale,(char *)&RxBuffer[index],RxCounter-index);
				debuginfo=atoi(dst_vale);
			}
			else if(strstr((char *)RxBuffer,uart_cmd[DEBUG].cmd))
			{
				index = uart_cmd[DEBUG].len;
				memset((void *)dst_vale,0,5);
				strncpy(dst_vale,(char *)&RxBuffer[index],RxCounter-index);
				runstatus=atoi(dst_vale);
			}
			else if(strstr((char *)RxBuffer,uart_cmd[SETPOINT].cmd))
			{
				index = uart_cmd[SETPOINT].len;
				memset((void *)dst_vale,0,5);
				strncpy(dst_vale,(char *)&RxBuffer[index],RxCounter-index);
				runstatus=atoi(dst_vale);
			}
			else
			{
				printf("\r\nCmd Error! please input again!\r\n");		
			}
			RxCounter=0;
			for(i = 0;i<RxBufferSize;i++)
			{		
				RxBuffer[i] = 0x00;
			}
			printf("\r\n******End!******\r\n\r\n");
		}
		else
		{
			  RxCounter++;
		}  
	}
	USART_ClearITPendingBit(RS232_USART, USART_IT_RXNE);   //清除中断标志位
}


extern uint8_t  Rs485Status;

//串口3中断函数
void RS485_USART_IRQHandler(void)
{
	if(USART_GetITStatus(RS485_USART, USART_IT_RXNE) != RESET)  //接收中断
	{
		 USART_ClearITPendingBit(RS485_USART, USART_IT_RXNE);   //清除中断标志位 
		 LED_ON;
		 prvvUARTRxISR();      
	}
    if(USART_GetITStatus(RS485_USART, USART_IT_TXE) != RESET)    //发送中断
	{
		USART_ClearITPendingBit(RS485_USART, USART_IT_TXE);   //清除中断标志位 
		LED_OFF;
		prvvUARTTxReadyISR();	 
	}
}

//modbus tim
void BASIC_TIM_IRQHandler(void)
{
	
	if (TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET)
	{
	
		TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);
		
	    prvvTIMERExpiredISR();   
	}
	
}

//计时1ms
uint32_t _ms_counter = 0;
void SECOND_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( SECOND_TIM, TIM_IT_Update) != RESET ) 
	{
		++_ms_counter;
		
	} 
	TIM_ClearITPendingBit(SECOND_TIM , TIM_IT_Update); //清除状态位 	


}

uint8_t screen_flag = 0;
//显示屏串口
void SCREEN_USART_IRQHandler(void)
{
 if (USART_GetITStatus(SCREEN_USART, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(SCREEN_USART);
        queue_push(data);
		screen_flag = 1;
    }  
// USART_ClearITPendingBit(SCREEN_USART, USART_IT_RXNE);   //清除中断标志位
}







/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
