#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"
#include "./screen/screen.h"



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
	float testtime;										//ʵ��ʱ��
	float testtemp;										//ʵ���¶�
	float temp_backdiff;								//�¶Ȼز�
	float flash_adjusttemp;								//�����¶�ֵ
	uint32_t  addup_testtime;							//�ۼ�����ʱ��
	uint8_t  Modbus_address;							//Modbus��ַ
	uint8_t  modbus_tran_rate_flag;						//Modbus��������
	Pid_Value pidvalue;									//pidֵ
	CoilValue flash_setvalue;							//��������
	AutoNoPowerTime autonopowertime;					//�Զ��ϵ�ʱ��
	uint8_t  warmstart_time[RTCBUFFSIZE];
	uint8_t  warmend_time[RTCBUFFSIZE];
	int compensatetemp;									//�����¶�
	
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
