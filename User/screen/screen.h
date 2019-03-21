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


//5����ʾ������궨��?
typedef struct
{
	uint8_t BIG_KEYBOARD;								//С����
	uint8_t	BIG_AIR_CHANGE_RATE_SCREEN; 	 			//���������༭����
	uint8_t BIG_SCREAT_PROTECT_SCREEN;		 			//�����������뱣������
	uint8_t BIG_ADJUST_PROTECT_SCREEN;					//�������뱣������
	uint8_t BIG_PID_SET_SCREEN;			 				//pidֵ���ý���
	uint8_t BIG_PARAM_SET_SCREEN;			 			//�������ý���
	uint8_t BIG_AUTO_NOPOWER_RECOVER;					//�Զ��ϵ�ָ��������
	uint8_t BIG_TIME_RECORD_SCREEN; 					//���ͺ�ʱ���¼����?
	uint8_t BIG_CONTROL_TIME_SET;						//������ʱ�����ý���
	uint8_t BIG_PASSWORD_ERROR_SCREEN;					//��������������
	uint8_t BIG_ARGUEMENT_SET_ERROR_SCREEN1;			//����������Ч����1
	uint8_t BIG_ARGUEMENT_SET_ERROR_SCREEN2;			//����������Ч����2
	uint8_t BIG_MAIN_SHOW_SCREEN;		 	 			//����ʾ����
	uint8_t BIG_ADJUST_SCREEN;							//���ص�������
}BIG_SCREEN_ID_TAB;


/*************************************BIG***********************************************/

//��ʾ����    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_CURRENT_TEMP_ID				    21		//ʵʱ�����¶ȿ���ID
#define  BIG_CURRENT_TEMP_DECIMAL_ID			22		//ʵʱ�¶�С������
#define  BIG_SET_TEMP_ID						23		//�趨�¶ȿ���ID
#define  BIG_SET_TEMP_DECIMAL_ID				24		//�趨�¶�С������
#define  BIG_START_TIME_ID						25		//���ȵ�Ŀ���¶ȵ���ʼʱ��
#define  BIG_END_TIME_ID						26		//ʵ�����ʱ��?
#define	 BIG_TEST_TIME_ID						27		//�趨��ʵ��ʱ��
#define  BIG_ADDUP_TIME_ID						28		//�ۼ�ʵ��ʱ��
#define	 BIG_TIME_LEFT_HOUR_ID					29		//ʣ��ʱ��_Сʱ
#define  BIG_TIME_LEFT_MIN_ID					30		//ʣ��ʱ��_����
#define  BIG_CHANGE_AIR_TIME					31		//��������
#define  BIG_SOFT_VER_ID				        34		//����汾��?
#define	 BIG_AIR_DOOR_ANGLE_INPUT_ID			32		//���ſ���Ƕ�����?
//ͼ����ʾID
#define  BIG_HEAT_SWITCH_ID						2		//������ͨ��ָʾ
#define  BIG_PID_RUN_ID							4		//PID����������ָʾ
#define  BIG_FAN_OPERATION_ID					5		//�������ָ�?
#define	 BIG_AIR_DOOR_OPEN_ANGLE_ID				8		//���ſ����Ƕ�ָʾ
#define	 BIG_RS485_COMMU_ID						33		//RS485ͨ��״̬
#define  BIG_DOOR_OPEN_ID						10		//�Ŵ�ָʾ
#define	 BIG_HEAT_OUTPUT_ID						3		//�������ָ�?
#define  BIG_TEMP_WARNING1_ID					6		//�¶ȱ���1
#define  BIG_TEMP_WARNING2_ID					7		//�¶ȱ���2
#define	 BIG_SAMPLE_FRAME_MOTOR_ID				9		//���ܵ������ָ�?
#define  BIG_TROUBLE_INDICATE_ID				11		//����ָʾ
#define  BIG_AR1_WORK_STATUS_ID					12		//�����̵���1����״ָ̬ʾ
#define  BIG_AR2_WORK_STATUS_ID					13		//�����̵���2����״ָ̬ʾ
#define  BIG_FR_WORK_STATUS_ID					14		//����̵�������״ָ̬�?
#define  BIG_RR_WORK_STATUS_ID					15		//���̵ܼ�������״ָ̬ʾ
//��ť����ID
#define  BIG_MENU_ID							16		//�˵�����
#define  BIG_TIME_RECORD_ID						17		//�¼���¼
#define	 BIG_SAMPLE_FRAME_MENU_ID				18		//����
#define  BIG_BLOWER_MENU_ID						19		//���?
#define  BIG_START_OR_PAUSE_ID					20		//��ֹͣ��ť
/*--------------------------------------------------------------------------*/

