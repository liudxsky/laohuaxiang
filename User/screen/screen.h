#ifndef _SCREEN_H
#define _SCREEN_H
#include "stm32f4xx.h"
#include "dataprocess.h"


#define  SHOW  0
#define  HIDE  1


#define  YES  0
#define  NO   1

#define  BIG_SIZE    1
#define  SMALL_SIZE  0

#define  CHANGE_AIR_SIZE 21
#define  PASSWORDLENGTH  6
#define  COMMONSIZE  6
#define  RTCBUFFSIZE 16
#define  ADDTESTTIMESIZE  10





//5寸显示屏界面宏定义
typedef struct
{
	uint8_t BIG_KEYBOARD;								//小键盘
	uint8_t	BIG_AIR_CHANGE_RATE_SCREEN; 	 			//换气次数编辑界面
	uint8_t BIG_SELF_TEST_NOTPASS_SCREEN;				//自检未通过弹出界面
	uint8_t BIG_SCREAT_PROTECT_SCREEN;		 			//参数设置密码保护界面
	uint8_t BIG_ADJUST_PROTECT_SCREEN;					//调试密码保护界面
	uint8_t BIG_PID_SET_SCREEN;			 				//pid值设置界面
	uint8_t BIG_PARAM_SET_SCREEN;			 			//参数设置界面
	uint8_t BIG_AUTO_NOPOWER_RECOVER;					//自动断电恢复密码界面
	uint8_t BIG_TIME_RECORD_SCREEN; 					//热滞后时间记录界面
	uint8_t BIG_CONTROL_TIME_SET;						//控制器时间设置界面
	uint8_t BIG_PASSWORD_ERROR_SCREEN;					//密码输入错误界面
	uint8_t BIG_ARGUEMENT_SET_ERROR_SCREEN;				//参数设置无效界面
	uint8_t BIG_MAIN_SHOW_SCREEN;		 	 			//主显示界面
	uint8_t BIG_ADJUST_SCREEN;							//隐藏调整界面
}BIG_SCREEN_ID_TAB;


/*************************************BIG***********************************************/

//显示界面    control_id  设置
/*--------------------------------------------------------------------------*/
#define  BIG_CURRENT_TEMP_ID				    21		//实时测量温度控制ID
#define  BIG_CURRENT_TEMP_DECIMAL_ID			22		//实时温度小数部分
#define  BIG_SET_TEMP_ID						23		//设定温度控制ID
#define  BIG_SET_TEMP_DECIMAL_ID				24		//设定温度小数部分
#define  BIG_START_TIME_ID						25		//加热到目标温度的起始时间
#define  BIG_END_TIME_ID						26		//实验结束时间
#define	 BIG_TEST_TIME_ID						27		//设定的实验时间
#define  BIG_ADDUP_TIME_ID						28		//累计实验时间
#define	 BIG_TIME_LEFT_HOUR_ID					29		//剩余时间_小时
#define  BIG_TIME_LEFT_MIN_ID					30		//剩余时间_分钟
#define  BIG_CHANGE_AIR_TIME					31		//换气次数
#define  BIG_SOFT_VER_ID				        34		//软件版本号
#define	 BIG_AIR_DOOR_ANGLE_INPUT_ID			32		//风门开启角度设置
//图标显示ID
#define  BIG_HEAT_SWITCH_ID						2		//加热器通断指示
#define  BIG_PID_RUN_ID							4		//PID自整定运行指示
#define  BIG_FAN_OPERATION_ID					5		//风机运行指示
#define	 BIG_AIR_DOOR_OPEN_ANGLE_ID				8		//风门开机角度指示
#define	 BIG_RS485_COMMU_ID						33		//RS485通信状态
#define  BIG_DOOR_OPEN_ID						10		//门打开指示
#define	 BIG_HEAT_OUTPUT_ID						3		//加热输出指示
#define  BIG_TEMP_WARNING1_ID					6		//温度报警1
#define  BIG_TEMP_WARNING2_ID					7		//温度报警2
#define	 BIG_SAMPLE_FRAME_MOTOR_ID				9		//样架电机运行指示
#define  BIG_TROUBLE_INDICATE_ID				11		//故障指示
#define  BIG_AR1_WORK_STATUS_ID					12		//报警继电器1工作状态指示
#define  BIG_AR2_WORK_STATUS_ID					13		//报警继电器2工作状态指示
#define  BIG_FR_WORK_STATUS_ID					14		//风机继电器工作状态指示
#define  BIG_RR_WORK_STATUS_ID					15		//样架继电器工作状态指示
//按钮控制ID
#define  BIG_MENU_ID							16		//菜单控制
#define  BIG_TIME_RECORD_ID						17		//事件记录
#define	 BIG_SAMPLE_FRAME_MENU_ID				18		//样架
#define  BIG_BLOWER_MENU_ID						19		//风机
#define  BIG_START_OR_PAUSE_ID					20		//启动停止按钮
/*--------------------------------------------------------------------------*/

