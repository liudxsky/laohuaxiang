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
#include "./dac/dac.h"

uint8_t  soft_ver[10] = "Ver:1.0";
uint8_t lefttimeflag = 0;			

uint8_t cmd_buffer[CMD_MAX_SIZE];		//screen data buffer
uint8_t change_air_time[CHANGE_AIR_SIZE] = {0};
uint32_t  timercount = 0;


extern RtcTime rtctime;
extern uint16_t current_screen_id,last_screen_id;
extern uint32_t _ms_counter;
extern float Ktemperature; 
extern dev_info_t dev_info;
extern uint8_t pwmgpiostatus;
extern uint16_t gpiostatus;
extern uint8_t thermocouple_flag;
extern int runstatus;

uint8_t slidervalue = 0;		
uint8_t autonopowerpassword[PASSWORDLENGTH] = {0};	


float adjusttemp = 0;			
RtcTime rtctime;					
AutoNoPowerTime  nopowertime;			

PCTRL_MSG msg;						
TextValueTab  textvalue;			//text control_id buff
MainShowTextValue	showtextvalue;	//text control_id value
CoilValue  coilvalue;				//menu text value




BIG_SCREEN_ID_TAB bigchinese_screen = {0,1,2,3,4,5,6,7,8,9,10,11,12};
BIG_SCREEN_ID_TAB bigenglish_screen = {13,14,15,16,17,18,19,20,21,22,23,24,25};
BIG_SCREEN_ID_TAB biglanguage_screen = {0};




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
	mergetimechar(inittime);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TROUBLE_INDICATE_ID,HIDE);
	delay_ms(200);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_OPEN_ANGLE_ID,SHOW);
	delay_ms(200);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,HIDE);
	delay_ms(200);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,HIDE);
	delay_ms(200);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,HIDE);
	delay_ms(200);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,HIDE);
	delay_ms(200);
	AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,HIDE);
	
	sprintf(textvalue.coilsavevalue.modbus_address,"%d",dev_info.Modbus_address);

	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_HOUR_ID," 00 ");	
	textvalue.coilsavevalue.modbus_tran_rate = 3;
	textvalue.coilsavevalue.menu_language = 1;


	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR1_TEXT,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR2_TEXT,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR3_TEXT,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR4_TEXT,HIDE);
	delay_ms(100);

	AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTEMP_SET_FAIL,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTIME_SET_FAIL,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_CHANGE_AIR_SET_FAIL,HIDE);
	delay_ms(100);
	AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_MODBUS_ADDRESS_SET_FAIL,HIDE);
	
	sprintf(textvalue.coilsavevalue.menu_password,"%06d",dev_info.flash_setvalue.menu_password);
		
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
	if(screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
	{
		for (i= 0; i < CHANGE_AIR_SIZE; ++i)
		{
			SetTextValueInt32(screen_id,BIG_CHANGE_AIR_TIME_SET_90+i,dev_info.change_air_time[i]);
		}
	}
	//Adjust screen
	if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)
	{
		
		SetTextValue(screen_id,BIG_NOPOWER_PASSWORD1,"");
		SetTextValue(screen_id,BIG_NOPOWER_PASSWORD2,"");
		SetTextValue(screen_id,BIG_AIR_DOOR_ANGLE_SET,textvalue.airdoor_value);
		SetTextValueFloat(screen_id,BIG_TEMP_VALUE_REVUSE_SET,dev_info.flash_adjusttemp);
		SetTextValueInt32(screen_id,BIG_YEAR_SET,dev_info.autonopowertime.year);
		SetTextValueInt32(screen_id,BIG_MONTH_SET,dev_info.autonopowertime.month);
		SetTextValueInt32(screen_id,BIG_DAY_SET,dev_info.autonopowertime.day);
	}
	//menu password
	if(screen_id == biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN)
	{
		SetTextValue(screen_id,BIG_PASSWORD_PROTECT_INPUT,"");
	}
	//auto no power recover password
	if(screen_id == biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER)
	{
		SetTextValue(screen_id,BIG_PASSWORD_PROTECT_INPUT,"");
	}
	//PID
	if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)
	{
		SetTextValueFloat(screen_id,BIG_P_VALUE_SET,dev_info.pidvalue.PID_P);

		SetTextValueFloat(screen_id,BIG_I_VALUE_SET,dev_info.pidvalue.PID_I*10000);

		SetTextValueFloat(screen_id,BIG_D_VALUE_SET,dev_info.pidvalue.PID_D);	
	}
	//param setting screen 
	if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
	{
		SetTextValueFloat(screen_id,BIG_TEST_TIME_VALUE,dev_info.testtime);
		
		SetTextValueFloat(screen_id,BIG_TEST_TEMP_VALUE,dev_info.testtemp);
	
		SetTextValue(screen_id,BIG_WARNING1_UP_VALUE,textvalue.coilsavevalue.warning1_up);
	
		SetTextValue(screen_id,BIG_WARNING2_UP_VALUE,textvalue.coilsavevalue.warning2_up);

		SetTextValue(screen_id,BIG_NEW_PASSWORD,"");
		
		SetTextValue(screen_id,BIG_SECOND_INPUT_PASSWORD,"");
		
		SetTextValue(screen_id,BIG_CHANGE_AIR_TIME_SET,textvalue.coilsavevalue.change_air_time);
	
		SetTextValue(screen_id,BIG_CHANGE_AIR_MAX_SET,textvalue.coilsavevalue.change_max_time);
	
		SetTextValueInt32(screen_id,BIG_MODBUS_NODE_ADDRESS,dev_info.Modbus_address);
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
	
	//param setting no invalid
	if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN)
	{

	}
	//device time setting
	if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{	
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
	if((screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)&&(control_id == BIG_AIR_RETURN_BUTTON))
	{
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			
	}
	//pid
	if((screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)&&(state == 1))
	{
		if(control_id == BIG_SELF_ADJUST)							//self adjust
		{
			runstatus=2;
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,SHOW);
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,SHOW);
		}
		else if(control_id == BIG_PID_RETURN_BUTTON)  				
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);		
		}
		
	}
	//main show screen
	if(screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
	{
		switch (control_id)
		{
			case BIG_MENU_ID:
				if(state)
				{
					MySetScreen(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN);
				}
				break;
			case BIG_TIME_RECORD_ID:
				if(state)
				{
					MySetScreen(biglanguage_screen.BIG_TIME_RECORD_SCREEN);
				}
				break;
			case BIG_SAMPLE_FRAME_MENU_ID:
				if(state)
				{
					SPINNER_RACK_ON;
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,SHOW);
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,HIDE);
				}
				else
				{
					SPINNER_RACK_OFF;
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,HIDE);
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,SHOW);
				}
				break;
			case BIG_BLOWER_MENU_ID:
				if(state)
				{
					CIRCULATING_FUN_ON;
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,SHOW);
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,HIDE);
					
				}
				else
				{
					CIRCULATING_FUN_OFF;
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,HIDE);
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,SHOW);
				}
				break;
			case BIG_START_OR_PAUSE_ID:
				if(runstatus>0)
				{
					SetPwmValue(0);
					runstatus = 0;			//ֹͣ
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,HIDE);
				}
				else
				{
					runstatus = 1;			
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,SHOW);
					AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,SHOW);
				}
			
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
	uint16_t backlight = 0;
	//change air times
	if(screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
	{
		switch (control_id)
		{
			case BIG_CHANGE_AIR_TIME_SET_100:
				memset(textvalue.change_air_times[20],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[20],str,sizeof(char)*4);
				dev_info.change_air_time[20] = atoi(textvalue.change_air_times[20]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_95:
				memset(textvalue.change_air_times[19],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[19],str,sizeof(char)*4);
				dev_info.change_air_time[19] = atoi(textvalue.change_air_times[19]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_90:
				memset(textvalue.change_air_times[18],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[18],str,sizeof(char)*4);
				dev_info.change_air_time[18] = atoi(textvalue.change_air_times[18]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_85:
				memset(textvalue.change_air_times[17],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[17],str,sizeof(char)*4);
				dev_info.change_air_time[17] = atoi(textvalue.change_air_times[17]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_80:
				memset(textvalue.change_air_times[16],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[16],str,sizeof(char)*4);
				dev_info.change_air_time[16] = atoi(textvalue.change_air_times[16]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_75:
				memset(textvalue.change_air_times[15],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[15],str,sizeof(char)*4);
				dev_info.change_air_time[15] = atoi(textvalue.change_air_times[15]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_70:
				memset(textvalue.change_air_times[14],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[14],str,sizeof(char)*4);
				dev_info.change_air_time[14] = atoi(textvalue.change_air_times[14]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_65:
				memset(textvalue.change_air_times[13],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[13],str,sizeof(char)*4);
				dev_info.change_air_time[13] = atoi(textvalue.change_air_times[13]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_60:
				memset(textvalue.change_air_times[12],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[12],str,sizeof(char)*4);
				dev_info.change_air_time[12] = atoi(textvalue.change_air_times[12]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_55:
				memset(textvalue.change_air_times[11],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[11],str,sizeof(char)*4);
				dev_info.change_air_time[11] = atoi(textvalue.change_air_times[11]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_50:
				memset(textvalue.change_air_times[10],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[10],str,sizeof(char)*4);
				dev_info.change_air_time[10] = atoi(textvalue.change_air_times[10]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_45:
				memset(textvalue.change_air_times[9],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[9],str,sizeof(char)*4);
				dev_info.change_air_time[9] = atoi(textvalue.change_air_times[9]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_40:
				memset(textvalue.change_air_times[8],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[8],str,sizeof(char)*4);
				dev_info.change_air_time[8] = atoi(textvalue.change_air_times[8]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_35:
				memset(textvalue.change_air_times[7],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[7],str,sizeof(char)*4);
				dev_info.change_air_time[7] = atoi(textvalue.change_air_times[7]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_30:
				memset(textvalue.change_air_times[6],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[6],str,sizeof(char)*4);
				dev_info.change_air_time[6] = atoi(textvalue.change_air_times[6]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_25:
				memset(textvalue.change_air_times[5],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[5],str,sizeof(char)*4);
				dev_info.change_air_time[5] = atoi(textvalue.change_air_times[5]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_20:
				memset(textvalue.change_air_times[4],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[4],str,sizeof(char)*4);
				dev_info.change_air_time[4] = atoi(textvalue.change_air_times[4]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_15:
				memset(textvalue.change_air_times[3],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[3],str,sizeof(char)*4);
				dev_info.change_air_time[3] = atoi(textvalue.change_air_times[3]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_10:
				memset(textvalue.change_air_times[2],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[2],str,sizeof(char)*4);
				dev_info.change_air_time[2] = atoi(textvalue.change_air_times[2]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_5:
				memset(textvalue.change_air_times[1],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[1],str,sizeof(char)*4);
				dev_info.change_air_time[1] = atoi(textvalue.change_air_times[1]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_0:
				memset(textvalue.change_air_times[0],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[0],str,sizeof(char)*4);
				dev_info.change_air_time[0] = atoi(textvalue.change_air_times[0]);
				break;
			default:
				break;
		}
		coilvalue.change_max_time = max_change_air(dev_info.change_air_time);
		SetTextValueInt32(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_CHANGE_AIR_MAX_SET,coilvalue.change_max_time);
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));				//дflash
	}
	
	//menu password
	if((screen_id == biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN)&&(control_id == BIG_PASSWORD_PROTECT_INPUT))
	{
		memset(textvalue.protect_password,0,sizeof(char)*PASSWORDLENGTH);
		memcpy(textvalue.protect_password ,str,sizeof(char)*PASSWORDLENGTH);
		coilvalue.menu_password = atoi(textvalue.protect_password);
		SetTextValue(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN,BIG_PASSWORD_PROTECT_INPUT,textvalue.protect_password);
		if(!strncmp(textvalue.protect_password,textvalue.coilsavevalue.menu_password,6))
		{
			AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,HIDE);
			MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);	
		}
		else
		{
			AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,SHOW);
			MySetScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
			
		}
	}
	//PID
	if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)
	{
		
		switch (control_id)
		{
			case BIG_P_VALUE_SET:
				memset(textvalue.Pidvalue[0],0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.Pidvalue[0],str,sizeof(char)*PASSWORDLENGTH);
				dev_info.pidvalue.PID_P = atof(textvalue.Pidvalue[0]);
				break;
			case BIG_I_VALUE_SET:
				memset(textvalue.Pidvalue[1],0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.Pidvalue[1],str,sizeof(char)*PASSWORDLENGTH);
				dev_info.pidvalue.PID_I = atof(textvalue.Pidvalue[1]);
				dev_info.pidvalue.PID_I=dev_info.pidvalue.PID_I/10000;
				break;
			case BIG_D_VALUE_SET:
				memset(textvalue.Pidvalue[2],0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.Pidvalue[2],str,sizeof(char)*PASSWORDLENGTH);
				dev_info.pidvalue.PID_D = atof(textvalue.Pidvalue[2]);
				break;
			default:
				break;
		}
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));		
	}
	//menu setting screen
	if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
	{
		switch (control_id)
		{
			case BIG_TEST_TIME_VALUE:	//set time
				memset(textvalue.coilsavevalue.test_duration,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.test_duration,str,sizeof(char)*COMMONSIZE);
				showtextvalue.test_time = atof(textvalue.coilsavevalue.test_duration);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEST_TIME_VALUE,textvalue.coilsavevalue.test_duration);
				
				if(showtextvalue.test_time < 0 || showtextvalue.test_time >= 4000)
				{
					dev_info.testtime = 0;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTIME_SET_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
				}
				else 
				{
					dev_info.testtime = showtextvalue.test_time;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTIME_SET_FAIL,HIDE);
					printf("test time is %f",dev_info.testtime );
					SetTextValueFloat(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEST_TIME_ID,dev_info.testtime);
				}
				
				break;
			case BIG_TEST_TEMP_VALUE:	//set temp
				if(runstatus>0)
					return;
				memset(textvalue.coilsavevalue.test_temp,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.test_temp,str,sizeof(char)*COMMONSIZE);	
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEST_TEMP_VALUE,textvalue.coilsavevalue.test_temp);			
				showtextvalue.setting_temp = atof(textvalue.coilsavevalue.test_temp);			
				if(showtextvalue.setting_temp < 0 || showtextvalue.setting_temp >= 1000)
				{
					dev_info.testtemp = 0;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTEMP_SET_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
				}
				else
				{
					dev_info.testtemp = showtextvalue.setting_temp;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTEMP_SET_FAIL,HIDE);
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_ID,(int32_t)(dev_info.testtemp/1));
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_DECIMAL_ID,(int32_t)(dev_info.testtemp*10)%10);
				}
				break;	
			case BIG_TEMP_RETURN_DIFF:
				memset(textvalue.coilsavevalue.temp_backdiff,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.temp_backdiff,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEMP_RETURN_DIFF,textvalue.coilsavevalue.temp_backdiff);			
				coilvalue.temp_backdiff = atof(textvalue.coilsavevalue.temp_backdiff);
				if(coilvalue.temp_backdiff < 0)
				{
					dev_info.temp_backdiff = 0;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_RETURN_DIFF_SET_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
				}
				else
				{
					dev_info.temp_backdiff = coilvalue.temp_backdiff;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_RETURN_DIFF_SET_FAIL,HIDE);
				}
				break;
				
			case BIG_WARNING1_UP_VALUE:
				memset(textvalue.coilsavevalue.warning1_up,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.warning1_up,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING1_UP_VALUE,textvalue.coilsavevalue.warning1_up);
				coilvalue.warning1_up = atof(textvalue.coilsavevalue.warning1_up);
				
				if(coilvalue.warning1_up < 0)
				{
					dev_info.flash_setvalue.warning1_up = 0;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TEMP_UP_SET_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
				}
				else
				{
					dev_info.flash_setvalue.warning1_up = coilvalue.warning1_up;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TEMP_UP_SET_FAIL,HIDE);
				}
				break;	
			case BIG_WARNING2_UP_VALUE:
				memset(textvalue.coilsavevalue.warning2_up,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.warning2_up,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING2_UP_VALUE,textvalue.coilsavevalue.warning2_up);
				coilvalue.warning2_up = atof(textvalue.coilsavevalue.warning2_up);
				
				if(coilvalue.warning2_up < 0)
				{
					dev_info.flash_setvalue.warning2_up = 0;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TEMP_UP_SET_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
				}
				else
				{
					dev_info.flash_setvalue.warning2_up = coilvalue.warning2_up;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TEMP_UP_SET_FAIL,HIDE);
				}
				break;	
				
			case BIG_NEW_PASSWORD:
				memset(textvalue.coilsavevalue.menu_password,0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.coilsavevalue.menu_password,str,sizeof(char)*PASSWORDLENGTH);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_NEW_PASSWORD,textvalue.coilsavevalue.menu_password);
				break;
			case BIG_SECOND_INPUT_PASSWORD:
				memset(textvalue.coilsavevalue.secondtime_password,0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.coilsavevalue.secondtime_password,str,sizeof(char)*PASSWORDLENGTH);
				
				if(strncmp(textvalue.coilsavevalue.secondtime_password,textvalue.coilsavevalue.menu_password,PASSWORDLENGTH))
				{
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);	
				}
				else
				{
					dev_info.flash_setvalue.menu_password = atoi(textvalue.coilsavevalue.secondtime_password);
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,HIDE);
				}
				
				break;
			case BIG_CHANGE_AIR_TIME_SET:
				memset(textvalue.coilsavevalue.change_air_time,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.change_air_time,str,sizeof(char)*COMMONSIZE);
				coilvalue.change_air_time = atoi(textvalue.coilsavevalue.change_air_time);
				if(judge_changeair_time(coilvalue.change_air_time))
				{
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_CHANGE_AIR_SET_FAIL,HIDE);
					SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_CHANGE_AIR_TIME_SET,textvalue.coilsavevalue.change_air_time);
					SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CHANGE_AIR_TIME,textvalue.coilsavevalue.change_air_time);
					change_air_times();
				}
				else
				{
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_CHANGE_AIR_SET_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);	
				}
				break;
			case BIG_SCREEN_BRIGHT_ADJUST:
				memset(textvalue.screen_light_value,0,sizeof(char)*4);
				memcpy(textvalue.screen_light_value ,str,sizeof(char)*4);
				backlight = (uint16_t)atoi(textvalue.screen_light_value)*255/100;
				if(backlight >= 255)
				{
					backlight = 0;
				}
				else
				{
					backlight = 255 - backlight;
				}
				SetBackLight(backlight);
				break;
			
			case BIG_MODBUS_NODE_ADDRESS:
				memset(textvalue.coilsavevalue.modbus_address,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.coilsavevalue.modbus_address,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_MODBUS_NODE_ADDRESS,textvalue.coilsavevalue.modbus_address);
				coilvalue.modbus_address = atoi(textvalue.coilsavevalue.modbus_address);
				if(coilvalue.modbus_address < 0 || coilvalue.modbus_address > 250)
				{
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);	
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_MODBUS_ADDRESS_SET_FAIL,SHOW);
				}
				else
				{
					dev_info.flash_setvalue.modbus_address = coilvalue.modbus_address;
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_MODBUS_ADDRESS_SET_FAIL,HIDE);
				}
				break;
				
			default:
				break;
		}
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));				//参数设置界面数据存入flash
	}
	//adjust screen setting
	if(screen_id == biglanguage_screen.BIG_ADJUST_SCREEN)
	{
		switch (control_id)
		{
			case BIG_NOPOWER_PASSWORD1:
				memset(textvalue.coilsavevalue.autonopowerpassword,0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.coilsavevalue.autonopowerpassword,str,sizeof(char)*PASSWORDLENGTH);
				SetTextValue(biglanguage_screen.BIG_ADJUST_SCREEN,BIG_NOPOWER_PASSWORD1,textvalue.coilsavevalue.autonopowerpassword);
				break;
			case BIG_NOPOWER_PASSWORD2:
				memset(textvalue.coilsavevalue.autonopower_secondpassword,0,sizeof(char)*PASSWORDLENGTH);
				memcpy(textvalue.coilsavevalue.autonopower_secondpassword,str,sizeof(char)*PASSWORDLENGTH);
				SetTextValue(biglanguage_screen.BIG_ADJUST_SCREEN,BIG_NOPOWER_PASSWORD2,textvalue.coilsavevalue.autonopower_secondpassword);
				if(strncmp(textvalue.coilsavevalue.autonopowerpassword,textvalue.coilsavevalue.autonopower_secondpassword,PASSWORDLENGTH))
				{
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,SHOW);
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);	
				}
				else
				{
					memcpy(dev_info.autonopower_password,textvalue.coilsavevalue.autonopower_secondpassword,sizeof(char)*PASSWORDLENGTH);
					dev_info.flash_setvalue.no_power_protect_password = atoi(textvalue.coilsavevalue.autonopowerpassword);
					AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,HIDE);
				}
				break;
			case BIG_AIR_DOOR_ANGLE_SET:
				memset(textvalue.airdoor_value,0,sizeof(char)*4);
				memcpy(textvalue.airdoor_value,str,sizeof(char)*4);
				SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,textvalue.airdoor_value);
				SetTextValue(biglanguage_screen.BIG_ADJUST_SCREEN,BIG_AIR_DOOR_ANGLE_SET,textvalue.airdoor_value);
				coilvalue.air_door_angle = atoi(textvalue.airdoor_value);
				Dac1_Set_Vol(3300*coilvalue.air_door_angle/100);
				break;
			case BIG_TEMP_VALUE_REVUSE_SET:
				memset(textvalue.temp_adjust_value,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.temp_adjust_value,str,sizeof(char)*COMMONSIZE);
				dev_info.flash_adjusttemp +=  atof(textvalue.temp_adjust_value);
			case BIG_YEAR_SET:
				memset(textvalue.autotime.year,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.autotime.year,str,sizeof(char)*COMMONSIZE);
				nopowertime.year = atoi(textvalue.autotime.year);
				nopowertime.year = DectoBCD(nopowertime.year);
				dev_info.autonopowertime.year = nopowertime.year; 
				break;
			case BIG_MONTH_SET:
				memset(textvalue.autotime.month,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.autotime.month,str,sizeof(char)*COMMONSIZE);
				nopowertime.month = atoi(textvalue.autotime.month);
				nopowertime.month = DectoBCD(nopowertime.month);
				dev_info.autonopowertime.month = nopowertime.month;
				break;	
			case BIG_DAY_SET:
				memset(textvalue.autotime.day,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.autotime.day,str,sizeof(char)*COMMONSIZE);
				nopowertime.day = atoi(textvalue.autotime.day);
				nopowertime.day = DectoBCD(nopowertime.day);
				dev_info.autonopowertime.day = nopowertime.day;
				break;	
			default:
				break;
		}
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
	}
	//auto no power will show screen
	if(screen_id == biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER)
	{
		memset(textvalue.autonopowerpassword,0,sizeof(char)*PASSWORDLENGTH);
		memcpy(textvalue.autonopowerpassword,str,sizeof(char)*PASSWORDLENGTH);
		if(strncmp(textvalue.autonopowerpassword,dev_info.autonopower_password,PASSWORDLENGTH) == 0)
		{
			//have power , control device work normal
		}
		else
		{
			//control device no work
		}
	}
	//device time set
	if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
	{
		switch (control_id)
		{
			case BIG_CONTROL_DATE_YEAR_SET:
				memset(textvalue.device_time_setting.Year,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Year,str,sizeof(char)*COMMONSIZE);	
				rtctime.Year = atoi(textvalue.device_time_setting.Year);
				rtctime.Year = DectoBCD(rtctime.Year);
				break;
			case BIG_CONTROL_DATE_MONTH_SET:
				memset(textvalue.device_time_setting.Mon,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Mon,str,sizeof(char)*COMMONSIZE);
				rtctime.Mon = atoi(textvalue.device_time_setting.Mon);
				rtctime.Mon = DectoBCD(rtctime.Mon);
				break;	
			case BIG_CONTROL_DATE_DAY_SET:
				memset(textvalue.device_time_setting.Day,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Day,str,sizeof(char)*COMMONSIZE);
				rtctime.Day = atoi(textvalue.device_time_setting.Day);
				rtctime.Day = DectoBCD(rtctime.Day);
				break;	
			case BIG_CONTROL_TIME_HOUR_SET:
				memset(textvalue.device_time_setting.Hour,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Hour,str,sizeof(char)*COMMONSIZE);
				rtctime.Hour = atoi(textvalue.device_time_setting.Hour);
				rtctime.Hour = DectoBCD(rtctime.Hour);
				break;
			case BIG_CONTROL_TIME_MINUTE_SET:
				memset(textvalue.device_time_setting.Min,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Min,str,sizeof(char)*COMMONSIZE);
				rtctime.Min = atoi(textvalue.device_time_setting.Min);
				rtctime.Min = DectoBCD(rtctime.Min);
				break;	
			case BIG_CONTROL_TIME_SECOND_SET:
				memset(textvalue.device_time_setting.Sec,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Sec,str,sizeof(char)*COMMONSIZE);
				rtctime.Sec = atoi(textvalue.device_time_setting.Sec);
				rtctime.Sec = DectoBCD(rtctime.Sec);
				break;	
			default:
				break;
		}
		SetRtcTime(rtctime.Sec,rtctime.Min,rtctime.Hour,rtctime.Day,0,rtctime.Mon,rtctime.Year);
	}
}

