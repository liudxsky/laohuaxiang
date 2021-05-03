#ifndef _SCREEN_H
#define _SCREEN_H
#include "stm32f4xx.h"
#include "dataprocess.h"
#include "./screen/screenStatus.h"

#define  SHOW  0
#define  HIDE  1


#define  YES  0
#define  NO   1

#define  BIG_SIZE    1
#define  SMALL_SIZE  0



typedef struct
{
	uint8_t BIG_KEYBOARD;								//keyboard screen
	uint8_t	BIG_AIR_CHANGE_RATE_SCREEN; 	 			//air times edit screen
	uint8_t BIG_SCREAT_PROTECT_SCREEN;		 			//menu protect password input screen
	uint8_t BIG_ADJUST_PROTECT_SCREEN;					//adjust protect password input screen 
	uint8_t BIG_PID_SET_SCREEN;			 				//pid setting screen 
	uint8_t BIG_PARAM_SET_SCREEN;			 			//param menu setting screen
	uint8_t BIG_AUTO_NOPOWER_RECOVER;					//auto no power recover password input screen
	uint8_t BIG_TIME_RECORD_SCREEN; 					//door open time record screen
	uint8_t BIG_CONTROL_TIME_SET;						//device time setting screen 
	uint8_t BIG_PASSWORD_ERROR_SCREEN;					//password input wrong screen
	uint8_t BIG_ARGUEMENT_SET_ERROR_SCREEN1;			//param setting wrong screen1
	uint8_t BIG_ARGUEMENT_SET_ERROR_SCREEN2;			//param setting wrong screen2
	uint8_t BIG_MAIN_SHOW_SCREEN;		 	 			//main show screen
	uint8_t BIG_ADJUST_SCREEN;							//hide adjust screen  
	uint8_t BIG_ABNORMAL_POWEROFF;						//nopower sudden
}BIG_SCREEN_ID_TAB;


/*************************************BIG***********************************************/

//main show screen control id 
/*--------------------------------------------------------------------------*/
//text show
#define  BIG_CURRENT_TEMP_ID				    21		//current temp value High  
#define  BIG_CURRENT_TEMP_DECIMAL_ID			22		//current temp value Low
#define  BIG_SET_TEMP_ID						23		//set temp value High 
#define  BIG_SET_TEMP_DECIMAL_ID				24		//set temp value Low
#define  BIG_START_TIME_ID						25		//test start time 
#define  BIG_END_TIME_ID						26		//test end time
#define	 BIG_TEST_TIME_ID						27		//test time
#define  BIG_ADDUP_TIME_ID						28		//total add test time  
#define	 BIG_TIME_LEFT_HOUR_ID					29		//test left time -- hour
#define  BIG_TIME_LEFT_MIN_ID					30		//test left time -- minute
#define  BIG_CHANGE_AIR_TIME					31		//change air times
#define  BIG_SOFT_VER_ID				        34		//soft versions
#define	 BIG_AIR_DOOR_ANGLE_INPUT_ID			32		//air door angle show
//icon ID
#define  BIG_HEAT_SWITCH_ID						2		//heat switch icon
#define	 BIG_HEAT_OUTPUT_ID						3		//heat output icon
#define  BIG_PID_RUN_ID							4		//PID icon
#define  BIG_FAN_OPERATION_ID					5		//fan icon
#define  BIG_TEMP_WARNING1_ID					6		//temp warning 1
#define  BIG_TEMP_WARNING2_ID					7		//temp warning 2
#define	 BIG_AIR_DOOR_OPEN_ANGLE_ID				8		//air door open angle icon
#define	 BIG_SAMPLE_FRAME_MOTOR_ID				9		//sample frame icon
#define  BIG_DOOR_OPEN_ID						10		//door open status icon
#define  BIG_TROUBLE_INDICATE_ID				11		//device warning icon
#define  BIG_AR1_WORK_STATUS_ID					12		//AR1 
#define  BIG_AR2_WORK_STATUS_ID					13		//AR2
#define  BIG_FR_WORK_STATUS_ID					14		//fan    FR 
#define  BIG_RR_WORK_STATUS_ID					15		//sample RR

