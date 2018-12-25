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
#include "time.h"
#include <ctime>
#include "./status/status.h"
#include "pidcontroller.h"

uint16_t current_screen_id = 0;
volatile uint32_t  timer_tick_count = 0; //��ʱ������
volatile uint32_t t_thread500=0;
extern uint16_t ADC_ConvertedValue[ADC_NOFCHANEL];

extern dev_info_t dev_info;
extern RtcTime rtctime;
extern uint8_t cmd_buffer[CMD_MAX_SIZE];		//ָ���
extern uint8_t press_flag;
extern MainShowTextValue	showtextvalue;	//��ҳ���ı��ؼ�����ֵ
int runstatus=2;
int debuginfo=0;
extern arm_pid_instance_f32 PID;
/* ----------------------- Defines ------------------------------------------*/

//����Ĵ�������
uint16_t    usRegInputBuf[REG_INPUT_NREGS]= {0};


//���ּĴ�������
uint16_t	usRegHoldingBuf[REG_HOLDING_NREGS] = {0};


//��Ȧ�Ĵ�������
uint16_t	ucRegCoilsBuf[REG_COILS_SIZE] = {0};


//��������Ĵ�������
uint16_t	ucRegDiscreteBuf[REG_DISCRETE_SIZE] = {0};

/* --------------------------------------------------------------------------*/


extern  uint32_t password1;
extern uint32_t password2;
extern float adjusttemp;
extern Touch_Times touchtimes;
extern MainShowTextValue showtextvalue;	//��ҳ���ı��ؼ�����ֵ
float temper_usart;



/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
	float temperFilter=0;
	float temperRaw=0;
	float SetPoint=150;
	float error=0;
	uint16_t pwmOut=0;
	int AutoTuningDone=0;
	struct AutoTuningParamStruct autoTuneParam;
	long t_thread100=0;
	uint16_t Ktemperature = 0;
  eMBErrorCode    eStatus;
	qsize  size = 0;
	System_Init();												//ϵͳ��ʼ������

	eStatus = eMBInit( MB_RTU, 0x01, 0x01, 9600, MB_PAR_NONE ); //��ʼ��Modbus
	eStatus = eMBEnable();									//��Free MODBUS
	delay_s(1);
	SetBackLight(20);											//��ʼ��Ļ��������
	
	startscreen();												//start screen
	//SetPoint=showtextvalue.setting_temp;
	PIDInit(PIDKP,PIDKI,PIDKD,SetPoint);//need to be reset after chage setpoint

	while(1)
    {
      eMBPoll(  );											//��ѯ����֡ 
      size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); 		//�ӻ������л�ȡһ��ָ��        
		if(size>0)												//���յ�ָ��
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
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
		//	printf("global T:%d", t_thread500);
			Ktemperature=Max6675_Read_Tem();
			temperRaw=Ktemperature*0.25;
			//SetPoint=100;
			temperFilter=getFilterTemper(temperRaw);
			printf("%f\n",temperFilter);
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
					runstatus=4;
					
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
						printf("Kp:%f,Ki:%f,Kd%f\n",PID.Kp,PID.Ki,PID.Kd);
					}
					else
					{
						//autotune failed
					}
					SetPwmValue(0);
					runstatus=0;
					while(1)
					{
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
			}
		}
		#endif
		if(getMsCounter() - timer_tick_count > 1000)
		{
			timer_tick_count = getMsCounter();
			ReadRtcTime();		
			start_endtime_set();							//��ʼ����ʱ������
			temp_detection(temperFilter);					//�¶Ȳ���
			if(press_flag)
			{
				get_combo_button_times();					//������ť��ת���溯��
				touchtimes.self_check_times = 0;
				touchtimes.menu_click_times = 0;
				press_flag = 0;
			}
				
			temp_curve_save();								//�¶����ߴ洢	
			Check_All_Status();			
		}
			device_timing_selfcheck();
    }
}





