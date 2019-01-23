#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "./tim/tim.h"
#include "./gpio/gpio.h"
#include "./usart/usart.h"
#include "./delaytime/delaytime.h"
#include "./dac/dac.h"
#include "./pwm/pwm.h"
#include "./max6675/max6675.h"
#include "cmd_queue.h"
#include "./screen/screen.h"
#include "hmi_user_uart.h"
#include "./adc/adc.h"
#include "./flash/deviceinfo.h"
#include "./flash/flash.h"
#include "time.h"
#include <ctime>
#include "./status/status.h"
#include "pidcontroller.h"
#include "crypto.h"

uint16_t current_screen_id = 0;
volatile uint32_t  timer_tick_count = 0; // tick timer

extern uint16_t ADC_ConvertedValue[ADC_NOFCHANEL];

extern dev_info_t dev_info;
extern RtcTime rtctime;
extern uint8_t cmd_buffer[CMD_MAX_SIZE];		//ָInstruction cache
extern uint8_t press_flag;
extern MainShowTextValue	showtextvalue;	//��ҳ���ı��ؼ�����ֵ
int runstatus=0;
int runstatus_last=0;
int debuginfo=0;
extern arm_pid_instance_f32 PID;
/* ----------------------- Defines ------------------------------------------*/

//input register
uint16_t    usRegInputBuf[REG_INPUT_NREGS]= {0};


//holding register
uint16_t	usRegHoldingBuf[REG_HOLDING_NREGS] = {0};


//coil  register
uint16_t	ucRegCoilsBuf[REG_COILS_SIZE] = {0};


//switch input register
uint16_t	ucRegDiscreteBuf[REG_DISCRETE_SIZE] = {0};

/* --------------------------------------------------------------------------*/


extern  uint32_t password1;
extern uint32_t password2;
extern uint16_t Modbus_rate;
float temper_usart;



/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
	float temperFilter=0;
	float temperRaw=0;
	float SetPoint=100;
	float error=0;
	uint16_t pwmOut=0;
	int AutoTuningDone=0;
	struct AutoTuningParamStruct autoTuneParam;
	volatile uint32_t t_thread100=0;
	volatile uint32_t t_thread500=0;
	volatile uint32_t t_thread3s=0;
	
	uint16_t Ktemperature = 0;
  	eMBErrorCode    eStatus;
	qsize  size = 0;
	System_Init();												//init

	eStatus = eMBInit( MB_RTU, dev_info.Modbus_address, 0x01, Modbus_rate, MB_PAR_NONE ); //Modbus Init
	eStatus = eMBEnable();									//Free MODBUS enable
	delay_s(1);
	SetBackLight(20);											//set screen Backlight brightness
	STM32_Read_ID();
	startscreen();	
	


	//start screen
	//SetPoint=showtextvalue.setting_temp;
	SetPoint=dev_info.testtemp;
	PIDInit(dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D,SetPoint);//need to be reset after chage setpoint
	dev_info.dev_status_changed_flag=1;
	//dev_info.pidvalue.PID_P=PIDKP;
	//dev_info.pidvalue.PID_I=PIDKI;
	//dev_info.pidvalue.PID_D=PIDKD;
	//dev_info.pwmscope=1000;
	while(1)
    {
      eMBPoll(  );											//analyze Modbus data 
      size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); 		//get one screen   command     
		if(size>0)												//received valid instructions
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָinstruction deal
		}
		#if 1
		if(getMsCounter()-t_thread100>200)
		{
			t_thread100=getMsCounter();
			ledBlinkThread();
		}
		if(getMsCounter()-t_thread500>500)
		{
			t_thread500=getMsCounter();
			if(runstatus>0&&runstatus_last==0)
			{
				SetPoint=dev_info.testtemp;
				PIDInit(dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D,SetPoint);
				HEAT_ON;
			}
			runstatus_last=runstatus;
		//	printf("global T:%d", t_thread500);
			Ktemperature=Max6675_Read_Tem();
			temperRaw=Ktemperature*0.25;
			//SetPoint=100;
			temperFilter=getFilterTemper(temperRaw);
//			printf("%f\n",temperFilter);
			//temperFilter=temper_usart;
			error=SetPoint-temperFilter;
//			if(debuginfo)
//			printf("Setpoint:%.2lf\n",SetPoint);
			//use button to change status
			if(runstatus==1)
			{
				pwmOut=pidCalc(error);
			}
			if(runstatus==2) //button event to set tuning flag
			{
				autoTuneParam.f_autoTuning=1;
				runstatus=3;
			}
			if(autoTuneParam.f_autoTuning)
			{
				pwmOut=autoTuning(error,NULL,&autoTuneParam);
				if(autoTuneParam.f_autoTuningDone)
				{//auto tune finished
					runstatus=0;
					//new parameters,should stop and re-run process
					printf("auto tune status:%d",autoTuneParam.AutoTuneStatus);
					if(autoTuneParam.AutoTuneStatus>0)
					{
						//auto tune success
						PID.Kp=autoTuneParam.Kp_auto;
						PID.Ki=autoTuneParam.Ki_auto;
						PID.Kd=autoTuneParam.Kd_auto;
						dev_info.pidvalue.PID_P=autoTuneParam.Kp_auto;
						dev_info.pidvalue.PID_I=autoTuneParam.Ki_auto;
						dev_info.pidvalue.PID_D=autoTuneParam.Kd_auto;
						dev_info.dev_status_changed_flag=1;
						printf("Kp:%f,Ki:%f,Kd%f\n",PID.Kp,PID.Ki,PID.Kd);
						FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));	
					}
					else
					{
						//autotune failed
					}
					
				}
				else
				{
					//auto-tuning still runing 
				}
			}

			if(runstatus>0)//start heating
			{
				SetPwmValue(pwmOut);
				//heat icon update
				HEAT_ON;
			}
			else//(runstatus==0)
			{
				SetPwmValue(0);
				//heat icon update
				HEAT_OFF;
			}
		}
		#endif
		if(getMsCounter() - timer_tick_count > 1000)
		{
			timer_tick_count = getMsCounter();
			ReadRtcTime();										//read current RTC time
			start_endtime_set();								//start and end time setting
			temp_detection(temperFilter);						//temp detection						
			Check_All_Status();	
//			modbus_register_handle();
		}
		if(getMsCounter() - t_thread3s > 3000)
		{
			t_thread3s = getMsCounter();
			//3s thread
			//add status and relay control here.
			if(dev_info.dev_status_changed_flag == 1)
			{
				dev_info.dev_status_changed_flag=0;
				SetPoint=dev_info.testtemp;
				update_dev_status();
			}
			else
			{
				dev_info.dev_status_changed_flag = 0;
			}	
		}
			device_timing_selfcheck();
    }
}





