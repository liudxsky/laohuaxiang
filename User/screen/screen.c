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

uint16_t warmflag = 0;
uint8_t SCREENSIZE = 1;
uint8_t cmd_buffer[CMD_MAX_SIZE];		//指令缓存
uint8_t press_flag = 0,touch_flag = 0;
uint8_t heattimes = 0;
uint8_t change_air_time[19] = {0};
static char Month[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
uint32_t  timercount = 0;


extern RtcTime rtctime;
extern uint16_t current_screen_id,last_screen_id;
extern uint32_t _ms_counter;
extern float Ktemperature; 
extern dev_info_t dev_info;
extern uint8_t pwmgpiostatus;
extern uint16_t gpiostatus;
extern uint8_t thermocouple_flag;


uint8_t slidervalue = 0;			//滑动进度条值
uint16_t autonopowerpassword = 0;	//自动断电恢复密码


ID_Table idtable;					//screen size id 
float adjusttemp1 = 0,adjusttemp2 = 0;				//温度值校正
RtcTime rtctime;					//RTC控制时间
AutoNoPowerTime  nopowertime;		//自动断电时间	

PCTRL_MSG msg;						//串口屏发送信息
TextValueTab  textvalue;			//文本控件保存值
MainShowTextValue	showtextvalue;	//主页面文本控件缓存值
CoilValue  coilvalue;				//设置界面值

Touch_Coord touch_press,touch_up;

Touch_Times touchtimes = {0,0};	//button is pressed times



BIG_SCREEN_ID_TAB bigchinese_screen = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
BIG_SCREEN_ID_TAB bigenglish_screen = {0};
BIG_SCREEN_ID_TAB biglanguage_screen = {0};


SMALL_SCREEN_ID_TAB smallchinese_screen = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
SMALL_SCREEN_ID_TAB smallenglish_screen = {0};
SMALL_SCREEN_ID_TAB smalllanguage_screen = {0};



void SetTextValueInt32(uint16_t screen_id, uint16_t control_id,int32_t value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%ld",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%.1f",value);//把浮点数转换为字符串(保留一位小数)
	SetTextValue(screen_id,control_id,buffer);
}

//check current screen size 
void check_screen_size(void)
{
	SCREENSIZE = GPIO_ReadOutputDataBit(GPIOG,GPIO_Pin_0);		//check gpio to confirm screen size
	if(SCREENSIZE)
	{
		idtable.screen_size = BIG_SIZE;			//5 inch screen 
	}
	else
	{
		idtable.screen_size = SMALL_SIZE;		//4.3 inch screen
	}	
}


//screen language select
void screenlanguage(void)
{
	if(SCREENSIZE)
	{
		biglanguage_screen = bigchinese_screen;			//5 inch screen 
		current_screen_id = 16;
	}
	else
	{
		smalllanguage_screen = smallchinese_screen; 	//4.3 inch screen
		
	}
}

//screen init
void screen_init(void)
{
	RtcTime inittime = {0};
	if(SCREENSIZE)
	{
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
		
		sprintf(textvalue.coilsavevalue.modbus_address,"%d",1);

		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_ID,"0 h 0 min");	
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_START_TIME_ID,textvalue.textvaluebuff.start_time);
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_END_TIME_ID,textvalue.textvaluebuff.start_time);

		textvalue.coilsavevalue.modbus_tran_rate[0] = 1;
		textvalue.coilsavevalue.modbus_tran_rate[1] = 1;
		textvalue.coilsavevalue.modbus_tran_rate[2] = 1;
		textvalue.coilsavevalue.modbus_tran_rate[3] = 0;
		
		textvalue.coilsavevalue.menu_language[0] = 0;
		textvalue.coilsavevalue.menu_language[1] = 1;

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
		AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TEMP1_UP_SET_FAIL,HIDE);
		delay_ms(100);
		AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_TEMP1_DOWN_SET_FAIL,HIDE);
		delay_ms(100);
		AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_CHANGE_AIR_SET_FAIL,HIDE);
		delay_ms(100);
		AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_MODBUS_ADDRESS_SET_FAIL,HIDE);
		
		sprintf(textvalue.coilsavevalue.menu_password,"%06d",123456);
	}
	else
	{
		smalllanguage_screen = smallchinese_screen; 	//4.3 inch screen
		
	}
		
}

//十进制转BCD
uint8_t DectoBCD(uint8_t Dec)
{
     return ((((Dec/10)<<4)&0xf0)|(Dec%10));
}
//BCD转十进制
uint8_t BcdToDec(uint8_t bcd)
{
	return (0xff & (bcd>>4))*10 +(0xf & bcd);
}