#define	 BIG_RS485_COMMU_ID						33		//RS485 trans status icon
#define BIG_INTERLOCK_ICON_ID 36
//button
#define  BIG_MENU_ID							16		//menu button
#define  BIG_TIME_RECORD_ID						17		//time record button
#define	 BIG_SAMPLE_FRAME_MENU_ID				18		//sample frame button
#define  BIG_BLOWER_MENU_ID						19		//blower button
#define  BIG_START_OR_PAUSE_ID					20		//start or pause warm button
#define BIG_INTERLOCK_BTN_ID 							35
/*--------------------------------------------------------------------------*/

//menu password input screen
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD_PROTECT_INPUT				2		//menu password input
#define  BIG_PASSWORD1_RETURN_BUTTON   			3  		//return button 
/*--------------------------------------------------------------------------*/

//adjust password input screen 
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD2_INPUT					2		//menu password input
#define  BIG_PASSWORD2_RETURN_BUTTON   			3  		//return button 

/*--------------------------------------------------------------------------*/


//menu param setting screen
/*--------------------------------------------------------------------------*/
#define  BIG_TEST_TIME_VALUE					3		//test time setting
#define  BIG_TEST_TEMP_VALUE					4		//test temp setting
#define	 BIG_WARNING1_UP_VALUE					5		//warning1 up setting
#define	 BIG_WARNING2_UP_VALUE					6		//warning2 up setting
#define  BIG_TEMP_RETURN_DIFF					7		//temp diff back setting
#define  BIG_ANALOG_OUTPUT						8		//analog sign output 
#define  BIG_NEW_PASSWORD						9		//menu password setting first
#define  BIG_SECOND_INPUT_PASSWORD				10		//menu password setting second
#define	 BIG_CHANGE_AIR_TIME_SET				11		//change air times setting 
#define	 BIG_CHANGE_AIR_MAX_SET					12		//max change air times setting
#define  BIG_MODBUS_NODE_ADDRESS				13		//modbus node address
#define  BIG_SCREEN_BRIGHT_ADJUST				18		//screen bright adjust
//icon id
#define  BIG_BPS_1200							14		//rate 1200bps
#define  BIG_BPS_2400							15		//rate 2400bps
#define  BIG_BPS_4800							16		//rate 4800bps
#define  BIG_BPS_9600							17		//rate 9600bps
#define  BIG_CHINESE_LANGUAGE					19		//chinese language select 
#define	 BIG_ENGLISH_LANGUAGE					20		//english language select 

#define  BIG_BPS_1200_BUTTON					22		//rate 1200bps button
#define  BIG_BPS_2400_BUTTON					23		//rate 2400bps button
#define  BIG_BPS_4800_BUTTON					24		//rate 4800bps button
#define  BIG_BPS_9600_BUTTON					25		//rate 9600bps button
#define  BIG_CHINESE_LANGUAGE_BUTTON			26		//chinese language select button
#define	 BIG_ENGLISH_LANGUAGE_BUTTON			27		//english language select button


#define	 BIG_SET_RETURN_BUTTON					2		//return button
#define  BIG_ADJUST_BUTTON						21		//to adjust screen button
/*--------------------------------------------------------------------------*/


//change air times edit screen
/*--------------------------------------------------------------------------*/
#define  BIG_AIR_RETURN_BUTTON					23		//return button


