#ifndef _STATUS_H
#define _STATUS_H
#include "stm32f4xx.h"
#include "./screen/screen.h"


#define OK 			0
#define ERROR 		1




typedef enum {
	HEAT_PIN,				//���ȿ���
	SAMPLE_PIN,				//���̵ܼ���
	BLOWER_PIN,				//����̵���	
	ALARM1_PIN,				//�����̵���1
	ALARM2_PIN,				//�����̵���2
	GPIO_TABLE_SIZE
} GpioTableIndex;	


typedef struct{

	RtcTime  heattime;				//����ʱ��
	RtcTime  closedoor_time;		//����ʱ��
	float set_temp;					//�趨�¶�
	float close_temp;				//�����¶�
	uint16_t opendoor_duration;		//����ʱ��
	uint32_t regain_set_temp_time;	//��,��
}ThermalLag;						//���ͺ�ʱ���¼�ṹ��


void check_pwmicon(void);

void changestruct(void);

void door_open_status(void);


uint16_t Get_GPIO_Status(void);


void check_screen_connect(void);

void check_nopowertime(void);

void Check_Rs485(void);

void check_powertime(void);

void check_warning(void);

void temp_detection(void);

void device_selfcheck(void);

void devicefunc_selfcheck(void);

void device_timing_selfcheck(void);

void control_mode_select(void);

void Check_All_Status(void);


#endif
