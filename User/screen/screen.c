#include "./screen/screen.h"
#include "hmi_driver.h"
#include "./usart/usart.h"
#include "./delaytime/delaytime.h"
#include "./gpio/gpio.h"
#include "cmd_queue.h"
#include "./tim/tim.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "./flash/flash.h"
#include "./flash/deviceinfo.h"
#include "./screen/screenFunc.h"
#include "./screen/screenStatus.h"



uint8_t cmd_buffer[CMD_MAX_SIZE];		//screen data buffer
uint8_t change_air_time[CHANGE_AIR_SIZE] = {0};


extern uint16_t current_screen_id;
extern uint32_t _ms_counter; 
extern dev_info_t dev_info;
extern uint8_t pwmgpiostatus;
extern uint16_t gpiostatus;
			

PCTRL_MSG msg;						
TextValueTab  textvalue;			//text control_id buff
//CoilValue coilvalue;//should update via dev_info, its value should be memoried

extern struct mainIconStruct mainIcon;
extern struct mainTextStruct mainPageText;
extern struct argSetErrorStruct argSetErrorIcon;
extern struct IOStatusStruct IOStatus;
BIG_SCREEN_ID_TAB bigchinese_screen = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
BIG_SCREEN_ID_TAB bigenglish_screen = {13,14,15,16,17,18,19,20,21,22,23,24,25};
BIG_SCREEN_ID_TAB biglanguage_screen = {0};



void SetTextValueRTC(uint16_t screen_id, uint16_t control_id,RtcTime datetime)
{
	char buff[17]={0};
	buff[0]='2';
	buff[1]='0';
	sprintf(buff+2,"%02d",datetime.Year); 
	buff[4]='/';
	sprintf(buff+5,"%02d",datetime.Mon);
	buff[7]='/';
	sprintf(buff+7,"%02d",datetime.Day);
	
	buff[10] = ' ';
		sprintf(buff+10,"%02d",datetime.Hour);
	buff[13] = ':';
	sprintf(buff+13,"%02d",datetime.Min);
	SetTextValue(screen_id,control_id,buff);
}
//convert  int to string
void SetTextValueInt32(uint16_t screen_id, uint16_t control_id,int32_t value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%ld",value); 
	SetTextValue(screen_id,control_id,buffer);
	
}
//convert float to string
void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%.1f",value);
	SetTextValue(screen_id,control_id,buffer);
}


//screen init
void screen_init(void)
{
	RtcTime inittime = {0};
	
	//mergetimechar(inittime);
	mainIcon.trouble_indicate=HIDE;
	mainIcon.air_door_open_angle=SHOW;
	mainIcon.fan_operation=HIDE;
	mainIcon.pid_run=HIDE;
	
	mainPageText.left_time_hour=0;
	mainPageText.left_time_min=0;
	
	argSetErrorIcon.change_air_set_fail = HIDE;
	argSetErrorIcon.modebus_addr_set_fail = HIDE;
	argSetErrorIcon.test_temp_set_fail = HIDE;
	argSetErrorIcon.test_time_set_fail = HIDE;
	argSetErrorIcon.return_diff_set_fail = HIDE;
	argSetErrorIcon.temp_up_set_fail = HIDE;
	argSetErrorIcon.temp_up2_set_fail = HIDE;
	argSetErrorIcon.light_set_fail = HIDE;
	argSetErrorIcon.pass1_set_fail = HIDE;
	argSetErrorIcon.pass2_set_fail = HIDE;
	argSetErrorIcon.air_angle_set_fail = HIDE;
	argSetErrorIcon.temp_adjust_fail = HIDE;
	argSetErrorIcon.auto_no_power_set_fail = HIDE;
	updater_mainScreen();
	updater_argSetErrorScreen1();
	updater_argSetErrorScreen2();
	update_dev_status();
	
}

//DEC TO BCD
uint8_t DectoBCD(uint8_t Dec)
{
     return ((((Dec/10)<<4)&0xf0)|(Dec%10));
}
//BCD TO DEC
uint8_t BcdToDec(uint8_t bcd)
{
	return (0xff & (bcd>>4))*10 +(0xf & bcd);
}

/*! 
 screen changed inform
 */
void NotifyScreen(uint16_t screen_id)
{
	uint8_t i = 0;
	current_screen_id = screen_id;
	//change air editor
	if (screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
	{
		//edit air door
		updater_editAirDoor(screen_id);
	}
	else if(screen_id == biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN)
	{
		//menu password
		SetTextValue(screen_id,BIG_PASSWORD_PROTECT_INPUT,"");
	}
	else if(screen_id == biglanguage_screen.BIG_ADJUST_PROTECT_SCREEN)
	{
		//adjust protect password
		SetTextValue(screen_id,BIG_PASSWORD2_INPUT,"");
	}	
	else if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)	//PID
	{
		updater_PIDScreen(screen_id);
	}
	else if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)	//menu param setting
	{
		updater_menuParam(screen_id);
	}
	else if(screen_id == biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER)	//auto no power recover password
	{
		SetTextValue(screen_id,BIG_PASSWORD_PROTECT_INPUT,"");
	}
	else if(screen_id == biglanguage_screen.BIG_TIME_RECORD_SCREEN)	//time record
	{
		
	}
	else if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)	//Adjust screen
	{
		updater_adjScreen(screen_id);
	}
	else if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{
		
	}
	else if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1)	//param setting no invalid1
	{
		updater_argSetErrorScreen1();
	}
	else if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2)	//param setting no invalid2
	{
		updater_argSetErrorScreen2();
	}
	else if(screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
	{
		update_dev_status();
	}
		
}