//密码保护界面    control_id  设置
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD_PROTECT_INPUT				2		//密码保护输入框
/*--------------------------------------------------------------------------*/

//密码保护界面2    control_id  设置
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD2_INPUT				2		//密码保护输入框
/*--------------------------------------------------------------------------*/


//参数设置界面    control_id 输入     设置
/*--------------------------------------------------------------------------*/
#define  BIG_TEST_TIME_VALUE					3		//试验时间设置
#define  BIG_TEST_TEMP_VALUE					4		//试验温度设置
#define	 BIG_WARNING1_UP_VALUE					5		//报警1上限
#define	 BIG_WARNING2_UP_VALUE					6		//报警2上限
#define  BIG_TEMP_RETURN_DIFF					7		//温度回差
#define  BIG_ANALOG_OUTPUT						8		//模拟信号输出
#define  BIG_NEW_PASSWORD						9		//新密码
#define  BIG_SECOND_INPUT_PASSWORD				10		//第二次输入密码
#define	 BIG_CHANGE_AIR_TIME_SET				11		//换气次数设置
#define	 BIG_CHANGE_AIR_MAX_SET					12		//换气次数设置最大值
#define  BIG_MODBUS_NODE_ADDRESS				13		//modbus节点地址
#define  BIG_SCREEN_BRIGHT_ADJUST				18		//屏幕亮度调节
//图标显示id
#define  BIG_BPS_1200							14		//通信速率1200bps
#define  BIG_BPS_2400							15		//通信速率2400bps
#define  BIG_BPS_4800							16		//通信速率4800bps
#define  BIG_BPS_9600							17		//通信速率9600bps
#define  BIG_CHINESE_LANGUAGE					19		//中文
#define	 BIG_ENGLISH_LANGUAGE					20		//英语


#define	 BIG_SET_RETURN_BUTTON					2		//设置界面返回按钮
#define  BIG_ADJUST_BUTTON						21		//调试按钮
/*--------------------------------------------------------------------------*/


//界面7    control_id  设置
/*--------------------------------------------------------------------------*/
#define  BIG_AIR_RETURN_BUTTON					23		//换气界面返回按钮
#define  BIG_CHANGE_AIR_TIME_SET_100			22		//换气次数编辑--100°
#define  BIG_CHANGE_AIR_TIME_SET_95				21		//换气次数编辑--95°
#define  BIG_CHANGE_AIR_TIME_SET_90				20		//换气次数编辑--90°
#define  BIG_CHANGE_AIR_TIME_SET_85				19		//换气次数编辑--85°
#define  BIG_CHANGE_AIR_TIME_SET_80				18		//换气次数编辑--80°
#define  BIG_CHANGE_AIR_TIME_SET_75				17		//换气次数编辑--75°
#define  BIG_CHANGE_AIR_TIME_SET_70				16		//换气次数编辑--70°
#define  BIG_CHANGE_AIR_TIME_SET_65				15		//换气次数编辑--65°
#define  BIG_CHANGE_AIR_TIME_SET_60				14		//换气次数编辑--60°
#define  BIG_CHANGE_AIR_TIME_SET_55				13		//换气次数编辑--55°
#define  BIG_CHANGE_AIR_TIME_SET_50				12		//换气次数编辑--50°
#define  BIG_CHANGE_AIR_TIME_SET_45				11		//换气次数编辑--45°
#define  BIG_CHANGE_AIR_TIME_SET_40				10		//换气次数编辑--40°
#define  BIG_CHANGE_AIR_TIME_SET_35				9		//换气次数编辑--35°
#define  BIG_CHANGE_AIR_TIME_SET_30				8		//换气次数编辑--30°
#define  BIG_CHANGE_AIR_TIME_SET_25				7		//换气次数编辑--25°
#define  BIG_CHANGE_AIR_TIME_SET_20				6		//换气次数编辑--20°
#define  BIG_CHANGE_AIR_TIME_SET_15				5		//换气次数编辑--15°
#define  BIG_CHANGE_AIR_TIME_SET_10				4		//换气次数编辑--10°
#define  BIG_CHANGE_AIR_TIME_SET_5				3		//换气次数编辑--5	°
#define  BIG_CHANGE_AIR_TIME_SET_0				2		//换气次数编辑--0	°
/*--------------------------------------------------------------------------*/

