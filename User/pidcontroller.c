#include "pidcontroller.h"
#define AutoTuneOutput 500

int AutoTuneStatus=0;
unsigned long cyclecnt=0;
int tbuffidx=0;
unsigned long sptime[20]={0};
int cycles[20]={0};
float m_max=0,m_min=0;
unsigned long t_adjTemp=0;

struct AutoTuningParamStruct autoTuningParam;
float temperbuff[T_BUFFLEN];
float outputF32[T_BUFFLEN];

float offsetbuff[T_BUFFLEN];
int f_errorreset=1;
float errorSum;
float errorLast;
int tb_idx=0;	
arm_pid_instance_f32 PID;
extern int debuginfo;
arm_fir_instance_f32 S;
static	float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
struct AutoTuningParamStruct pidSP[3];

//const float32_t firCoeffs32[NUM_TAPS] = {
//0.001152,0.001879,0.003375,0.006014,0.010080,0.015708,0.022854,0.031277,0.040550,0.050093,0.059231,0.067262,0.073539,0.077533,0.078904,0.077533,0.073539,0.067262,0.059231,0.050093,0.040550,0.031277,0.022854,0.015708,0.010080,0.006014,0.003375,0.001879,0.001152};
const float32_t firCoeffs32[NUM_TAPS] = {
	-0.000197,-0.000358,-0.000552,-0.000796,
	-0.001100,-0.001467,-0.001888,-0.002340,
	-0.002788,-0.003182,-0.003463,-0.003558,
	-0.003393,-0.002889,-0.001973,-0.000580,
	0.001342,0.003824,0.006874,0.010476,
	0.014584,0.019126,0.024001,0.029087,
	0.034241,0.039309,0.044128,0.048538,
	0.052386,0.055535,0.057871,0.059308,
	0.059793,0.059308,0.057871,0.055535,
	0.052386,0.048538,0.044128,0.039309,
	0.034241,0.029087,0.024001,0.019126,
	0.014584,0.010476,0.006874,0.003824,
	0.001342,-0.000580,-0.001973,-0.002889,
	-0.003393,-0.003558,-0.003463,-0.003182,
	-0.002788,-0.002340,-0.001888,-0.001467,
	-0.001100,-0.000796,-0.000552,-0.000358,-0.000197
};
void pidSPinit()
{
	pidSP[0].Kp_auto=25;
	pidSP[0].Ki_auto=0.005;
	pidSP[0].Kd_auto=26809;
	pidSP[0].SetPoint=100;
	pidSP[0].kiadj=5;//integral time;
	
	pidSP[1].Kp_auto=38;
	pidSP[1].Ki_auto=0.01;
	pidSP[1].Kd_auto=29809;
	pidSP[1].SetPoint=200;
	pidSP[1].kiadj=10;//integral time;
	
	pidSP[2].Kp_auto=38.5;
	pidSP[2].Ki_auto=0.01;
	pidSP[2].Kd_auto=39697;
	pidSP[2].SetPoint=300;
	pidSP[2].kiadj=10;//integral time;
}
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
	if(debuginfo)
	{
		printf("Kp:%5f, Ki:%5f, Kd:%5f\n",PID.Kp,PID.Ki,PID.Kd);
	}
	arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], 32);
	f_errorreset=1;
	return 1;
}
uint16_t pidCalc(float e)
{
	float errorNow=e;
	float  duty=0;
	float outKp,outKi,outKd;
	float derror=0;
	int error_threashold=5;
	if(autoTuningParam.SetPoint<150)
	{
		error_threashold=5;
	}
	else
	{
		error_threashold=10;
	}
	if(errorNow>error_threashold)
	{
		errorSum=0;
	}
	else
	{
		errorSum=errorSum+errorNow;
	}
//	if(errorNow>0.5&&errorNow<1&&autoTuningParam.SetPoint<150)
//	{
//		f_errorreset=0;
//		errorSum=-(10/PID.Ki);
//	}
	derror=errorNow-errorLast;
	outKp=PID.Kp*errorNow;
	outKi=PID.Ki*errorSum;
	outKd=PID.Kd*derror;
	duty=outKp+outKi+outKd;
	errorLast=errorNow;
	//duty=arm_pid_f32(&PID, errorNow);//use arm lib
	if(debuginfo)
	{
		printf("errorNow: %f, errorSum: %f, derror:%f\n",errorNow,errorSum,derror);
		printf("outKp: %5f, outKi: %5f,outKd: %5f,Out: %f\n",outKp,outKi,outKd,duty);
	}
	if(duty<0)
	{
		duty=0;
	}
	if(duty>PWMOUTLIMIT)
	{
		duty=PWMOUTLIMIT;
	}
	
	return (uint16_t)duty;
}
float getFilterTemper(float in)
{
	int i;
	float outtemp1=0;
	float outtemp2=0;
//	float tempsum=0;
//	if(tb_idx>T_BUFFLEN-1)
//		tb_idx=0;
//	temperbuff[tb_idx]=in;
//	tb_idx++;
//	arm_mean_f32(temperbuff,T_BUFFLEN,&outtemp2);
	memcpy(temperbuff,temperbuff+1,sizeof(float)*(T_BUFFLEN-1));
	temperbuff[T_BUFFLEN-1]=in;
	if(temperbuff[0]!=0)
	{
		arm_fir_f32(&S, temperbuff, outputF32 , T_BUFFLEN);
	}
	else
	{
		return in;
	}
	outtemp2=outputF32[T_BUFFLEN-1];
if(outtemp2>2000)
	outtemp2=2000;
if(outtemp2<-2000)
	outtemp2=-2000;
//	printf("firout: %f,outtemp:%f\n",outputF32[T_BUFFLEN-1],outtemp2);
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
uint16_t autoTuning(float errornow,int * pwm_out,struct AutoTuningParamStruct* ats)
{
	uint16_t out=0;
	int i=1;	
	int pc_auto=0;
	float calctemp;
	int f_autoTuning=ats->f_autoTuning;
	if(!f_autoTuning)
		return 0;
	autoTuningParam.elapse_time++;
	
	if(f_autoTuning&&errornow<13)
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
//		if(debuginfo)
//		{
//			printf("autotune out :%d\n",out);
//		}
		if(errornow<0.1&&errornow>-0.2)
		{
			//
			if(tbuffidx>0&&((autoTuningParam.elapse_time)-sptime[tbuffidx-1]>(CycleLimit*60*2)))
			{
//			if(debuginfo)
//			{
//				printf("elapse:%d,sptime:%d, idx: %d\n",autoTuningParam.elapse_time,sptime[tbuffidx-1],tbuffidx);
//			}
				sptime[tbuffidx]=autoTuningParam.elapse_time;
				tbuffidx++;
			}
			else if(tbuffidx==0)
			{
				sptime[tbuffidx]=autoTuningParam.elapse_time;
				tbuffidx++;
			}
		}
		if(tbuffidx>1)
		{
			if(errornow<-0.2)//y-sp>0.2, sp-y<-0.2
			{
				
				if(fabs(errornow)>fabs(m_max))
				{
					
					m_max=fabs(errornow);
					
				}
			}
			if(errornow>0.1)
			{
				if(fabs(errornow)>fabs(m_min))
				{
//						if(debuginfo)
//					{
//						printf("min:%f\n",errornow);
//					}
					m_min=fabs(errornow);
				}
			}
				
		}
		if(tbuffidx>7)
		{//auto tuning finish
			
			int temp=tbuffidx-2;
			for(i=1;i<temp;i++)
			{
				cycles[i-1]=sptime[i+2]-sptime[i];//init cycles buffer
			}
			
			
			arm_mean_q31(cycles,i-1,&pc_auto);//q31,int compare
			calctemp=pc_auto;
			pc_auto=(int)calctemp*Ts;
			autoTuningParam.pc_auto=pc_auto;
			
			autoTuningParam.a=(fabs(m_max)+fabs(m_min))/2;
			autoTuningParam.b=AutoTuneOutput/2;
			
			autoTuningParam.kc_auto=4/3.14*autoTuningParam.b/autoTuningParam.a;
			
			autoTuningParam.Ti_auto=0.5*pc_auto;
			autoTuningParam.Td_auto=0.3*pc_auto;
			autoTuningParam.kiadj=0.5;
			autoTuningParam.kdadj=0.7;
			autoTuningParam.Kp_auto=autoTuningParam.kc_auto*autoTuningParam.kpadj;
			autoTuningParam.Ki_auto=autoTuningParam.Kp_auto*Ts/autoTuningParam.Ti_auto*autoTuningParam.kiadj;
			autoTuningParam.Kd_auto=autoTuningParam.Kp_auto*autoTuningParam.Td_auto/Ts*autoTuningParam.kdadj;
			
			f_autoTuning=0;
			autoTuningParam.f_autoTuning=0;
			autoTuningParam.f_autoTuningDone=1;
			autoTuningParam.AutoTuneStatus=1;
			memcpy(ats,&autoTuningParam,sizeof(autoTuningParam));
			
			printf("cycles:");
		for(i=0;i<(tbuffidx-1)/2;i++)
		{
			printf("%d,",cycles[i]);
		}
		printf("\n");
		
		printf("Kc:%f,Pc:%d\n,",autoTuningParam.kc_auto,autoTuningParam.pc_auto);
		
		goto FINISH;
		}
			
	}

	 if(f_autoTuning&&errornow>10)
	{
		out=1000;
		
		if(AutoTuneStatus==1)
		{
			if(tbuffidx<4)
			{
				autoTuningParam.AutoTuneStatus=-2;
				//AutoTuneOutput=AutoTuneOutput+100;
			}
			else
			{
				autoTuningParam.AutoTuneStatus=-3;
				//AutoTuneOutput=AutoTuneOutput-100;
			}
		}
		
	}
	if(autoTuningParam.elapse_time>AUTOTUNE_TIMEOUT)
	{//time out, 5 hours
		AutoTuneStatus=-4;
		autoTuningParam.f_autoTuningDone=1;
		autoTuningParam.f_autoTuning=0;
		f_autoTuning=0;
		printf("autotune -4\n");
		goto FINISH;
	}
	
	return out;
	
FINISH:
	printf("max:%f,min:%f\n",m_max,m_min);
		
		printf("sptime:");
		for(i=0;i<tbuffidx;i++)
		{
			printf("%d,",sptime[i]);
		}
		printf("\n");
		printf("e_time:%d\n",autoTuningParam.elapse_time);
	
	return 0;

}
