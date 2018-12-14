#ifndef __DATAPROCESS_H
#define	__DATAPROCESS_H


#include "stm32f4xx.h"


#define COILNUM  6
#define TIMERECORDNUM     20

#define MODBUSBUFFSIZE    20








typedef struct{
	uint8_t test_duration[MODBUSBUFFSIZE];				//实验时长，单位：小时
	uint8_t test_temp[MODBUSBUFFSIZE];					//实验温度，单位：摄氏度
	uint8_t warning1_up[MODBUSBUFFSIZE];				//报警1上限
	uint8_t warning1_down[MODBUSBUFFSIZE];				//报警1下限
	uint8_t warning2_up[MODBUSBUFFSIZE];				//报警2上限
	uint8_t warning2_down[MODBUSBUFFSIZE];				//报警2下限
	uint8_t menu_password[MODBUSBUFFSIZE];			//菜单密码
	uint8_t secondtime_password[MODBUSBUFFSIZE];	//再输入一次菜单密码
	uint8_t change_air_time[MODBUSBUFFSIZE];			//换气次数
	uint8_t change_max_time[MODBUSBUFFSIZE];			//换气次数最大值
	uint8_t modbus_address[MODBUSBUFFSIZE];			//modbus节点地址
	uint8_t modbus_tran_rate[4];		//modbus通信速率
	uint8_t menu_language[2];			//菜单语言
}CoilSaveValue;							//保持寄存器存储值





void  read_coil(void);

void  write_coil(uint8_t coilvalue);


void read_input_register(void);


void read_coilregister(void);

void  writer_register(void);


void modbus_register_handle(void);








#endif