//���뱣������    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD_PROTECT_INPUT				2		//���뱣�������?
/*--------------------------------------------------------------------------*/

//���뱣������2    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_PASSWORD2_INPUT				2		//���뱣�������?
/*--------------------------------------------------------------------------*/


//�������ý���    control_id ����     ����
/*--------------------------------------------------------------------------*/
#define  BIG_TEST_TIME_VALUE					3		//����ʱ������
#define  BIG_TEST_TEMP_VALUE					4		//�����¶�����
#define	 BIG_WARNING1_UP_VALUE					5		//����1����
#define	 BIG_WARNING2_UP_VALUE					6		//����2����
#define  BIG_TEMP_RETURN_DIFF					7		//�¶Ȼز�
#define  BIG_ANALOG_OUTPUT						8		//ģ���ź����?
#define  BIG_NEW_PASSWORD						9		//������
#define  BIG_SECOND_INPUT_PASSWORD				10		//�ڶ�����������
#define	 BIG_CHANGE_AIR_TIME_SET				11		//������������
#define	 BIG_CHANGE_AIR_MAX_SET					12		//����������������?
#define  BIG_MODBUS_NODE_ADDRESS				13		//modbus�ڵ���?
#define  BIG_SCREEN_BRIGHT_ADJUST				18		//��Ļ���ȵ���
//ͼ����ʾid
#define  BIG_BPS_1200							14		//ͨ������1200bps
#define  BIG_BPS_2400							15		//ͨ������2400bps
#define  BIG_BPS_4800							16		//ͨ������4800bps
#define  BIG_BPS_9600							17		//ͨ������9600bps
#define  BIG_CHINESE_LANGUAGE					19		//����
#define	 BIG_ENGLISH_LANGUAGE					20		//Ӣ��


#define	 BIG_SET_RETURN_BUTTON					2		//���ý��淵�ذ�ť
#define  BIG_ADJUST_BUTTON						21		//���԰�ť
/*--------------------------------------------------------------------------*/


//����7    control_id  ����
/*--------------------------------------------------------------------------*/
#define  BIG_AIR_RETURN_BUTTON					23		//�������淵�ذ�ť
#define  BIG_CHANGE_AIR_TIME_SET_100			22		//���������༭--100��
#define  BIG_CHANGE_AIR_TIME_SET_95				21		//���������༭--95��
#define  BIG_CHANGE_AIR_TIME_SET_90				20		//���������༭--90��
#define  BIG_CHANGE_AIR_TIME_SET_85				19		//���������༭--85��
#define  BIG_CHANGE_AIR_TIME_SET_80				18		//���������༭--80��
#define  BIG_CHANGE_AIR_TIME_SET_75				17		//���������༭--75��
#define  BIG_CHANGE_AIR_TIME_SET_70				16		//���������༭--70��
#define  BIG_CHANGE_AIR_TIME_SET_65				15		//���������༭--65��
#define  BIG_CHANGE_AIR_TIME_SET_60				14		//���������༭--60��
#define  BIG_CHANGE_AIR_TIME_SET_55				13		//���������༭--55��
#define  BIG_CHANGE_AIR_TIME_SET_50				12		//���������༭--50��
#define  BIG_CHANGE_AIR_TIME_SET_45				11		//���������༭--45��
#define  BIG_CHANGE_AIR_TIME_SET_40				10		//���������༭--40��
#define  BIG_CHANGE_AIR_TIME_SET_35				9		//���������༭--35��
#define  BIG_CHANGE_AIR_TIME_SET_30				8		//���������༭--30��
#define  BIG_CHANGE_AIR_TIME_SET_25				7		//���������༭--25��
#define  BIG_CHANGE_AIR_TIME_SET_20				6		//���������༭--20��
#define  BIG_CHANGE_AIR_TIME_SET_15				5		//���������༭--15��
#define  BIG_CHANGE_AIR_TIME_SET_10				4		//���������༭--10��
#define  BIG_CHANGE_AIR_TIME_SET_5				3		//���������༭--5	��
#define  BIG_CHANGE_AIR_TIME_SET_0				2		//���������༭--0	��
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

