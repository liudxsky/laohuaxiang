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



#define  BUFFSIZE  20

typedef struct 
{
	uint8_t screen_size;	
	uint8_t language;
	uint16_t screen_id;
	uint16_t control_id;
}ID_Table;





//5����ʾ������궨��
typedef struct
{
	uint8_t BIG_KEYBOARD;								//С����
	uint8_t	BIG_AIR_CHANGE_RATE_SCREEN; 	 			//���������༭����
	uint8_t BIG_AIR_DOOR_SCREEN; 			 			//���ŽǶȿ��Ƶ��ڽ���
	uint8_t BIG_BRIGHT_ADJUST_SCREEN;		 			//��Ļ���ȵ��ڽ���
	uint8_t BIG_SELF_TEST_NOTPASS_SCREEN;				//�Լ�δͨ����������
	uint8_t BIG_SCREAT_PROTECT_SCREEN;		 			//�����������뱣������
	uint8_t BIG_PID_SET_SCREEN;			 				//pidֵ���ý���
	uint8_t BIG_PARAM_SET_SCREEN;			 			//�������ý���
	uint8_t BIG_AUTO_POWEROFF_TIMESET_SCREEN;			//�Զ��ϵ�ʱ�����ý���
	uint8_t BIG_START_INIT_SCREEN;  		 			//��������
	uint8_t BIG_AUTO_NOPOWER_RECOVER;					//�Զ��ϵ�ָ��������
	uint8_t BIG_TEMP_VALUE_REVISE_SCREEN;				//�¶�ֵУ������
	uint8_t BIG_SELF_TEST_SCREEN; 		 	 			//�Լ����
	uint8_t BIG_TIME_RECORD_SCREEN; 					//���ͺ�ʱ���¼����
	uint8_t BIG_CONTROL_TIME_SET;						//������ʱ�����ý���
	uint8_t BIG_PASSWORD_ERROR_SCREEN;					//��������������
	uint8_t BIG_ARGUEMENT_SET_ERROR_SCREEN;				//����������Ч����
	uint8_t BIG_MAIN_SHOW_SCREEN;		 	 			//����ʾ����
	uint8_t BIG_TEMP_CURVE_SHOW_SCREEN;					//�¶�������ʾ����
	uint8_t BIG_AUTO_POWEROFF_POPUPWINDOWS_SCREEN;		//�Զ��ϵ絯������
}BIG_SCREEN_ID_TAB;


//��������    control_id  ����
/*************************************BIG***********************************************/
/*--------------------------------------------------------------------------*/
#define  BIG_START_INIT_ID						2		//��������������ID
/*--------------------------------------------------------------------------*/

