#ifndef _SCREEN_FUNC
#define _SCREEN_FUNC
#include "./screen/screen.h"
#include "./screen/screenStatus.h"
#define TRUE 1
#define FALSE 0
void changeAirTimes(uint16_t control_id,uint8_t *str);
void check_heat_switch(void);
void updatePIDScreen(uint16_t id);
void adjustScreenSetting(uint16_t control_id,uint8_t *str);
void deviceTimeSet(uint16_t control_id,uint8_t *str);
void menuSettingScreen(uint16_t control_id, uint8_t *str);
void changeAirTimes(uint16_t control_id,uint8_t *str);
void pidSettingScreen(uint16_t control_id,uint8_t *str);
void autoNoPowerScreen(uint16_t control_id,uint8_t *str);
void adjustProtectPassword(uint16_t control_id,uint8_t *str);
void menuPassword(uint16_t control_id, uint8_t *str);
void readRTCFromScreen(RtcTime t);

uint8_t judge_changeair_time(uint16_t change_time);
uint8_t max_change_air(uint8_t *buff);
uint8_t getChangeAirTimes(int changeTimes);
void start_endtime_set(void);
void lefttimecalculate(void);
void addup_testtime(void);
float myabs(float a,float b);
uint32_t diff_time(RtcTime starttime,RtcTime endtime);
void endtimecalcu(RtcTime starttime,float testtime);
void endtimecalcu(RtcTime starttime,float testtime);
uint32_t to_day(RtcTime time);
uint8_t DectoBCD(uint8_t Dec);
uint8_t BcdToDec(uint8_t bcd);
void  mergetimechar(RtcTime datetime);
void mergehour_min(uint16_t hour,uint16_t min);
void check_pidstatus(void);
#endif