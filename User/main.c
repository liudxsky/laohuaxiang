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
extern uint8_t writecoilflag,readcoilflag,readholdingflag,writeholdingflag,readinputflag;

extern dev_info_t dev_info;
extern uint8_t cmd_buffer[CMD_MAX_SIZE];		//ָInstruction cache
extern uint8_t press_flag;

int runstatus_last=0;
int debuginfo=0;
extern arm_pid_instance_f32 PID;
float kalman_temp=0;


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
	volatile uint32_t t_thread30s=0; 
	volatile uint32_t t_thread1h=0;
	uint32_t t_thread1min=0;
	uint16_t Ktemperature = 0;
  eMBErrorCode    eStatus;

	qsize  size = 0;
	System_Init();												//init
	eStatus = eMBInit( MB_RTU, dev_info.flash_setvalue.modbus_address, 0x01, dev_info.flash_setvalue.modbus_tran_rate, MB_PAR_NONE ); //Modbus Init
	eStatus = eMBEnable();									//Free MODBUS enable
	delay_s(1);
	STM32_Read_ID();
	startscreen();	

	SetPoint=dev_info.setTemp;
	PIDInit(dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D,SetPoint);//need to be reset after chage setpoint

	while(1)
    {
      eMBPoll();											//analyze Modbus data 
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
			if(dev_info.runstatus>0&&runstatus_last==0)
			{
				SetPoint=dev_info.setTemp;
				PIDInit(dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D,SetPoint);
			}
			runstatus_last=dev_info.runstatus;
		//	printf("global T:%d", t_thread500);
			Ktemperature=Max6675_Read_Tem()+ dev_info.compensatetemp;
			temperRaw=Ktemperature*0.25-7;
			//SetPoint=100;
			temperFilter=getFilterTemper(temperRaw);
//			printf("%f\n",temperFilter);
			//temperFilter=temper_usart;
			error=SetPoint-temperFilter;
//			if(debuginfo)
//			printf("Setpoint:%.2lf\n",SetPoint);
			//use button to change status
			if(dev_info.runstatus==1)
			{
				pwmOut=pidCalc(error);
			}
			if(dev_info.runstatus==2) //button event to set tuning flag
			{
				autoTuneParam.f_autoTuning=1;
				autoTuneParam.SetPoint=SetPoint;
				dev_info.runstatus=3;
			}
			if(autoTuneParam.f_autoTuning)
			{
				pwmOut=autoTuning(error,NULL,&autoTuneParam);
				if(autoTuneParam.f_autoTuningDone)
				{//auto tune finished
					
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
						if(dev_info.testTime!=0)
						{
							dev_info.runstatus=1;
						}
						else
						{
							dev_info.runstatus=0;
						}
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

			if(dev_info.runstatus>0)//start heating
			{
				SetPwmValue(pwmOut);
			}
			else//(dev_info.runstatus==0)
			{
				SetPwmValue(0);
			}
		}
		
		if(getMsCounter() - timer_tick_count > 1000)
		{
			timer_tick_count = getMsCounter();
			ReadRtcTime();										//read current RTC time
			start_endtime_set();								//start and end time setting
			
			if(dev_info.useKalman==1)
			{
				kalman_temp=adj_display(temperFilter);
			}
			else
			{
				kalman_temp=temperFilter;
			}
			temp_detection(kalman_temp);						//temp detection
			Check_All_Status();	
			update_dev_status();
			//modebus register update;
			//main text and icon update
			//
			printf("%f,%f\n",temperFilter,kalman_temp);
		}
		if(getMsCounter()-t_thread3s>3000)
		{
			t_thread3s=getMsCounter();
			//update dev_info into other text and icon
			//
	
		}
		if(getMsCounter()-t_thread30s>30000)
		{
			t_thread30s=getMsCounter();
			addup_testtime();
			if(dev_info.dev_status_changed_flag==1)//fast write
			{
				dev_info.dev_status_changed_flag=0;
				writeFlash();
			}
		}
		if(getMsCounter() - t_thread1h > 60000)
		{
			t_thread1h = getMsCounter();
			//1h thread

			if(dev_info.dev_status_changed_flag > 1)//slow write
			{
				dev_info.dev_status_changed_flag=0;
				SetPoint=dev_info.setTemp;
				writeFlash();
//				__disable_irq(); 
//				FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));	
//				__enable_irq(); 
			}

		}
		#endif
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
	check_language_select();//language select
	Analog_Init();			//adc init						
	DAC1_Init();			//dac init						
	Dac_Select_Init();		//dac select
	Max6675_Gpio_Init();	//thermocouple gpio init						
	control_mode_select();	//control mode select						
	screen_init();			//screen data init						
	modbus_register_init();
	ReadRtcTime();
//	autogeneratepassword();
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
		readinputflag = 1;
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
				readholdingflag = 1;
				while( usNRegs > 0 )
				{
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;
				}
			break;
			case MB_REG_WRITE:
				writeholdingflag = 1;
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
				readcoilflag = 1;
				while( iNCoils > 0 )
				{
					*pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
					( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
					iNCoils -= 8;
					usBitOffset += 8;
				}
			break;

	
			case MB_REG_WRITE:	
				writecoilflag = 1;
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