//��ʾ����    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_CURRENT_TEMP_ID				    18		//��ʱ�����¶ȿ���ID
#define  BIG_SET_TEMP_ID						19		//�趨�¶ȿ���ID
#define  BIG_START_TIME_ID						11		//���ȵ�Ŀ���¶ȵ���ʼʱ��
#define  BIG_END_TIME_ID						12		//ʵ�����ʱ��
#define	 BIG_TEST_TIME_ID						13		//�趨��ʵ��ʱ��
#define  BIG_ADDUP_TIME_ID						16		//�ۼ�ʵ��ʱ��
#define	 BIG_TIME_LEFT_ID						14		//ʣ��ʱ��
#define  BIG_CHANGE_AIR_TIME					15		//��������
#define  BIG_SOFT_VER_ID				        17		//����汾��
#define	 BIG_AIR_DOOR_ANGLE_INPUT_ID			29		//���ſ����Ƕ�����
//ͼ����ʾID
#define  BIG_HEAT_SWITCH_ID						2		//������ͨ��ָʾ
#define  BIG_PID_RUN_ID							4		//PID����������ָʾ
#define  BIG_FAN_OPERATION_ID					5		//�������ָʾ
#define	 BIG_AIR_DOOR_OPEN_ANGLE_ID				8		//���ſ����Ƕ�ָʾ
#define	 BIG_RS485_COMMU_ID						20		//RS485ͨ��״̬
#define  BIG_DOOR_OPEN_ID						10		//�Ŵ�ָʾ
#define	 BIG_HEAT_OUTPUT_ID						3		//�������ָʾ
#define  BIG_TEMP_WARNING1_ID					6		//�¶ȱ���1
#define  BIG_TEMP_WARNING2_ID					7		//�¶ȱ���2
#define	 BIG_SAMPLE_FRAME_MOTOR_ID				9		//���ܵ������ָʾ
#define  BIG_TROUBLE_INDICATE_ID				21		//����ָʾ
#define  BIG_AR1_WORK_STATUS_ID					27		//�����̵���1����״ָ̬ʾ
#define  BIG_AR2_WORK_STATUS_ID					28		//�����̵���2����״ָ̬ʾ
#define  BIG_FR_WORK_STATUS_ID					30		//����̵�������״ָ̬ʾ
#define  BIG_RR_WORK_STATUS_ID					31		//���̵ܼ�������״ָ̬ʾ
//��ť����ID
#define  BIG_MENU_ID							22		//�˵�����
#define  BIG_SELF_TEST_MENU_ID					23		//�Լ�
#define	 BIG_SAMPLE_FRAME_MENU_ID				24		//����
#define  BIG_BLOWER_MENU_ID						25		//���
#define  BIG_START_OR_PAUSE_ID					26		//����ֹͣ��ť
/*--------------------------------------------------------------------------*/

//�Լ����    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_SELF_TEST_PROGRAM					2		//�Լ���ƽ�����
/*--------------------------------------------------------------------------*/

//���뱣������    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD_PROTECT_INPUT				2		//���뱣�������
/*--------------------------------------------------------------------------*/

//�������ý���    control_id ����     ����
/*--------------------------------------------------------------------------*/
#define  BIG_TEST_TIME_VALUE					17		//����ʱ������
#define  BIG_TEST_TEMP_VALUE					18		//�����¶�����
#define	 BIG_WARNING1_UP_VALUE					11		//����1����
#define	 BIG_WARNING1_DOWN_VALUE				12		//����1����
#define	 BIG_WARNING2_UP_VALUE					13		//����2����
#define	 BIG_WARNING2_DOWN_VALUE				14		//����2����
#define  BIG_ANALOG_OUTPUT						19		//ģ���ź����
#define  BIG_NEW_PASSWORD						20		//������
#define  BIG_SECOND_INPUT_PASSWORD				21		//�ڶ�����������
#define	 BIG_CHANGE_AIR_TIME_SET				15		//������������
#define	 BIG_CHANGE_AIR_MAX_SET					10		//���������������ֵ
#define  BIG_MODBUS_NODE_ADDRESS				16		//modbus�ڵ��ַ
//ͼ����ʾid
#define  BIG_BPS_1200							3		//ͨ������1200bps
#define  BIG_BPS_2400							4		//ͨ������2400bps
#define  BIG_BPS_4800							5		//ͨ������4800bps
#define  BIG_BPS_9600							6		//ͨ������9600bps
#define  BIG_CHINESE_LANGUAGE					7		//����
#define	 BIG_ENGLISH_LANGUAGE					8		//Ӣ��


#define	 BIG_SET_RETURN_BUTTON					9		//���ý��淵�ذ�ť
/*--------------------------------------------------------------------------*/


//��Ļ���ȵ������ý���    control_id �ı�����     ����
/*--------------------------------------------------------------------------*/
#define  BIG_SCREEN_BRIGHT_ADJUST				2		//��Ļ���ȵ�������
/*--------------------------------------------------------------------------*/

//���ŽǶȿ��Ƶ��ڽ���            control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_AIR_DOOR_ANGLE_SET					2		//���ŽǶȿ��Ƶ���
/*--------------------------------------------------------------------------*/

