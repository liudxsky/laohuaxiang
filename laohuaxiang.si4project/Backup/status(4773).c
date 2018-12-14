#include "./status/status.h"
#include "./gpio/gpio.h"
#include "hmi_driver.h"
#include "./dataprocess/dataprocess.h"
#include "./screen/screen.h"
#include "./delaytime/delaytime.h"
#include "./pwm/pwm.h"
#include "./flash/deviceinfo.h"


extern BIG_SCREEN_ID_TAB bigchinese_screen ;
extern BIG_SCREEN_ID_TAB bigenglish_screen; 
extern BIG_SCREEN_ID_TAB biglanguage_screen;

extern SMALL_SCREEN_ID_TAB smallchinese_screen;
extern SMALL_SCREEN_ID_TAB smallenglish_screen;
extern SMALL_SCREEN_ID_TAB smalllanguage_screen;

extern uint16_t current_screen_id;
extern RtcTime rtctime;							//系统当前时间
extern TextValueTab  textvalue;					//文本控件保存值
extern MainShowTextValue	showtextvalue;		//主页面文本控件缓存值
extern uint8_t SCREENSIZE; 
extern CoilValue  coilvalue;				//设置界面值
extern float adjusttemp2;
extern uint8_t slidervalue;					//滑动进度条值
extern uint16_t autonopowerpassword;
extern AutoNoPowerTime  nopowertime;		//自动断电时间	
extern uint8_t cmd_buffer[CMD_MAX_SIZE];
extern uint8_t thermocouple_flag;
extern dev_info_t dev_info;
extern uint8_t  soft_ver[10];


uint8_t  pwmgpiostatus = 0;
uint16_t gpiostatus = 0;
uint32_t door_openstatus = 0,door_closestatus = 0;


ThermalLag heattime_log;						//热滞后时间记录结构体
uint8_t  control_mode[2][8] = {"4-20mA","2-10V"};
uint8_t  Rs485Status = 2;  						//0表示通信正常，1表示通信异常，2表示485通信未连接






//检测继电器反馈引脚状态变化
uint16_t Get_GPIO_Status(void)
{
	uint8_t i;
	uint16_t status = 0;
	uint8_t readgpio[GPIO_TABLE_SIZE] = {0},gpioback[GPIO_TABLE_SIZE] = {0};
	
	readgpio[0] = GPIO_ReadInputDataBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	gpioback[0] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN1);
	readgpio[1] = GPIO_ReadInputDataBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	gpioback[1] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN2);
	readgpio[2] = GPIO_ReadInputDataBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	gpioback[2] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN3);
	readgpio[3] = GPIO_ReadInputDataBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	gpioback[3] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN4);
	readgpio[4] = GPIO_ReadInputDataBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	gpioback[4] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN5);
	
	for(i = 0;i < GPIO_TABLE_SIZE;i++)
	{
//		printf("gpio %d (%d %d) \n",i,readgpio[i],gpioback[i]);
		status |= ((readgpio[i]^(!gpioback[i]))<<i);
	}	
	return status;
}



//上电自检显示屏，如果不通，则可以闪烁LED灯
extern uint8_t screen_flag;
//检测屏幕连接
void check_screen_connect(void)
{
	if(screen_flag)
	{
		if(current_screen_id == biglanguage_screen.BIG_START_INIT_SCREEN)
		{
			
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
		}
	}
	else
	{
		screen_flag = 0;
//		led_blink(3);
//		delay_ms(100);	
	}
}


//检测是否到定时断电时间
void check_nopowertime(void)
{
	if(SCREENSIZE)
	{
		if((rtctime.Year == nopowertime.year)&&(rtctime.Mon == nopowertime.month)&&(rtctime.Day == nopowertime.day))
		{
			MySetScreen(biglanguage_screen.BIG_AUTO_POWEROFF_POPUPWINDOWS_SCREEN);
			RCC_AHB1PeriphClockCmd(DRIVER_GPIO_CLK|BACK_GPIO_CLK|BOX_DOOR_GPIO_CLK,DISABLE);

			if(autonopowerpassword == coilvalue.menu_password)
			{
				RCC_AHB1PeriphClockCmd(DRIVER_GPIO_CLK|BACK_GPIO_CLK|BOX_DOOR_GPIO_CLK,ENABLE);
				MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
			}			
			
		}
	}
	else
	{
		
	}

}

