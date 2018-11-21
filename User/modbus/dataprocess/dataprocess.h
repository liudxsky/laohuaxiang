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
	uint16_t opendoor_duration;		//开门时长
	uint16_t regain_set_temp_time;	//时、分
}ThermalLag;						//热滞后时间记录结构体



typedef struct{
	uint8_t doorstatus_year;
	uint8_t doorstatus_month;
	uint8_t doorstatus_day;
	uint8_t doorstatus_hour;
	uint8_t doorstatus_minute;
	uint8_t doorstatus_second;
}DoorStatusChangeTime;				//状态改变时间点






typedef struct{
	uint16_t test_duration;				//实验时长，单位：小时
	uint16_t test_temp;					//实验温度，单位：摄氏度
	uint16_t warning1_up;				//报警1上限
	uint16_t warning1_down;				//报警1下限
	uint16_t warning2_up;				//报警2上限
	uint16_t warning2_down;				//报警2下限
	uint8_t analog_output[10];			//模拟信号输出
	uint32_t menu_password;				//菜单密码
	uint32_t secondtime_password;		//再输入一次菜单密码
	uint16_t change_air_time;			//换气次数
	uint16_t change_max_time;			//换气次数最大值
	uint16_t modbus_address;			//modbus节点地址
	uint16_t menu_language;				//菜单语言
}CoilSaveValue;							//保持寄存器存储值





void  read_coil(void);

void  write_coil(void);


void read_input_register(void);


void read_coilregister(void);

void  writer_register(void);










#endif