//����7    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_AIR_RETURN_BUTTON					2		//�������淵�ذ�ť
#define  BIG_CHANGE_AIR_TIME_SET_90				3		//���������༭--90��
#define  BIG_CHANGE_AIR_TIME_SET_85				4		//���������༭--85��
#define  BIG_CHANGE_AIR_TIME_SET_80				5		//���������༭--80��
#define  BIG_CHANGE_AIR_TIME_SET_75				6		//���������༭--75��
#define  BIG_CHANGE_AIR_TIME_SET_70				7		//���������༭--70��
#define  BIG_CHANGE_AIR_TIME_SET_65				8		//���������༭--65��
#define  BIG_CHANGE_AIR_TIME_SET_60				9		//���������༭--60��
#define  BIG_CHANGE_AIR_TIME_SET_55				10		//���������༭--55��
#define  BIG_CHANGE_AIR_TIME_SET_50				11		//���������༭--50��
#define  BIG_CHANGE_AIR_TIME_SET_45				12		//���������༭--45��
#define  BIG_CHANGE_AIR_TIME_SET_40				13		//���������༭--40��
#define  BIG_CHANGE_AIR_TIME_SET_35				14		//���������༭--35��
#define  BIG_CHANGE_AIR_TIME_SET_30				15		//���������༭--30��
#define  BIG_CHANGE_AIR_TIME_SET_25				16		//���������༭--25��
#define  BIG_CHANGE_AIR_TIME_SET_20				17		//���������༭--20��
#define  BIG_CHANGE_AIR_TIME_SET_15				18		//���������༭--15��
#define  BIG_CHANGE_AIR_TIME_SET_10				19		//���������༭--10��
#define  BIG_CHANGE_AIR_TIME_SET_5				20		//���������༭--5	��
#define  BIG_CHANGE_AIR_TIME_SET_0				21		//���������༭--0	��
/*--------------------------------------------------------------------------*/

//����    control_id PID ����
/*--------------------------------------------------------------------------*/
//ͼ�����id
#define  BIG_SELF_ADJUST						2		//����������
#define  BIG_PID_RETURN_BUTTON					3	    //PID���ذ���
#define  BIG_P_VALUE_SET						4		//Pֵ�趨
#define  BIG_I_VALUE_SET						5		//Iֵ�趨
#define  BIG_D_VALUE_SET						6		//Dֵ�趨
/*--------------------------------------------------------------------------*/

//����    control_id �Զ��ϵ�ʱ�� ����
/*--------------------------------------------------------------------------*/
#define  BIG_YEAR_SET							2		//����趨
#define  BIG_MONTH_SET							3		//�·��趨
#define  BIG_DAY_SET							4		//���趨
#define  BIG_NO_POWER_RETURN_BUTTON				9		//���ذ���
/*--------------------------------------------------------------------------*/

//�¶�ֵ����  ���ý���    control_id 
/*--------------------------------------------------------------------------*/
#define  BIG_TEMP_VALUE_REVUSE_SET				2		//�¶�ֵУ�����Ƶ���
/*--------------------------------------------------------------------------*/

//����    control_id ���ͺ�ʱ���¼����
/*--------------------------------------------------------------------------*/
#define  BIG_DATA_RECORD						2
/*--------------------------------------------------------------------------*/

//����    control_id ������ʱ������
/*--------------------------------------------------------------------------*/
#define  BIG_CONTROL_DATE_YEAR_SET       		2		//������ʱ������:��
#define  BIG_CONTROL_DATE_MONTH_SET       		3		//������ʱ������:��
#define  BIG_CONTROL_DATE_DAY_SET       		4		//������ʱ������:��
#define  BIG_CONTROL_TIME_HOUR_SET       		5		//������ʱ������:ʱ
#define  BIG_CONTROL_TIME_MINUTE_SET       		6		//������ʱ������:��
#define  BIG_CONTROL_TIME_SECOND_SET       		7		//������ʱ������:��
#define  BIG_TIME_SET_RETURN_BUTTON				9		//���ذ���
/*--------------------------------------------------------------------------*/