//检测加热通断
void check_pwmicon(void)
{
	if(dev_info.pwmvalue != 0)
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,HIDE);
	}
}


//检测485通信状态
void Check_Rs485(void)
{
	
	switch (Rs485Status)
	{
		case 0:
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,0);
			break;
		case 1:
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,1);
			delay_ms(300);
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,0);
			delay_ms(300);
			break;
		case 2:
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,2);
			break;
		default:
			break;
	}
}


//检测主板是否有电通过判断上次断电时间是否小于2001年判断
void check_powertime(void)
{
	if(rtctime.Year == 0)
	{
		MySetScreen(biglanguage_screen.BIG_CONTROL_TIME_SET);
	}		
}

//检测PWM输出
void check_pwm(void)
{
	pwmgpiostatus = GPIO_ReadInputDataBit(PWM_GPIO_PORT,BACK_PWM_PIN);
}

//检测报警
void check_warning(void)
{
	if(showtextvalue.current_temp_vlaue >= coilvalue.warning1_down &&  showtextvalue.current_temp_vlaue <= coilvalue.warning1_up)
	{
		ALARM1_ON;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING1_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR1_WORK_STATUS_ID,HIDE);
	}
	else
	{
		ALARM1_OFF;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING1_ID,HIDE);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR1_WORK_STATUS_ID,SHOW);
	}
	if(showtextvalue.current_temp_vlaue >= coilvalue.warning2_down &&  showtextvalue.current_temp_vlaue <= coilvalue.warning2_up)
	{
		ALARM2_ON;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING2_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR2_WORK_STATUS_ID,HIDE);
	}
	else
	{
		ALARM2_OFF;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TEMP_WARNING2_ID,HIDE);
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_AR2_WORK_STATUS_ID,SHOW);
	}
}


//温度测量
void temp_detection(void)
{
	showtextvalue.current_temp_vlaue = adjusttemp2 + (float)Max6675_Read_Tem()*0.25;						//实时温度显示
	if(thermocouple_flag)
	{
		SetTextValueFloat(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CURRENT_TEMP_ID, 999.9);	
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR1_TEXT,HIDE);
		SetTextValueFloat(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CURRENT_TEMP_ID,showtextvalue.current_temp_vlaue);
	}
}

void Check_All_Status(void)
{
	gpiostatus = Get_GPIO_Status();
	check_pwm();
	Check_Rs485();
	check_powertime();
	check_screen_connect();
	check_nopowertime();
	check_warning();
	door_open_status();
	devicefunc_selfcheck();
}

//设备定时自检
void device_timing_selfcheck(void)
{
	if((rtctime.Hour == 12)&&(rtctime.Min == 0)&&(rtctime.Sec - 0 <= 3))
	{
		devicefunc_selfcheck();		
	}
}

//设备自检故障显示
void device_selfcheck(void)
{
	uint8_t i,temp[5] = {0};
	HEAT_ON;
	delay_s(1);
	temp[0] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	if(temp[0] != Bit_SET)
	{
		//设置错误图标第一行显示
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR1_TEXT,SHOW);				
	}
	HEAT_OFF;
	delay_s(1);
	SPINNER_RACK_ON;
	delay_s(1);
	temp[1] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	if(temp[1] != Bit_SET)
	{
		//设置错误图标第二行显示
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR2_TEXT,SHOW);
	}
	SPINNER_RACK_OFF;
	delay_s(1);
	CIRCULATING_FUN_ON;
	delay_s(1);
	temp[2] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	if(temp[2] != Bit_SET)
	{
		//设置错误图标第三行显示
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR3_TEXT,SHOW);
	}
	CIRCULATING_FUN_OFF;
	delay_s(1);
	ALARM1_ON;
	delay_s(1);
	temp[3] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	if(temp[3] != Bit_SET)
	{
		//设置错误图标第四行显示
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR4_TEXT,SHOW);
	}
	ALARM1_OFF;
	delay_s(1);
	ALARM2_ON;
	temp[4] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	if(temp[4] != Bit_SET)
	{
		//设置错误图标第五行显示
	}
	delay_s(1);
	ALARM2_OFF;
	for(i = 0;i < 5;i++)
	{
		if(temp[i] != Bit_SET)
		{
			MySetScreen(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN);
		}
	}


}


