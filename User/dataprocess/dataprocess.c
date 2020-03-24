#include "dataprocess.h"
#include "stdlib.h"
#include "./gpio/gpio.h"
#include "main.h"
#include "./status/status.h"
#include "hmi_driver.h"
#include "./screen/screen.h"
#include "./flash/deviceinfo.h"

extern uint16_t     usRegInputBuf[REG_INPUT_NREGS];				//04
extern uint16_t		usRegHoldingBuf[REG_HOLDING_NREGS];			//03,06,16
extern uint16_t		ucRegCoilsBuf[REG_COILS_SIZE];				//01,05,15
extern uint16_t		ucRegDiscreteBuf[REG_DISCRETE_SIZE];		//02

extern dev_info_t dev_info;							
extern uint8_t cmd_buffer[CMD_MAX_SIZE];							
extern BIG_SCREEN_ID_TAB biglanguage_screen;;						
CoilSaveValue  coilsavevalue;										
extern struct IOStatusStruct IOStatus;

extern ThermalLag heattime_log;									
extern RtcTime changetime;											

PCTRL_MSG tempmessage;

uint8_t writecoilflag = 0,readcoilflag = 0,readholdingflag = 0,writeholdingflag = 0,readinputflag = 0;

uint8_t temp[COILNUM] = {0};



#define READ_GPIO_VALUE_TABLE(_i, _port, _pin)   do { temp[(_i)] = GPIO_ReadOutputDataBit((_port),(_pin)); } while(0)





//0为ON,1为OFF,read coil register
void  read_Coilregister(void)
{
	
	READ_GPIO_VALUE_TABLE(0,DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);					
	READ_GPIO_VALUE_TABLE(1,DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);			
	READ_GPIO_VALUE_TABLE(2,DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);		
	READ_GPIO_VALUE_TABLE(3,DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);				
	READ_GPIO_VALUE_TABLE(4,DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);				
	READ_GPIO_VALUE_TABLE(5,BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);					
	
	ucRegCoilsBuf[0] = temp[0]|temp[1]<<1|temp[2]<<2|temp[3]<<3|temp[4]<<4|temp[5]<<5;
	readcoilflag = 0;
}

//write coil register
void  write_Coilregister(void)
{
	if(ucRegCoilsBuf[0] & 0x01)
	{
		GPIO_WriteBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN,Bit_SET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,SHOW);
	}
	else
	{
		GPIO_WriteBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN,Bit_RESET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,HIDE);
	}
	if(ucRegCoilsBuf[0] & 0x02)
	{	
		GPIO_WriteBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN,Bit_SET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,HIDE);
	}
	else
	{
		GPIO_WriteBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN,Bit_RESET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,HIDE);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,SHOW);
	}
	
	if(ucRegCoilsBuf[0] & 0x04)
	{	
		IOStatus.blower=1;
//		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,SHOW);
//		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,HIDE);
//		GPIO_WriteBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN,Bit_SET);
	}
	else
	{
		IOStatus.blower=0;
//		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,HIDE);
//		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,SHOW);
//		GPIO_WriteBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN,Bit_RESET);
	}
	
	if(ucRegCoilsBuf[0] & 0x08)
	{	
		GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN,Bit_SET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING1_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR1_WORK_STATUS_ID,HIDE);
	}
	else
	{
		GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN,Bit_RESET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING1_ID,HIDE);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR1_WORK_STATUS_ID,SHOW);
	}
	if(ucRegCoilsBuf[0] & 0x10)
	{	
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING2_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR2_WORK_STATUS_ID,HIDE);
		GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN,Bit_SET);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING2_ID,HIDE);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR2_WORK_STATUS_ID,SHOW);
		GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN,Bit_RESET);
	}
	if(ucRegCoilsBuf[0] & 0x20)
	{	
		GPIO_WriteBit(DRIVER_GPIO_PORT,BOX_DOOR_PIN,Bit_SET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,SHOW);
	}
	else
	{
		GPIO_WriteBit(DRIVER_GPIO_PORT,BOX_DOOR_PIN,Bit_RESET);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,HIDE);
	}
	writecoilflag = 0;
}


//read input register
void read_Inputregister(void)
{
	
}

