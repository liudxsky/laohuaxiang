#include "./screen/screenFunc.h"
#include "./flash/deviceinfo.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
extern dev_info_t dev_info;
extern TextValueTab  textvalue;			//text control_id buff 
extern BIG_SCREEN_ID_TAB biglanguage_screen;
uint8_t passwordwrongflag = 0;
uint8_t global_str_temp[COMMONSIZE];

extern struct mainIconStruct mainIcon;
extern struct mainTextStruct mainPageText;
extern struct argSetErrorStruct argSetErrorIcon;
extern struct IOStatusStruct IOStatus;

void changeAirTimes(uint16_t control_id,uint8_t *str)
{
	switch (control_id)
		{
			case BIG_CHANGE_AIR_TIME_SET_0:
				memset(textvalue.change_air_times[0],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[0],str,sizeof(char)*4);
				dev_info.change_air_time[0] = atoi(textvalue.change_air_times[0]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_5:
				memset(textvalue.change_air_times[1],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[1],str,sizeof(char)*4);
				dev_info.change_air_time[1] = atoi(textvalue.change_air_times[1]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_10:
				memset(textvalue.change_air_times[2],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[2],str,sizeof(char)*4);
				dev_info.change_air_time[2] = atoi(textvalue.change_air_times[2]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_15:
				memset(textvalue.change_air_times[3],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[3],str,sizeof(char)*4);
				dev_info.change_air_time[3] = atoi(textvalue.change_air_times[3]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_20:
				memset(textvalue.change_air_times[4],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[4],str,sizeof(char)*4);
				dev_info.change_air_time[4] = atoi(textvalue.change_air_times[4]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_25:
				memset(textvalue.change_air_times[5],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[5],str,sizeof(char)*4);
				dev_info.change_air_time[5] = atoi(textvalue.change_air_times[5]);
				break;
		    case BIG_CHANGE_AIR_TIME_SET_30:
				memset(textvalue.change_air_times[6],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[6],str,sizeof(char)*4);
				dev_info.change_air_time[6] = atoi(textvalue.change_air_times[6]);
				break;
		    case BIG_CHANGE_AIR_TIME_SET_35:
				memset(textvalue.change_air_times[7],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[7],str,sizeof(char)*4);
				dev_info.change_air_time[7] = atoi(textvalue.change_air_times[7]);
				break;
		    case BIG_CHANGE_AIR_TIME_SET_40:
				memset(textvalue.change_air_times[8],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[8],str,sizeof(char)*4);
				dev_info.change_air_time[8] = atoi(textvalue.change_air_times[8]);
				break;
		  	case BIG_CHANGE_AIR_TIME_SET_45:
				memset(textvalue.change_air_times[9],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[9],str,sizeof(char)*4);
				dev_info.change_air_time[9] = atoi(textvalue.change_air_times[9]);
				break;

		  	case BIG_CHANGE_AIR_TIME_SET_50:
				memset(textvalue.change_air_times[10],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[10],str,sizeof(char)*4);
				dev_info.change_air_time[10] = atoi(textvalue.change_air_times[10]);
				break;

		  	case BIG_CHANGE_AIR_TIME_SET_55:
				memset(textvalue.change_air_times[11],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[11],str,sizeof(char)*4);
				dev_info.change_air_time[11] = atoi(textvalue.change_air_times[11]);
				break;
		 	case BIG_CHANGE_AIR_TIME_SET_60:
				memset(textvalue.change_air_times[12],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[12],str,sizeof(char)*4);
				dev_info.change_air_time[12] = atoi(textvalue.change_air_times[12]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_65:
				memset(textvalue.change_air_times[13],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[13],str,sizeof(char)*4);
				dev_info.change_air_time[13] = atoi(textvalue.change_air_times[13]);
				break;	
			case BIG_CHANGE_AIR_TIME_SET_70:
				memset(textvalue.change_air_times[14],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[14],str,sizeof(char)*4);
				dev_info.change_air_time[14] = atoi(textvalue.change_air_times[14]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_75:
				memset(textvalue.change_air_times[15],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[15],str,sizeof(char)*4);
				dev_info.change_air_time[15] = atoi(textvalue.change_air_times[15]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_80:
				memset(textvalue.change_air_times[16],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[16],str,sizeof(char)*4);
				dev_info.change_air_time[16] = atoi(textvalue.change_air_times[16]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_85:
				memset(textvalue.change_air_times[17],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[17],str,sizeof(char)*4);
				dev_info.change_air_time[17] = atoi(textvalue.change_air_times[17]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_90:
				memset(textvalue.change_air_times[18],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[18],str,sizeof(char)*4);
				dev_info.change_air_time[18] = atoi(textvalue.change_air_times[18]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_95:
				memset(textvalue.change_air_times[19],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[19],str,sizeof(char)*4);
				dev_info.change_air_time[19] = atoi(textvalue.change_air_times[19]);
				break;
			case BIG_CHANGE_AIR_TIME_SET_100:
				memset(textvalue.change_air_times[20],0,sizeof(char)*4);
				memcpy(textvalue.change_air_times[20],str,sizeof(char)*4);
				dev_info.change_air_time[20] = atoi(textvalue.change_air_times[20]);
				break;			
			
			default:
				break;
		}
		dev_info.flash_setvalue.change_max_time = max_change_air(dev_info.change_air_time);
		//argSetText.change_max_time
		dev_info.dev_status_changed_flag = 1;
}
void menuPassword(uint16_t control_id, uint8_t *str)
{
	memset(textvalue.menu_password,0,sizeof(char)*PASSWORDLENGTH);
		memcpy(textvalue.menu_password,str,sizeof(char)*PASSWORDLENGTH);
		if(!strncmp(textvalue.menu_password,dev_info.flash_setvalue.menu_password,PASSWORDLENGTH))
		{
			argSetErrorIcon.pass_update_fail=HIDE;
			setCurrentScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);
			//???
			
		}
		else
		{
			argSetErrorIcon.pass_update_fail=SHOW;
			setCurrentScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
			passwordwrongflag = 1;	
		}
}
void adjustProtectPassword(uint16_t control_id,uint8_t *str)
{
	memset(textvalue.protect_password,0,sizeof(char)*PASSWORDLENGTH);
		memcpy(textvalue.protect_password,str,sizeof(char)*PASSWORDLENGTH);
		if(!strncmp(textvalue.protect_password,dev_info.flash_setvalue.protect_password,PASSWORDLENGTH))
		{
			argSetErrorIcon.pass_update_fail=HIDE;
			setCurrentScreen(biglanguage_screen.BIG_ADJUST_PROTECT_SCREEN);
			passwordwrongflag = 0;
		}
		else
		{
			argSetErrorIcon.pass_update_fail=SHOW;
			//AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,SHOW);
			setCurrentScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
			passwordwrongflag = 2;	
		}
}
void autoNoPowerScreen(uint16_t control_id,uint8_t *str)
{
		memset(textvalue.autonopowerpassword,0,sizeof(char)*PASSWORDLENGTH);
		memcpy(textvalue.autonopowerpassword,str,sizeof(char)*PASSWORDLENGTH);
		if(!strncmp(str,dev_info.autonopowerpassword,PASSWORDLENGTH))	
		{
			//have power , control device work normal
			argSetErrorIcon.pass_update_fail=HIDE;
			//AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,HIDE);
			setCurrentScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			passwordwrongflag = 0;
		}
		else
		{
			//control device no work
			argSetErrorIcon.pass_update_fail=SHOW;
			//AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,SHOW);
			setCurrentScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
			passwordwrongflag = 3;
		}
}
void pidSettingScreen(uint16_t control_id,uint8_t *str)
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
		dev_info.dev_status_changed_flag = 1;
}
void menuSettingScreen(uint16_t control_id, uint8_t *str)
{
	uint8_t str_temp[COMMONSIZE];
	double d_temp;
	int i_temp;
	memset(str_temp,0,sizeof(char)*COMMONSIZE);
		switch (control_id)
		{
			case BIG_TEST_TIME_VALUE:	//set time
				
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);
				
				d_temp = atof(str_temp);
				
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEST_TIME_VALUE,str_temp);
				if(mainPageText.test_time < 0.1 || mainPageText.test_time >= 9999.9)
				{
					dev_info.testTime = 0;
					argSetErrorIcon.test_temp_set_fail=SHOW;
					//£¿£¿£¿£¿
					//AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TESTTIME_SET_FAIL,SHOW);
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
				}
				else 
				{
					dev_info.testTime = mainPageText.test_time;
					argSetErrorIcon.test_temp_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				
				break;
			case BIG_TEST_TEMP_VALUE:	//set temp
				if(dev_info.runstatus>0)
					return;
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);	
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEST_TEMP_VALUE,str);			
				d_temp = atof(str_temp);			
				if(d_temp < 50 || d_temp >= 400)
				{
					dev_info.setTemp = 0;
					argSetErrorIcon.test_temp_set_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
				}
				else
				{
					dev_info.setTemp = d_temp;
					argSetErrorIcon.test_temp_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				
				break;	
			case BIG_TEMP_RETURN_DIFF:
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEMP_RETURN_DIFF,str);			
				d_temp = atof(str_temp);
				if(d_temp < 0.1 || d_temp > 20)
				{
					dev_info.temp_backdiff = 0;
					argSetErrorIcon.return_diff_set_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
				}
				else
				{
					dev_info.temp_backdiff = d_temp;
					argSetErrorIcon.return_diff_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				
				break;
				
			case BIG_WARNING1_UP_VALUE:
				
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING1_UP_VALUE,str);
				d_temp = atof(str_temp);
				
				if(d_temp < 0.1 || d_temp > 50)
				{
					dev_info.flash_setvalue.warning1_up = 0;
					argSetErrorIcon.temp_up_set_fail=SHOW;
					
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
				}
				else
				{
					dev_info.flash_setvalue.warning1_up = d_temp;
					argSetErrorIcon.temp_up_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				
				break;	
			case BIG_WARNING2_UP_VALUE:
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING2_UP_VALUE,str);
				d_temp = atof(str_temp);
				
				if(d_temp < 0.1 || d_temp > 50)
				{
					dev_info.flash_setvalue.warning2_up = 0;
					argSetErrorIcon.temp_up2_set_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
				}
				else
				{
					dev_info.flash_setvalue.warning2_up = d_temp;
					argSetErrorIcon.temp_up2_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
			
				break;	
				
			case BIG_NEW_PASSWORD:
				memcpy(global_str_temp,str,sizeof(char)*PASSWORDLENGTH);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_NEW_PASSWORD,str);
//				printf("password is :%s\r\n",str);
				break;
			case BIG_SECOND_INPUT_PASSWORD:
				memcpy(str_temp,str,sizeof(char)*PASSWORDLENGTH);
//				printf("second password is :%s\r\n",str);
				if(strncmp(global_str_temp,str_temp,PASSWORDLENGTH))
				{
//					printf("second password is OK!\r\n");
					argSetErrorIcon.pass_update_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);	
				}
				else
				{
					memcpy(dev_info.flash_setvalue.menu_password,str_temp,sizeof(char)*COMMONSIZE);
					argSetErrorIcon.pass_update_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				
				break;
			case BIG_CHANGE_AIR_TIME_SET:
				//in menu, change air set
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);
				i_temp= atoi(str_temp);
				if(judge_changeair_time(i_temp))
				{
					argSetErrorIcon.change_air_set_fail=HIDE;
					dev_info.flash_setvalue.change_air_time = i_temp;
					dev_info.airdooropenangle = getChangeAirTimes(i_temp);
					dev_info.dev_status_changed_flag = 1;
				}
				else
				{
					argSetErrorIcon.change_air_set_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);	
				}
				
				break;
			case BIG_SCREEN_BRIGHT_ADJUST:
				memcpy(str_temp ,str,sizeof(char)*4);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_SCREEN_BRIGHT_ADJUST,str);
				i_temp = atoi(str_temp);
				
				if(i_temp > 0 && i_temp <= 100)
				{
					if (i_temp > 100) i_temp = 100;
					dev_info.flash_setvalue.screen_light_value = i_temp;
					SetBackLight(200 - i_temp*2);
					argSetErrorIcon.light_set_fail=HIDE;
				}
				else
				{
					argSetErrorIcon.light_set_fail=SHOW;
				}
				dev_info.dev_status_changed_flag = 1;
				break;
			
			case BIG_MODBUS_NODE_ADDRESS:
				memcpy(str_temp,str,sizeof(char)*COMMONSIZE);
				SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_MODBUS_NODE_ADDRESS,str);
				i_temp = atoi(str_temp);
				if(i_temp < 1 || i_temp > 32)
				{
					
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);	
					argSetErrorIcon.modebus_addr_set_fail=SHOW;
					
				}
				else
				{
					dev_info.flash_setvalue.modbus_address = i_temp;
					argSetErrorIcon.modebus_addr_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				break;
				
		}
}
void deviceTimeSet(uint16_t control_id,uint8_t *str)
{
	switch (control_id)
		{
			case BIG_CONTROL_DATE_YEAR_SET:
				memset(textvalue.device_time_setting.Year,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Year,str,sizeof(char)*COMMONSIZE);	
				dev_info.timenow.Year = atoi(textvalue.device_time_setting.Year);
				dev_info.timenow.Year = DectoBCD(dev_info.timenow.Year);
				break;
			case BIG_CONTROL_DATE_MONTH_SET:
				memset(textvalue.device_time_setting.Mon,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Mon,str,sizeof(char)*COMMONSIZE);
				dev_info.timenow.Mon = atoi(textvalue.device_time_setting.Mon);
				dev_info.timenow.Mon = DectoBCD(dev_info.timenow.Mon);
				break;	
			case BIG_CONTROL_DATE_DAY_SET:
				memset(textvalue.device_time_setting.Day,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Day,str,sizeof(char)*COMMONSIZE);
				dev_info.timenow.Day = atoi(textvalue.device_time_setting.Day);
				dev_info.timenow.Day = DectoBCD(dev_info.timenow.Day);
				break;	
			case BIG_CONTROL_TIME_HOUR_SET:
				memset(textvalue.device_time_setting.Hour,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Hour,str,sizeof(char)*COMMONSIZE);
				dev_info.timenow.Hour = atoi(textvalue.device_time_setting.Hour);
				dev_info.timenow.Hour = DectoBCD(dev_info.timenow.Hour);
				break;
			case BIG_CONTROL_TIME_MINUTE_SET:
				memset(textvalue.device_time_setting.Min,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Min,str,sizeof(char)*COMMONSIZE);
				dev_info.timenow.Min = atoi(textvalue.device_time_setting.Min);
				dev_info.timenow.Min = DectoBCD(dev_info.timenow.Min);
				break;	
			case BIG_CONTROL_TIME_SECOND_SET:
				memset(textvalue.device_time_setting.Sec,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.device_time_setting.Sec,str,sizeof(char)*COMMONSIZE);
				dev_info.timenow.Sec = atoi(textvalue.device_time_setting.Sec);
				dev_info.timenow.Sec = DectoBCD(dev_info.timenow.Sec);
				break;	
//			default:
//				break;
		}
		SetRtcTime(dev_info.timenow.Sec,dev_info.timenow.Min,dev_info.timenow.Hour,dev_info.timenow.Day,0,dev_info.timenow.Mon,dev_info.timenow.Year);
}
void adjustScreenSetting(uint16_t control_id,uint8_t *str)
{
	uint8_t str_temp[COMMONSIZE];
	int i_temp;
	float32_t  data_temp = 0;
	AutoNoPowerTime  nopowertime = {1985,12,12};			
	switch (control_id)
		{
			case BIG_ADJUST_PASSWORD1:
				memcpy(global_str_temp,str,sizeof(char)*PASSWORDLENGTH);
				SetTextValue(biglanguage_screen.BIG_ADJUST_SCREEN,BIG_ADJUST_PASSWORD1,str);
				break;
			case BIG_ADJUST_PASSWORD2:
				memset(str_temp,0,sizeof(char)*PASSWORDLENGTH);
				memcpy(str_temp,str,sizeof(char)*PASSWORDLENGTH);
				SetTextValue(biglanguage_screen.BIG_ADJUST_SCREEN,BIG_ADJUST_PASSWORD2,str);
				if(strncmp(global_str_temp,str_temp,PASSWORDLENGTH))
				{
					argSetErrorIcon.pass2_set_fail=SHOW;
					MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2);	
				}
				else
				{
					memcpy(dev_info.flash_setvalue.protect_password,str_temp,sizeof(char)*PASSWORDLENGTH);
					argSetErrorIcon.pass2_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				break;
			case BIG_AIR_DOOR_ANGLE_SET:
				memset(textvalue.airdoor_value,0,sizeof(char)*4);
				memcpy(textvalue.airdoor_value,str,sizeof(char)*4);
				SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,str);
				SetTextValue(biglanguage_screen.BIG_ADJUST_SCREEN,BIG_AIR_DOOR_ANGLE_SET,str);
				i_temp = atoi(textvalue.airdoor_value);
				if (i_temp >= 0 && i_temp <= 100)
				{
					Dac1_Set_Vol(33*i_temp);
					dev_info.flash_setvalue.air_door_angle = i_temp;
					argSetErrorIcon.air_angle_set_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				else
				{
					argSetErrorIcon.air_angle_set_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2);
				}
				break;
			case BIG_TEMP_VALUE_REVUSE_SET:
				memset(textvalue.temp_adjust_value,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.temp_adjust_value,str,sizeof(char)*COMMONSIZE);
				data_temp = =  atof(textvalue.temp_adjust_value);
				if(data_temp >= -20 && data_temp <= 20)
				{
					dev_info.flash_adjusttemp =  data_temp;
					argSetErrorIcon.temp_adjust_fail=HIDE;
					dev_info.dev_status_changed_flag = 1;
				}
				else
				{
					argSetErrorIcon.temp_adjust_fail=SHOW;
					setCurrentScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2);
				}
				break;
			case BIG_YEAR_SET:
				memset(textvalue.autotime.year,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.autotime.year,str,sizeof(char)*COMMONSIZE);
				nopowertime.year = atoi(textvalue.autotime.year);
				nopowertime.year = DectoBCD(nopowertime.year);
				dev_info.autonopowertime.Year = nopowertime.year; 
				break;
			case BIG_MONTH_SET:
				memset(textvalue.autotime.month,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.autotime.month,str,sizeof(char)*COMMONSIZE);
				nopowertime.month = atoi(textvalue.autotime.month);
				nopowertime.month = DectoBCD(nopowertime.month);
				dev_info.autonopowertime.Mon = nopowertime.month;
				break;	
			case BIG_DAY_SET:
				memset(textvalue.autotime.day,0,sizeof(char)*COMMONSIZE);
				memcpy(textvalue.autotime.day,str,sizeof(char)*COMMONSIZE);
				nopowertime.day = atoi(textvalue.autotime.day);
				nopowertime.day = DectoBCD(nopowertime.day);
				dev_info.autonopowertime.Day = nopowertime.day;
				dev_info.dev_status_changed_flag = 1;
				break;	
		}
}
void readRTCFromScreen(RtcTime t)
{
	dev_info.timenow=t;
}