//������
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
	FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
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
	rtctime.Year = BcdToDec(year);
	rtctime.Mon = BcdToDec(month);
	rtctime.Day = BcdToDec(day);
	rtctime.Hour = BcdToDec(hour);
	rtctime.Min = BcdToDec(minute);
	rtctime.Sec = BcdToDec(second);
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
							dev_info.dev_status_changed_flag = 1;
							break;
						case kCtrlText:
							NotifyText(screen_id,control_id,msg->param);
							dev_info.dev_status_changed_flag = 1;
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



//start screen 
void startscreen(void)
{
	printf("%s\r\n",soft_ver);																			
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SOFT_VER_ID,soft_ver);	
	delay_s(2);
	MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);							
}
//check pid's status
void check_pidstatus(void)
{
	if(runstatus==3)
	{		
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,HIDE);
		
	}
}
void check_heat_switch()
{
	if(runstatus>0)
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,HIDE);
	}
	
}


//merge time str
void  mergetimechar(RtcTime datetime)
{
	uint8_t buff[2] = {0};
	textvalue.textvaluebuff.start_time[0] = '2';
	textvalue.textvaluebuff.start_time[1] = '0';
		
	sprintf(buff,"%02d",datetime.Year); 
	textvalue.textvaluebuff.start_time[2] = buff[0];
	textvalue.textvaluebuff.start_time[3] = buff[1];

	sprintf(buff,"%02d",datetime.Mon);
	textvalue.textvaluebuff.start_time[4] = '/';
	textvalue.textvaluebuff.start_time[5] = buff[0];
	textvalue.textvaluebuff.start_time[6] = buff[1];

	sprintf(buff,"%02d",datetime.Day);
	textvalue.textvaluebuff.start_time[7] = '/';
	textvalue.textvaluebuff.start_time[8] = buff[0];
	textvalue.textvaluebuff.start_time[9] = buff[1];
	
	textvalue.textvaluebuff.start_time[10] = ' ';

	sprintf(buff,"%02d",datetime.Hour);
	textvalue.textvaluebuff.start_time[11] = buff[0];
	textvalue.textvaluebuff.start_time[12] = buff[1];
	textvalue.textvaluebuff.start_time[13] = ':';

	sprintf(buff,"%02d",datetime.Min);
	textvalue.textvaluebuff.start_time[14] = buff[0];
	textvalue.textvaluebuff.start_time[15] = buff[1];
}



