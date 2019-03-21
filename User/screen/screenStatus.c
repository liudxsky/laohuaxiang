#include "./screen/screenStatus.h"
#include "./flash/deviceinfo.h"
#include "./gpio/gpio.h"
#include "./dac/dac.h"
#include "./screen/screen.h"
#include "./SCREEN_DRIVE/hmi_driver.h"
#include "./status/status.h"
extern dev_info_t dev_info;
extern BIG_SCREEN_ID_TAB biglanguage_screen;
struct mainTextStruct mainPageText;
struct argSetErrorStruct argSetErrorIcon;
struct IOStatusStruct IOStatus;
struct mainIconStruct mainIcon;
struct menuPageStruct menuPage;
uint16_t currentScreen;
void check_heat_switch(void)
{
	if(dev_info.runstatus>0)
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,HIDE);
	}
	
}
//start screen 
void startscreen(void)
{
	
	uint8_t strtemp[COMMONSIZE];
	sprintf(strtemp,"Ver:1.0");
	printf("%s\r\n",strtemp);				
  memcpy(mainPageText.softversion,strtemp,COMMONSIZE);
	MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);							
}
//check pid's status
void check_pidstatus(void)
{
	if(dev_info.runstatus==3)
	{		
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,HIDE);
		
	}
}

void updater_mainScreen()
{
	updater_mainIcon();
	updater_mainText();
}
void updater_mainText()
{
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SOFT_VER_ID,mainPageText.softversion);	
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_ID,mainPageText.setTempHi);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_DECIMAL_ID,mainPageText.setTempLo);
	SetTextValueFloat(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEST_TIME_ID,mainPageText.test_time);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CHANGE_AIR_TIME,mainPageText.change_air_time);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,mainPageText.open_angle);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_ADDUP_TIME_ID,mainPageText.addup_testtime);
	Dac1_Set_Vol(33*dev_info.airdooropenangle);
	
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_HOUR_ID,mainPageText.left_time_hour);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_MIN_ID,mainPageText.left_time_min);
}
void updater_IOStatus(void)
{
	CIRCULATING(IOStatus.blower);
	SPINNER_RACK(IOStatus.sample_frame);
	HEAT_SWITCH(IOStatus.heat_switch);
	ALARM1(IOStatus.alarm1);
	ALARM2(IOStatus.alarm2);
	memcpy(&mainIcon.IO,&IOStatus,sizeof(struct IOStatusStruct));
}
void dev2mainScreen()
{
	const static uint8_t open_angle[CHANGE_AIR_SIZE] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
	mainPageText.setTempHi=dev_info.setTemp/1;
	mainPageText.setTempLo=(int32_t)(dev_info.setTemp*10)%10;
	mainPageText.currentTempHi=dev_info.currentTemp/1;
	mainPageText.currentTempLo=(int32_t)(dev_info.currentTemp*10)%10;
	mainPageText.test_time=dev_info.testTime;
	mainPageText.addup_testtime=dev_info.addup_testtime/60;
	mainPageText.open_angle=open_angle[dev_info.airdooropenangle];
	mainPageText.change_air_time=dev_info.flash_setvalue.change_air_time;
	
}
void dev2IOstatus()
{
	if(dev_info.pwmvalue>0)
	{
		IOStatus.heat_output=1;
	}
	else
	{
		IOStatus.heat_output=0;
	}
	
	if(dev_info.runstatus>0&&dev_info.temp_warnning1==0)
	{
		IOStatus.heat_switch=1;
	}
	else
	{
		IOStatus.heat_switch=0;
	}
}
void dev2modBus()
{
	
}