/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16_t screen_id)
{
	uint8_t i = 0;
	current_screen_id = screen_id;
//	printf("current screen id is %d \n",current_screen_id);
	if(SCREENSIZE)
	{
		//按键页面
		if(screen_id == biglanguage_screen.BIG_KEYBOARD)
		{
		}
		//换气次数编辑界面
		if(screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
		{
			for (i= 0; i < 19; ++i)
			{
				SetTextValue(screen_id,BIG_CHANGE_AIR_TIME_SET_90+i,textvalue.change_air_times[i]);
			}
		}
		//风门角度控制调节界面
		if(screen_id == biglanguage_screen.BIG_AIR_DOOR_SCREEN)
		{
			SetTextValue(screen_id,BIG_AIR_DOOR_ANGLE_SET,textvalue.airdoor_value);
		}
		//屏幕亮度
		if(screen_id == biglanguage_screen.BIG_BRIGHT_ADJUST_SCREEN)
		{
		}
		//菜单密码
		if(screen_id == biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN)
		{
			SetTextValue(screen_id,BIG_PASSWORD_PROTECT_INPUT,"");
		}
		//PID
		if(screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)
		{
			SetTextValue(screen_id,BIG_P_VALUE_SET,textvalue.Pidvalue[0]);

			SetTextValue(screen_id,BIG_I_VALUE_SET,textvalue.Pidvalue[1]);

			SetTextValue(screen_id,BIG_D_VALUE_SET,textvalue.Pidvalue[2]);	
		}
		//参数设置界面
		if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
		{
			SetTextValueInt32(screen_id,BIG_TEST_TIME_VALUE,showtextvalue.test_time );
			
			SetTextValueInt32(screen_id,BIG_TEST_TEMP_VALUE,showtextvalue.setting_temp);
		
			SetTextValue(screen_id,BIG_WARNING1_UP_VALUE,textvalue.coilsavevalue.warning1_up);
		
			SetTextValue(screen_id,BIG_WARNING1_DOWN_VALUE,textvalue.coilsavevalue.warning1_down);

			SetTextValue(screen_id,BIG_WARNING2_UP_VALUE,textvalue.coilsavevalue.warning2_up);

			SetTextValue(screen_id,BIG_WARNING2_DOWN_VALUE,textvalue.coilsavevalue.warning2_down);
			
			SetTextValue(screen_id,BIG_NEW_PASSWORD,"");
			
			SetTextValue(screen_id,BIG_SECOND_INPUT_PASSWORD,"");
			
			SetTextValue(screen_id,BIG_CHANGE_AIR_TIME_SET,textvalue.coilsavevalue.change_air_time);
		
			SetTextValue(screen_id,BIG_CHANGE_AIR_MAX_SET,textvalue.coilsavevalue.change_max_time);
		
			SetTextValue(screen_id,BIG_MODBUS_NODE_ADDRESS,textvalue.coilsavevalue.modbus_address);
			
			AnimationPlayFrame(screen_id,BIG_BPS_1200,textvalue.coilsavevalue.modbus_tran_rate[0]);
			AnimationPlayFrame(screen_id,BIG_BPS_2400,textvalue.coilsavevalue.modbus_tran_rate[1]);
			AnimationPlayFrame(screen_id,BIG_BPS_4800,textvalue.coilsavevalue.modbus_tran_rate[2]);
			AnimationPlayFrame(screen_id,BIG_BPS_9600,textvalue.coilsavevalue.modbus_tran_rate[3]);

			AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,textvalue.coilsavevalue.menu_language[0]);
			AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,textvalue.coilsavevalue.menu_language[1]);
		}
		//自动断电时间设定
		if(screen_id == biglanguage_screen.BIG_AUTO_POWEROFF_TIMESET_SCREEN)
		{	
			SetTextValue(screen_id,BIG_YEAR_SET,textvalue.autotime.year);
		
			SetTextValue(screen_id,BIG_MONTH_SET,textvalue.autotime.month);
		
			SetTextValue(screen_id,BIG_DAY_SET,textvalue.autotime.day);
		}
		//温度值校正
		if(screen_id == biglanguage_screen.BIG_TEMP_VALUE_REVISE_SCREEN)
		{
		}
		//参数设置无效界面
		if(screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN)
		{

		}
		//设备时间设置
		if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
		{	
		}
		//温度曲线显示界面
		if(screen_id == biglanguage_screen.BIG_TEMP_CURVE_SHOW_SCREEN)
		{
		}
		//自检界面
		if(screen_id == biglanguage_screen.BIG_SELF_TEST_SCREEN)
		{
		}
	}
	else
	{
		//light adjust screen
		if(screen_id == smalllanguage_screen.SMALL_BRIGHT_ADJUST_SCREEN)	
		{
		}
		//selfcheck no pass
		if(screen_id == smalllanguage_screen.SMALL_SELF_TEST_NOTPASS_SCREEN)	
		{
		}
		//password protect
		if(screen_id == smalllanguage_screen.SMALL_SCREAT_PROTECT_SCREEN)
		{
		}
		//auto break power
		if(screen_id == smalllanguage_screen.SMALL_AUTO_POWEROFF_POPUPWINDOWS_SCREEN)
		{
		}
		//auto break power time setting
		if(screen_id == smalllanguage_screen.SMALL_AUTO_POWEROFF_TIMESET_SCREEN)
		{
		}
		//temperature value revise
		if(screen_id == smalllanguage_screen.SMALL_TEMP_VALUE_REVISE_SCREEN)
		{
		}
		//function of self check
		if(screen_id == smalllanguage_screen.SMALL_SELF_TEST_SCREEN)
		{
		}
		//device time setting
		if(screen_id == smalllanguage_screen.SMALL_CONTROL_TIME_SET)
		{	
		}
		//password error
		if(screen_id == smalllanguage_screen.SMALL_CONTROL_TIME_SET)
		{	
		}
		//pram set error
		if(screen_id == smalllanguage_screen.SMALL_CONTROL_TIME_SET)
		{	
		}
		//pram set screen
		if(screen_id == smalllanguage_screen.SMALL_PARAM_SET_SCREEN)
		{
		}
		//pid set screen
		if(screen_id == smalllanguage_screen.SMALL_PID_SET_SCREEN)
		{	
		}
		//main show screen
		if(screen_id == smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN)
		{	
		}
	}
}



/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	touch_flag++;
	if(press == 1)
	{
		touch_press.touch_x = x;
		touch_press.touch_y = y;
	}
	if(press == 3)
	{
		touch_up.touch_x = x;
		touch_up.touch_y = y;
	}
	

	if(!(touch_flag%2))
	{
		touch_flag = 0;
		if((current_screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)&&(touch_press.touch_x - touch_up.touch_x < 50))
		{
			if((touch_press.touch_x > 650)&&(touch_press.touch_y > 350)) //右下角
			{
				touchtimes.rightdown_times++;
			}
			if((touch_press.touch_x > 650)&&(touch_press.touch_y < 120)) //右上角
			{
				touchtimes.rightup_times++;
			}
			if((touch_press.touch_x < 200)&&(touch_press.touch_y >= 120)&&(touch_press.touch_y <= 300)) //左中间
			{
				touchtimes.leftmiddle_times++;
			}
			if((touch_press.touch_x < 200)&&(touch_press.touch_y > 300)) //左下角
			{
				touchtimes.leftdown_times++;
			}
			if((touch_press.touch_x >= 300)&&(touch_press.touch_x <= 500)&&(touch_press.touch_y >= 120)&&(touch_press.touch_y <= 300)) //上中间
			{
				touchtimes.upmiddle_times++;
			}
		}
		touchtoscreen();
	}
//	printf("press is (%d,%d),current screen id  is (%d)\n",touch_press.touch_x ,touch_press.touch_y,current_screen_id );
}


