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
#pragma pack(1)						//按字节对齐


typedef struct
{
	uint16_t valid_flag;    							//有效标志位
	uint8_t  dev_status_changed_flag;					//数据状态标志位
	uint16_t pwmscope;									//pwm输出范围
	uint16_t pwmvalue;									//pwm控制温度值
	uint8_t  change_air_time[CHANGE_AIR_SIZE];			//风门角度换气次数
	uint8_t  biglanguagestatus;							//语言选择
	float testTime;										//实验时间
	float setTemp;										//实验温度
	float currentTemp;
	float temp_backdiff;								//温度回差
	float flash_adjusttemp;								//调整温度值
	uint8_t airdooropenangle;						//风门开启角度
	uint32_t  addup_testtime;							//累计试验时间
	uint8_t  Modbus_address;							//Modbus地址
	uint16_t  modbus_tran_rate;						//Modbus传输速率
	Pid_Value pidvalue;									//pid值
	CoilValue flash_setvalue;							//参数设置
	RtcTime autonopowertime;							//自动断电时间
	uint8_t  autonopowerpassword[PASSWORDLENGTH];			//自动断电密码
	//
	//
	RtcTime start_time;
	RtcTime end_time;

	int compensatetemp;									//补偿温度
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