void update_dev_status(void)//back compatility
{
	//push dev_info into screen
	dev2mainScreen();
	dev2IOstatus();
	dev2modBus();
	updater_mainText();
	//push dev_info into IO
	updater_IOStatus();
	
}
void updater_mainIcon()
{
	//SHOW =0, HIDE=1, inverse of IOstatus
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,!mainIcon.IO.sample_frame);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,mainIcon.IO.sample_frame);
	
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,!mainIcon.IO.blower);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,mainIcon.IO.blower);
	
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING1_ID,!mainIcon.IO.alarm1);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR1_WORK_STATUS_ID,mainIcon.IO.alarm1);
	
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING2_ID,!mainIcon.IO.alarm2);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR2_WORK_STATUS_ID,mainIcon.IO.alarm2);
	
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,!mainIcon.IO.heat_output);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,!mainIcon.IO.heat_switch);
	
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,!mainIcon.IO.door_open);
	if(dev_info.runstatus==3)
	{		
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,HIDE);
		
	}
	//BIG_AIR_DOOR_OPEN_ANGLE_ID???
	Check_Rs485();//status.c
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TROUBLE_INDICATE_ID,!mainIcon.trouble_indicate);

}
void updater_editAirDoor(uint16_t screen_id)
{
	int i;
	for (i= 0; i < CHANGE_AIR_SIZE; ++i)
		{
			
			SetTextValueInt32(biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN,BIG_CHANGE_AIR_TIME_SET_0+i,dev_info.change_air_time[i]);
		}
}
void updater_PIDScreen(uint16_t screen_id)
{
	screen_id=biglanguage_screen.BIG_PID_SET_SCREEN;
	if(dev_info.runstatus>0)
				return;
		SetTextValueFloat(screen_id,BIG_P_VALUE_SET,dev_info.pidvalue.PID_P);
		SetTextValueFloat(screen_id,BIG_I_VALUE_SET,dev_info.pidvalue.PID_I*10000);
		SetTextValueFloat(screen_id,BIG_D_VALUE_SET,dev_info.pidvalue.PID_D);	
}
void updater_menuParam(uint16_t screen_id)
{
		SetTextValueFloat(screen_id,BIG_TEST_TIME_VALUE,dev_info.testTime);
		
		SetTextValueFloat(screen_id,BIG_TEST_TEMP_VALUE,dev_info.setTemp);
	
		SetTextValueFloat(screen_id,BIG_WARNING1_UP_VALUE,dev_info.flash_setvalue.warning1_up);
	
		SetTextValueFloat(screen_id,BIG_WARNING2_UP_VALUE,dev_info.flash_setvalue.warning2_up);
		
		SetTextValueFloat(screen_id,BIG_TEMP_RETURN_DIFF,dev_info.temp_backdiff);
		
		SetTextValue(screen_id,BIG_NEW_PASSWORD,"");
		
		SetTextValue(screen_id,BIG_SECOND_INPUT_PASSWORD,"");
		
		SetTextValueInt32(screen_id,BIG_CHANGE_AIR_TIME_SET,dev_info.flash_setvalue.change_air_time);
	
		SetTextValueInt32(screen_id,BIG_CHANGE_AIR_MAX_SET,dev_info.flash_setvalue.change_max_time);
	
		SetTextValueInt32(screen_id,BIG_MODBUS_NODE_ADDRESS,dev_info.Modbus_address);
		
		SetTextValueInt32(screen_id,BIG_SCREEN_BRIGHT_ADJUST,dev_info.flash_setvalue.screen_light_value);
		
		switch (dev_info.modbus_tran_rate_flag)
		{
			case 0:
				AnimationPlayFrame(screen_id,BIG_BPS_1200,SHOW);
				AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
				break;
			case 1:
				AnimationPlayFrame(screen_id,BIG_BPS_2400,SHOW);
				AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
				break;
			case 2:
				AnimationPlayFrame(screen_id,BIG_BPS_4800,SHOW);
				AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
				break;
			case 3:
				AnimationPlayFrame(screen_id,BIG_BPS_9600,SHOW);
				AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
				AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
				break;
		}
		if (dev_info.biglanguagestatus)
		{
			AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,HIDE);
			AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,SHOW);	
		}
		else
		{
			AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,SHOW);
			AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,HIDE);	
		}
}
void updater_adjScreen(uint16_t screen_id)
{
	SetTextValue(screen_id,BIG_NOPOWER_PASSWORD1,"");
		SetTextValue(screen_id,BIG_NOPOWER_PASSWORD2,"");
		SetTextValueInt32(screen_id,BIG_AIR_DOOR_ANGLE_SET,dev_info.flash_setvalue.air_door_angle);
		SetTextValueFloat(screen_id,BIG_TEMP_VALUE_REVUSE_SET,dev_info.flash_adjusttemp);
		SetTextValueInt32(screen_id,BIG_YEAR_SET,dev_info.autonopowertime.year);
		SetTextValueInt32(screen_id,BIG_MONTH_SET,dev_info.autonopowertime.month);
		SetTextValueInt32(screen_id,BIG_DAY_SET,dev_info.autonopowertime.day);
}
void setCurrentScreen(uint16_t screen_id)
{
	currentScreen=screen_id;
	MySetScreen(currentScreen);	
}
void updater_setCurrentScreen(uint16_t screen_id)
{
	//check current screen
	//if not, set
	currentScreen=screen_id;
	MySetScreen(currentScreen);	
}
void updater_menuPageUpdater(uint16_t screen_id)
{
	SetTextValueInt32(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_CHANGE_AIR_MAX_SET,dev_info.flash_setvalue.change_max_time);
}
void updater_argSetErrorScreen()
{
	
}