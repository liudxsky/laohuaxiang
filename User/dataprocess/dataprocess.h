#ifndef __DATAPROCESS_H
#define	__DATAPROCESS_H


#include "stm32f4xx.h"


#define COILNUM  6
#define TIMERECORDNUM     20

#define MODBUSBUFFSIZE    20








typedef struct{
	uint8_t test_duration[MODBUSBUFFSIZE];				//ʵ��ʱ������λ��Сʱ
	uint8_t test_temp[MODBUSBUFFSIZE];					//ʵ���¶ȣ���λ�����϶�
	uint8_t warning1_up[MODBUSBUFFSIZE];				//����1����
	uint8_t warning1_down[MODBUSBUFFSIZE];				//����1����
	uint8_t warning2_up[MODBUSBUFFSIZE];				//����2����
	uint8_t warning2_down[MODBUSBUFFSIZE];				//����2����
	uint8_t menu_password[MODBUSBUFFSIZE];			//�˵�����
	uint8_t secondtime_password[MODBUSBUFFSIZE];	//������һ�β˵�����
	uint8_t change_air_time[MODBUSBUFFSIZE];			//��������
	uint8_t change_max_time[MODBUSBUFFSIZE];			//�����������ֵ
	uint8_t modbus_address[MODBUSBUFFSIZE];			//modbus�ڵ��ַ
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