#define  BIG_CHANGE_AIR_TIME_SET_100			22		//angle--100
#define  BIG_CHANGE_AIR_TIME_SET_95				21		//angle--95
#define  BIG_CHANGE_AIR_TIME_SET_90				20		//angle--90
#define  BIG_CHANGE_AIR_TIME_SET_85				19		//angle--85
#define  BIG_CHANGE_AIR_TIME_SET_80				18		//angle--80
#define  BIG_CHANGE_AIR_TIME_SET_75				17		//angle--75
#define  BIG_CHANGE_AIR_TIME_SET_70				16		//angle--70
#define  BIG_CHANGE_AIR_TIME_SET_65				15		//angle--65
#define  BIG_CHANGE_AIR_TIME_SET_60				14		//angle--60
#define  BIG_CHANGE_AIR_TIME_SET_55				13		//angle--55
#define  BIG_CHANGE_AIR_TIME_SET_50				12		//angle--50
#define  BIG_CHANGE_AIR_TIME_SET_45				11		//angle--45
#define  BIG_CHANGE_AIR_TIME_SET_40				10		//angle--40
#define  BIG_CHANGE_AIR_TIME_SET_35				9		//angle--35
#define  BIG_CHANGE_AIR_TIME_SET_30				8		//angle--30
#define  BIG_CHANGE_AIR_TIME_SET_25				7		//angle--25
#define  BIG_CHANGE_AIR_TIME_SET_20				6		//angle--20
#define  BIG_CHANGE_AIR_TIME_SET_15				5		//angle--15
#define  BIG_CHANGE_AIR_TIME_SET_10				4		//angle--10
#define  BIG_CHANGE_AIR_TIME_SET_5				3		//angle--5	
#define  BIG_CHANGE_AIR_TIME_SET_0				2		//angle--0	
/*--------------------------------------------------------------------------*/

//pid setting screen
/*--------------------------------------------------------------------------*/
#define  BIG_SELF_ADJUST						2		//self adjust button
#define  BIG_PID_RETURN_BUTTON					3	    //return button

#define  BIG_P_VALUE_SET						4		// P value
#define  BIG_I_VALUE_SET						5		// I value
#define  BIG_D_VALUE_SET						6		// D value
/*--------------------------------------------------------------------------*/

//hide adjust screen 
/*--------------------------------------------------------------------------*/
#define  BIG_ADJUST_PASSWORD1					2		//adjust password setting first
#define  BIG_ADJUST_PASSWORD2					3		//adjust password setting second
#define  BIG_AIR_DOOR_ANGLE_SET					4		//air door angle setting
#define  BIG_TEMP_VALUE_REVUSE_SET				5		//temp adjust value setting
#define  BIG_YEAR_SET							6		//auto no power time -- year setting
#define  BIG_MONTH_SET							7		//auto no power time -- month setting
#define  BIG_DAY_SET							8		//auto no power time -- day setting
#define  BIG_MAX_TEMPER_SET 12

#define  BIG_NO_POWER_RETURN_BUTTON				9		//return button
#define  BIG_EDIT_BUTTON						10		//to air times edit screen
#define  BIG_PID_BUTTON							11		//to PID setting screen

/*--------------------------------------------------------------------------*/


//door open time record screen
/*--------------------------------------------------------------------------*/
#define  BIG_DATA_RECORD						2		//data record table
#define  BIG_RECORD_RETURN						3		//return button
/*--------------------------------------------------------------------------*/

//device time setting screen
/*--------------------------------------------------------------------------*/
#define  BIG_CONTROL_DATE_YEAR_SET       		2		//device time setting -- Year
#define  BIG_CONTROL_DATE_MONTH_SET       		3		//device time setting -- Mon
#define  BIG_CONTROL_DATE_DAY_SET       		4		//device time setting -- Day
#define  BIG_CONTROL_TIME_HOUR_SET       		5		//device time setting -- Hour
#define  BIG_CONTROL_TIME_MINUTE_SET       		6		//device time setting -- Min
#define  BIG_CONTROL_TIME_SECOND_SET       		7		//device time setting -- Sec


#define  BIG_TIME_SET_RETURN_BUTTON				8		//return button
/*--------------------------------------------------------------------------*/

//auto no power password input screen 
/*--------------------------------------------------------------------------*/
#define  BIG_AUTO_NO_POWER_PASSWOORD_INPUT		2		//input password text
/*--------------------------------------------------------------------------*/