extern uint8_t thermalbuff[38];
extern uint8_t savethermalbuff[TIMERECORDNUM][38];

//read Holding register
void read_Holdingregister(void)
{
	uint8_t i = 0;
	usRegHoldingBuf[0] = dev_info.currentTemp*10;									//0x100
	usRegHoldingBuf[1] = dev_info.setTemp*10;
	usRegHoldingBuf[2] = dev_info.testTime*10;
	usRegHoldingBuf[3] = dev_info.flash_setvalue.warning1_up*10;
	usRegHoldingBuf[4] = dev_info.flash_setvalue.warning2_up*10;
	usRegHoldingBuf[5] = dev_info.flash_setvalue.temp_backdiff*10;
	usRegHoldingBuf[6]=IOStatus.blower;
	usRegHoldingBuf[7]=IOStatus.door_open;
	usRegHoldingBuf[8]=IOStatus.heat_output;
	usRegHoldingBuf[9]=IOStatus.heat_switch;
	usRegHoldingBuf[10]=IOStatus.sample_frame;
	usRegHoldingBuf[11]=IOStatus.temp_warnning1;
	usRegHoldingBuf[12]=IOStatus.temp_warnning2;
	usRegHoldingBuf[13]=dev_info.runstatus;
//	usRegHoldingBuf[6] = atoi(dev_info.flash_setvalue.menu_password)>>16;
//	usRegHoldingBuf[7] = atoi(dev_info.flash_setvalue.menu_password)&0x0000ffff;
//	usRegHoldingBuf[8] = atoi(dev_info.flash_setvalue.secondtime_password)>>16;
//	usRegHoldingBuf[9] = atoi(dev_info.flash_setvalue.secondtime_password)&0x0000ffff;   //不赞成用modbus改密码
//	usRegHoldingBuf[10] = atoi(dev_info.flash_setvalue.protect_password)>>16;
//	usRegHoldingBuf[11] = atoi(dev_info.flash_setvalue.protect_password)&0x0000ffff;
//	usRegHoldingBuf[12] = atoi(dev_info.flash_setvalue.protect_secondtime_password)>>16;
//	usRegHoldingBuf[13] = atoi(dev_info.flash_setvalue.protect_secondtime_password)&0x0000ffff;
//	usRegHoldingBuf[6] = heattime_log.heattime.Year<<8|heattime_log.heattime.Mon;
//	usRegHoldingBuf[7] = heattime_log.heattime.Day<<8|heattime_log.heattime.Hour;
//	usRegHoldingBuf[8] = heattime_log.heattime.Min<<8|heattime_log.heattime.Sec;
	for(i = 0;i < TIMERECORDNUM;i++)
	{
		memcpy(usRegHoldingBuf[19*i+14],savethermalbuff[i],sizeof(char)*38);
	}
	
	readholdingflag = 0;
}


//write Holding register
void  write_Holdingregister(void)
{
	if (!dev_info.runstatus)
	{
		dev_info.setTemp = (float)usRegHoldingBuf[512]/10;            										//0x300
	}
		dev_info.testTime = (float)usRegHoldingBuf[513]/10;		
		dev_info.flash_setvalue.warning1_up = (float)usRegHoldingBuf[514]/10;
		dev_info.flash_setvalue.warning2_up = (float)usRegHoldingBuf[515]/10;
		dev_info.flash_setvalue.temp_backdiff = (float)usRegHoldingBuf[516]/10;	
	IOStatus.blower=	usRegHoldingBuf[517];
	IOStatus.sample_frame=usRegHoldingBuf[518];
	
//		my_itoa((usRegHoldingBuf[517]<<16)|usRegHoldingBuf[518],dev_info.flash_setvalue.menu_password,PASSWORDLENGTH);
//		my_itoa((usRegHoldingBuf[519]<<16)|usRegHoldingBuf[520],dev_info.flash_setvalue.secondtime_password,PASSWORDLENGTH);
//		my_itoa((usRegHoldingBuf[521]<<16)|usRegHoldingBuf[522],dev_info.flash_setvalue.protect_password,PASSWORDLENGTH);
//		my_itoa((usRegHoldingBuf[523]<<16)|usRegHoldingBuf[524],dev_info.flash_setvalue.protect_secondtime_password,PASSWORDLENGTH);
		dev_info.runstatus=(int)usRegHoldingBuf[519];
	dev_info.dev_status_changed_flag=1;
		writeholdingflag = 0;
	
}

