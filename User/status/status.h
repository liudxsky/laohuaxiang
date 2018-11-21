#ifndef _STATUS_H
#define _STATUS_H
#include "stm32f4xx.h"
#include "./screen/screen.h"


#define OK 			0
#define ERROR 		1




typedef enum {
	HEAT_PIN,				//加热控制
	SAMPLE_PIN,				//样架继电器
	BLOWER_PIN,				//风机继电器	
	ALARM1_PIN,				//报警继电器1
	ALARM2_PIN,				//报警继电器2
	GPIO_TABLE_SIZE
} GpioTableIndex;	


typedef struct{

	RtcTime  heattime;				//开门时间
	RtcTime  closedoor_time;		//关门时间
	float set_temp;					//设定温度
	float close_temp;				//关门温度
	uint16_t opendoor_duration;		//开门时长
	uint32_t regain_set_temp_time;	//分,秒
}ThermalLag;						//热滞后时间记录结构体


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