//merge time str
//void  mergetimechar(RtcTime datetime)
//{
//	uint8_t buff[2] = {0};
//	textvalue.textvaluebuff.start_time[0] = '2';
//	textvalue.textvaluebuff.start_time[1] = '0';
//		
//	sprintf(buff,"%02d",datetime.Year); 
//	textvalue.textvaluebuff.start_time[2] = buff[0];
//	textvalue.textvaluebuff.start_time[3] = buff[1];

//	sprintf(buff,"%02d",datetime.Mon);
//	textvalue.textvaluebuff.start_time[4] = '/';
//	textvalue.textvaluebuff.start_time[5] = buff[0];
//	textvalue.textvaluebuff.start_time[6] = buff[1];

//	sprintf(buff,"%02d",datetime.Day);
//	textvalue.textvaluebuff.start_time[7] = '/';
//	textvalue.textvaluebuff.start_time[8] = buff[0];
//	textvalue.textvaluebuff.start_time[9] = buff[1];
//	
//	textvalue.textvaluebuff.start_time[10] = ' ';

//	sprintf(buff,"%02d",datetime.Hour);
//	textvalue.textvaluebuff.start_time[11] = buff[0];
//	textvalue.textvaluebuff.start_time[12] = buff[1];
//	textvalue.textvaluebuff.start_time[13] = ':';