//to time total what minutes,start time is : 2000\1\1 0:00
uint32_t to_day(RtcTime time)
{
	uint8_t mon[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	uint16_t day = 0;
	uint32_t hour = 0,minute = 0,seconds = 0;
	uint16_t i;
	time.Year = time.Year + 2000;
	for(i = 2000;i < time.Year; i++)
	{
		day += (i%4 == 0 && i%100 != 0 || i%400 == 0)? 366:365;
	}
	if(time.Year%4 == 0 && time.Year%100!=0 || time.Year%400 == 0) 
	{
		mon[1] = 29;
	}

	for(i = 0;i < time.Mon - 1;i++)
	{
		day += mon[i];
	}
	hour = (day+time.Day-1)*24 + time.Hour;

	minute = hour * 60 + time.Min;

	seconds = minute * 60 + time.Sec;
	return seconds;
}


//end time calculate
void endtimecalcu(RtcTime starttime,float testtime)
{
	char timebuff[25] = {0};
	time_t currenttime = 946684800;
	currenttime += to_day(starttime) + testtime*3600;
	strftime(timebuff,20,"%Y/%m/%d %H:%M:%S",localtime(&currenttime));
	memcpy(textvalue.textvaluebuff.end_time,timebuff,16);
	memcpy(dev_info.warmend_time,textvalue.textvaluebuff.end_time,sizeof(textvalue.textvaluebuff.end_time));
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_END_TIME_ID,dev_info.warmend_time);		
}



