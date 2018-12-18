#ifndef __DEVICEID_H
#define __DEVICEID_H
#include "stm32f4xx.h"
#include "./screen/screen.h"



#define INVALID_INFO		0xFFFF
#define PASSWORDLEN			4


#pragma pack(push)
#pragma pack(1)						//���ֽڶ���


typedef struct
{
	uint16_t valid_flag;    			//��Ч��־λ
	uint16_t pwmscope;					//pwm�����Χ
	uint16_t pwmvalue;					//pwm�����¶�ֵ
	uint16_t airvalveangle; 			//�緧�Ƕ�ֵ
	uint8_t  change_air_time[19];		//���ŽǶȻ�������
	uint8_t  biglanguagestatus;			//����ѡ��
	float testtime;						//ʵ��ʱ��
	float testtemp;						//ʵ���¶�
	Pid_Value pidvalue;					//pidֵ
	CoilValue flash_setvalue;			//��������
	AutoNoPowerTime autonopowertime;	//�Զ��ϵ�ʱ��
}dev_info_t;


#pragma pack(pop)


void DeviceInfo_Init(void);

void STM32_Read_ID(void);


void  encryp_password(void);


//uint16_t encry_password1(void);


//uint16_t encry_password2(void);



                                                
#endif