/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	if(SCREENSIZE)
	{
		press_flag = 1;
		
		//换气次数编辑界面
		if((screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)&&(control_id == BIG_AIR_RETURN_BUTTON))
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//pid值设置界面
		if((screen_id == biglanguage_screen.BIG_PID_SET_SCREEN)&&(state == 1))
		{
			if(control_id == BIG_SELF_ADJUST)							//自整定按键
			{
			}
			else if(control_id == BIG_PID_RETURN_BUTTON)  				//返回主界面
			{
				MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			//主显示界面
			}
			
		}
		//参数设置界面
		if((screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)&&(control_id == BIG_SET_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//自动断电时间设置界面
		if((screen_id == biglanguage_screen.BIG_AUTO_POWEROFF_TIMESET_SCREEN)&&(control_id == BIG_NO_POWER_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//时间设置界面
		if((screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)&&(control_id == BIG_TIME_SET_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//密码错误提示界面
		if((screen_id == biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN)&&(control_id == BIG_PASS_ERROR_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//参数设置无效界面
		if((screen_id == biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN)&&(control_id == BIG_FAIL_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);	
		}
		//主显示界面
		if(screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
		{
			switch (control_id)
			{
				case BIG_MENU_ID:
					if(state)
					{
						touchtimes.menu_click_times++;
					}
					break;
				case BIG_SELF_TEST_MENU_ID:
					if(state)
					{
						touchtimes.self_check_times++;
					}
					break;
				case BIG_SAMPLE_FRAME_MENU_ID:
					if(state)
					{
						SPINNER_RACK_ON;
						//样架图标显示
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,SHOW);
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,HIDE);
					}
					else
					{
						SPINNER_RACK_OFF;
						//样架图标消失
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SAMPLE_FRAME_MOTOR_ID,HIDE);
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RR_WORK_STATUS_ID,SHOW);
					}
					break;
				case BIG_BLOWER_MENU_ID:
					if(state)
					{
						CIRCULATING_FUN_ON;
						//风机图标显示
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,SHOW);
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,HIDE);
						
					}
					else
					{
						CIRCULATING_FUN_OFF;
						//风机图标消失
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FAN_OPERATION_ID,HIDE);
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_FR_WORK_STATUS_ID,SHOW);
					}
					break;
				case BIG_START_OR_PAUSE_ID:
					if(state)	
					{		
//						Pwm_Output(ENABLE);
						heattimes++;
						if(heattimes == 1)
						{
							AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,SHOW);
						}
						else
						{
							heattimes = 2;
							AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_PID_RUN_ID,HIDE);
						}
						HEAT_ON;
						//显示加热器图标
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,SHOW);
					}
					else
					{
//						Pwm_Output(DISABLE);
						HEAT_OFF;
						//显示加热器图标消失
						AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_SWITCH_ID,HIDE);
					}
					break;
				default:
					break;
			}
		}
	}
	else
	{
		press_flag = 1;
		//pid值设置界面
		if(screen_id == smalllanguage_screen.SMALL_PID_SET_SCREEN)
		{
			
		}
		//参数设置界面
		if((screen_id == smalllanguage_screen.SMALL_PARAM_SET_SCREEN)&&(control_id == SMALL_SET_RETURN_BUTTON))
		{
			MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//自动断电时间设置界面
		if((screen_id == smalllanguage_screen.SMALL_AUTO_POWEROFF_TIMESET_SCREEN)&&(control_id == SMALL_NO_POWER_RETURN_BUTTON))
		{
			MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//时间设置界面
		if((screen_id == smalllanguage_screen.SMALL_CONTROL_TIME_SET)&&(control_id == SMALL_TIME_SET_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//密码错误提示界面
		if((screen_id == smalllanguage_screen.SMALL_PASSWORD_ERROR_SCREEN)&&(control_id == SMALL_PASS_ERROR_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);			//主显示界面
		}
		//参数设置无效界面
		if((screen_id == smalllanguage_screen.SMALL_ARGUEMENT_SET_ERROR_SCREEN)&&(control_id == SMALL_FAIL_RETURN_BUTTON)&&(state == 1))
		{
			MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);			
		}
		//主显示界面
		if(screen_id == smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN)
		{
			switch (control_id)
			{
				case SMALL_MENU_ID:
					touchtimes.menu_click_times++;
					
					break;
				case SMALL_BLOWER_SAMPLE_MENU_ID:
					touchtimes.self_check_times++;
					
					break;
				case SMALL_START_OR_PAUSE_ID:
				
					break;
				default:
					break;
			}
		}
	}
}


/*! 
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
	uint16_t backlight = 0;
	if(SCREENSIZE)
	{
		//页面1，换气次数编辑界面
		if(SCREENSIZE)
		{
			if(screen_id == biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN)
			{
				switch (control_id)
				{
					case BIG_CHANGE_AIR_TIME_SET_90:
						memset(textvalue.change_air_times[0],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[0],str,sizeof(char)*4);
						dev_info.change_air_time[0] = atoi(textvalue.change_air_times[0]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_85:
						memset(textvalue.change_air_times[1],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[1],str,sizeof(char)*4);
						dev_info.change_air_time[1] = atoi(textvalue.change_air_times[1]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_80:
						memset(textvalue.change_air_times[2],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[2],str,sizeof(char)*4);
						dev_info.change_air_time[2] = atoi(textvalue.change_air_times[2]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_75:
						memset(textvalue.change_air_times[3],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[3],str,sizeof(char)*4);
						dev_info.change_air_time[3] = atoi(textvalue.change_air_times[3]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_70:
						memset(textvalue.change_air_times[4],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[4],str,sizeof(char)*4);
						dev_info.change_air_time[4] = atoi(textvalue.change_air_times[4]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_65:
						memset(textvalue.change_air_times[5],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[5],str,sizeof(char)*4);
						dev_info.change_air_time[5] = atoi(textvalue.change_air_times[5]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_60:
						memset(textvalue.change_air_times[6],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[6],str,sizeof(char)*4);
						dev_info.change_air_time[6] = atoi(textvalue.change_air_times[6]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_55:
						memset(textvalue.change_air_times[7],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[7],str,sizeof(char)*4);
						dev_info.change_air_time[7] = atoi(textvalue.change_air_times[7]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_50:
						memset(textvalue.change_air_times[8],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[8],str,sizeof(char)*4);
						dev_info.change_air_time[8] = atoi(textvalue.change_air_times[8]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_45:
						memset(textvalue.change_air_times[9],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[9],str,sizeof(char)*4);
						dev_info.change_air_time[9] = atoi(textvalue.change_air_times[9]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_40:
						memset(textvalue.change_air_times[10],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[10],str,sizeof(char)*4);
						dev_info.change_air_time[10] = atoi(textvalue.change_air_times[10]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_35:
						memset(textvalue.change_air_times[11],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[11],str,sizeof(char)*4);
						dev_info.change_air_time[11] = atoi(textvalue.change_air_times[11]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_30:
						memset(textvalue.change_air_times[12],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[12],str,sizeof(char)*4);
						dev_info.change_air_time[12] = atoi(textvalue.change_air_times[12]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_25:
						memset(textvalue.change_air_times[13],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[13],str,sizeof(char)*4);
						dev_info.change_air_time[13] = atoi(textvalue.change_air_times[13]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_20:
						memset(textvalue.change_air_times[14],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[14],str,sizeof(char)*4);
						dev_info.change_air_time[14] = atoi(textvalue.change_air_times[14]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_15:
						memset(textvalue.change_air_times[15],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[15],str,sizeof(char)*4);
						dev_info.change_air_time[15] = atoi(textvalue.change_air_times[15]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_10:
						memset(textvalue.change_air_times[16],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[16],str,sizeof(char)*4);
						dev_info.change_air_time[16] = atoi(textvalue.change_air_times[16]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_5:
						memset(textvalue.change_air_times[17],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[17],str,sizeof(char)*4);
						dev_info.change_air_time[17] = atoi(textvalue.change_air_times[17]);
						break;
					case BIG_CHANGE_AIR_TIME_SET_0:
						memset(textvalue.change_air_times[18],0,sizeof(char)*4);
						memcpy(textvalue.change_air_times[18],str,sizeof(char)*4);
						dev_info.change_air_time[18] = atoi(textvalue.change_air_times[18]);
						break;
					default:
						break;
				}
				coilvalue.change_max_time = max_change_air(dev_info.change_air_time,19);
				SetTextValueInt32(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_CHANGE_AIR_MAX_SET,coilvalue.change_max_time);
				FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
			}
			//风门角度
			if((screen_id == biglanguage_screen.BIG_AIR_DOOR_SCREEN)&&(control_id == BIG_AIR_DOOR_ANGLE_SET))
			{
				memset(textvalue.airdoor_value,0,sizeof(char)*4);
				memcpy(textvalue.airdoor_value,str,sizeof(char)*4);
				SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,textvalue.airdoor_value);
				coilvalue.air_door_angle = atof(textvalue.airdoor_value);
				Dac1_Set_Vol(3300*coilvalue.air_door_angle/90);
				MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);		
			}
			//屏幕亮度
			if((screen_id == biglanguage_screen.BIG_BRIGHT_ADJUST_SCREEN)&&(control_id == BIG_SCREEN_BRIGHT_ADJUST))
			{
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
				MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);		
			}
			//菜单密码
			if((screen_id == biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN)&&(control_id == BIG_PASSWORD_PROTECT_INPUT))
			{
				memset(textvalue.protect_password,0,sizeof(char)*6);
				memcpy(textvalue.protect_password ,str,sizeof(char)*6);
				coilvalue.menu_password = atoi(textvalue.protect_password);
//				printf("password is : %s , %d ,size is %d\n",textvalue.protect_password,coilvalue.menu_password,sizeof(char)*6);
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
						memset(textvalue.Pidvalue[0],0,sizeof(char)*4);
						memcpy(textvalue.Pidvalue[0],str,sizeof(char)*4);
						break;
					case BIG_I_VALUE_SET:
						memset(textvalue.Pidvalue[1],0,sizeof(char)*4);
						memcpy(textvalue.Pidvalue[1],str,sizeof(char)*4);
						break;
					case BIG_D_VALUE_SET:
						memset(textvalue.Pidvalue[2],0,sizeof(char)*4);
						memcpy(textvalue.Pidvalue[2],str,sizeof(char)*4);
						break;
					default:
						break;
				}
			}
			//参数设置界面
			if(screen_id == biglanguage_screen.BIG_PARAM_SET_SCREEN)
			{
				switch (control_id)
				{
					case BIG_TEST_TIME_VALUE:	//set time
						memset(textvalue.coilsavevalue.test_duration,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.test_duration,str,sizeof(char)*4);				
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEST_TIME_VALUE,textvalue.coilsavevalue.test_duration);
						showtextvalue.test_time = atoi(textvalue.coilsavevalue.test_duration);
						if(showtextvalue.test_time < 0 || showtextvalue.test_time >= 4000)
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TESTTIME_SET_FAIL,SHOW);
							MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
						}
						else 
						{
							strcat(textvalue.coilsavevalue.test_duration," h");
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TESTTIME_SET_FAIL,HIDE);
							SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEST_TIME_ID,textvalue.coilsavevalue.test_duration);
						}
						
						break;
					case BIG_TEST_TEMP_VALUE:	//set temp
						memset(textvalue.coilsavevalue.test_temp,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.test_temp,str,sizeof(char)*4);	
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_TEST_TEMP_VALUE,textvalue.coilsavevalue.test_temp);
						showtextvalue.setting_temp = atof(textvalue.coilsavevalue.test_temp);
						if(showtextvalue.setting_temp < 0 || showtextvalue.setting_temp >= 1000)
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TESTTEMP_SET_FAIL,SHOW);
							MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
						}
						else
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TESTTEMP_SET_FAIL,HIDE);
							SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SET_TEMP_ID,textvalue.coilsavevalue.test_temp);
						}
						break;	
					case BIG_WARNING1_UP_VALUE:
						memset(textvalue.coilsavevalue.warning1_up,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.warning1_up,str,sizeof(char)*4);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING1_UP_VALUE,textvalue.coilsavevalue.warning1_up);
						coilvalue.warning1_up = atof(textvalue.coilsavevalue.warning1_up);
						if(coilvalue.warning1_up <= coilvalue.warning1_down)
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TEMP1_UP_SET_FAIL,SHOW);
							MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
						}
						else
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TEMP1_UP_SET_FAIL,HIDE);
						}
						break;	
					case BIG_WARNING1_DOWN_VALUE:
						memset(textvalue.coilsavevalue.warning1_down,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.warning1_down,str,sizeof(char)*4);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING1_DOWN_VALUE,textvalue.coilsavevalue.warning1_down);
						coilvalue.warning1_down = atof(textvalue.coilsavevalue.warning1_down);
						if(coilvalue.warning1_down >= coilvalue.warning1_up)
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TEMP1_DOWN_SET_FAIL,SHOW);
							MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);
						}
						else
						{
							AnimationPlayFrame(biglanguage_screen.BIG_PASSWORD_ERROR_SCREEN,BIG_TEMP1_DOWN_SET_FAIL,HIDE);
						}
						break;	
					case BIG_WARNING2_UP_VALUE:
						memset(textvalue.coilsavevalue.warning2_up,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.warning2_up,str,sizeof(char)*4);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING2_UP_VALUE,textvalue.coilsavevalue.warning2_up);
						coilvalue.warning2_up = atof(textvalue.coilsavevalue.warning2_up);
						break;	
					case BIG_WARNING2_DOWN_VALUE:
						memset(textvalue.coilsavevalue.warning2_down,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.warning2_down,str,sizeof(char)*4);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_WARNING2_DOWN_VALUE,textvalue.coilsavevalue.warning2_down);
						coilvalue.warning2_down = atof(textvalue.coilsavevalue.warning2_down);
						break;	
					case BIG_NEW_PASSWORD:
						memset(textvalue.coilsavevalue.menu_password,0,sizeof(char)*6);
						memcpy(textvalue.coilsavevalue.menu_password,str,sizeof(char)*6);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_NEW_PASSWORD,textvalue.coilsavevalue.menu_password);
						break;
					case BIG_SECOND_INPUT_PASSWORD:
						memset(textvalue.coilsavevalue.secondtime_password,0,sizeof(char)*6);
						memcpy(textvalue.coilsavevalue.secondtime_password,str,sizeof(char)*6);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_NEW_PASSWORD,textvalue.coilsavevalue.menu_password);
						if(strncmp(textvalue.coilsavevalue.secondtime_password,textvalue.coilsavevalue.menu_password,4))
						{
							AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,SHOW);
							MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);	
						}
						else
						{
							AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_PASS_UPDATE_FAIL,HIDE);
						}
						break;
					case BIG_CHANGE_AIR_TIME_SET:
						memset(textvalue.coilsavevalue.change_air_time,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.change_air_time,str,sizeof(char)*4);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_CHANGE_AIR_TIME_SET,textvalue.coilsavevalue.change_air_time);
						SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CHANGE_AIR_TIME,textvalue.coilsavevalue.change_air_time);
						coilvalue.change_air_time = atoi(textvalue.coilsavevalue.change_air_time);
						change_air_times();
						break;
					case BIG_MODBUS_NODE_ADDRESS:
						memset(textvalue.coilsavevalue.modbus_address,0,sizeof(char)*4);
						memcpy(textvalue.coilsavevalue.modbus_address,str,sizeof(char)*4);
						SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_MODBUS_NODE_ADDRESS,textvalue.coilsavevalue.modbus_address);
						coilvalue.modbus_address = atoi(textvalue.coilsavevalue.modbus_address);
						if(coilvalue.modbus_address < 0 || coilvalue.modbus_address > 250)
						{
							MySetScreen(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN);	
							AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_MODBUS_ADDRESS_SET_FAIL,SHOW);
						}
						else
						{
							AnimationPlayFrame(biglanguage_screen.BIG_ARGUEMENT_SET_ERROR_SCREEN,BIG_MODBUS_ADDRESS_SET_FAIL,HIDE);
						}
						break;
						
					default:
						break;
				}
			}
			//自动断电设置
			if(screen_id == biglanguage_screen.BIG_AUTO_POWEROFF_TIMESET_SCREEN)
			{
				switch (control_id)
				{
					case BIG_YEAR_SET:
						memset(textvalue.autotime.year,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.autotime.year,str,sizeof(char)*BUFFSIZE);
						nopowertime.year = atoi(textvalue.autotime.year);
						nopowertime.year = DectoBCD(nopowertime.year);
						break;
					case BIG_MONTH_SET:
						memset(textvalue.autotime.month,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.autotime.month,str,sizeof(char)*BUFFSIZE);
						nopowertime.month = atoi(textvalue.autotime.month);
						nopowertime.month = DectoBCD(nopowertime.month);
						break;	
					case BIG_DAY_SET:
						memset(textvalue.autotime.day,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.autotime.day,str,sizeof(char)*BUFFSIZE);
						nopowertime.day = atoi(textvalue.autotime.day);
						nopowertime.day = DectoBCD(nopowertime.day);
						break;	
					default:
						break;
				}
			}
			//自动断电弹出界面
			if(screen_id == biglanguage_screen.BIG_AUTO_POWEROFF_POPUPWINDOWS_SCREEN)
			{
				memset(textvalue.autonopowerpassword,0,sizeof(char)*BUFFSIZE);
				memcpy(textvalue.autonopowerpassword,str,sizeof(char)*BUFFSIZE);
				autonopowerpassword =  atoi(textvalue.autonopowerpassword);
			}
			//温度值校正
			if(screen_id == biglanguage_screen.BIG_TEMP_VALUE_REVISE_SCREEN)
			{
				memset(textvalue.temp_adjust_value,0,sizeof(char)*BUFFSIZE);
				memcpy(textvalue.temp_adjust_value,str,sizeof(char)*BUFFSIZE);
				adjusttemp1 =  atof(textvalue.temp_adjust_value);
				adjusttemp2 += adjusttemp1; 
				MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			}
			//设备时间设置
			if(screen_id == biglanguage_screen.BIG_CONTROL_TIME_SET)
			{
				switch (control_id)
				{
					case BIG_CONTROL_DATE_YEAR_SET:
						memset(textvalue.device_time_setting.Year,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.device_time_setting.Year,str,sizeof(char)*BUFFSIZE);	
						rtctime.Year = atoi(textvalue.device_time_setting.Year);
						rtctime.Year = DectoBCD(rtctime.Year);
						break;
					case BIG_CONTROL_DATE_MONTH_SET:
						memset(textvalue.device_time_setting.Mon,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.device_time_setting.Mon,str,sizeof(char)*BUFFSIZE);
						rtctime.Mon = atoi(textvalue.device_time_setting.Mon);
						rtctime.Mon = DectoBCD(rtctime.Mon);
						break;	
					case BIG_CONTROL_DATE_DAY_SET:
						memset(textvalue.device_time_setting.Day,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.device_time_setting.Day,str,sizeof(char)*BUFFSIZE);
						rtctime.Day = atoi(textvalue.device_time_setting.Day);
						rtctime.Day = DectoBCD(rtctime.Day);
						break;	
					case BIG_CONTROL_TIME_HOUR_SET:
						memset(textvalue.device_time_setting.Hour,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.device_time_setting.Hour,str,sizeof(char)*BUFFSIZE);
						rtctime.Hour = atoi(textvalue.device_time_setting.Hour);
						rtctime.Hour = DectoBCD(rtctime.Hour);
						break;
					case BIG_CONTROL_TIME_MINUTE_SET:
						memset(textvalue.device_time_setting.Min,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.device_time_setting.Min,str,sizeof(char)*BUFFSIZE);
						rtctime.Min = atoi(textvalue.device_time_setting.Min);
						rtctime.Min = DectoBCD(rtctime.Min);
						break;	
					case BIG_CONTROL_TIME_SECOND_SET:
						memset(textvalue.device_time_setting.Sec,0,sizeof(char)*BUFFSIZE);
						memcpy(textvalue.device_time_setting.Sec,str,sizeof(char)*BUFFSIZE);
						rtctime.Sec = atoi(textvalue.device_time_setting.Sec);
						rtctime.Sec = DectoBCD(rtctime.Sec);
						break;	
					default:
						break;
				}
				SetRtcTime(rtctime.Sec,rtctime.Min,rtctime.Hour,rtctime.Day,0,rtctime.Mon,rtctime.Year);
			}
//			//主显示界面
//			if(screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
//			{
//				switch (control_id)
//				{
//					case BIG_CURRENT_TEMP_ID:
//						sprintf(textvalue.textvaluebuff.current_temp_vlaue,"%.1f",showtextvalue.current_temp_vlaue);//把浮点数转换为字符串(保留一位小数)
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.current_temp_vlaue);
//						SetTextValueFloat(screen_id,control_id, showtextvalue.current_temp_vlaue);
//						break;
//					case BIG_SET_TEMP_ID:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.setting_temp);
//						break;	
//					case BIG_START_TIME_ID:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.start_time);
//						break;	
//					case BIG_END_TIME_ID:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.end_time);
//						break;
//					case BIG_TEST_TIME_ID:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.test_time);
//						break;	
//					case BIG_ADDUP_TIME_ID:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.add_all_time);
//						break;	
//					case BIG_TIME_LEFT_ID:
//						SetTextValueInt32(screen_id,control_id,textvalue.textvaluebuff.left_time_hou);
//						break;	
//					case BIG_CHANGE_AIR_TIME:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.change_air_time);
//						break;		
//					case BIG_AIR_DOOR_ANGLE_INPUT_ID:
//						SetTextValue(screen_id,control_id,textvalue.textvaluebuff.air_door_angle);
//						break;	
//					default:
//						break;
//				}
//			}
		}
		else
		{
			//屏幕亮度
			if((screen_id == smalllanguage_screen.SMALL_BRIGHT_ADJUST_SCREEN)&&(control_id == SMALL_SCREEN_BRIGHT_ADJUST))
			{
				SetTextValue(screen_id,control_id,textvalue.screen_light_value);
				SetBackLight(atoi(textvalue.screen_light_value)*255/100);
				MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);
			}
		}
	}
	else
	{
		
	}

}

/*! 
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
	if((screen_id == biglanguage_screen.BIG_START_INIT_SCREEN)&&(control_id == BIG_START_INIT_ID))
	{
		if(value == 100)
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
	}
	if((screen_id == biglanguage_screen.BIG_SELF_TEST_SCREEN)&&(control_id == BIG_SELF_TEST_PROGRAM))
	{
		if(value == 100)
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
	}
}


/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
}


/*! 
 *  \brief  动画控件通知
 *  \details  当动画改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param starus    0x00表示触摸按下，0x01表示弹起
 *  \param iconimage_id 值
 */
void  NotifyAnimation(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id)
{
	
		
}

/*! 
 *  \brief  图标控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param status    0x00表示触摸按下，0x01表示弹起
 *  \param iconimage_id 值
 */
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
				}
				break;
			case BIG_BPS_2400:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
				}
				break;
			case BIG_BPS_4800:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_9600,HIDE);
				}
				break;
			case BIG_BPS_9600:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_BPS_1200,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_4800,HIDE);
					AnimationPlayFrame(screen_id,BIG_BPS_2400,HIDE);
				}
				break;
			case BIG_CHINESE_LANGUAGE:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_ENGLISH_LANGUAGE,HIDE);
					biglanguage_screen = bigchinese_screen;
				}
				break;
			case BIG_ENGLISH_LANGUAGE:
				if(!status)
				{
					AnimationPlayFrame(screen_id,control_id,SHOW);
					AnimationPlayFrame(screen_id,BIG_CHINESE_LANGUAGE,HIDE);
					biglanguage_screen = bigenglish_screen;
				}
				break;
			default:
				break;
		}
	}

}