//����    control_id�Զ��ϵ����������
/*--------------------------------------------------------------------------*/
#define  BIG_AUTO_NO_POWER_PASSWOORD_INPUT		1		//�Զ��ϵ�ָ�����
/*--------------------------------------------------------------------------*/

//����    control_id�Լ�δͨ����������
/*--------------------------------------------------------------------------*/
#define  BIG_ERROR1_TEXT						2		//�ȵ�ż�������
#define  BIG_ERROR2_TEXT						3		//�������ڲ�����1
#define  BIG_ERROR3_TEXT						4		//�������ڲ�����2
#define  BIG_ERROR4_TEXT						5		//�綯ִ�л��������ж�
/*--------------------------------------------------------------------------*/

//����    control_id����������Ч����
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_UPDATE_FAIL					2		//�������ʧ��
#define  BIG_TESTTEMP_SET_FAIL					3		//ʵ���¶��趨��Ч
#define  BIG_TESTTIME_SET_FAIL					4		//ʵ��ʱ���趨��Ч
#define  BIG_TEMP1_UP_SET_FAIL					5		//�¶�1�����趨��Ч
#define  BIG_TEMP1_DOWN_SET_FAIL				6		//�¶�1�����趨��Ч
#define  BIG_CHANGE_AIR_SET_FAIL				7		//���������趨��Ч
#define  BIG_MODBUS_ADDRESS_SET_FAIL			8		//MOdbus��ַ�趨��Ч
#define	 BIG_FAIL_RETURN_BUTTON					9		//���ذ���

#define  BIG_TEMP2_UP_SET_FAIL					10		//�¶�2�����趨��Ч
#define  BIG_TEMP2_DOWN_SET_FAIL				11		//�¶�2�����趨��Ч
/*--------------------------------------------------------------------------*/

//����    control_id �¶�������ʾ����
/*--------------------------------------------------------------------------*/
#define  BIG_TEMP_CURVE_SHOW					1		//�¶�������ʾ
/*--------------------------------------------------------------------------*/



//����    control_id ���������ʾ����
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_ERROR_RETURN_BUTTON			1		//������󷵻�
/*--------------------------------------------------------------------------*/
/************************************************************************************/



//4.3��ʾ������궨��
typedef struct
{
	uint8_t SMALL_BRIGHT_ADJUST_SCREEN;							//��Ļ���ȵ��ڽ���
	uint8_t SMALL_SELF_TEST_NOTPASS_SCREEN;						//�Լ�δͨ����������(���Ͻ���)
	uint8_t SMALL_SCREAT_PROTECT_SCREEN;		 				//�����������뱣������
	uint8_t SMALL_AUTO_POWEROFF_POPUPWINDOWS_SCREEN;			//�Զ��ϵ絯������
	uint8_t SMALL_AUTO_POWEROFF_TIMESET_SCREEN;					//�Զ��ϵ�ʱ�����ý���
	uint8_t SMALL_START_INIT_SCREEN;  		 					//��������
	uint8_t SMALL_TEMP_VALUE_REVISE_SCREEN;						//�¶�ֵУ������
	uint8_t SMALL_SELF_TEST_SCREEN; 		 	 				//�Լ����
	uint8_t SMALL_CONTROL_TIME_SET;								//������ʱ�����ý���
	uint8_t SMALL_PASSWORD_ERROR_SCREEN;						//��������������
	uint8_t SMALL_ARGUEMENT_SET_ERROR_SCREEN;					//����������Ч����
	uint8_t SMALL_MAIN_SHOW_SCREEN;		 	 					//����ʾ����
	uint8_t SMALL_PARAM_SET_SCREEN;			 					//�������ý���
	uint8_t SMALL_PID_SET_SCREEN;			 					//pidֵ���ý���
}SMALL_SCREEN_ID_TAB;

/*************************************SMALL***********************************************/
/*--------------------------------------------------------------------------*/
#define  SMALL_START_INIT_ID					2		//��������������ID
/*--------------------------------------------------------------------------*/