//界面    control_id PID 设置
/*--------------------------------------------------------------------------*/
//图标控制id
#define  BIG_SELF_ADJUST						2		//自整定按键
#define  BIG_PID_RETURN_BUTTON					3	    //PID返回按键
#define  BIG_P_VALUE_SET						4		//P值设定
#define  BIG_I_VALUE_SET						5		//I值设定
#define  BIG_D_VALUE_SET						6		//D值设定
/*--------------------------------------------------------------------------*/

//界面    ADJUST MENU  设置
/*--------------------------------------------------------------------------*/
#define  BIG_NOPOWER_PASSWORD1					2		//断电恢复密码1
#define  BIG_NOPOWER_PASSWORD2					3		//断电恢复密码2
#define  BIG_AIR_DOOR_ANGLE_SET					4		//风门角度控制调节
#define  BIG_TEMP_VALUE_REVUSE_SET				5		//温度值校正控制调节
#define  BIG_YEAR_SET							6		//自动断电年份设定
#define  BIG_MONTH_SET							7		//自动断电月份设定
#define  BIG_DAY_SET							8		//自动断电日设定


#define  BIG_NO_POWER_RETURN_BUTTON				9		//返回按键
#define  BIG_EDIT_BUTTON						10		//换气次数编辑界面按键
#define  BIG_PID_BUTTON							11		//PID按键
/*--------------------------------------------------------------------------*/


//界面    control_id 热滞后时间记录界面
/*--------------------------------------------------------------------------*/
#define  BIG_DATA_RECORD						2
#define  BIG_RECORD_RETURN						3
/*--------------------------------------------------------------------------*/

//界面    control_id 控制器时间设置
/*--------------------------------------------------------------------------*/
#define  BIG_CONTROL_DATE_YEAR_SET       		2		//控制器时间设置:年
#define  BIG_CONTROL_DATE_MONTH_SET       		3		//控制器时间设置:月
#define  BIG_CONTROL_DATE_DAY_SET       		4		//控制器时间设置:日
#define  BIG_CONTROL_TIME_HOUR_SET       		5		//控制器时间设置:时
#define  BIG_CONTROL_TIME_MINUTE_SET       		6		//控制器时间设置:分
#define  BIG_CONTROL_TIME_SECOND_SET       		7		//控制器时间设置:秒
#define  BIG_TIME_SET_RETURN_BUTTON				8		//返回按键
/*--------------------------------------------------------------------------*/

//界面    control_id自动断电密码输入框
/*--------------------------------------------------------------------------*/
#define  BIG_AUTO_NO_POWER_PASSWOORD_INPUT		2		//自动断电恢复密码
/*--------------------------------------------------------------------------*/

//界面    control_id自检未通过弹出界面
/*--------------------------------------------------------------------------*/
#define  BIG_ERROR1_TEXT						2		//热电偶输入故障
#define  BIG_ERROR2_TEXT						3		//控制器内部故障1
#define  BIG_ERROR3_TEXT						4		//控制器内部故障2
#define  BIG_ERROR4_TEXT						5		//电动执行机构连接中断
/*--------------------------------------------------------------------------*/

//界面    control_id参数设置无效界面
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_UPDATE_FAIL					2		//密码更新失败
#define  BIG_TESTTEMP_SET_FAIL					3		//实验温度设定无效
#define  BIG_TESTTIME_SET_FAIL					4		//实验时间设定无效
#define  BIG_TEMP_UP_SET_FAIL					5		//温度上限设定无效
#define  BIG_RETURN_DIFF_SET_FAIL				6		//温度回差设定无效
#define  BIG_CHANGE_AIR_SET_FAIL				7		//换气次数设定无效
#define  BIG_MODBUS_ADDRESS_SET_FAIL			8		//MOdbus地址设定无效

#define	 BIG_FAIL_RETURN_BUTTON					9		//返回按键
/*--------------------------------------------------------------------------*/


//界面    control_id 密码错误显示界面
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_ERROR_RETURN_BUTTON			2		//密码错误返回
/*--------------------------------------------------------------------------*/
/************************************************************************************/



#define NOTIFY_TOUCH_PRESS     	 	0X01   	//触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE  		0X03  	//触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK  		0X0C  	//写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD  	0X0D  	//写FLASH失败
#define NOTIFY_READ_FLASH_OK  		0X0B  	//读FLASH成功
#define NOTIFY_READ_FLASH_FAILD  	0X0F  	//读FLASH失败
#define NOTIFY_MENU                 0X14  	//菜单事件通知
#define NOTIFY_TIMER                0X43  	//定时器超时通知
#define NOTIFY_CONTROL              0XB1  	//控件更新通知
#define NOTIFY_READ_RTC             0XF7  	//读取RTC时间

#define MSG_GET_CURRENT_SCREEN 		0X01	//画面ID变化通知
#define MSG_GET_DATA                0X11	//控件数据通知
#define MSG_GET_ICON				0X26	//图标动画控件更新通知

