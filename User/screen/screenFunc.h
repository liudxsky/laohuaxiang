#ifndef _SCREEN_FUNC
#define _SCREEN_FUNC
#include "./screen/screen.h"
#include "./screen/screenStatus.h"
#define TRUE 1
#define FALSE 0


void pidScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);
void menuScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);
void mainShowScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);
void passwordInputWrongScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);
void adjustScreenButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);


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

uint8_t judge_changeair_time(uint16_t change_time);
uint8_t max_change_air(uint8_t *buff);
uint8_t getChangeAirTimes(int changeTimes);
uint16_t select_suitable_airtimes(uint16_t settimes);




void start_endtime_set(void);
void lefttimecalculate(void);
void addup_testtime(void);
float myabs(float a,float b);
int32_t diff_time(RtcTime starttime,RtcTime endtime);
void endtimecalcu(RtcTime starttime,float testtime);
void endtimecalcu(RtcTime starttime,float testtime);
uint32_t to_day(RtcTime time);
uint8_t DectoBCD(uint8_t Dec);
uint8_t BcdToDec(uint8_t bcd);
void  mergetimechar(RtcTime datetime);
void mergehour_min(uint16_t hour,uint16_t min);

void my_itoa(long i,char *string, uint8_t num);
#endif

