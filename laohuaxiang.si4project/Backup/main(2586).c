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
#include "./rtc/rtc.h"
#include "time.h"
#include <ctime>
#include "./status/status.h"

uint16_t current_screen_id = 0;
volatile uint32_t  timer_tick_count = 0; //��ʱ������
uint32_t countnum2 = 0,start = 0,end = 0,starttime,endtime;
extern uint16_t ADC_ConvertedValue[ADC_NOFCHANEL];
//extern BIG_SCREEN_ID_TAB biglanguage_screen;
//extern BIG_SCREEN_ID_TAB bigchinese_screen;
//extern BIG_SCREEN_ID_TAB bigenglish_screen;
 
extern RtcTime rtctime;
extern uint8_t cmd_buffer[CMD_MAX_SIZE];		//ָ���
extern uint8_t press_flag;
extern MainShowTextValue	showtextvalue;	//��ҳ���ı��ؼ�����ֵ

/* ----------------------- Defines ------------------------------------------*/

//����Ĵ�������
uint16_t    usRegInputBuf[REG_INPUT_NREGS];


//���ּĴ�������
uint16_t	usRegHoldingBuf[REG_HOLDING_NREGS] = {0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e};


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




/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
    eMBErrorCode    eStatus;
	qsize  size = 0;
	System_Init();												//ϵͳ��ʼ������

	eStatus = eMBInit( MB_RTU, 0x01, 0x01, 9600, MB_PAR_NONE ); //��ʼ��Modbus
	eStatus = eMBEnable();									//����Free MODBUS
	delay_s(1);
	SetBackLight(20);											//��ʼ��Ļ��������
	
	startscreen();												//start screen
	

	while(1)
    {
			
        eMBPoll(  );											//��ѯ����֡ 
        size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); 		//�ӻ������л�ȡһ��ָ��        
		if(size>0)												//���յ�ָ��
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
		}

		if(getMsCounter() - timer_tick_count > 1500)
		{
			timer_tick_count = getMsCounter();
			ReadRtcTime();		
			start_endtime_set();							//��ʼ����ʱ������
			temp_detection();								//�¶Ȳ�����2.5ms
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