uint32_t diff_time(RtcTime starttime,RtcTime endtime)
{
	return (to_day(endtime) - to_day(starttime));
}


float myabs(float a,float b)
{
	if(a >= b)  
		return a-b;
	else
		return b-a;
}

uint32_t nowtimediff_addup=0,lasttimediff_addup=0;	
RtcTime starttime_addup={0};

void addup_testtime(void)
{
	
	if(runstatus>0)
	{
		if(starttime_addup.Year==0)//init, runstatus pos edge ____|````
		{
			starttime_addup=rtctime;
			
		}
		nowtimediff_addup=diff_time(starttime_addup, rtctime)/60;
		if(nowtimediff_addup/60 - lasttimediff_addup)
		{
			dev_info.addup_testtime += 1;
			lasttimediff_addup = nowtimediff_addup/60;
			SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_ADDUP_TIME_ID,dev_info.addup_testtime);
			FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));				//update addup time
		}
	}
	else//runstatus=0
	{
		if(starttime_addup.Year!=0)  // runstatus neg edge````|____
		{
			//shutdown action
			starttime_addup.Year=0;
		}
	}
}


void lefttimecalculate(void)
{
	uint32_t nowtesttime,temptime;	
	if((runstatus)&&(dev_info.testtime != 0))
	{
		nowtesttime = diff_time(showtextvalue.start_time, rtctime)/60;
		temptime = showtextvalue.test_time*60 - nowtesttime;
		showtextvalue.left_time_hou = temptime/60;
		showtextvalue.left_time_min = temptime%60;
		SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_HOUR_ID,showtextvalue.left_time_hou);
		SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_MIN_ID,showtextvalue.left_time_min);
		
		if(temptime == 0)
		{
			dev_info.testtemp = 0;
			dev_info.testtime = 0;
			runstatus = 0;
			lefttimeflag = 0;
		}
	}
}