//	sprintf(buff,"%02d",datetime.Min);
//	textvalue.textvaluebuff.start_time[14] = buff[0];
//	textvalue.textvaluebuff.start_time[15] = buff[1];
//}



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
	memcpy(mainPageText.warmend_time,timebuff,16);
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_END_TIME_ID,mainPageText.warmend_time);		
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
uint16_t writeflashcount = 0;

void addup_testtime(void)
{
	
	if(dev_info.runstatus>0)
	{
		if(starttime_addup.Year==0)//init, dev_info.runstatus pos edge ____|````
		{
			starttime_addup=dev_info.timenow;
			
		}
		nowtimediff_addup=diff_time(starttime_addup, dev_info.timenow)/60;
		if(nowtimediff_addup - lasttimediff_addup)
		{
			dev_info.addup_testtime += 1;
			writeflashcount++;
			lasttimediff_addup = nowtimediff_addup;
			SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_ADDUP_TIME_ID,dev_info.addup_testtime/60);
			if(writeflashcount > 30)
			{
				dev_info.dev_status_changed_flag = 2;
			}
		}
	}
	else//dev_info.runstatus=0
	{
		if(starttime_addup.Year!=0)  // dev_info.runstatus neg edge````|____
		{
			//shutdown action
			starttime_addup.Year=0;
		}
	}
}


