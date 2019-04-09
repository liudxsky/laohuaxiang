#include "./screen/screenFunc.h"
#include "./flash/deviceinfo.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "main.h"

extern dev_info_t dev_info;
extern TextValueTab  textvalue;			//text control_id buff 
extern BIG_SCREEN_ID_TAB biglanguage_screen;
uint8_t global_str_temp[COMMONSIZE];
RtcTime  nopowertime = {28,12,12,0,0,0};				

extern struct mainIconStruct mainIcon;
extern struct mainTextStruct mainPageText;
extern struct argSetErrorStruct argSetErrorIcon;
extern struct IOStatusStruct IOStatus;

/************************************BUTTON FUNC***********************************************************/

void pidScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	switch (control_id)
	{
		case BIG_SELF_ADJUST:							//self adjust
			dev_info.runstatus=2;
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			break;
		case BIG_PID_RETURN_BUTTON: 				
			//this should be in dev_info or screen status
			MySetScreen(biglanguage_screen.BIG_ADJUST_SCREEN);
			break;
	}
}

void menuScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	switch (control_id)
		{
			case BIG_SET_RETURN_BUTTON:
				MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
				break;
			case BIG_ADJUST_BUTTON:
				MySetScreen(biglanguage_screen.BIG_ADJUST_PROTECT_SCREEN);
				break;
			case BIG_BPS_1200_BUTTON:
				if(!state)
				{
					dev_info.modbus_tran_rate = 1200;
					AnimationPlayFrame(screen_id,BIG_BPS_1200,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
					eMBInit( MB_RTU, dev_info.Modbus_address, 0x01, dev_info.modbus_tran_rate, MB_PAR_NONE ); //Modbus Init
				}
				break;
			case BIG_BPS_2400_BUTTON:
				if(!state)
				{
					dev_info.modbus_tran_rate = 2400;
					AnimationPlayFrame(screen_id,BIG_BPS_2400,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
					eMBInit( MB_RTU, dev_info.Modbus_address, 0x01, dev_info.modbus_tran_rate, MB_PAR_NONE ); //Modbus Init
				}
				break;
			case BIG_BPS_4800_BUTTON:
				if(!state)
				{
					dev_info.modbus_tran_rate = 4800;
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
					eMBInit( MB_RTU, dev_info.Modbus_address, 0x01, dev_info.modbus_tran_rate, MB_PAR_NONE ); //Modbus Init
				}
				break;
			case BIG_BPS_9600_BUTTON:
				if(!state)
				{
					dev_info.modbus_tran_rate = 9600;
					AnimationPlayFrame(screen_id,BIG_BPS_9600,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					eMBInit( MB_RTU, dev_info.Modbus_address, 0x01, dev_info.modbus_tran_rate, MB_PAR_NONE ); //Modbus Init
				}
				break;
			case BIG_CHINESE_LANGUAGE_BUTTON:
				if(!state)
				{
					dev_info.biglanguagestatus = 1;
					AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,HIDE);
					AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,SHOW);
				}
				check_language_select();
				MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);
				break;
			case BIG_ENGLISH_LANGUAGE_BUTTON:
				if(!state)
				{
					dev_info.biglanguagestatus = 0;
					AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,SHOW);
					AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,HIDE);
				}
				check_language_select();
				MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);
				break;
			default:
				dev_info.modbus_tran_rate = 9600;
				dev_info.biglanguagestatus = 1;
		}
		dev_info.dev_status_changed_flag = 1;

}

void mainShowScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	switch (control_id)
		{
			case BIG_MENU_ID:
				MySetScreen(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN);
				break;
			case BIG_TIME_RECORD_ID:
				MySetScreen(biglanguage_screen.BIG_TIME_RECORD_SCREEN);
				break;
			case BIG_SAMPLE_FRAME_MENU_ID:
				if(state)
				{
					//should chage dev_info, and update to screen status
					IOStatus.sample_frame=1;
					mainIcon.sample_frame=SHOW;
					mainIcon.rr_work_status=HIDE;
					//SPINNER_RACK_ON;
				}
				else
				{
					//SPINNER_RACK_OFF;
					IOStatus.sample_frame=0;
					mainIcon.sample_frame=HIDE;
					mainIcon.rr_work_status=SHOW;
				}

				break;
			case BIG_BLOWER_MENU_ID:
				if(state)
				{
					IOStatus.blower = 1;
					mainIcon.fan_operation=SHOW;
					mainIcon.fr_work_status=HIDE;
				}
				else
				{
					IOStatus.blower = 0;
					mainIcon.fan_operation=HIDE;
					mainIcon.fr_work_status=SHOW;	
				}
				break;
			case BIG_START_OR_PAUSE_ID:
				if(dev_info.runstatus>0)
				{
					SetPwmValue(0);
					IOStatus.heat_output = 0;
					dev_info.runstatus = 0;
					dev_info.lefttimeflag=0;
				}
				else
				{
					IOStatus.heat_output = 1;
					dev_info.runstatus = 1;			
				}
				break;
			default:
				break;
		}

}

void passwordInputWrongScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	if(dev_info.passwordwrongflag == 1)
	{
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);	
	}
	else if(dev_info.passwordwrongflag == 2)
	{
		MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);
	}
	else if(dev_info.passwordwrongflag == 3)
	{
		MySetScreen(biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER);
	}
}



void adjustScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	switch (control_id)
	{
		case BIG_NO_POWER_RETURN_BUTTON:
			MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);	
			break;
		case BIG_EDIT_BUTTON:
			MySetScreen(biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN);
			break;
		case BIG_PID_BUTTON:
			MySetScreen(biglanguage_screen.BIG_PID_SET_SCREEN);
			break;
		default:
			break;
	}
}


/***********************************************************************************************************************/

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
		MySetScreen(biglanguage_screen.BIG_PARAM_SET_SCREEN);
	}
	else
	{
		MySetScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
		dev_info.passwordwrongflag = 1;	
	}
}
void adjustProtectPassword(uint16_t control_id,uint8_t *str)
{
	memset(textvalue.protect_password,0,sizeof(char)*PASSWORDLENGTH);
	memcpy(textvalue.protect_password,str,sizeof(char)*PASSWORDLENGTH);
	if(!strncmp(textvalue.protect_password,dev_info.flash_setvalue.protect_password,PASSWORDLENGTH))
	{
		MySetScreen(biglanguage_screen.BIG_ADJUST_SCREEN);
		dev_info.passwordwrongflag = 0;
	}
	else
	{
		MySetScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
		dev_info.passwordwrongflag = 2;	
	}
}
void autoNoPowerScreen(uint16_t control_id,uint8_t *str)
{
		memset(textvalue.autonopowerpassword,0,sizeof(char)*PASSWORDLENGTH);
		memcpy(textvalue.autonopowerpassword,str,sizeof(char)*PASSWORDLENGTH);
		if(!strncmp(str,dev_info.autonopowerpassword,PASSWORDLENGTH))	
		{
			//have power , control device work normal
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			dev_info.autonopowertime.Year = 50;
			dev_info.autonopowertimeflag = 0;
			dev_info.passwordwrongflag = 0;
		}
		else
		{
			//control device no work
			MySetScreen(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN);
			dev_info.passwordwrongflag = 3;
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
			if(d_temp < 0.1 || d_temp >= 9999.9)
			{
				argSetErrorIcon.test_temp_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
			}
			else 
			{
				dev_info.testTime = d_temp;
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
			if(d_temp < 50 || d_temp >= 400)//debug
			{
//				dev_info.setTemp = 0;
				argSetErrorIcon.test_temp_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
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
//				dev_info.temp_backdiff = 0;
				argSetErrorIcon.return_diff_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
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
//				dev_info.flash_setvalue.warning1_up = 0;
				argSetErrorIcon.temp_up_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
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
				argSetErrorIcon.temp_up2_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);
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
			break;
		case BIG_SECOND_INPUT_PASSWORD:
			memcpy(str_temp,str,sizeof(char)*PASSWORDLENGTH);
			if(strncmp(global_str_temp,str_temp,PASSWORDLENGTH))
			{
				argSetErrorIcon.pass1_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);	
			}
			else
			{
				memcpy(dev_info.flash_setvalue.menu_password,str_temp,sizeof(char)*COMMONSIZE);
				argSetErrorIcon.pass1_set_fail=HIDE;
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
				dev_info.airdooropenangle = 5*getChangeAirTimes(i_temp);
				dev_info.dev_status_changed_flag = 1;
			}
			else
			{
				argSetErrorIcon.change_air_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);	
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
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN1);	
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
			memcpy(textvalue.device_time_setting.Year,str+2,sizeof(char)*COMMONSIZE);	
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
	}
	SetRtcTime(dev_info.timenow.Sec,dev_info.timenow.Min,dev_info.timenow.Hour,dev_info.timenow.Day,0,dev_info.timenow.Mon,dev_info.timenow.Year);
}
void adjustScreenSetting(uint16_t control_id,uint8_t *str)
{
	uint8_t str_temp[COMMONSIZE];
	int i_temp;
	float  data_temp = 0;	
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
				dev_info.airdooropenangle = i_temp;
				argSetErrorIcon.air_angle_set_fail=HIDE;
				//dev_info.dev_status_changed_flag = 1;
			}
			else
			{
				argSetErrorIcon.air_angle_set_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2);
			}
			break;
		case BIG_TEMP_VALUE_REVUSE_SET:
			memset(textvalue.temp_adjust_value,0,sizeof(char)*COMMONSIZE);
			memcpy(textvalue.temp_adjust_value,str,sizeof(char)*COMMONSIZE);
			data_temp =  atof(textvalue.temp_adjust_value);
			if(data_temp >= -20 && data_temp <= 20)
			{
				dev_info.flash_adjusttemp =  data_temp;
				argSetErrorIcon.temp_adjust_fail=HIDE;
				dev_info.dev_status_changed_flag = 1;
			}
			else
			{
				argSetErrorIcon.temp_adjust_fail=SHOW;
				MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2);
			}
			break;
		case BIG_YEAR_SET://bug here, but I don't understand
			memset(textvalue.autotime.year,0,sizeof(char)*COMMONSIZE);
			memcpy(textvalue.autotime.year,str+2,sizeof(char)*COMMONSIZE);
			nopowertime.Year = atoi(textvalue.autotime.year);
			break;
		case BIG_MONTH_SET:
			memset(textvalue.autotime.month,0,sizeof(char)*COMMONSIZE);
			memcpy(textvalue.autotime.month,str,sizeof(char)*COMMONSIZE);
			nopowertime.Mon = atoi(textvalue.autotime.month);
			break;	
		case BIG_DAY_SET:
			memset(textvalue.autotime.day,0,sizeof(char)*COMMONSIZE);
			memcpy(textvalue.autotime.day,str,sizeof(char)*COMMONSIZE);
			nopowertime.Day = atoi(textvalue.autotime.day);
			break;	
	}
	if(diff_time(dev_info.timenow, nopowertime) > 0)
	{
		dev_info.autonopowertime.Year = nopowertime.Year;
		dev_info.autonopowertime.Mon = nopowertime.Mon;
		dev_info.autonopowertime.Day = nopowertime.Day;
		dev_info.dev_status_changed_flag = 1;
		argSetErrorIcon.auto_no_power_set_fail=HIDE;
	}
	else
	{
		argSetErrorIcon.auto_no_power_set_fail=SHOW;
		MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN2);
	}
	
}