//SYSTEM init
void System_Init(void)
{
	delay_init();			//delay function init					
	Gpio_Init();			//gpio init						
	USART1_Config();		//debug usart1 config			
	UartInit();				//screen usart init											
	delay_s(1);
	DeviceInfo_Init();		//device message init
	Crypto_DeInit();		//DeInitialize STM32 Cryptographic Library												
	TIMx_Configuration();	//pwm time config						
	TIMx_Init();			//timekeeping init	
	check_modbuss_rate();	//modbus rate
	check_language_select();//language select
	Analog_Init();			//adc init						
	DAC1_Init();			//dac init						
	Dac_Select_Init();		//dac select
	Max6675_Gpio_Init();	//thermocouple gpio init						
	control_mode_select();	//control mode select						
	screen_init();			//screen data init						
	autogeneratepassword();	//generatepassword
}











#if 1
//input register deal with function
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
   	int16_t iRegIndex;
	if( ( (int16_t)usAddress >= REG_INPUT_START ) \
	&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
	{

		iRegIndex = ( int16_t )( usAddress - REG_INPUT_START );
		
		while( usNRegs > 0 )			
		{
			
			*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );			
			
			*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );		
			
			iRegIndex++;		
			
			usNRegs--;			
		}
	}
	else
	{
		eStatus = MB_ENOREG;	 
	}
    return eStatus;
}



//holding register deal with function

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
	eMBErrorCode eStatus = MB_ENOERR;				
	int16_t iRegIndex;							

	if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
	&& ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
	{	
		iRegIndex = ( int16_t )( usAddress - REG_HOLDING_START);   

		switch ( eMode )
		{	
			case MB_REG_READ:			
				while( usNRegs > 0 )
				{
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;
				}
			break;
			case MB_REG_WRITE:			
				while( usNRegs > 0 )
				{
					usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
					usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
					iRegIndex++;
					usNRegs--;
				}
			break;
		 }
	}
	else
	{
		eStatus = MB_ENOREG;			
	}
    return eStatus;
}


//coils register deal with function
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{

	eMBErrorCode eStatus = MB_ENOERR;		    	

	int16_t iNCoils = ( int16_t )usNCoils;			
	
	int16_t usBitOffset;						

	if( ( (int16_t)usAddress >= REG_COILS_START ) &&
	( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
	{
		usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
		switch ( eMode )
		{
			case MB_REG_READ:								
				while( iNCoils > 0 )
				{
					*pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
					( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
					iNCoils -= 8;
					usBitOffset += 8;
				}
			break;

	
			case MB_REG_WRITE:								
				while( iNCoils > 0 )
				{
					xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
					( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
					*pucRegBuffer++ );
					iNCoils -= 8;
				}
			break;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}




//switch register deal with function
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{  
	eMBErrorCode eStatus = MB_ENOERR;		 			
	
	int16_t iNDiscrete = ( int16_t )usNDiscrete;		
	
	uint16_t usBitOffset;								
	
	if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
	( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
	{
		usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );

		while( iNDiscrete > 0 )
		{
			*pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
			( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
			iNDiscrete -= 8;
			usBitOffset += 8;
		}

	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}

#endif