void lefttimecalculate(void)
{
		uint32_t elapsedTestTime;	
	uint32_t  leftTime = 0;
	if(dev_info.testTime != 0)
	{
		//already runed time span
		elapsedTestTime = diff_time(dev_info.start_time, dev_info.timenow)/60;//min
		leftTime = dev_info.testTime*60 - elapsedTestTime;//left time,min
		mainPageText.left_time_hour = leftTime/60;
		mainPageText.left_time_min = leftTime%60;
		if(leftTime == 0)
		{
			mainPageText.left_time_hour=0;
			mainPageText.left_time_min=0;
			//dev_info.setTemp = 0;
			dev_info.testTime = 0;
			dev_info.runstatus = 0;
			dev_info.lefttimeflag = 0;
		}
	}
}


uint8_t steadytempcount = 0;	
void start_endtime_set(void)
{
	if(myabs(dev_info.setTemp,dev_info.currentTemp)<=3)
	{
		steadytempcount++;
	}
	else
	{
		steadytempcount = 0;
	}
	if((dev_info.runstatus)&&(dev_info.setTemp != 0)&&(dev_info.lefttimeflag == 0)&&(steadytempcount>=1))
	{	
		dev_info.lefttimeflag = 1;
		dev_info.start_time=dev_info.timenow;
		SetTextValueRTC(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_START_TIME_ID,dev_info.start_time);
		endtimecalcu(dev_info.start_time,dev_info.testTime);//->mainPageText.warmend_time
		dev_info.dev_status_changed_flag = 1;
	}
	if(dev_info.lefttimeflag == 1)
	{
		lefttimecalculate();
	}
}


//find which change air time is selected, will push to main screen
uint8_t getChangeAirTimes(int changeTimes)
{
	uint8_t i = 0;
	for(i = 0;i < CHANGE_AIR_SIZE;i++)
	{
		if(changeTimes == dev_info.change_air_time[i])
		{
			Dac1_Set_Vol(165*i);
			return i;
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
