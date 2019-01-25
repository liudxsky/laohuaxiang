#include "dataprocess.h"
#include "stdlib.h"
#include "./gpio/gpio.h"
#include "main.h"
#include "./status/status.h"
#include "hmi_driver.h"
#include "./screen/screen.h"
#include "./flash/deviceinfo.h"









extern uint16_t     usRegInputBuf[REG_INPUT_NREGS];				//输入寄存器      操作码：04
extern uint16_t		usRegHoldingBuf[REG_HOLDING_NREGS];			//保持寄存器	操作码：03,06,16
extern uint16_t		ucRegCoilsBuf[REG_COILS_SIZE];					//线圈寄存器	操作码：01,05,15
extern uint16_t		ucRegDiscreteBuf[REG_DISCRETE_SIZE];		//开关输入寄存器	操作码：02

extern dev_info_t dev_info;
extern MainShowTextValue showtextvalue;									//主页面文本控件缓存值
extern uint8_t cmd_buffer[CMD_MAX_SIZE];								//指令缓存
extern BIG_SCREEN_ID_TAB biglanguage_screen;;							//界面语言选择
CoilSaveValue  coilsavevalue;											//保持寄存器存储值



extern ThermalLag heattime_log;										//热滞后时间记录结构体
extern RtcTime changetime;												//状态改变时间点

PCTRL_MSG tempmessage;



uint8_t temp[COILNUM] = {0};



#define READ_GPIO_VALUE_TABLE(_i, _port, _pin)   do { temp[(_i)] = GPIO_ReadOutputDataBit((_port),(_pin)); } while(0)





//读线圈状态,0为ON,1为OFF
void  read_coil(void)
{
	
	READ_GPIO_VALUE_TABLE(0,DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);					//加热
	READ_GPIO_VALUE_TABLE(1,DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);			//旋转架
	READ_GPIO_VALUE_TABLE(2,DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);		//风机
	READ_GPIO_VALUE_TABLE(3,DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);				//温度报警1
	READ_GPIO_VALUE_TABLE(4,DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);				//温度报警2
	READ_GPIO_VALUE_TABLE(5,BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);					//箱门
	
	ucRegCoilsBuf[0] = temp[0]|temp[1]<<1|temp[2]<<2|temp[3]<<3|temp[4]<<4|temp[5]<<5;
}


//写线圈状态
void  write_coil(void)
{
	GPIO_WriteBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN,ucRegCoilsBuf[0] & 0x01);
	GPIO_WriteBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN,ucRegCoilsBuf[0] & 0x02);
	GPIO_WriteBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN,ucRegCoilsBuf[0] & 0x04);
	GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN,ucRegCoilsBuf[0] & 0x08);
	GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN,ucRegCoilsBuf[0] & 0x10);
	GPIO_WriteBit(DRIVER_GPIO_PORT,BOX_DOOR_PIN,ucRegCoilsBuf[0] & 0x20);
}

extern uint8_t thermalbuff[38];

//读输入寄存器
void read_input_register(void)
{
	uint8_t i = 0;
	uint32_t testtemp = showtextvalue.current_temp_vlaue*100;
	//即时温度
	usRegInputBuf[0] = testtemp;
	usRegInputBuf[1] = testtemp>>16;
//	printf("temp is %d,%d\r\n",usRegInputBuf[0],usRegInputBuf[1]);
	
//	for(i = 0;i < TIMERECORDNUM;i++)
//	{
//		usRegInputBuf[38*i] = thermalbuff[i];
//	}
	
//	usRegInputBuf[381] = heattime_log.heattime.Year<<8|heattime_log.heattime.Mon;
//	usRegInputBuf[382] = heattime_log.heattime.Day<<8|heattime_log.heattime.Hour;
//	usRegInputBuf[383] = heattime_log.heattime.Min<<8|heattime_log.heattime.Sec;
}




//读保持寄存器值
void read_coilregister(void)
{
	usRegHoldingBuf[0] = dev_info.testtime*100;
	usRegHoldingBuf[1] = dev_info.testtemp*100;
//	usRegHoldingBuf[2] = coilsavevalue.test_temp;
//	usRegHoldingBuf[3] = coilsavevalue.warning1_up;
//	usRegHoldingBuf[4] = coilsavevalue.warning1_down;
//	usRegHoldingBuf[5] = coilsavevalue.warning2_up;
//	usRegHoldingBuf[6] = coilsavevalue.warning2_down;
//	usRegHoldingBuf[8] = coilsavevalue.menu_password&0x0000ffff;
//	usRegHoldingBuf[9] = coilsavevalue.menu_password>>16;
//	usRegHoldingBuf[10] = coilsavevalue.secondtime_password&0x0000ffff;
//	usRegHoldingBuf[11] = coilsavevalue.secondtime_password>>16;
//	usRegHoldingBuf[12] = coilsavevalue.change_air_time;
//	usRegHoldingBuf[13] = coilsavevalue.menu_language;
}





//写入保持寄存器值
void  writer_register(void)
{
	dev_info.testtime = (float)usRegHoldingBuf[0]/100;
	dev_info.testtemp = (float)usRegHoldingBuf[1]/100;
	update_dev_status();			
}




//读取GPIO状态值
void readgpiostatus(void)
{
	
	READ_GPIO_VALUE_TABLE(0,DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	READ_GPIO_VALUE_TABLE(1,DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	READ_GPIO_VALUE_TABLE(2,DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	READ_GPIO_VALUE_TABLE(3,DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	READ_GPIO_VALUE_TABLE(4,DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	READ_GPIO_VALUE_TABLE(5,BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);

//	if(temp[1] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,SAMPLE_FRAME_MOTOR_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,RR_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,SAMPLE_FRAME_MOTOR_ID,HIDE); 
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,RR_WORK_STATUS_ID,SHOW);	
//	}

//	if(temp[2] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FAN_OPERATION_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FR_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FAN_OPERATION_ID,HIDE);	
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FR_WORK_STATUS_ID,SHOW);	
//	}
//	
//	if(temp[3] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING1_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR1_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR1_WORK_STATUS_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING1_ID,HIDE);	
//	}
//	if(temp[4] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING2_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR2_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING2_ID,HIDE);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR2_WORK_STATUS_ID,SHOW);	
//	}
//	if(temp[5] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,DOOR_OPEN_ID,SHOW);
//		//记录开门时间，温度
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,DOOR_OPEN_ID,HIDE);
//		//记录关门时间，温度，并存储信息
//	}
}





void modbus_register_handle(void)
{
	read_coil();		//读线圈
	write_coil();		//写线圈
	
	writer_register();

	read_input_register();	//读输入寄存器
	read_coilregister();	//读保持寄存器值
}