//����ʾ����    control_id  ����
/*--------------------------------------------------------------------------*/
//�ı�id
#define  SMALL_CURRENT_TEMP_INT_ID				15		//ʵʱ�����¶ȿ���ID��������
#define  SMALL_CURRENT_TEMP_DEC_ID				7		//ʵʱ�����¶ȿ���IDС������

#define  SMALL_SET_TEMP_INT_ID					16		//�趨�¶ȿ���ID��������
#define  SMALL_SET_TEMP_DEC_ID					12		//�趨�¶ȿ���IDС������

#define  SMALL_START_TIME_ID					17		//���ȵ�Ŀ���¶ȵ���ʼʱ��
#define  SMALL_END_TIME_ID						18		//ʵ�����ʱ��
#define	 SMALL_TEST_TIME_ID						21		//�趨��ʵ��ʱ��h
#define  SMALL_ADDUP_TIME_ID					20		//�ۼ�ʵ��ʱ��h

#define	 SMALL_TIME_LEFT_HOUR_ID				19		//ʣ��ʱ�� Hou
#define	 SMALL_TIME_LEFT_MIN_ID					22		//ʣ��ʱ�� Min

#define  SMALL_SOFT_VER_ID				        2		//����汾��
//ͼ����ʾID
#define  SMALL_HEAT_SWITCH_ID					6		//������ͨ��ָʾ
#define	 SMALL_HEAT_OUTPUT_ID					4		//�������ָʾ
#define	 SMALL_RS485_COMMU_ID					9		//RS485ͨ��״̬
#define  SMALL_PID_RUN_ID						8		//PID����������ָʾ
#define  SMALL_TEMP_WARNING_ID					10		//�¶ȱ���
#define	 SMALL_BLOWER_SAMPLE_ID					11		//��������ܵ������ָʾ

#define	 SMALL_AR_ID							13		//AR�����̵���
#define  SMALL_FR_ID							14		//FR����̵���


//��ť����ID
#define  SMALL_MENU_ID							3		//�˵�����
#define	 SMALL_BLOWER_SAMPLE_MENU_ID			23		//���/����	
#define  SMALL_START_OR_PAUSE_ID				5		//����ֹͣ��ť
/*--------------------------------------------------------------------------*/

//�Լ����    control_id  ����
/*--------------------------------------------------------------------------*/
#define  SMALL_SELF_TEST_PROGRAM				2		//�Լ���ƽ�����
/*--------------------------------------------------------------------------*/

//���뱣������    control_id  ����
/*--------------------------------------------------------------------------*/
#define  SMALL_PASSWORD_PROTECT_INPUT			2		//���뱣�������
/*--------------------------------------------------------------------------*/

//�������ý���    control_id ����     ����
/*--------------------------------------------------------------------------*/
//�ı�id
#define  SMALL_TEST_TIME_VALUE					2		//����ʱ������
#define  SMALL_TEST_TEMP_VALUE					4		//�����¶�����
#define	 SMALL_WARNING_UP_VALUE					5		//��������
#define	 SMALL_WARNING_DOWN_VALUE				6		//��������
#define  SMALL_NEW_PASSWORD						8		//������
#define  SMALL_SECOND_INPUT_PASSWORD			9		//�ڶ�����������
#define  SMALL_MODBUS_NODE_ADDRESS				7		//modbus�ڵ��ַ
//ͼ����ʾid
#define  SMALL_BPS_1200							12		//ͨ������1200bps
#define  SMALL_BPS_2400							13		//ͨ������2400bps
#define  SMALL_BPS_4800							14		//ͨ������4800bps
#define  SMALL_BPS_9600							15		//ͨ������9600bps
#define  SMALL_CHINESE_LANGUAGE					10		//����
#define	 SMALL_ENGLISH_LANGUAGE					11		//Ӣ��

#define	 SMALL_SET_RETURN_BUTTON				3		//���ý��淵�ذ�ť
/*--------------------------------------------------------------------------*/


//��Ļ���ȵ������ý���    control_id �ı�����     ����
/*--------------------------------------------------------------------------*/
#define  SMALL_SCREEN_BRIGHT_ADJUST				2		//��Ļ���ȵ�������
/*--------------------------------------------------------------------------*/


