#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//#define FromUart
#define PIDKP 38//sp100->25
#define PIDKI 0.01
#define PIDKD 29809
#define T_BUFFLEN 64
#define BLOCK_SIZE T_BUFFLEN
#define PWMOUTLIMIT 1000
#define NUM_TAPS      65
#define CycleLimit 3 //each period should be larger than 5 mins
#define Ts 0.5 //sample time




struct AutoTuningParamStruct
{
	float a;
	float b;
	int pc_auto;
	float kc_auto;
	
	float kiadj;
	float kdadj;
	float kpadj;
	
	float Ti_auto;
	float Td_auto;
			
	float Kp_auto;
	float Ki_auto;
	float Kd_auto;
	
	int f_autoTuningDone;
	int f_autoTuning;
	int SetPoint;
	unsigned long elapse_time;
	int AutoTuneStatus;
};

int PIDInit(float kp,float ki, float kd,float sp);
double pidCalc(float e);
float getFilterTemper(float in);
int autoTuning(float errornow,int * pwm_out,struct AutoTuningParamStruct* ats);