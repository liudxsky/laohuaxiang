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
#pragma pack(1)						//按字节对齐


typedef struct
{
	uint16_t valid_flag;    							//有效标志位
	uint8_t  dev_status_changed_flag;					//数据状态标志位
	uint16_t pwmscope;									//pwm输出范围
	uint16_t pwmvalue;									//pwm控制温度值
	uint8_t  change_air_time[CHANGE_AIR_SIZE];			//风门角度换气次数
	uint8_t  biglanguagestatus;							//语言选择
	float testtime;										//实验时间
	float testtemp;										//实验温度
	float temp_backdiff;								//温度回差
	float flash_adjusttemp;								//调整温度值
	uint32_t  addup_testtime;							//累计试验时间
	uint8_t  Modbus_address;							//Modbus地址
	uint8_t  modbus_tran_rate_flag;						//Modbus传输速率
	Pid_Value pidvalue;									//pid值
	CoilValue flash_setvalue;							//参数设置
	AutoNoPowerTime autonopowertime;					//自动断电时间
	uint8_t  warmstart_time[RTCBUFFSIZE];
	uint8_t  warmend_time[RTCBUFFSIZE];
	int compensatetemp;									//补偿温度
	
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
