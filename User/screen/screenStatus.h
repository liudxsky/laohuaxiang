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

//�豸����RTC ʱ��
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
	uint8_t change_air_times[CHANGE_AIR_SIZE][COMMONSIZE];		//������ŽǶȲ��ֵ
	uint8_t airdoor_value[COMMONSIZE];				//������ŽǶ�����ֵ
	uint8_t screen_light_value[COMMONSIZE];			//������Ļ����ֵ
	uint8_t menu_password[COMMONSIZE];				//�˵�����
	uint8_t  protect_password[PASSWORDLENGTH];		//����������������
	uint8_t autonopowerpassword[PASSWORDLENGTH];	//�Զ��ϵ��ָ�����
	uint8_t Pidvalue[3][PASSWORDLENGTH];			//PIDֵ���ñ���
	uint8_t temp_adjust_value[COMMONSIZE];			//�¶�ֵУ��
	//CoilSaveValue  coilsavevalue;					//���ּĴ����洢ֵ
	AutoNoPowerTimeSet autotime;					//�Զ��ϵ�ʱ���趨
	//struct mainTextStruct   textvaluebuff;					//��ҳ���ı��ؼ����滺������
	RtcSetTime  device_time_setting;				//ʱ������
}TextValueTab;//this should be screen obj

//struct mainPageStruct
//{
//	//text
//	struct mainTextStruct mainText;
//	struct mainIconStruct mainIcon;
//	//icon
//}mainPage;
//��ҳ���ı�����ֵ


typedef struct{
	float warning1_up;												//����1����
	float warning2_up;												//����2����
	float temp_backdiff;											//�¶Ȼز�
	uint8_t menu_password[PASSWORDLENGTH];							//�˵�����
	uint8_t secondtime_password[PASSWORDLENGTH];					//������һ�β˵�����
	uint8_t protect_password[PASSWORDLENGTH];				//������������
	uint8_t protect_secondtime_password[PASSWORDLENGTH];	//������һ�α�������
	uint16_t change_air_time;						//��������
	uint16_t change_max_time;						//�����������ֵ
	uint8_t screen_light_value;						//��Ļ���ȵ���
	uint8_t air_door_angle;							//���ŽǶ�
	uint8_t modbus_address;							//modbus�ڵ��ַ
	uint8_t modbus_tran_rate;						//modbusͨ������
	uint8_t menu_language;							//�˵�����
}CoilValue;					//���ּĴ����洢ֵ
//��ҳ���ı���ʾ
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
