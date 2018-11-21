#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"



#define INVALID_INFO		0xFFFF
#define PASSWORDLEN			4




typedef struct
{
	uint16_t valid_flag;    //��Ч��־λ
	uint16_t pwmscope;		//pwm�����Χ
	uint16_t pwmvalue;		//pwm�����¶�ֵ
	uint16_t airvalveangle; //�緧�Ƕ�ֵ
	uint8_t  change_air_time[19];
	uint8_t  biglanguagestatus;			//��������ѡ��
	uint8_t  smalllanguagestatus;		//С������ѡ��
}dev_info_t;




void DeviceInfo_Init(void);

void STM32_Read_ID(void);


void  encryp_password(void);


//uint16_t encry_password1(void);


//uint16_t encry_password2(void);



                                                
#endif