uint8_t steadytempcount = 0;
void start_endtime_set(void)
{
	if(myabs(dev_info.testtemp,showtextvalue.current_temp_vlaue)<=3)
	{
		steadytempcount++;
	}
	else
	{
		steadytempcount = 0;
	}
	if((runstatus)&&(dev_info.testtemp != 0)&&(lefttimeflag == 0)&&(steadytempcount>=120))
	{	
		lefttimeflag = 1;
		showtextvalue.start_time = rtctime;
		mergetimechar(showtextvalue.start_time);
		memcpy(dev_info.warmstart_time,textvalue.textvaluebuff.start_time,sizeof(textvalue.textvaluebuff.start_time));
		
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_START_TIME_ID,textvalue.textvaluebuff.start_time);
		endtimecalcu(showtextvalue.start_time,showtextvalue.test_time);
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));		
	}
	if(lefttimeflag == 1)
	{
		lefttimecalculate();
	}
}



void change_air_times(void)
{
	uint8_t i = 0;
	for(i = 0;i < CHANGE_AIR_SIZE;i++)
	{
		if(coilvalue.change_air_time == dev_info.change_air_time[i])
		{
			Dac1_Set_Vol(33*i);
		}
	}
}

uint8_t max_change_air(uint8_t *buff)
{
	uint8_t i = 0, temp = 0;
	for(i = 1;i<CHANGE_AIR_SIZE;i++)
	{
		temp = buff[0];
		if(buff[i] >= temp)
		{
			temp = buff[i];
		}
	}
	return temp;
}


uint8_t judge_changeair_time(uint16_t change_time)
{
	uint8_t i;
	for (i = 0; i < CHANGE_AIR_SIZE; ++i)
	{
		if(dev_info.change_air_time[i] == change_time)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void update_dev_status(void)
{
	SetTextValueFloat(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEST_TIME_ID,dev_info.testtime);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_ID,(int32_t)(dev_info.testtemp/1));
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_DECIMAL_ID,(int32_t)(dev_info.testtemp*10)%10);
	SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CHANGE_AIR_TIME,dev_info.flash_setvalue.change_air_time);
}


