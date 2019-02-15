#ifndef __DATAPROCESS_H
#define	__DATAPROCESS_H


#include "stm32f4xx.h"


#define COILNUM  6
#define TIMERECORDNUM     20






typedef struct{
	uint8_t test_duration[COILNUM];				//实验时长，单位：小时
	uint8_t test_temp[COILNUM];					//实验温度，单位：摄氏度
	uint8_t warning1_up[COILNUM];				//报警1上限
	uint8_t warning2_up[COILNUM];				//报警2上限
	uint8_t temp_backdiff[COILNUM];				//温度回差
	uint8_t menu_password[COILNUM];				//菜单密码
	uint8_t secondtime_password[COILNUM];		//再输入一次菜单密码
	uint8_t adjustprotectpassword[COILNUM];		//保护界面密码
	uint8_t adjustprotect_secondpassword[COILNUM];//第二次输入保护界面密码
	uint8_t change_air_time[COILNUM];			//换气次数
	uint8_t change_max_time[COILNUM];			//换气次数最大值
	uint8_t modbus_address[COILNUM];			//modbus节点地址
	uint8_t modbus_tran_rate;					//modbus通信速率
	uint8_t menu_language;						//菜单语言
}CoilSaveValue;							//保持寄存器存储值





void  read_Coilregister(void);
void  write_Coilregister(void);


void read_Inputregister(void);

void read_Holdingregister(void);
void  write_Holdingregister(void);


void read_Discreteregister(void);


void modbus_register_init(void);

void modbus_register_handle(void);








#endif