/*! 
 touch x,y response
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{

}

/*! 
	button control inform
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	//change air times editor
	if(screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
	{
		//air change rate return button goes here
		//this should be in dev_info or screen status
		MySetScreen(biglanguage_screen.BIG_ADJUST_SCREEN);			
	}
	//pid
	if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)
	{
		pidScreenButton(screen_id,control_id,state);
	}
	//menu screen
	if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
	{
		menuScreenButton(screen_id,control_id,state);
	}
	//time record
	if(screen_id == biglanguage_screen.BIG_TIME_RECORD_SCREEN)
	{
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
	}
	//password input error
	if(screen_id == biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN)
	{		
		passwordInputWrongScreenButton(screen_id,control_id, state);	
	}
	//time setting
	if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			
	}
	//invalid setting1
	if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1)
	{
		MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);		
	}
	//invalid setting2
	if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2)
	{
		MySetScreen(biglanguage_screen.BIG_ADJUST_SCREEN);		
	}
	//main show screen
	if(screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
	{
		mainShowScreenButton(screen_id,control_id,state);
	}
	//adjust screen
	if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)
	{
		adjustScreenButton(screen_id,control_id,state);
	}
}


/*! 
text control inform
 */

void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
	//change air times
	if(screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
	{
		changeAirTimes(control_id,str);
	}
	//menu password
	if(screen_id == biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN)
	{
		menuPassword(control_id,str);
	}
	//adjust protect password
	if(screen_id == biglanguage_screen.BIG_ADJUST_PROTECT_SCREEN)
	{
		adjustProtectPassword(control_id,str);
	}
	//auto no power will show screen
	if(screen_id == biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER)
	{
		autoNoPowerScreen(control_id,str);
	}
	//PID
	if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)
	{
		pidSettingScreen(control_id,str);
	}
	//menu setting screen
	if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
	{
		menuSettingScreen(control_id,str);
	}
	//adjust screen setting
	if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)
	{
		adjustScreenSetting(control_id,str);
	}
	//device time set
	if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{
		deviceTimeSet(control_id,str);
	}
}

void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO
}



void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO
}



void  NotifyAnimation(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id)
{
	//TODO	
}


void NotifyIcon(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id)
{
	//TODO
}




void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO
	
	
}


void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO
}


void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO

}


void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
	//TODO
	
}


void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO
	
}


void NotifyWriteFlash(uint8_t status)
{
	//TODO
}


/*! 
 *  \brief  read RTC time,return BCD
 *  \param year 
 *  \param month 
 *  \param week
 *  \param day 
 *  \param hour 
 *  \param minute 
 *  \param second 
 */
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{	
	dev_info.timenow.Year = BcdToDec(year);
	dev_info.timenow.Mon = BcdToDec(month);
	dev_info.timenow.Day = BcdToDec(day);
	dev_info.timenow.Hour = BcdToDec(hour);
	dev_info.timenow.Min = BcdToDec(minute);
	dev_info.timenow.Sec = BcdToDec(second);
//	printf("%d/%d/%d  %02d:%02d:%02d\r\n",dev_info.timenow.Year,dev_info.timenow.Mon,dev_info.timenow.Day,dev_info.timenow.Hour,dev_info.timenow.Min,dev_info.timenow.Sec);
}

/*! 
	message deal process
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;					//cmd type
	uint8_t ctrl_msg = msg->ctrl_msg;   				//message type
	uint8_t control_type = msg->control_type;			//control type
	uint16_t screen_id = PTR2U16(&msg->screen_id);		//screen id
	uint16_t control_id = PTR2U16(&msg->control_id);	//control id
	uint32_t value = PTR2U32(msg->param);				//value

	switch(cmd_type)
	{		
		case NOTIFY_TOUCH_PRESS:		
			NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
			break;
		case NOTIFY_TOUCH_RELEASE:		
			NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
			break;	
		case NOTIFY_WRITE_FLASH_OK:		
			NotifyWriteFlash(1);
			break;
		case NOTIFY_WRITE_FLASH_FAILD:	
			NotifyWriteFlash(0);
			break;
		case NOTIFY_READ_FLASH_OK:		
			NotifyReadFlash(1,cmd_buffer+2,size-6);		
			break;
		case NOTIFY_READ_FLASH_FAILD:	
			NotifyReadFlash(0,0,0);
			break;
		case NOTIFY_READ_RTC:			
			NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
			break;
		case NOTIFY_CONTROL:
			{
				if(ctrl_msg==MSG_GET_CURRENT_SCREEN)	
				{
					NotifyScreen(screen_id);
				}
				else if(ctrl_msg == MSG_GET_ICON)
				{
					NotifyIcon(screen_id,control_id,msg->param[0],msg->param[1]);
				}
				else
				{
					switch(control_type)
					{
						case kCtrlButton: 
							NotifyButton(screen_id,control_id,msg->param[1]);
							//dev_info.dev_status_changed_flag = 1;
							break;
						case kCtrlText:
							NotifyText(screen_id,control_id,msg->param);
							//dev_info.dev_status_changed_flag = 1;
							break;
						case kCtrlProgress:
							NotifyProgress(screen_id,control_id,value);
							break;
						case kCtrlSlider: 
							NotifySlider(screen_id,control_id,value);
							break;
						case kCtrlAnimation: 
							NotifyAnimation(screen_id, control_id,msg->param[0],msg->param[1]);
						case kCtrlMeter: 
							NotifyMeter(screen_id,control_id,value);
							break;
						case kCtrlMenu:
							NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
							break;
						case kCtrlSelector:
							NotifySelector(screen_id,control_id,msg->param[0]);
							break;
						case kCtrlRTC:
							NotifyTimer(screen_id,control_id);
							break;
						default:
							break;
					}
				}			
			}
			break;
		default:
			break;
	}
}





