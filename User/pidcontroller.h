#include "arm_math.h"
#include "stdio.h"
#include "string.h"

#define PIDKP 42
#define PIDKI 0.000073
#define PIDKD 39230
#define T_BUFFLEN 32
#define BLOCK_SIZE T_BUFFLEN
#define PWMOUTLIMIT 1000
#define NUM_TAPS      29
//const float32_t firCoeffs32[NUM_TAPS] = {
//	0.0012,0.0019,0.0034,0.0060,0.0101,0.0157
//	 0.0229,0.0313,0.0406,0.0501,0.0592,0.0673
//	0.0735,0.0775,0.0789,0.0775,0.0735,0.0673
//	 0.0592,0.0501,0.0406,0.0313,0.0229,0.0157
//	 0.0101,0.0060,0.0034,0.0019,0.0012};
float temperbuff[T_BUFFLEN];
//float outputF32[T_BUFFLEN];
double errorSum;
double errorLast;
int tb_idx=0;	
arm_pid_instance_f32 PID;
extern int debuginfo;
//arm_fir_instance_f32 S;
//static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

int AutoTuneOutput=400;
#define CycleLimit 5 //each period should be larger than 5 mins
#define Ts 0.5 //sample time
int AutoTuneStatus=0;
unsigned long cyclecnt=0;
int f_autoTuning=0;
int tbuffidx=0;
int sptime[2048]={0};
int cycles[2048]={0};
int maxbuff[2048]={0};
int minbuff[2048]={0};
int m_max=0,m_min=0;
struct AutoTuningParamStruct
{
	int a;
	int b;
	int pc_auto;
	float kc_auto;
			
	float Ti_auto;
	float Td_auto;
			
	float Kp_auto;
	float Ki_auto;
	float Kd_auto;
	
	int f_autoTuningDone;
	int f_autoTuning;
	int SetPoint;
	unsigned long long elapse_time;
	int AutoTuneStatus;
};
struct AutoTuningParamStruct autoTuningParam;
int PIDInit()
{
	PID.Kp=PIDKP;
	PID.Ki=PIDKI;
	PID.Kd=PIDKD;
	arm_pid_init_f32(&PID, 1);
	errorSum=0;
	errorLast=0;
	memset(temperbuff,0,T_BUFFLEN);
	memset(&autoTuningParam,0,sizeof(autoTuningParam));
	printf("Kp:%5f, Ki:%5f, Kd:%5f\n",PID.Kp,PID.Ki,PID.Kd);
	//arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], T_BUFFLEN);
	return 1;
}
double pidCalc(float e)
{
	//
	
	float errorNow=e;
	float  duty=0;
	float outKp,outKi,outKd;
	//out=PID.Kp*errorNow+PID.Ki*errorSum+PID.Kd*(errorNow-errorLast);
	duty=arm_pid_f32(&PID, errorNow);//use arm lib
	if(debuginfo)
	{
		printf("errorNow: %5f, errorSum: %5f, Out: %5f\n",errorNow,errorSum,duty);
		
		errorSum=errorSum+errorNow;
		outKp=PID.Kp*errorNow;
		outKi=PID.Ki*errorSum;
		outKd=PID.Kd*(errorNow-errorLast);
		printf("outKp: %5f, outKi: %5f,outKd: %5f  \n",outKp,outKi,outKd);
		errorLast=errorNow;
	}
	if(duty<0)
	{
		duty=0;
	}
	if(duty>PWMOUTLIMIT)
	{
		duty=PWMOUTLIMIT;
	}
	return duty;
}
float getFilterTemper(float in)
{
	int i;
	float outtemp1=0;
	float outtemp2=0;
	float tempsum=0;
	if(tb_idx>T_BUFFLEN-1)
		tb_idx=0;
	temperbuff[tb_idx]=in;
	tb_idx++;
//	for(i=0;i<T_BUFFLEN;i++)
//	{
//		tempsum=tempsum+temperbuff[i];
//		
//	}
	//outtemp1=tempsum/T_BUFFLEN;
	arm_mean_f32(temperbuff,T_BUFFLEN,&outtemp2);
	//printf("calc outtemp1: %f, armLib mean:%f",outtemp1,outtemp2);
	return outtemp2;
}

