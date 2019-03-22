#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"
#include "./screen/screen.h"
#include "./screen/screenStatus.h"


#define INVALID_INFO		0xFFFF
#define PASSWORDLEN			4


#define HMAC_LENGTH 		20
#define PLAINTEXT_LENGTH    64

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


#pragma pack(push)
#pragma pack(1)						//���ֽڶ���


typedef struct
{
	uint16_t valid_flag;    							//��Ч��־λ
	uint8_t  dev_status_changed_flag;					//����״̬��־λ
	uint16_t pwmscope;									//pwm�����Χ
	uint16_t pwmvalue;									//pwm�����¶�ֵ
	uint8_t  change_air_time[CHANGE_AIR_SIZE];			//���ŽǶȻ�������
	uint8_t  biglanguagestatus;							//����ѡ��
	float testTime;										//ʵ��ʱ��
	float setTemp;										//ʵ���¶�
	float currentTemp;
	float temp_backdiff;								//�¶Ȼز�
	float flash_adjusttemp;								//�����¶�ֵ
	uint8_t airdooropenangle;						//���ſ����Ƕ�
	uint32_t  addup_testtime;							//�ۼ�����ʱ��
	uint8_t  Modbus_address;							//Modbus��ַ
	uint16_t  modbus_tran_rate;						//Modbus��������
	Pid_Value pidvalue;									//pidֵ
	CoilValue flash_setvalue;							//��������
	RtcTime autonopowertime;							//�Զ��ϵ�ʱ��
	uint8_t  autonopowerpassword[PASSWORDLENGTH];			//�Զ��ϵ�����
	//
	//
	RtcTime start_time;
	RtcTime end_time;

	int compensatetemp;									//�����¶�
	//exame start time
	//exame stop time
	//f_fast_dev_status_chagned
	//struct IOStatusStruct IOStatus;
	int runstatus;
	uint8_t temp_warnning1;
	uint8_t temp_warnning2;
	uint8_t Rs485Status;
	uint8_t thermocouple_flag;//error flag
	RtcTime timenow;//system time now
	uint8_t lefttimeflag;
	uint8_t passwordwrongflag;
}dev_info_t;


#pragma pack(pop)


void DeviceInfo_Init(void);

void STM32_Read_ID(void);



int32_t STM32_SHA1_HMAC_Compute(uint8_t* InputMessage,
                          uint32_t InputMessageLength,
                          uint8_t *HMAC_key,
                          uint32_t HMAC_keyLength,
                          uint8_t *MessageDigest,
                          int32_t* MessageDigestLength);

TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);


void autogeneratepassword(void);






#endif