//to time total what minutes,start time is : 2000\1\1 0:00
uint32_t to_day(RtcTime time)
{
	uint8_t mon[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	uint16_t day = 0;
	uint32_t hour = 0,minute = 0,seconds = 0;
	uint16_t i;
	time.Year = time.Year + 2000;
//	printf("year is %d\n",time.Year);
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



int32_t diff_time(RtcTime starttime,RtcTime endtime)
{
	int32_t diff=0;
//	printf("%4d/%2d/%2d,%2d:%2d:%2d",endtime.Year,endtime.Mon,endtime.Day,endtime.Hour,endtime.Min,endtime.Sec);
//	printf("starttime is %x \n  end time is   %x\n",to_day(starttime),to_day(endtime));
	diff=to_day(endtime)-to_day(starttime);
//	printf("diff is %.8x \n",diff);
	return diff;
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
	if((dev_info.runstatus==1)&&(dev_info.setTemp != 0)&&(dev_info.lefttimeflag == 0)&&(steadytempcount>=120))
	{	
		dev_info.lefttimeflag = 1;
		dev_info.start_time=dev_info.timenow;
		SetTextValueRTC(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_START_TIME_ID,dev_info.start_time);
		if(!dev_info.testTime)
		{
			SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_END_TIME_ID,"......");
//			dev_info.end_time.Year=0;
		}
		else
		{
			endtimecalcu(dev_info.start_time,dev_info.testTime);//->mainPageText.warmend_time
		}
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
		
			return i;
		}
	}
}

uint8_t max_change_air(uint8_t *buff)
{
	uint8_t i = 0;
	uint8_t temp = buff[0];
	for(i = 1;i<CHANGE_AIR_SIZE;i++)
	{
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

//int to str
void my_itoa(long i, char *string, uint8_t num)
{
	int power = 0, j = 0;
 
	j = i;
	for (power = 1; j>10; j /= 10)
	{
		power *= 10;
	}
	for (; power>0 && num > 0; power /= 10)
	{
		*string++ = '0' + i / power;
		num --;
		i %= power;
	}
	*string = '\0';
}


