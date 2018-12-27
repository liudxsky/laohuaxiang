#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"
#include "./screen/screen.h"



#define INVALID_INFO		0xFFFF
#define PASSWORDLEN			4


#define HMAC_LENGTH 		20
#define PLAINTEXT_LENGTH    64


#pragma pack(push)
#pragma pack(1)						//���ֽڶ���


typedef struct
{
	uint16_t valid_flag;    							//��Ч��־λ
	uint8_t  dev_status_changed_flag;			//����״̬��־λ
	uint16_t pwmscope;									//pwm�����Χ
	uint16_t pwmvalue;									//pwm�����¶�ֵ
	uint8_t  change_air_time[19];						//���ŽǶȻ�������
	uint8_t  biglanguagestatus;							//����ѡ��
	float testtime;										//ʵ��ʱ��
	float testtemp;										//ʵ���¶�
	uint32_t  addup_testtime;							//�ۼ�����ʱ��
	Pid_Value pidvalue;									//pidֵ
	CoilValue flash_setvalue;							//��������
	AutoNoPowerTime autonopowertime;					//�Զ��ϵ�ʱ��
	uint8_t autonopower_password[PASSWORDLENGTH];		//�Զ��ϵ�����
	uint8_t  warmstart_time[RTCBUFFSIZE];
	uint8_t  warmend_time[RTCBUFFSIZE];
}dev_info_t;

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;




#pragma pack(pop)


void DeviceInfo_Init(void);

void STM32_Read_ID(void);


void  encryp_password(void);


//uint16_t encry_password1(void);


//uint16_t encry_password2(void);


int32_t STM32_SHA1_HMAC_Compute(uint8_t* InputMessage,
                          uint32_t InputMessageLength,
                          uint8_t *HMAC_key,
                          uint32_t HMAC_keyLength,
                          uint8_t *MessageDigest,
                          int32_t* MessageDigestLength);

TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);


void testpassword(void);






#endif