void read_Discreteregister(void)
{


}

void modbus_register_init(void)
{
	uint8_t i = 0;
	usRegHoldingBuf[0] = dev_info.setTemp * 10;
	usRegHoldingBuf[1] = dev_info.testTime * 100;
	usRegHoldingBuf[2] = dev_info.flash_setvalue.warning1_up*10;
	usRegHoldingBuf[3] = dev_info.flash_setvalue.warning2_up*10;
	usRegHoldingBuf[4] = dev_info.flash_setvalue.temp_backdiff*10;

//	usRegHoldingBuf[6] = atoi(dev_info.flash_setvalue.menu_password)>>16;
//	usRegHoldingBuf[7] = atoi(dev_info.flash_setvalue.menu_password)&0xffff;
//	usRegHoldingBuf[8] = atoi(dev_info.flash_setvalue.secondtime_password)>>16;
//	usRegHoldingBuf[9] = atoi(dev_info.flash_setvalue.secondtime_password)&0xffff;
//	usRegHoldingBuf[10] = atoi(dev_info.flash_setvalue.protect_password)>>16;
//	usRegHoldingBuf[11] = atoi(dev_info.flash_setvalue.protect_password)&0xffff;
//	usRegHoldingBuf[12] = atoi(dev_info.flash_setvalue.protect_secondtime_password)>>16;
//	usRegHoldingBuf[13] = atoi(dev_info.flash_setvalue.protect_secondtime_password)&0xffff;
	for(i = 0;i < TIMERECORDNUM;i++)
	{
		memcpy(usRegHoldingBuf[19*i+14],savethermalbuff[i],sizeof(char)*38);
	}
	usRegHoldingBuf[205] = heattime_log.heattime.Year<<8|heattime_log.heattime.Mon;
	usRegHoldingBuf[206] = heattime_log.heattime.Day<<8|heattime_log.heattime.Hour;
	usRegHoldingBuf[207] = heattime_log.heattime.Min<<8|heattime_log.heattime.Sec;

	usRegHoldingBuf[512] = dev_info.setTemp * 10;
	usRegHoldingBuf[513] = dev_info.testTime * 100;
	usRegHoldingBuf[514] = dev_info.flash_setvalue.warning1_up*10;
	usRegHoldingBuf[515] = dev_info.flash_setvalue.warning2_up*10;
	usRegHoldingBuf[516] = dev_info.flash_setvalue.temp_backdiff*10;

//	usRegHoldingBuf[517] = atoi(dev_info.flash_setvalue.menu_password)>>16;
//	usRegHoldingBuf[518] = atoi(dev_info.flash_setvalue.menu_password)&0xffff;
//	usRegHoldingBuf[519] = atoi(dev_info.flash_setvalue.secondtime_password)>>16;
//	usRegHoldingBuf[520] = atoi(dev_info.flash_setvalue.secondtime_password)&0xffff;
//	usRegHoldingBuf[521] = atoi(dev_info.flash_setvalue.protect_password)>>16;
//	usRegHoldingBuf[522] = atoi(dev_info.flash_setvalue.protect_password)&0xffff;
//	usRegHoldingBuf[523] = atoi(dev_info.flash_setvalue.protect_secondtime_password)>>16;
//	usRegHoldingBuf[524] = atoi(dev_info.flash_setvalue.protect_secondtime_password)&0xffff;
	for(i = 0;i < TIMERECORDNUM;i++)
	{
		memcpy(usRegHoldingBuf[19*i+525],savethermalbuff[i],sizeof(char)*38);
	}
	usRegHoldingBuf[716] = heattime_log.heattime.Year<<8|heattime_log.heattime.Mon;
	usRegHoldingBuf[717] = heattime_log.heattime.Day<<8|heattime_log.heattime.Hour;
	usRegHoldingBuf[718] = heattime_log.heattime.Min<<8|heattime_log.heattime.Sec; 
	
}


