//����    control_id PID ����
/*--------------------------------------------------------------------------*/
//ͼ�����id
#define  SMALL_SELF_ADJUST						4		//����������
#define  SMALL_PID_RETURN_BUTTON				5	    //PID���ذ���
#define  SMALL_P_VALUE_SET						7		//Pֵ�趨
#define  SMALL_I_VALUE_SET						2		//Iֵ�趨
#define  SMALL_D_VALUE_SET						3		//Dֵ�趨
/*--------------------------------------------------------------------------*/

//����    control_id �Զ��ϵ�ʱ�� ����
/*--------------------------------------------------------------------------*/
#define  SMALL_YEAR_SET							2		//����趨
#define  SMALL_MONTH_SET						3		//�·��趨
#define  SMALL_DAY_SET							4		//���趨
#define  SMALL_NO_POWER_RETURN_BUTTON			8		//���ذ���
/*--------------------------------------------------------------------------*/

//�¶�ֵ����  ���ý���    control_id 
/*--------------------------------------------------------------------------*/
#define  SMALL_TEMP_VALUE_REVUSE_SET			2		//�¶�ֵУ�����Ƶ���
/*--------------------------------------------------------------------------*/

//����    control_id ���ͺ�ʱ���¼����
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

//����    control_id ������ʱ������
/*--------------------------------------------------------------------------*/
#define  SMALL_CONTROL_DATE_YEAR_SET       		2		//������ʱ������:��
#define  SMALL_CONTROL_DATE_MONTH_SET       	3		//������ʱ������:��
#define  SMALL_CONTROL_DATE_DAY_SET       		4		//������ʱ������:��
#define  SMALL_CONTROL_TIME_HOUR_SET       		5		//������ʱ������:ʱ
#define  SMALL_CONTROL_TIME_MINUTE_SET       	6		//������ʱ������:��
#define  SMALL_CONTROL_TIME_SECOND_SET       	7		//������ʱ������:��
#define  SMALL_TIME_SET_RETURN_BUTTON			8		//���ذ���
/*--------------------------------------------------------------------------*/

//����    control_id�Զ��ϵ����������
/*--------------------------------------------------------------------------*/
#define  SMALL_AUTO_NO_POWER_PASSWOORD_INPUT	2		//�Զ��ϵ�ָ�����
/*--------------------------------------------------------------------------*/

//����    control_id�Լ�δͨ����������
/*--------------------------------------------------------------------------*/
#define  SMALL_ERROR1_TEXT						2		//�������ڲ�����1
#define  SMALL_ERROR2_TEXT						3		//�������ڲ�����2
#define  SMALL_ERROR3_TEXT						4		//�ȵ�ż�������
/*--------------------------------------------------------------------------*/

//����    control_id����������Ч����
/*--------------------------------------------------------------------------*/
#define  SMALL_PASS_UPDATE_FAIL					2		//�������ʧ��
#define  SMALL_TESTTEMP_SET_FAIL				4		//ʵ���¶��趨��Ч
#define  SMALL_TESTTIME_SET_FAIL				6		//ʵ��ʱ���趨��Ч
#define  SMALL_TEMP_UP_SET_FAIL					7		//�¶������趨��Ч
#define  SMALL_TEMP_DOWN_SET_FAIL				8		//�¶������趨��Ч
#define  SMALL_MODBUS_ADDRESS_SET_FAIL			9		//MOdbus��ַ�趨��Ч
#define	 SMALL_FAIL_RETURN_BUTTON				5		//���ذ���

/*--------------------------------------------------------------------------*/

//����    control_id ���������ʾ����
/*--------------------------------------------------------------------------*/
#define  SMALL_PASS_ERROR_RETURN_BUTTON			5		//������󷵻�
/*--------------------------------------------------------------------------*/

/************************************************************************************/


