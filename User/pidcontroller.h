#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//#define FromUart
#define PIDKP 25//sp100->25
#define PIDKI 0.005
#define PIDKD 26809
#define T_BUFFLEN 64
#define BLOCK_SIZE T_BUFFLEN
#define PWMOUTLIMIT 1000
#define NUM_TAPS      65
#define CycleLimit 3 //each period should be larger than 5 mins
#define Ts 0.5 //sample time
#define AUTOTUNE_TIMEOUT (5*60*60*2)

	
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
void pidSPinit();
int PIDInit(float kp,float ki, float kd,float sp);
uint16_t pidCalc(float e);
float getFilterTemper(float in);
uint16_t autoTuning(float errornow,int * pwm_out,struct AutoTuningParamStruct* ats);