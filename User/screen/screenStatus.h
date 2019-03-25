#ifndef _SCREEN_STATUS
#define _SCREEN_STATUS
#include "stm32f4xx.h"


#define  CHANGE_AIR_SIZE 21
#define  PASSWORDLENGTH  6
#define  COMMONSIZE  6
#define  RTCBUFFSIZE 16
#define  ADDTESTTIMESIZE  10

typedef struct
{
	uint16_t Year;
	uint8_t Mon;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
}RtcTime;

//设备设置RTC 时间
typedef struct
{
	uint8_t Year[COMMONSIZE];
	uint8_t Mon[COMMONSIZE];
	uint8_t Day[COMMONSIZE];
	uint8_t Hour[COMMONSIZE];
	uint8_t Min[COMMONSIZE];
	uint8_t Sec[COMMONSIZE];
}RtcSetTime;

typedef struct 
{
	uint8_t year[COMMONSIZE];
	uint8_t month[COMMONSIZE];
	uint8_t day[COMMONSIZE];
}AutoNoPowerTimeSet;

typedef struct
{
	uint8_t change_air_times[CHANGE_AIR_SIZE][COMMONSIZE];		//保存风门角度查表值
	uint8_t airdoor_value[COMMONSIZE];				//保存风门角度设置值
	uint8_t screen_light_value[COMMONSIZE];			//保存屏幕亮度值
	uint8_t menu_password[COMMONSIZE];				//菜单密码
	uint8_t  protect_password[PASSWORDLENGTH];		//保护界面输入密码
	uint8_t autonopowerpassword[PASSWORDLENGTH];	//自动断电后恢复密码
	uint8_t Pidvalue[3][PASSWORDLENGTH];			//PID值设置保存
	uint8_t temp_adjust_value[COMMONSIZE];			//温度值校正
	//CoilSaveValue  coilsavevalue;					//保持寄存器存储值
	AutoNoPowerTimeSet autotime;					//自动断电时间设定
	//struct mainTextStruct   textvaluebuff;					//主页面文本控件保存缓存数组
	RtcSetTime  device_time_setting;				//时间设置
}TextValueTab;//this should be screen obj

//struct mainPageStruct
//{
//	//text
//	struct mainTextStruct mainText;
//	struct mainIconStruct mainIcon;
//	//icon
//}mainPage;
//主页面文本设置值


typedef struct{
	float warning1_up;												//报警1上限
	float warning2_up;												//报警2上限
	float temp_backdiff;											//温度回差
	uint8_t menu_password[PASSWORDLENGTH];							//菜单密码
	uint8_t secondtime_password[PASSWORDLENGTH];					//再输入一次菜单密码
	uint8_t protect_password[PASSWORDLENGTH];				//保护界面密码
	uint8_t protect_secondtime_password[PASSWORDLENGTH];	//再输入一次保护密码
	uint16_t change_air_time;						//换气次数
	uint16_t change_max_time;						//换气次数最大值
	uint8_t screen_light_value;						//屏幕亮度调节
	uint8_t air_door_angle;							//风门角度
	uint8_t modbus_address;							//modbus节点地址
	uint8_t modbus_tran_rate;						//modbus通信速率
	uint8_t menu_language;							//菜单语言
}CoilValue;					//保持寄存器存储值
//主页面文本显示
struct mainTextStruct
{
	uint8_t  warmstart_time[RTCBUFFSIZE];
	uint8_t  warmend_time[RTCBUFFSIZE];
	RtcTime start_time;
	uint8_t  softversion[ADDTESTTIMESIZE];

	uint16_t test_time;
	uint16_t left_time_hour;
	uint16_t left_time_min;
	int32_t setTempHi;
	uint32_t setTempLo;
	uint32_t currentTempHi;
	uint32_t currentTempLo;
	uint32_t change_air_time;
	uint32_t addup_testtime;
	uint32_t open_angle;

};

struct argSetErrorStruct
{
	uint8_t test_temp_set_fail;
	uint8_t test_time_set_fail;
	uint8_t change_air_set_fail;
	uint8_t modebus_addr_set_fail;
	uint8_t return_diff_set_fail;
	uint8_t temp_up_set_fail;
	uint8_t temp_up2_set_fail;
	uint8_t light_set_fail;
	uint8_t pass1_set_fail;
	uint8_t pass2_set_fail;
	uint8_t air_angle_set_fail;
	uint8_t temp_adjust_fail;
	uint8_t auto_no_power_set_fail;
};

struct IOStatusStruct
{
	uint8_t sample_frame;
	uint8_t blower;
	uint8_t heat_switch;
	uint8_t door_open;
	uint8_t heat_output;
	uint8_t temp_warnning1;
	uint8_t temp_warnning2;
};

struct mainIconStruct
{
	uint8_t sample_frame;
	uint8_t trouble_indicate;
	uint8_t heat_switch;
	uint8_t pid_run;
	uint8_t fan_operation;
	uint8_t air_door_open_angle;
	uint8_t rs485_comm;
	uint8_t door_open;
	uint8_t heat_output;
	uint8_t temp_warnning1;
	uint8_t temp_warnning2;
	uint8_t ar1_work_status;
	uint8_t ar2_work_status;
	uint8_t fr_work_status;
	uint8_t rr_work_status;
	
};

struct menuTextStruct
{
	float setTemp;
};

struct menuIconStruct
{
	uint8_t com485;
};
struct menuPageStruct
{
	//text
	struct menuTextStruct menuText;
	struct menuIconStruct menuIcon;
	//icon
};



void update_dev_status(void);
void updater_mainScreen(void);
void updater_mainIcon(void);
void updater_mainText(void);
void updater_argSetErrorScreen1(void);
void updater_argSetErrorScreen2(void);

void updater_editAirDoor(uint16_t screen_id);
void updater_PIDScreen(uint16_t screen_id);
void updater_menuParam(uint16_t screen_id);
void updater_adjScreen(uint16_t screen_id);
void updater_menuPageUpdater(uint16_t screen_id);

#endif