//param setting invalid screen 1
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_UPDATE_FAIL1					2		//password update fail
#define  BIG_TESTTEMP_SET_FAIL1					3		//test temp setting fail 
#define  BIG_TESTTIME_SET_FAIL1					4		//test time setting fail
#define  BIG_TEMP_UP_SET_FAIL1					5		//temp up limit setting fail
#define  BIG_RETURN_DIFF_SET_FAIL1				6		//temp back diff setting fail
#define  BIG_CHANGE_AIR_SET_FAIL1				7		//change air times setting fail
#define  BIG_MODBUS_ADDRESS_SET_FAIL1			8		//modbus address setting fail 
#define  BIG_SCREEN_LIGHT_SET_FAIL1				9		//screen light setting fail

#define	 BIG_FAIL_RETURN_BUTTON1				10		//return button
/*--------------------------------------------------------------------------*/

//param setting invalid screen 2
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_UPDATE_FAIL2					2		//adjust password input fail 
#define  BIG_AIRDOOR_ANGLE_SET_FAIL2			3		//air door angle setting fail
#define  BIG_TEMP_ADJUST_SET_FAIL2				4		//temp adjust setting fail
#define  BIG_AUTO_NO_POWER_SET_FAIL2			5		//auto no power time setting fail


#define	 BIG_FAIL_RETURN_BUTTON2				6		//return button
/*--------------------------------------------------------------------------*/

//abnormal power off screen
#define BIG_ABNORMAL_RETURN_BUTTON 2
#define BIG_ABNORMAL_TIME 3


//password input wrong screen
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_ERROR_RETURN_BUTTON			2		//return button
/*--------------------------------------------------------------------------*/
/************************************************************************************/

//da cai screen some command type
/************************************************************************************/
#define NOTIFY_TOUCH_PRESS     	 	0X01   	
#define NOTIFY_TOUCH_RELEASE  		0X03  	
#define NOTIFY_WRITE_FLASH_OK  		0X0C  	
#define NOTIFY_WRITE_FLASH_FAILD  	0X0D  	
#define NOTIFY_READ_FLASH_OK  		0X0B  	
#define NOTIFY_READ_FLASH_FAILD  	0X0F  	
#define NOTIFY_MENU                 0X14  	
#define NOTIFY_TIMER                0X43  	
#define NOTIFY_CONTROL              0XB1  	
#define NOTIFY_READ_RTC             0XF7  	

#define MSG_GET_CURRENT_SCREEN 		0X01	
#define MSG_GET_DATA                0X11	
#define MSG_GET_ICON				0X26	

#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])  

/****************************************************************************************************************************************/

enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  				
	kCtrlText,  					
	kCtrlProgress,  				
	kCtrlSlider,    				
	kCtrlMeter,  					
	kCtrlDropList, 					
	kCtrlAnimation, 				
	kCtrlRTC, 						
	kCtrlGraph, 					
	kCtrlTable, 					
	kCtrlMenu,						
	kCtrlSelector,					
	kCtrlQRCode,					
	kCtrlIcon,						
};

#pragma pack(push)
#pragma pack(1)					

typedef struct
{
	uint8_t    cmd_head;  			//head 
	uint8_t    cmd_type;  			//(UPDATE_CONTROL)	
	uint8_t    ctrl_msg;   			//CtrlMsgType
	uint16_t   screen_id;  			//screen ID
	uint16_t   control_id;  		//control ID
	uint8_t    control_type; 		//control type
	uint8_t    param[256];			//data
	uint8_t    cmd_tail[4];   		//tail
}CTRL_MSG,*PCTRL_MSG;



typedef struct
{
	float PID_P;
	float PID_I;
	float PID_D;
}Pid_Value;


#pragma pack(pop)

void SetTextValueRTC(uint16_t screen_id, uint16_t control_id,RtcTime datetime);

void SetTextValueInt32(uint16_t screen_id, uint16_t control_id,int32_t value);

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value);

void NotifyScreen(uint16_t screen_id);

void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y);

void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);

void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str);

void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value);

void  NotifyAnimation(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id);

void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value);

void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value);

void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state);

void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item);

void NotifyTimer(uint16_t screen_id, uint16_t control_id);

void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length);

void NotifyWriteFlash(uint8_t status);

void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);

void ProcessMessage( PCTRL_MSG msg, uint16_t size );



#endif














