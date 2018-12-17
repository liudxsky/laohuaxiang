#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define PIDKP 25
#define PIDKI 0.0053
#define PIDKD 26809
#define T_BUFFLEN 64
#define BLOCK_SIZE T_BUFFLEN
#define PWMOUTLIMIT 1000
#define NUM_TAPS      29
#define CycleLimit 5 //each period should be larger than 5 mins
#define Ts 0.5 //sample time
//const float32_t firCoeffs32[NUM_TAPS] = {
//	0.0012,0.0019,0.0034,0.0060,0.0101,0.0157
//	 0.0229,0.0313,0.0406,0.0501,0.0592,0.0673
//	0.0735,0.0775,0.0789,0.0775,0.0735,0.0673
//	 0.0592,0.0501,0.0406,0.0313,0.0229,0.0157
//	 0.0101,0.0060,0.0034,0.0019,0.0012};

//arm_fir_instance_f32 S;
//static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

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