//ϵͳ��ʼ��
void System_Init(void)
{
	delay_init();									//��ʱ������ʼ��
	Gpio_Init();									//GPIO��ʼ������
	USART1_Config();								//����1����		
	UartInit();										//��ʾ����������
	screenlanguage();								//��Ļ����ѡ��
	delay_s(1);
	DeviceInfo_Init();								//�豸��ʼ��

	TIMx_Configuration();							//pwmʱ������
	TIMx_Init();									//��ʱtim
	
	Analog_Init();									//adc��ʼ������
	DAC1_Init();									//DAC��ʼ������
	Dac_Select_Init();
	Max6675_Gpio_Init();							//�ȵ�żGPIO��ʼ��	 
	control_mode_select();							//���ſ���ѡ��
	screen_init();									//��Ļ���ݳ�ʼ��
	
}











#if 1






/**
* @brief ����Ĵ���������������Ĵ����ɶ���������д��
* @param pucRegBuffer ��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* @retval eStatus �Ĵ���״̬
*/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
   	int16_t iRegIndex;
	/*��ѯ�Ƿ��ڼĴ�����Χ��.Ϊ�˱��⾯�棬�޸�Ϊ�з�������*/
	if( ( (int16_t)usAddress >= REG_INPUT_START ) \
	&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
	{
		/*��ò���ƫ���������β�����ʼ��ַ-����Ĵ����ĳ�ʼ��ַ*/
		iRegIndex = ( int16_t )( usAddress - REG_INPUT_START );
		
		while( usNRegs > 0 )			//�����ֵ
		{
			
			*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );			//��ֵ���ֽ�
			
			*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );		//��ֵ���ֽ�
			
			iRegIndex++;		//ƫ��������
			
			usNRegs--;			//�������Ĵ��������ݼ�
		}
	}
	else
	{
		eStatus = MB_ENOREG;		//���ش���״̬���޼Ĵ��� 
	}
    return eStatus;
}




/**
* @brief ���ּĴ��������������ּĴ����ɶ����ɶ���д
* @param pucRegBuffer ������ʱ--��������ָ�룬д����ʱ--��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* eMode ������ʽ��������д
* @retval eStatus �Ĵ���״̬
*/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
	eMBErrorCode eStatus = MB_ENOERR;			//����״̬	
	int16_t iRegIndex;							//ƫ����
	/*�жϼĴ����ǲ����ڷ�Χ��*/
	if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
	&& ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
	{	
		iRegIndex = ( int16_t )( usAddress - REG_HOLDING_START);   //����ƫ����

		switch ( eMode )
		{	
			case MB_REG_READ:			//�������� 
				while( usNRegs > 0 )
				{
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;
				}
			break;
			case MB_REG_WRITE:			//д������ 
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
		eStatus = MB_ENOREG;			//���ش���״̬
	}
    return eStatus;
}



/**
* @brief ��Ȧ�Ĵ�������������Ȧ�Ĵ����ɶ����ɶ���д
* @param pucRegBuffer ������---��������ָ�룬д����--��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* eMode ������ʽ��������д
* @retval eStatus �Ĵ���״̬
*/

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{

	eMBErrorCode eStatus = MB_ENOERR;		    	//����״̬

	int16_t iNCoils = ( int16_t )usNCoils;			//�Ĵ�������
	
	int16_t usBitOffset;							//�Ĵ���ƫ����

	/*���Ĵ����Ƿ���ָ����Χ��*/
	if( ( (int16_t)usAddress >= REG_COILS_START ) &&
	( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
	{
		/*����Ĵ���ƫ����*/
		usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
		switch ( eMode )
		{
			case MB_REG_READ:								//������
				while( iNCoils > 0 )
				{
					*pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
					( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
					iNCoils -= 8;
					usBitOffset += 8;
				}
			break;

	
			case MB_REG_WRITE:								//д����
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




/**
* @brief ��������Ĵ�������������������Ĵ������ɶ�
* @param pucRegBuffer ������---��������ָ�룬д����--��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* eMode ������ʽ��������д
* @retval eStatus �Ĵ���״̬
*/

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
   
	eMBErrorCode eStatus = MB_ENOERR;		 			//����״̬
	
	int16_t iNDiscrete = ( int16_t )usNDiscrete;		//�����Ĵ�������
	
	uint16_t usBitOffset;								//ƫ����

	/*�жϼĴ���ʱ�����ƶ���Χ��*/
	if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
	( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
	{
		/*���ƫ����*/
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







