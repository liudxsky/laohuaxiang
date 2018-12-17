#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"
#include "./screen/screen.h"



#define INVALID_INFO		0xFFFF
#define PASSWORDLEN			4




typedef struct
{
	uint16_t valid_flag;    			//有效标志位
	uint16_t pwmscope;					//pwm输出范围
	uint16_t pwmvalue;					//pwm控制温度值
	uint16_t airvalveangle; 			//风阀角度值
	uint8_t  change_air_time[19];		//风门角度换气次数
	uint8_t  biglanguagestatus;			//语言选择
	Pid_Value pidvalue;					//pid值
	float testtime;						//实验时间
	float testtemp;						//实验温度
	CoilValue flash_setvalue;			//参数设置
	AutoNoPowerTime autonopowertime;	//自动断电时间
}dev_info_t;




void DeviceInfo_Init(void);

void STM32_Read_ID(void);


void  encryp_password(void);


//uint16_t encry_password1(void);


//uint16_t encry_password2(void);



                                                
#endif