/*! 
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
	
	
}

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO: 添加用户代码

}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO: 添加用户代码

}

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
	//TODO: 添加用户代码
	
}

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO: 添加用户代码
	
}

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8_t status)
{
	//TODO: 添加用户代码
	
}


/*! 
 *  \brief  读取RTC时间，注意返回的是BCD码
 *  \param year 年（BCD）
 *  \param month 月（BCD）
 *  \param week 星期（BCD）
 *  \param day 日（BCD）
 *  \param hour 时（BCD）
 *  \param minute 分（BCD）
 *  \param second 秒（BCD）
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
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;					//指令类型
	uint8_t ctrl_msg = msg->ctrl_msg;   				//消息的类型
	uint8_t control_type = msg->control_type;			//控件类型
	uint16_t screen_id = PTR2U16(&msg->screen_id);		//画面ID
	uint16_t control_id = PTR2U16(&msg->control_id);	//控件ID
	uint32_t value = PTR2U32(msg->param);				//数值

	switch(cmd_type)
	{		
		case NOTIFY_TOUCH_PRESS:		//触摸屏按下
			NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
			break;
		case NOTIFY_TOUCH_RELEASE:		//触摸屏松开
			NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
			break;	
		case NOTIFY_WRITE_FLASH_OK:		//写FLASH成功
			NotifyWriteFlash(1);
			break;
		case NOTIFY_WRITE_FLASH_FAILD:	//写FLASH失败
			NotifyWriteFlash(0);
			break;
		case NOTIFY_READ_FLASH_OK:		//读取FLASH成功
			NotifyReadFlash(1,cmd_buffer+2,size-6);		//去除帧头帧尾
			break;
		case NOTIFY_READ_FLASH_FAILD:	//读取FLASH失败
			NotifyReadFlash(0,0,0);
			break;
		case NOTIFY_READ_RTC:			//读取RTC时间
			NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
			break;
		case NOTIFY_CONTROL:
			{
				if(ctrl_msg==MSG_GET_CURRENT_SCREEN)	//画面ID变化通知
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
						case kCtrlButton: //按钮控件
							NotifyButton(screen_id,control_id,msg->param[1]);
							break;
						case kCtrlText://文本控件
							NotifyText(screen_id,control_id,msg->param);
							break;
						case kCtrlProgress: //进度条控件
							NotifyProgress(screen_id,control_id,value);
							break;
						case kCtrlSlider: //滑动条控件
							NotifySlider(screen_id,control_id,value);
							break;
						case kCtrlAnimation: //动画控件
							NotifyAnimation(screen_id, control_id,msg->param[0],msg->param[1]);
						case kCtrlMeter: //仪表控件
							NotifyMeter(screen_id,control_id,value);
							break;
						case kCtrlMenu://菜单控件
							NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
							break;
						case kCtrlSelector://选择控件
							NotifySelector(screen_id,control_id,msg->param[0]);
							break;
						case kCtrlRTC://倒计时控件
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



//启动界面
void startscreen(void)
{
	uint8_t startvalue;
	printf("%s\r\n",soft_ver);									//软件版本
	if(SCREENSIZE)
	{
		MySetScreen(biglanguage_screen.BIG_START_INIT_SCREEN);
		for(startvalue = 0;startvalue <= 100;startvalue++)
		{
			delay_ms(30);
			SetSliderValue(biglanguage_screen.BIG_START_INIT_SCREEN,BIG_START_INIT_ID,startvalue);
		}
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_SOFT_VER_ID,soft_ver);
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
		
	}
	else
	{
		MySetScreen(smalllanguage_screen.SMALL_START_INIT_SCREEN);
		for(startvalue = 0;startvalue <= 100;startvalue++)
		{
			delay_ms(30);
			SetSliderValue(smalllanguage_screen.SMALL_START_INIT_SCREEN,SMALL_START_INIT_ID,startvalue);
		}
		MySetScreen(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN);
		SetTextValue(smalllanguage_screen.SMALL_MAIN_SHOW_SCREEN,SMALL_SOFT_VER_ID,soft_ver);
	}
}






//连击触控跳转界面函数
void  touchtoscreen(void)
{
	if((touch_press.touch_x - touch_up.touch_x > 100)&&(current_screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN))
	{
		MySetScreen(biglanguage_screen.BIG_TEMP_CURVE_SHOW_SCREEN);
	}
	else if((touch_up.touch_x - touch_press.touch_x > 100)&&(current_screen_id == biglanguage_screen.BIG_TEMP_CURVE_SHOW_SCREEN))
	{
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
	}
	if(getMsCounter() - timercount > 1000)
	{
		timercount = getMsCounter();	

		if(touchtimes.rightdown_times == 3)
		{
			printf("touchtimes.rightdown_times is %d \n",touchtimes.rightdown_times);
			MySetScreen(biglanguage_screen.BIG_AIR_DOOR_SCREEN);
		}

		if(touchtimes.rightup_times == 3)
		{
			printf("touchtimes.rightup_times is %d \n",touchtimes.rightup_times);
			MySetScreen(biglanguage_screen.BIG_AIR_CHANGE_RATE_SCREEN);
		}

		if(touchtimes.leftmiddle_times == 3)
		{
			printf("touchtimes.leftmiddle_times is %d \n",touchtimes.leftmiddle_times);
			MySetScreen(biglanguage_screen.BIG_PID_SET_SCREEN);
		}
		
		if(touchtimes.leftdown_times == 3)
		{
			printf("touchtimes.leftdown_times is %d \n",touchtimes.leftdown_times);
			MySetScreen(biglanguage_screen.BIG_AUTO_POWEROFF_TIMESET_SCREEN);
		}
		if(touchtimes.upmiddle_times == 3)
		{
			printf("touchtimes.upmiddle_times is %d \n",touchtimes.upmiddle_times);
			MySetScreen(biglanguage_screen.BIG_TEMP_VALUE_REVISE_SCREEN);
		}
		touchtimes.rightdown_times = 0;
		touchtimes.rightup_times = 0;
		touchtimes.leftdown_times = 0;
		touchtimes.leftmiddle_times = 0;
		touchtimes.upmiddle_times = 0;
	}
	

	
}


//连击按钮跳转界面函数
void get_combo_button_times(void)
{
	//菜单按钮
	if(touchtimes.menu_click_times == 1)
	{
		printf("menu 1button times is %d \n",touchtimes.menu_click_times);
		MySetScreen(biglanguage_screen.BIG_SCREAT_PROTECT_SCREEN);
		touchtimes.menu_click_times = 0;
	}
	else if(touchtimes.menu_click_times == 4)
	{
		printf("menu 4button times is %d \n",touchtimes.menu_click_times);
		MySetScreen(biglanguage_screen.BIG_BRIGHT_ADJUST_SCREEN);
		touchtimes.menu_click_times = 0;
	}
	else if(touchtimes.menu_click_times != 0)
	{
		printf("menu 1button times is %d \n",touchtimes.menu_click_times);
	}

	//自检按钮
	if(touchtimes.self_check_times == 1)
	{
		if(thermocouple_flag|gpiostatus) //有故障
		{
			MySetScreen(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN);
		}
		else
		{
			printf("check 1button times is %d \n",touchtimes.self_check_times);
			MySetScreen(biglanguage_screen.BIG_SELF_TEST_SCREEN);
			device_selfcheck();
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			touchtimes.self_check_times = 0;
		}
	}
	else if(touchtimes.self_check_times == 3)
	{
		printf("check 3button times is %d \n",touchtimes.self_check_times);
		MySetScreen(biglanguage_screen.BIG_TIME_RECORD_SCREEN);
		delay_s(20);
		MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
		touchtimes.self_check_times = 0;
	}
	else if(touchtimes.self_check_times != 0)
	{
		printf("check 3button times is %d \n",touchtimes.self_check_times);
	}
}



//温度曲线存储
void temp_curve_save(void)
{
	GraphTempDataAdd(biglanguage_screen.BIG_TEMP_CURVE_SHOW_SCREEN, BIG_TEMP_CURVE_SHOW,(uint16_t)showtextvalue.current_temp_vlaue);
}







//合并时间字符
void  mergetimechar(RtcTime datetime)
{
	uint8_t buff[2] = {0};
	sprintf(buff,"%02d",datetime.Year); //把整数转换为字符串
	textvalue.textvaluebuff.start_time[0] = buff[0];
	textvalue.textvaluebuff.start_time[1] = buff[1];

	sprintf(buff,"%02d",datetime.Mon);
	textvalue.textvaluebuff.start_time[2] = '/';
	textvalue.textvaluebuff.start_time[3] = buff[0];
	textvalue.textvaluebuff.start_time[4] = buff[1];

	sprintf(buff,"%02d",datetime.Day);
	textvalue.textvaluebuff.start_time[5] = '/';
	textvalue.textvaluebuff.start_time[6] = buff[0];
	textvalue.textvaluebuff.start_time[7] = buff[1];
	
	textvalue.textvaluebuff.start_time[8] = ' ';

	sprintf(buff,"%02d",datetime.Hour);
	textvalue.textvaluebuff.start_time[9] = buff[0];
	textvalue.textvaluebuff.start_time[10] = buff[1];
	textvalue.textvaluebuff.start_time[11] = ':';

	sprintf(buff,"%02d",datetime.Min);
	textvalue.textvaluebuff.start_time[12] = buff[0];
	textvalue.textvaluebuff.start_time[13] = buff[1];
}

//合并小时分钟字符串
void mergehour_min(uint16_t hour,uint16_t min)
{
	uint8_t buff[5] = {0};
	sprintf(buff,"%04d",hour); //把整数转换为字符串
	textvalue.textvaluebuff.left_time[0] = buff[0];
	textvalue.textvaluebuff.left_time[1] = buff[1];
	textvalue.textvaluebuff.left_time[2] = buff[2];
	textvalue.textvaluebuff.left_time[3] = buff[3];
	textvalue.textvaluebuff.left_time[4] = 'h';
	sprintf(buff,"%02d",min);
	textvalue.textvaluebuff.left_time[5] = buff[0];
	textvalue.textvaluebuff.left_time[6] = buff[1];
	textvalue.textvaluebuff.left_time[7] = 'm';
	textvalue.textvaluebuff.left_time[8] = 'i';
	textvalue.textvaluebuff.left_time[9] = 'n';
}

//到某时的分钟数，从2000年1月1号，0:00开始
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


//月份识别
char * monselect(char *monbuff)
{
	uint8_t i = 0,temp = 0;
	char * buffer;
	for(i = 0; i < 12;i++)
	{
		temp = strncmp(monbuff,Month[i],3);
		if(temp == 0)
		{
			sprintf(buffer,"%d",i+1); 
			return buffer;
		}
	}
}

//结束时间格式转换
void  adjustchar(char *timebuff)
{
	char *monbuff;
	char yearbuff[4]= {0},daybuff[2]= {0},clockbuff[5]= {0};
	monbuff=malloc(sizeof(char)*2);
	memcpy(yearbuff,timebuff+22,4);
	textvalue.textvaluebuff.end_time[0] = yearbuff[0];
	textvalue.textvaluebuff.end_time[1] = yearbuff[1];
	textvalue.textvaluebuff.end_time[2] = '/';
	
	strncpy(monbuff,timebuff+4,3);
	monbuff = monselect(monbuff);
	textvalue.textvaluebuff.end_time[3] = monbuff[0];
	textvalue.textvaluebuff.end_time[4] = monbuff[1];
	textvalue.textvaluebuff.end_time[5] = '/';
	
	memcpy(daybuff,timebuff+8,2);
	if(daybuff[0] == ' ')
	{
		daybuff[0] = '0';
	}
	textvalue.textvaluebuff.end_time[6] = daybuff[0];
	textvalue.textvaluebuff.end_time[7] = daybuff[1];
	textvalue.textvaluebuff.end_time[8] = ' ';
	
	memcpy(clockbuff,timebuff+11,5);
	textvalue.textvaluebuff.end_time[9] = clockbuff[0];
	textvalue.textvaluebuff.end_time[10] = clockbuff[1];
	textvalue.textvaluebuff.end_time[11] = clockbuff[2];
	textvalue.textvaluebuff.end_time[12] = clockbuff[3];
	textvalue.textvaluebuff.end_time[13] = clockbuff[4];
	free(monbuff);
//	printf("### %s ###\n",textvalue.textvaluebuff.end_time);
	
}


//结束时间计算
void endtimecalcu(RtcTime starttime,uint16_t testtime)
{
	char * timebuff;
	time_t currenttime = 946684800;
	timebuff=malloc(sizeof(char)*25);

	currenttime += to_day(starttime) + testtime*3600;
	timebuff = ctime(&currenttime);
	adjustchar(timebuff);
	free(timebuff);
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_END_TIME_ID,textvalue.textvaluebuff.end_time);		
}


//时间差，返回秒数
uint32_t diff_time(RtcTime starttime,RtcTime endtime)
{
	return (to_day(endtime) - to_day(starttime));
}

//两个实数的绝对值
float myabs(float a,float b)
{
	if(a >= b)  
		return a-b;
	else
		return b-a;
}

uint32_t temptime = 0;		//剩余实验时间

//剩余时间计算 ，分钟
void lefttimecalculate(void)
{
	temptime = showtextvalue.test_time*60 - diff_time(showtextvalue.start_time, rtctime)/60;
	showtextvalue.left_time_hou = temptime/60;
	showtextvalue.left_time_min = temptime%60;
	mergehour_min(showtextvalue.left_time_hou ,showtextvalue.left_time_min);
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_ID,textvalue.textvaluebuff.left_time);
	if(temptime == 0)
	{
		//实验结束
		addup_testtime();   //累计实验时间
		warmflag = 0;
	}
}

//起始结束时间设置
void start_endtime_set(void)
{
	RtcTime inittime = {0};
	
	if(myabs(showtextvalue.setting_temp,showtextvalue.current_temp_vlaue) <= 2)
	{
		warmflag++;			//加热到 目标温度次数
	}
	if(warmflag == 0)
	{
		mergetimechar(inittime);
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_START_TIME_ID,textvalue.textvaluebuff.start_time);
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_END_TIME_ID,textvalue.textvaluebuff.start_time); 	
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TIME_LEFT_ID,"0 h 0 min");	
	}
	else if((warmflag == 1)&&(showtextvalue.setting_temp != 0))	//第一次加热到目标温度且设定温度不为0
	{		
		//开始时间设置
		mergetimechar(rtctime);
		showtextvalue.start_time = rtctime;
		SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_START_TIME_ID,textvalue.textvaluebuff.start_time);
		//结束时间设置
		endtimecalcu(showtextvalue.start_time,showtextvalue.test_time);
		lefttimecalculate();
	}
	else 
	{
		lefttimecalculate();
	}
}

//累计实验时间
void addup_testtime(void)
{
	uint32_t addtime = 0;
	uint8_t buffer[10] = {0};
	addtime += showtextvalue.test_time;
	sprintf(buffer,"%09ld",addtime);
	strcat(buffer,"h");
	SetTextValue(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_ADDUP_TIME_ID,buffer);
}



//风门角度次数界面编辑
void change_air_times(void)
{
	uint8_t i = 0;
	for(i = 0;i < 19;i++)
	{
		if(coilvalue.change_air_time == dev_info.change_air_time[i])
		{
			switch (i)
			{
				case 0:
					showtextvalue.air_door_angle = 90;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);
					Dac1_Set_Vol(3300);
					break;
				case 1:
					showtextvalue.air_door_angle = 85;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(3115);
					break;
				case 2:
					showtextvalue.air_door_angle = 80;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(2933);
					break;
				case 3:
					showtextvalue.air_door_angle = 75;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(2750);
					break;
				case 4:
					showtextvalue.air_door_angle = 70;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(2566);
					break;
				case 5:
					showtextvalue.air_door_angle = 65;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(2383);
					break;
				case 6:
					showtextvalue.air_door_angle = 60;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(2200);
					break;
				case 7:
					showtextvalue.air_door_angle = 55;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(2017);
					break;
				case 8:
					showtextvalue.air_door_angle = 50;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(1834);
					break;
				case 9:
					showtextvalue.air_door_angle = 45;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(1651);
					break;
				case 10:
					showtextvalue.air_door_angle = 40;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(1468);
					break;
				case 11:
					showtextvalue.air_door_angle = 35;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(1285);
					break;
				case 12:
					showtextvalue.air_door_angle = 30;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(1102);
					break;
				case 13:
					showtextvalue.air_door_angle = 25;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(919);
					break;
				case 14:
					showtextvalue.air_door_angle = 20;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(736);
					break;
				case 15:
					showtextvalue.air_door_angle = 15;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(553);
					break;
				case 16:
					showtextvalue.air_door_angle = 10;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(370);
					break;
				case 17:
					showtextvalue.air_door_angle = 5;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(185);
					break;
				case 18:
					showtextvalue.air_door_angle = 0;
					SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AIR_DOOR_ANGLE_INPUT_ID,showtextvalue.air_door_angle);	
					Dac1_Set_Vol(0);
					break;
				default:
					break;
			}
		}
	}
}

//最大换气次数编辑
uint8_t max_change_air(uint8_t *buff,uint8_t size)
{
	uint8_t i = 0, temp = 0;
	for(i = 1;i<size;i++)
	{
		temp = buff[0];
		if(buff[i] >= temp)
		{
			temp = buff[i];
		}
	}
	return temp;
}