int autoTuning(float errornow,int * pwm_out,struct AutoTuningParamStruct* ats)
{
	int i=1;
	int out=0;
	int pc_auto=0;
	
	int f_autoTuning=autoTuningParam.f_autoTuning;
	if(!f_autoTuning)
		return 0;
	autoTuningParam.elapse_time++;
	if(f_autoTuning&&errornow<10)
	{
		if(errornow>0)
		{
			out=AutoTuneOutput;
			AutoTuneStatus=1;
		}
		else
		{
			out=0;
		}
		if(errornow<0.1&&errornow>-0.1)
		{
			//
			if(tbuffidx>1&&((cyclecnt+1)-sptime[tbuffidx-1])>(CycleLimit*60*2))
			{
				sptime[tbuffidx]=cyclecnt+1;
				tbuffidx=tbuffidx+1;
			}
			else if(tbuffidx==1)
			{
				sptime[tbuffidx]=cyclecnt+1;
				tbuffidx=tbuffidx+1;
			}
		}
		if(tbuffidx>0)
		{
			if(errornow<-0.2)//y-sp>0.2, sp-y<-0.2
			{
				if(abs(m_max)>(errornow))
				{
					m_max=errornow;
				}
			}
			if(errornow>0.2)
			{
				if(abs(m_min)<abs(errornow))
				{
					m_min=errornow;
				}
			}
				
		}
		if(tbuffidx>5)
		{//auto tuning finish
			
			int temp=(tbuffidx-1)/2;
			for(i=1;i<temp;i++)
			{
				cycles[i-1]=sptime[i+2]-sptime[i];//init cycles buffer
			}
			
			
			arm_mean_q31(cycles,i,&pc_auto);//q31,int compare
			autoTuningParam.pc_auto=pc_auto;
			
			autoTuningParam.a=abs(m_max)+abs(m_min)/2;
			autoTuningParam.b=AutoTuneOutput/2;
			
			autoTuningParam.kc_auto=4/3.14/autoTuningParam.b/autoTuningParam.a;
			
			autoTuningParam.Ti_auto=0.5*pc_auto;
			autoTuningParam.Td_auto=0.3*pc_auto;
			
			autoTuningParam.Kp_auto=0.4*autoTuningParam.kc_auto;
			autoTuningParam.Ki_auto=autoTuningParam.Kp_auto*0.01/autoTuningParam.Ti_auto;
			autoTuningParam.Kd_auto=autoTuningParam.Kp_auto*autoTuningParam.Td_auto/Ts;
			f_autoTuning=0;
			autoTuningParam.f_autoTuning=0;
			autoTuningParam.f_autoTuningDone=1;
			memcpy(ats,&autoTuningParam,sizeof(autoTuningParam));
		}
			
	}
	else if(f_autoTuning&&errornow>10)
	{
		out=1000;
		if(AutoTuneStatus==1)
		{
			if(tbuffidx<4)
			{
				autoTuningParam.AutoTuneStatus=-2;
				AutoTuneOutput=AutoTuneOutput+100;
			}
			else
			{
				autoTuningParam.AutoTuneStatus=-3;
				AutoTuneOutput=AutoTuneOutput-100;
			}
		}
	}
	if(autoTuningParam.elapse_time>(5*60/Ts))
	{//time out, 5 hours
		AutoTuneStatus=-4;
		autoTuningParam.f_autoTuningDone=1;
		autoTuningParam.f_autoTuning=0;
		f_autoTuning=0;
	}
	*pwm_out=out;
	return 1;
}