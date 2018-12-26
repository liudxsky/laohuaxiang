#ifndef __DATAPROCESS_H
#define	__DATAPROCESS_H


#include "stm32f4xx.h"


#define COILNUM  6
#define TIMERECORDNUM     20









typedef struct{
	uint8_t test_duration[COILNUM];				//ʵ��ʱ������λ��Сʱ
	uint8_t test_temp[COILNUM];					//ʵ���¶ȣ���λ�����϶�
	uint8_t warning1_up[COILNUM];				//����1����
	uint8_t warning1_down[COILNUM];				//����1����
	uint8_t warning2_up[COILNUM];				//����2����
	uint8_t warning2_down[COILNUM];				//����2����
	uint8_t menu_password[COILNUM];				//�˵�����
	uint8_t secondtime_password[COILNUM];		//������һ�β˵�����
	uint8_t change_air_time[COILNUM];			//��������
	uint8_t change_max_time[COILNUM];			//�����������ֵ
	uint8_t modbus_address[COILNUM];			//modbus�ڵ��ַ
	uint8_t modbus_tran_rate[4];		//modbusͨ������
	uint8_t menu_language[2];			//�˵�����
}CoilSaveValue;							//���ּĴ����洢ֵ





void  read_coil(void);

void  write_coil(uint8_t coilvalue);


void read_input_register(void);


void read_coilregister(void);

void  writer_register(void);


void modbus_register_handle(void);








#endif

