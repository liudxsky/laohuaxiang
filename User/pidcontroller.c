#include "pidcontroller.h"

int AutoTuneStatus=0;
unsigned long cyclecnt=0;
int f_autoTuning=0;
int tbuffidx=0;
unsigned long sptime[2048]={0};
int cycles[2048]={0};
int maxbuff[2048]={0};
int minbuff[2048]={0};
int m_max=0,m_min=0;
unsigned long t_adjTemp=0;
int AutoTuneOutput=500;
struct AutoTuningParamStruct autoTuningParam;
float temperbuff[T_BUFFLEN];
float offsetbuff[T_BUFFLEN];
//float outputF32[T_BUFFLEN];
float errorSum;
float errorLast;
int tb_idx=0;	
arm_pid_instance_f32 PID;
uint8_t ab[16] = {0};
extern int debuginfo;
 int PIDInit(float kp,float ki, float kd,float sp)
{
	PID.Kp=kp;
	PID.Ki=ki;
	PID.Kd=kd;
	arm_pid_init_f32(&PID, 1);
	errorSum=0;
	errorLast=0;
	memset(temperbuff,0,T_BUFFLEN);
	memset(&autoTuningParam,0,sizeof(autoTuningParam));
	autoTuningParam.SetPoint=sp;
	autoTuningParam.kpadj=0.5;
	autoTuningParam.kiadj=0.0;
	autoTuningParam.kdadj=1;
	printf("Kp:%5f, Ki:%5f, Kd:%5f\n",PID.Kp,PID.Ki,PID.Kd);
	//arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], T_BUFFLEN);
	return 1;
}
double pidCalc(float e)
{
	float errorNow=e;
	float  duty=0;
	float outKp,outKi,outKd;
	float derror=0;
	if(errorNow>10)
	{
		errorSum=0;
	}
	else
	{
		errorSum=errorSum+errorNow;
	}
	derror=errorNow-errorLast;
	outKp=PID.Kp*errorNow;
	outKi=PID.Ki*errorSum;
	outKd=PID.Kd*derror;
	duty=outKp+outKi+outKd;
	errorLast=errorNow;
	//duty=arm_pid_f32(&PID, errorNow);//use arm lib
	if(debuginfo)
	{
//		printf("errorNow: %f, errorSum: %f, derror:%f\n",errorNow,errorSum,derror);
//		printf("outKp: %5f, outKi: %5f,outKd: %5f,Out: %f\n",outKp,outKi,outKd,duty);
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
/*
int ctlState=0;

float adj_display(float in_temp,float sp,struct AutoTuningParamStruct* PIDadj)
{
	float offset,avg_temp,offset_o;
	float rst=0;
	float kcadj,kiadj=0;
	int temp;
	
	if(in_temp<sp-10)
	{
		ctlState=1;
		
	}
	else if(in_temp<sp-5&&ctrlState==1)
	{
		ctrlState=2;
		
	}
	else if(in_temp<sp-1&&ctrlState==2)
	{
		ctrlState=3;
		
	}
	
	if(ctrlState==3)
	{
		offset=arm_mean_f32(offsetbuff,T_BUFFLEN,&avg_temp)-sp;
		frac=offset-(int)(offset);
		if(abs(frac)>0.5)
		{
			offset_o=(int)(offset)+signof(offset)*0.5
		}
		else
		{
			offset_o=(int)(offset);
		}
		PIDadj
		kcadj=kcadj-0.1;
		kiadj=kiadj-0.01;
		PIDadj->Kp=kcadj*Kc;
		PIDadj->Ki=PID->Kp*kiadj*Ts/Ti;
		
		rst=in_temp-offset_o;
	}
	return rst;
}
*/
int autoTuning(float errornow,int * pwm_out,struct AutoTuningParamStruct* ats)
{
	int i=1;
	int out=0;
	int pc_auto=0;
	
	int f_autoTuning=ats->f_autoTuning;
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
			if(tbuffidx>0&&((autoTuningParam.elapse_time)-sptime[tbuffidx-1]>(CycleLimit*60*2/50)))
			{
				printf("elapse:%d,sptime:%d, idx: %d\n",autoTuningParam.elapse_time,sptime[tbuffidx-1],tbuffidx);
				sptime[tbuffidx]=autoTuningParam.elapse_time;
				tbuffidx++;
			}
			else if(tbuffidx==0)
			{
				sptime[tbuffidx]=autoTuningParam.elapse_time;
				tbuffidx++;
			}
		}
		if(tbuffidx>0)
		{
			if(errornow<-0.2)//y-sp>0.2, sp-y<-0.2
			{
				
				if(fabs(errornow)>fabs(m_max))
				{
					
					m_max=fabs(errornow);
					
				}
			}
			if(errornow>0.2)
			{
				if(fabs(errornow)<fabs(m_min))
				{
					printf("min:%f\n",errornow);
					m_min=fabs(errornow);
				}
			}
				
		}
		if(tbuffidx>7)
		{//auto tuning finish
			
			int temp=(tbuffidx-1)/2;
			for(i=1;i<temp;i++)
			{
				cycles[i-1]=sptime[i+2]-sptime[i];//init cycles buffer
			}
			
			
			arm_mean_q31(cycles,i,&pc_auto);//q31,int compare
			autoTuningParam.pc_auto=pc_auto;
			
			autoTuningParam.a=(fabs(m_max)+fabs(m_min))/2;
			autoTuningParam.b=AutoTuneOutput/2;
			
			autoTuningParam.kc_auto=4/3.14*autoTuningParam.b/autoTuningParam.a;
			
			autoTuningParam.Ti_auto=0.5*pc_auto;
			autoTuningParam.Td_auto=0.3*pc_auto;
			
			autoTuningParam.Kp_auto=autoTuningParam.kc_auto*autoTuningParam.kpadj;
			autoTuningParam.Ki_auto=autoTuningParam.Kp_auto*Ts/autoTuningParam.Ti_auto*autoTuningParam.kiadj;
			autoTuningParam.Kd_auto=autoTuningParam.Kp_auto*autoTuningParam.Td_auto/Ts*autoTuningParam.kdadj;
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