//����    ADJUST MENU  ����
/*--------------------------------------------------------------------------*/
#define  BIG_ADJUST_PASSWORD1					2		//�ϵ�ָ�����?
#define  BIG_ADJUST_PASSWORD2					3		//�ϵ�ָ�����?
#define  BIG_AIR_DOOR_ANGLE_SET					4		//���ŽǶȿ��Ƶ���
#define  BIG_TEMP_VALUE_REVUSE_SET				5		//�¶�ֵУ�����Ƶ���
#define  BIG_YEAR_SET							6		//�Զ��ϵ������?
#define  BIG_MONTH_SET							7		//�Զ��ϵ��·��趨
#define  BIG_DAY_SET							8		//�Զ��ϵ����趨


#define  BIG_NO_POWER_RETURN_BUTTON				9		//���ذ���
#define  BIG_EDIT_BUTTON						10		//���������༭���水��
#define  BIG_PID_BUTTON							11		//PID����
/*--------------------------------------------------------------------------*/


//����    control_id ���ͺ�ʱ���¼����?
/*--------------------------------------------------------------------------*/
#define  BIG_DATA_RECORD						2
#define  BIG_RECORD_RETURN						3
/*--------------------------------------------------------------------------*/

//����    control_id ������ʱ������
/*--------------------------------------------------------------------------*/
#define  BIG_CONTROL_DATE_YEAR_SET       		2		//������ʱ������:��
#define  BIG_CONTROL_DATE_MONTH_SET       		3		//������ʱ������:��
#define  BIG_CONTROL_DATE_DAY_SET       		4		//������ʱ������:��
#define  BIG_CONTROL_TIME_HOUR_SET       		5		//������ʱ������:ʱ
#define  BIG_CONTROL_TIME_MINUTE_SET       		6		//������ʱ������:��
#define  BIG_CONTROL_TIME_SECOND_SET       		7		//������ʱ������:��
#define  BIG_TIME_SET_RETURN_BUTTON				8		//���ذ���
/*--------------------------------------------------------------------------*/

//����    control_id�Զ��ϵ����������?
/*--------------------------------------------------------------------------*/
#define  BIG_AUTO_NO_POWER_PASSWOORD_INPUT		2		//�Զ��ϵ�ָ�����?
/*--------------------------------------------------------------------------*/



//����    control_id����������Ч����1
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_UPDATE_FAIL1					2		//�������ʧ��?
#define  BIG_TESTTEMP_SET_FAIL1					3		//ʵ���¶��趨��Ч
#define  BIG_TESTTIME_SET_FAIL1					4		//ʵ��ʱ���趨��Ч
#define  BIG_TEMP_UP_SET_FAIL1					5		//�¶������趨��Ч
#define  BIG_RETURN_DIFF_SET_FAIL1				6		//�¶Ȼز��趨��Ч
#define  BIG_CHANGE_AIR_SET_FAIL1				7		//���������趨��Ч
#define  BIG_MODBUS_ADDRESS_SET_FAIL1			8		//MOdbus��ַ�趨��Ч
#define  BIG_SCREEN_LIGHT_SET_FAIL1				9		//��Ļ���ȵ����趨��Ч

#define	 BIG_FAIL_RETURN_BUTTON1				10		//���ذ���
/*--------------------------------------------------------------------------*/

//����    control_id����������Ч����2
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_UPDATE_FAIL2					2		//�������ʧ��?
#define  BIG_AIRDOOR_ANGLE_SET_FAIL2			3		//���ſ����趨��Ч
#define  BIG_TEMP_ADJUST_SET_FAIL2				4		//�¶�У���趨��Ч
#define  BIG_AUTO_NO_POWER_SET_FAIL2			5		//�Զ��ϵ��趨��Ч


#define	 BIG_FAIL_RETURN_BUTTON2				6		//���ذ���
/*--------------------------------------------------------------------------*/



//����    control_id ���������ʾ����?
/*--------------------------------------------------------------------------*/
#define  BIG_PASS_ERROR_RETURN_BUTTON			2		//������󷵻�?
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
	kCtrlSelector,					//ѡ��ؼ�?
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
	uint8_t    param[256];			//�ɱ䳤�Ȳ��������?56���ֽ�
	uint8_t    cmd_tail[4];   		//֡β
}CTRL_MSG,*PCTRL_MSG;





//�Զ��ϵ�ʱ��
typedef struct 
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
}AutoNoPowerTime;


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

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı��?�����GetScreen)��ִ�д˺���
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
 *  \brief  �Ǳ�ؼ�ͨ�?
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
 *  \brief  ѡ��ؼ�ͨ�?
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���?
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



#endif

















