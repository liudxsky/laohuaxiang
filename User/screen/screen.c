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

uint8_t  soft_ver[10] = "Ver:1.0";

//uint8_t passwordwrongflag = 0;

uint8_t cmd_buffer[CMD_MAX_SIZE];		//screen data buffer
uint8_t change_air_time[CHANGE_AIR_SIZE] = {0};


extern uint16_t current_screen_id;
extern uint32_t _ms_counter; 
extern dev_info_t dev_info;
extern uint8_t pwmgpiostatus;
extern uint16_t gpiostatus;
			
RtcTime rtctime,settime;					
RtcTime  nopowertime = {2028,12,12,0,0,0};			


PCTRL_MSG msg;						
TextValueTab  textvalue;			//text control_id buff
CoilValue coilvalue;//should update via dev_info, its value should be memoried

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
	mainIcon.IO.sample_frame=HIDE;
	mainIcon.pid_run=HIDE;
	mainIcon.IO.heat_switch=HIDE;
	mainIcon.IO.heat_output=HIDE;
	
	mainPageText.left_time_hour=0;
	mainPageText.left_time_min=0;
	
	updater_mainScreen();
	
	sprintf(coilvalue.modbus_address,"%d",dev_info.Modbus_address);
	coilvalue.modbus_tran_rate = 3;
	coilvalue.menu_language = 1;

	argSetErrorIcon.change_air_set_fail=HIDE;
	argSetErrorIcon.modebus_addr_set_fail=HIDE;
	argSetErrorIcon.pass_update_fail=HIDE;
	argSetErrorIcon.test_temp_set_fail=HIDE;
	argSetErrorIcon.test_time_set_fail=HIDE;
	
	updater_argSetErrorScreen();
	
	update_dev_status();
	
	//	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR1_TEXT,HIDE);
//	delay_ms(100);
//	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR2_TEXT,HIDE);
//	delay_ms(100);
//	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR3_TEXT,HIDE);
//	delay_ms(100);
//	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR4_TEXT,HIDE);
//	delay_ms(100);
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
	else if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)	//Adjust screen
	{
		updater_adjScreen(screen_id);
		
	}
	else if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{

	}
	else if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1)	//param setting no invalid1
	{
		
	}
	else if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2)	//param setting no invalid2
	{

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
		//this should be in dev_info or screen status
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			
	}
	//pid
	if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)
	{
		if(control_id == BIG_SELF_ADJUST)							//self adjust
		{
			dev_info.runstatus=2;
		}
		else if(control_id == BIG_PID_RETURN_BUTTON)  				
		{
			//this should be in dev_info or screen status
			setCurrentScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			//MySetScreen(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN);
		}
		
	}
	//menu screen
	if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
	{
		if(control_id == BIG_SET_RETURN_BUTTON)							
		{
			setCurrentScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			
		}
		else if(control_id == BIG_ADJUST_BUTTON)				
		{
			
			setCurrentScreen(biglanguage_screen.BIG_ADJUST_PROTECT_SCREEN);
		}
	}
	//time record
	if(screen_id == biglanguage_screen.BIG_TIME_RECORD_SCREEN)
	{
		setCurrentScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
	}
	if(screen_id == biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN)
	{
		
		if(dev_info.passwordwrongflag == 1)
		{
			setCurrentScreen(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN);
			
		}
		else if(dev_info.passwordwrongflag == 2)
		{
			setCurrentScreen(biglanguage_screen.BIG_ADJUST_PROTECT_SCREEN);
		}
		else if(dev_info.passwordwrongflag == 3)
		{
			MySetScreen(biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER);
		}
		
	}
	
	//time setting
	if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{
		SetRtcTime(settime.Sec,settime.Min,settime.Hour,settime.Day,0,settime.Mon,settime.Year);
//		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			
	}

	//invalid setting
	if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1)
	{
		setCurrentScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);		
	}
	//main show screen
	if(screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
	{
		switch (control_id)
		{
			case BIG_MENU_ID:
				setCurrentScreen(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN);
				break;
			case BIG_TIME_RECORD_ID:
				setCurrentScreen(biglanguage_screen.BIG_TIME_RECORD_SCREEN);
				break;
			case BIG_SAMPLE_FRAME_MENU_ID:
				if(state)
				{
					//should chage dev_info, and update to screen status
					IOStatus.sample_frame=1;
					mainIcon.rr_work_status=HIDE;
					//SPINNER_RACK_ON;
				}
				else
				{
					//SPINNER_RACK_OFF;
					IOStatus.sample_frame=0;
					mainIcon.rr_work_status=SHOW;
				}

				break;
			case BIG_BLOWER_MENU_ID:
				if(state)
				{
					mainIcon.fan_operation=SHOW;
					mainIcon.fr_work_status=HIDE;
				}
				else
				{
					mainIcon.fan_operation=HIDE;
					mainIcon.fr_work_status=SHOW;
					
				}
				break;
			case BIG_START_OR_PAUSE_ID:
				if(dev_info.runstatus>0)
				{
					SetPwmValue(0);
					dev_info.runstatus = 0;
					dev_info.lefttimeflag=0;

				}
				else
				{
					dev_info.runstatus = 1;			
				}
			
				break;
			default:
				break;
		}
	}
	//adjust screen
	if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)
	{
		switch (control_id)
		{
			case BIG_NO_POWER_RETURN_BUTTON:
				setCurrentScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);
				
				break;
			case BIG_EDIT_BUTTON:
				setCurrentScreen(biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN);
				break;
			case BIG_PID_BUTTON:
				setCurrentScreen(biglanguage_screen.BIG_PID_SET_SCREEN);
				break;
			default:
				break;
		}
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
	
		
}


void NotifyIcon(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id)
{
	if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
	{
		switch (control_id)
		{
			case BIG_BPS_1200:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
					dev_info.modbus_tran_rate_flag = 0;
				}
				break;
			case BIG_BPS_2400:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
					dev_info.modbus_tran_rate_flag = 1;
				}
				break;
			case BIG_BPS_4800:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
					dev_info.modbus_tran_rate_flag = 2;
				}
				break;
			case BIG_BPS_9600:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					dev_info.modbus_tran_rate_flag = 3;
				}
				break;
			case BIG_CHINESE_LANGUAGE:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,HIDE);
					dev_info.biglanguagestatus = 1;
				}
				break;
			case BIG_ENGLISH_LANGUAGE:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,HIDE);
					dev_info.biglanguagestatus = 0;
				}
				break;
			default:
				dev_info.modbus_tran_rate_flag = 3;
				dev_info.biglanguagestatus = 1;
		}
	}
	dev_info.dev_status_changed_flag = 1;
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
	RtcTime rtctime;
	rtctime.Year = BcdToDec(year);
	rtctime.Mon = BcdToDec(month);
	rtctime.Day = BcdToDec(day);
	rtctime.Hour = BcdToDec(hour);
	rtctime.Min = BcdToDec(minute);
	rtctime.Sec = BcdToDec(second);
	readRTCFromScreen(rtctime);
//	printf("%d/%d/%d  %02d:%02d:%02d\r\n",rtctime.Year,rtctime.Mon,rtctime.Day,rtctime.Hour,rtctime.Min,rtctime.Sec);
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





