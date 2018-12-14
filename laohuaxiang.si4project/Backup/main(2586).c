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
volatile uint32_t  timer_tick_count = 0; //定时器节拍
uint32_t countnum2 = 0,start = 0,end = 0,starttime,endtime;
extern uint16_t ADC_ConvertedValue[ADC_NOFCHANEL];
//extern BIG_SCREEN_ID_TAB biglanguage_screen;
//extern BIG_SCREEN_ID_TAB bigchinese_screen;
//extern BIG_SCREEN_ID_TAB bigenglish_screen;
 
extern RtcTime rtctime;
extern uint8_t cmd_buffer[CMD_MAX_SIZE];		//指令缓存
extern uint8_t press_flag;
extern MainShowTextValue	showtextvalue;	//主页面文本控件缓存值

/* ----------------------- Defines ------------------------------------------*/

//输入寄存器内容
uint16_t    usRegInputBuf[REG_INPUT_NREGS];


//保持寄存器内容
uint16_t	usRegHoldingBuf[REG_HOLDING_NREGS] = {0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e};


//线圈寄存器内容
uint16_t	ucRegCoilsBuf[REG_COILS_SIZE] = {0};


//开关输入寄存器内容
uint16_t	ucRegDiscreteBuf[REG_DISCRETE_SIZE] = {0};

/* --------------------------------------------------------------------------*/


extern  uint32_t password1;
extern uint32_t password2;
extern float adjusttemp;
extern Touch_Times touchtimes;

extern MainShowTextValue showtextvalue;	//主页面文本控件缓存值




/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
    eMBErrorCode    eStatus;
	qsize  size = 0;
	System_Init();												//系统初始化设置

	eStatus = eMBInit( MB_RTU, 0x01, 0x01, 9600, MB_PAR_NONE ); //初始化Modbus
	eStatus = eMBEnable();									//启动Free MODBUS
	delay_s(1);
	SetBackLight(20);											//初始屏幕背光亮度
	
	startscreen();												//start screen
	

	while(1)
    {
			
        eMBPoll(  );											//查询数据帧 
        size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); 		//从缓冲区中获取一条指令        
		if(size>0)												//接收到指令
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
		}

		if(getMsCounter() - timer_tick_count > 1500)
		{
			timer_tick_count = getMsCounter();
			ReadRtcTime();		
			start_endtime_set();							//起始结束时间设置
			temp_detection();								//温度测量，2.5ms
			if(press_flag)
			{
				get_combo_button_times();					//连击按钮跳转界面函数
				touchtimes.self_check_times = 0;
				touchtimes.menu_click_times = 0;
				press_flag = 0;
			}
				
			temp_curve_save();								//温度曲线存储	
			Check_All_Status();			
		}
			device_timing_selfcheck();
    }
}





//系统初始化
void System_Init(void)
{
	delay_init();									//延时函数初始化
	Gpio_Init();									//GPIO初始化配置
	USART1_Config();								//串口1配置		
	UartInit();										//显示屏串口配置
	screenlanguage();								//屏幕语言选择
	delay_s(1);
	DeviceInfo_Init();								//设备初始化

	TIMx_Configuration();							//pwm时钟配置
	TIMx_Init();									//计时tim
	
	Analog_Init();									//adc初始化配置
	DAC1_Init();									//DAC初始化配置
	Max6675_Gpio_Init();							//热电偶GPIO初始化	 
	control_mode_select();							//阀门控制选择
	screen_init();									//屏幕内容初始化
	
}











#if 1






/**
* @brief 输入寄存器处理函数，输入寄存器可读，但不可写。
* @param pucRegBuffer 返回数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* @retval eStatus 寄存器状态
*/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
   	int16_t iRegIndex;

	/*查询是否在寄存器范围内.为了避免警告，修改为有符号整数*/
	if( ( (int16_t)usAddress >= REG_INPUT_START ) \
	&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
	{
		/*获得操作偏移量，本次操作起始地址-输入寄存器的初始地址*/
		iRegIndex = ( int16_t )( usAddress - REG_INPUT_START );
		
		while( usNRegs > 0 )			//逐个赋值
		{
			
			*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );			//赋值高字节
			
			*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );		//赋值低字节
			
			iRegIndex++;		//偏移量增加
			
			usNRegs--;			//被操作寄存器数量递减
		}
	}
	else
	{
		eStatus = MB_ENOREG;		//返回错误状态，无寄存器 
	}
    return eStatus;
}




/**
* @brief 保持寄存器处理函数，保持寄存器可读，可读可写
* @param pucRegBuffer 读操作时--返回数据指针，写操作时--输入数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* eMode 操作方式，读或者写
* @retval eStatus 寄存器状态
*/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
	eMBErrorCode eStatus = MB_ENOERR;			//错误状态	
	int16_t iRegIndex;							//偏移量
	/*判断寄存器是不是在范围内*/
	if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
	&& ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
	{	
		iRegIndex = ( int16_t )( usAddress - REG_HOLDING_START);   //计算偏移量

		switch ( eMode )
		{	
			case MB_REG_READ:			//读处理函数 
				while( usNRegs > 0 )
				{
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;
				}
			break;
			case MB_REG_WRITE:			//写处理函数 
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
		eStatus = MB_ENOREG;			//返回错误状态
	}
    return eStatus;
}



/**
* @brief 线圈寄存器处理函数，线圈寄存器可读，可读可写
* @param pucRegBuffer 读操作---返回数据指针，写操作--返回数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* eMode 操作方式，读或者写
* @retval eStatus 寄存器状态
*/

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{

	eMBErrorCode eStatus = MB_ENOERR;		    	//错误状态

	int16_t iNCoils = ( int16_t )usNCoils;			//寄存器个数
	
	int16_t usBitOffset;							//寄存器偏移量

	/*检查寄存器是否在指定范围内*/
	if( ( (int16_t)usAddress >= REG_COILS_START ) &&
	( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
	{
		/*计算寄存器偏移量*/
		usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
		switch ( eMode )
		{
			case MB_REG_READ:								//读操作
				while( iNCoils > 0 )
				{
					*pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
					( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
					iNCoils -= 8;
					usBitOffset += 8;
				}
			break;

	
			case MB_REG_WRITE:								//写操作
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
* @brief 开关输入寄存器处理函数，开关输入寄存器，可读
* @param pucRegBuffer 读操作---返回数据指针，写操作--返回数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* eMode 操作方式，读或者写
* @retval eStatus 寄存器状态
*/

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
   
	eMBErrorCode eStatus = MB_ENOERR;		 			//错误状态
	
	int16_t iNDiscrete = ( int16_t )usNDiscrete;		//操作寄存器个数
	
	uint16_t usBitOffset;								//偏移量

	/*判断寄存器时候再制定范围内*/
	if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
	( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
	{
		/*获得偏移量*/
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