#define NOTIFY_TOUCH_PRESS     	 	0X01   	//����������֪ͨ
#define NOTIFY_TOUCH_RELEASE  		0X03  	//�������ɿ�֪ͨ
#define NOTIFY_WRITE_FLASH_OK  		0X0C  	//дFLASH�ɹ�
#define NOTIFY_WRITE_FLASH_FAILD  	0X0D  	//дFLASHʧ��
#define NOTIFY_READ_FLASH_OK  		0X0B  	//��FLASH�ɹ�
#define NOTIFY_READ_FLASH_FAILD  	0X0F  	//��FLASHʧ��
#define NOTIFY_MENU                 0X14  	//�˵��¼�֪ͨ
#define NOTIFY_TIMER                0X43  	//��ʱ����ʱ֪ͨ
#define NOTIFY_CONTROL              0XB1  	//�ؼ�����֪ͨ
#define NOTIFY_READ_RTC             0XF7  	//��ȡRTCʱ��

#define MSG_GET_CURRENT_SCREEN 		0X01	//����ID�仯֪ͨ
#define MSG_GET_DATA                0X11	//�ؼ�����֪ͨ
#define MSG_GET_ICON				0X26	//ͼ�궯���ؼ�����֪ͨ

#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])  //�ӻ�����ȡ32λ����

enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  				//��ť
	kCtrlText,  					//�ı�
	kCtrlProgress,  				//������
	kCtrlSlider,    				//������
	kCtrlMeter,  					//�Ǳ�
	kCtrlDropList, 					//�����б�
	kCtrlAnimation, 				//����
	kCtrlRTC, 						//ʱ����ʾ
	kCtrlGraph, 					//����ͼ�ؼ�
	kCtrlTable, 					//���ؼ�
	kCtrlMenu,						//�˵��ؼ�
	kCtrlSelector,					//ѡ��ؼ�
	kCtrlQRCode,					//��ά��
	kCtrlIcon,						//ͼ��
};

#pragma pack(push)
#pragma pack(1)						//���ֽڶ���

typedef struct
{
	uint8_t    cmd_head;  			//֡ͷ
	uint8_t    cmd_type;  			//��������(UPDATE_CONTROL)	
	uint8_t    ctrl_msg;   			//CtrlMsgType-ָʾ��Ϣ������
	uint16_t   screen_id;  			//������Ϣ�Ļ���ID
	uint16_t   control_id;  		//������Ϣ�Ŀؼ�ID
	uint8_t    control_type; 		//�ؼ�����
	uint8_t    param[256];			//�ɱ䳤�Ȳ��������256���ֽ�
	uint8_t    cmd_tail[4];   		//֡β
}CTRL_MSG,*PCTRL_MSG;

//RTC ʱ��
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
	uint8_t Year[BUFFSIZE];
	uint8_t Mon[BUFFSIZE];
	uint8_t Day[BUFFSIZE];
	uint8_t Hour[BUFFSIZE];
	uint8_t Min[BUFFSIZE];
	uint8_t Sec[BUFFSIZE];
}RtcSetTime;


//�Զ��ϵ�ʱ��
typedef struct 
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
}AutoNoPowerTime;

//�Զ��ϵ�ʱ������
typedef struct 
{
	uint8_t year[BUFFSIZE];
	uint8_t month[BUFFSIZE];
	uint8_t day[BUFFSIZE];
}AutoNoPowerTimeSet;


//��ҳ���ı�����ֵ
typedef struct 
{
	float 	current_temp_vlaue;
	float 	setting_temp;
	RtcTime start_time;
	RtcTime end_time;
	uint16_t test_time;
	uint16_t left_time_hou;
	uint16_t left_time_min;
	uint8_t add_all_time;
	uint8_t change_air_time;
	uint8_t air_door_angle;
}MainShowTextValue;


