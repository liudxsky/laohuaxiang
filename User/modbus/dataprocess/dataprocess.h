#ifndef __DATAPROCESS_H
#define	__DATAPROCESS_H


#include "stm32f4xx.h"


#define COILNUM  6
#define TIMERECORDNUM     20

typedef struct{

	uint8_t opendoor_year;
	uint8_t opendoor_month;
	uint8_t opendoor_day;
	uint8_t opendoor_hour;
	uint8_t opendoor_minute;
	uint8_t opendoor_second;
	uint16_t set_temp;
	uint16_t close_temp;
	uint16_t opendoor_duration;		//����ʱ��
	uint16_t regain_set_temp_time;	//ʱ����
}ThermalLag;						//���ͺ�ʱ���¼�ṹ��



typedef struct{
	uint8_t doorstatus_year;
	uint8_t doorstatus_month;
	uint8_t doorstatus_day;
	uint8_t doorstatus_hour;
	uint8_t doorstatus_minute;
	uint8_t doorstatus_second;
}DoorStatusChangeTime;				//״̬�ı�ʱ���






typedef struct{
	uint16_t test_duration;				//ʵ��ʱ������λ��Сʱ
	uint16_t test_temp;					//ʵ���¶ȣ���λ�����϶�
	uint16_t warning1_up;				//����1����
	uint16_t warning1_down;				//����1����
	uint16_t warning2_up;				//����2����
	uint16_t warning2_down;				//����2����
	uint8_t analog_output[10];			//ģ���ź����
	uint32_t menu_password;				//�˵�����
	uint32_t secondtime_password;		//������һ�β˵�����
	uint16_t change_air_time;			//��������
	uint16_t change_max_time;			//�����������ֵ
	uint16_t modbus_address;			//modbus�ڵ��ַ
	uint16_t menu_language;				//�˵�����
}CoilSaveValue;							//���ּĴ����洢ֵ





void  read_coil(void);

void  write_coil(void);


void read_input_register(void);


void read_coilregister(void);

void  writer_register(void);










#endif

