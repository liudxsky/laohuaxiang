#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"



#define INVALID_INFO		0xFFFF
#define PASSWORDLEN			4




typedef struct
{
	uint16_t valid_flag;    //有效标志位
	uint16_t pwmscope;		//pwm输出范围
	uint16_t pwmvalue;		//pwm控制温度值
	uint16_t airvalveangle; //风阀角度值
	uint8_t  change_air_time[19];
	uint8_t  biglanguagestatus;			//大屏语言选择
	uint8_t  smalllanguagestatus;		//小屏语言选择
}dev_info_t;




void DeviceInfo_Init(void);

void STM32_Read_ID(void);


void  encryp_password(void);


//uint16_t encry_password1(void);


//uint16_t encry_password2(void);



                                                
#endif