//��ҳ���ı���ʾ
typedef struct 
{
	uint8_t  softversion[BUFFSIZE];
	uint8_t current_temp_vlaue[BUFFSIZE];
	uint8_t setting_temp[BUFFSIZE];
	uint8_t  start_time[BUFFSIZE];
	uint8_t  end_time[BUFFSIZE];
	uint8_t test_time[BUFFSIZE];
	uint8_t left_time[BUFFSIZE];
	uint8_t add_all_time[BUFFSIZE];
	uint8_t change_air_time[BUFFSIZE];
	uint8_t air_door_angle[BUFFSIZE];
}MainShowText;


typedef struct{
	float warning1_up;				//����1����
	float warning1_down;				//����1����
	float warning2_up;				//����2����
	float warning2_down;				//����2����
	uint32_t menu_password;			//�˵�����
	uint32_t secondtime_password;	//������һ�β˵�����
	uint16_t change_air_time;			//��������
	uint16_t change_max_time;			//�����������ֵ
	uint8_t air_door_angle;				//���ŽǶ�
	uint8_t modbus_address;				//modbus�ڵ��ַ
	uint8_t modbus_tran_rate[4];					//modbusͨ������
	uint8_t menu_language[2];						//�˵�����
}CoilValue;											//���ּĴ����洢ֵ


typedef struct
{
	uint8_t change_air_times[19][BUFFSIZE];		//������ŽǶȲ��ֵ
	uint8_t airdoor_value[BUFFSIZE];			//������ŽǶ�����ֵ
	uint8_t screen_light_value[BUFFSIZE];		//������Ļ����ֵ
	uint8_t  protect_password[BUFFSIZE];		//����������������
	uint8_t Pidvalue[3][BUFFSIZE];				//PIDֵ���ñ���
	uint8_t temp_adjust_value[BUFFSIZE];		//�¶�ֵУ��
	uint8_t autonopowerpassword[BUFFSIZE];		//�Զ��ϵ��ָ�����
	CoilSaveValue  coilsavevalue;				//���ּĴ����洢ֵ
	AutoNoPowerTimeSet autotime;				//�Զ��ϵ�ʱ���趨
	MainShowText   textvaluebuff;				//��ҳ���ı��ؼ����滺������
	RtcSetTime  device_time_setting;			//ʱ������
}TextValueTab;

typedef struct
{
	uint16_t touch_x;
	uint16_t touch_y;
}Touch_Coord;

typedef struct
{
	uint8_t menu_click_times;
	uint8_t self_check_times;
	uint8_t rightdown_times;
	uint8_t rightup_times;
	uint8_t leftmiddle_times;
	uint8_t leftdown_times;
	uint8_t upmiddle_times;
}Touch_Times;





#pragma pack(pop)


void SetTextValueInt32(uint16_t screen_id, uint16_t control_id,int32_t value);
void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value);

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16_t screen_id);

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y);

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str);

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value);

/*! 
 *  \brief  �����ؼ�֪ͨ
 *  \details  �������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param starus    0x00��ʾ�������£�0x01��ʾ����
 *  \param iconimage_id ֵ
 */
void  NotifyAnimation(uint16_t screen_id, uint16_t control_id,uint8_t status,uint8_t iconimage_id);

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value);

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value);

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state);

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item);

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id);

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length);

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8_t status);

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);

void ProcessMessage( PCTRL_MSG msg, uint16_t size );


void startscreen(void);

void  touchtoscreen(void);

void get_combo_button_times(void);

void temp_curve_save(void);

void  mergetimechar(RtcTime datetime);

void mergehour_min(uint16_t hour,uint16_t min);

uint32_t to_day(RtcTime time);

char * monselect(char *monbuff);

void  adjustchar(char *timebuff);

void endtimecalcu(RtcTime starttime,uint16_t testtime);

uint32_t diff_time(RtcTime starttime,RtcTime endtime);

float myabs(float a,float b);

void lefttimecalculate(void);

void start_endtime_set(void);

void addup_testtime(void);

void change_air_times(void);




void screen_init(void);

void check_screen_size(void);

void screenlanguage(void);


uint8_t DectoBCD(uint8_t Dec);

uint8_t BcdToDec(uint8_t bcd);

uint8_t max_change_air(uint8_t *buff,uint8_t size);


#endif