#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])  //从缓冲区取32位数据

enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  				//按钮
	kCtrlText,  					//文本
	kCtrlProgress,  				//进度条
	kCtrlSlider,    				//滑动条
	kCtrlMeter,  					//仪表
	kCtrlDropList, 					//下拉列表
	kCtrlAnimation, 				//动画
	kCtrlRTC, 						//时间显示
	kCtrlGraph, 					//曲线图控件
	kCtrlTable, 					//表格控件
	kCtrlMenu,						//菜单控件
	kCtrlSelector,					//选择控件
	kCtrlQRCode,					//二维码
	kCtrlIcon,						//图标
};

#pragma pack(push)
#pragma pack(1)						//按字节对齐

typedef struct
{
	uint8_t    cmd_head;  			//帧头
	uint8_t    cmd_type;  			//命令类型(UPDATE_CONTROL)	
	uint8_t    ctrl_msg;   			//CtrlMsgType-指示消息的类型
	uint16_t   screen_id;  			//产生消息的画面ID
	uint16_t   control_id;  		//产生消息的控件ID
	uint8_t    control_type; 		//控件类型
	uint8_t    param[256];			//可变长度参数，最多256个字节
	uint8_t    cmd_tail[4];   		//帧尾
}CTRL_MSG,*PCTRL_MSG;

//RTC 时间
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


//自动断电时间
typedef struct 
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
}AutoNoPowerTime;

//自动断电时间设置
typedef struct 
{
	uint8_t year[COMMONSIZE];
	uint8_t month[COMMONSIZE];
	uint8_t day[COMMONSIZE];
}AutoNoPowerTimeSet;


//主页面文本设置值
typedef struct 
{
	float 	current_temp_vlaue;
	float 	setting_temp;
	RtcTime start_time;
	RtcTime end_time;
	float test_time;
	uint16_t left_time_hou;
	uint16_t left_time_min;
	uint8_t add_all_time;
	uint8_t change_air_time;
	uint8_t air_door_angle;
}MainShowTextValue;


//主页面文本显示
typedef struct 
{
	uint8_t  softversion[ADDTESTTIMESIZE];
	uint8_t current_temp_int[COMMONSIZE];
	uint8_t current_temp_dec[COMMONSIZE];
	uint8_t setting_temp_int[COMMONSIZE];
	uint8_t setting_temp_dec[COMMONSIZE];
	uint8_t  start_time[RTCBUFFSIZE];
	uint8_t  end_time[RTCBUFFSIZE];
	uint8_t test_time[COMMONSIZE];
	uint8_t left_time_hour[COMMONSIZE];
	uint8_t left_time_min[COMMONSIZE];
	uint8_t add_all_time[ADDTESTTIMESIZE];
	uint8_t change_air_time[COMMONSIZE];
	uint8_t air_door_angle[COMMONSIZE];
}MainShowText;


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
	CoilSaveValue  coilsavevalue;					//保持寄存器存储值
	AutoNoPowerTimeSet autotime;					//自动断电时间设定
	MainShowText   textvaluebuff;					//主页面文本控件保存缓存数组
	RtcSetTime  device_time_setting;				//时间设置
}TextValueTab;



typedef struct
{
	float PID_P;
	float PID_I;
	float PID_D;
}Pid_Value;


#pragma pack(pop)


void SetTextValueInt32(uint16_t screen_id, uint16_t control_id,int32_t value);
void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value);

/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16_t screen_id);

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y);

/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);

/*! 
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str);

/*! 
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value);

/*! 
 *  \brief  动画控件通知
 *  \details  当动画改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param starus    0x00表示触摸按下，0x01表示弹起
 *  \param iconimage_id 值
 */
void  NotifyAnimation(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id);

/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value);

/*! 
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value);

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state);

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item);

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id);

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length);

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8_t status);

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
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);

void ProcessMessage( PCTRL_MSG msg, uint16_t size );


void startscreen(void);


void  mergetimechar(RtcTime datetime);

void mergehour_min(uint16_t hour,uint16_t min);

uint32_t to_day(RtcTime time);

void endtimecalcu(RtcTime starttime,float testtime);

uint32_t diff_time(RtcTime starttime,RtcTime endtime);

float myabs(float a,float b);

void lefttimecalculate(void);

void start_endtime_set(void);

void addup_testtime(void);

uint8_t change_air_times(void);

void check_pidstatus(void);



void screen_init(void);



uint8_t DectoBCD(uint8_t Dec);

uint8_t BcdToDec(uint8_t bcd);

uint8_t max_change_air(uint8_t *buff);

uint8_t judge_changeair_time(uint16_t change_time);

void update_dev_status(void);

void check_heat_switch(void);



#endif

