//设备功能自检故障显示
void devicefunc_selfcheck(void)
{
	if(thermocouple_flag)//有故障
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TROUBLE_INDICATE_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR1_TEXT,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR1_TEXT,HIDE);
	}
//	if(!pwmgpiostatus)
//	{
//		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TROUBLE_INDICATE_ID,SHOW);
//		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR2_TEXT,SHOW);
//		MySetScreen(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN);
//	}
//	else
//	{
//		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR2_TEXT,HIDE);
//		if(current_screen_id == biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN)
//		{
//			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
//		}
//	}
	if(gpiostatus)//有故障
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TROUBLE_INDICATE_ID,SHOW);
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR3_TEXT,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN,BIG_ERROR3_TEXT,HIDE);
	}
	if(!(thermocouple_flag|gpiostatus))
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_TROUBLE_INDICATE_ID,HIDE);
		if(current_screen_id == biglanguage_screen.BIG_SELF_TEST_NOTPASS_SCREEN)
		{
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
		}
	}
}

uint8_t thermalbuff[38] = {0};

//格式转换
void changestruct(void)
{
	uint8_t j;
	char a = '/',b = ':',c = ';',d = ' ';
	j  = sprintf(thermalbuff,"%02d",heattime_log.heattime.Year);
	j += sprintf(thermalbuff+j,"%c",a);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Mon);
	j += sprintf(thermalbuff+j,"%c",a);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Day);
	
	j += sprintf(thermalbuff+j,"%c",d);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Hour);
	j += sprintf(thermalbuff+j,"%c",b);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Min);
	j += sprintf(thermalbuff+j,"%c",b);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Sec);
	j += sprintf(thermalbuff+j,"%c",c);
	
	j += sprintf(thermalbuff+j,"%.1f",heattime_log.set_temp);
	j += sprintf(thermalbuff+j,"%c",c);

	
	j += sprintf(thermalbuff+j,"%.1f",heattime_log.close_temp);
	j += sprintf(thermalbuff+j,"%c",c);

	
	j += sprintf(thermalbuff+j,"%ld",heattime_log.opendoor_duration);
	j += sprintf(thermalbuff+j,"%c",c);
	
	j += sprintf(thermalbuff+j,"%ld",heattime_log.regain_set_temp_time/60);
	j += sprintf(thermalbuff+j,"%c",b);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.regain_set_temp_time%60);
	j += sprintf(thermalbuff+j,"%c",c);
//	printf(" temp is : %s\n",thermalbuff);
	
}


//箱门状态监测,热滞后数据记录
void door_open_status(void)
{
	uint8_t doorflag = 0;
	doorflag = GPIO_ReadInputDataBit(BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);
	if(doorflag)			//开门状态
	{
		door_openstatus++;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,SHOW);
		if(door_openstatus == 1)
		{
			door_closestatus = 0;
			heattime_log.heattime = rtctime;
	//		printf("%d/%d/%d  %02d:%02d:%02d\r\n",heattime_log.heattime.Year,heattime_log.heattime.Mon,heattime_log.heattime.Day,heattime_log.heattime.Hour,heattime_log.heattime.Min,heattime_log.heattime.Sec);
			heattime_log.set_temp = showtextvalue.setting_temp;
	//		printf("\r\nset temp is %f\r\n",heattime_log.set_temp);
		}
	}
	else
	{
		door_closestatus++;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,HIDE);
		if(door_closestatus  == 1)
		{
			door_openstatus = 0;
			heattime_log.closedoor_time = rtctime;
			heattime_log.close_temp = showtextvalue.current_temp_vlaue;
			heattime_log.opendoor_duration = diff_time(heattime_log.heattime,heattime_log.closedoor_time)/60;
			if(myabs(showtextvalue.current_temp_vlaue,showtextvalue.setting_temp) < 2)
			{
				heattime_log.regain_set_temp_time = diff_time(heattime_log.closedoor_time,rtctime);
				changestruct();
				AddDataRecord(biglanguage_screen.BIG_TIME_RECORD_SCREEN,BIG_DATA_RECORD,thermalbuff);
			}
		}
	}
}


//阀门控制方式选择
void control_mode_select(void)
{	
	uint8_t controlmode = 0;
	controlmode = Check_ControlWay();
	if(controlmode)
	{
		SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_ANALOG_OUTPUT,control_mode[0]);
	}
	else
	{
		SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_ANALOG_OUTPUT,control_mode[1]);
	}
	
}



