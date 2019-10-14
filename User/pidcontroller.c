#include "pidcontroller.h"
#include "./flash/deviceinfo.h"
#include "stdlib.h"
#define MEDBUFFLEN 30//tested on 50, but freeze
extern dev_info_t dev_info;
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
int  AutoTuneOutput=500;
int f_ReachedSP=0;
//64 params
const float32_t firCoeffs32[NUM_TAPS] = {0.001624,0.001716,0.001904,0.002194,0.002590,0.003095,0.003711,0.004436,0.005270,0.006207,0.007244,0.008372,0.009583,0.010868,0.012214,0.013608,0.015038,0.016489,0.017946,0.019392,0.020812,0.022189,0.023510,0.024757,0.025917,0.026976,0.027920,0.028740,0.029425,0.029966,0.030358,0.030595,0.030674,0.030595,0.030358,0.029966,0.029425,0.028740,0.027920,0.026976,0.025917,0.024757,0.023510,0.022189,0.020812,0.019392,0.017946,0.016489,0.015038,0.013608,0.012214,0.010868,0.009583,0.008372,0.007244,0.006207,0.005270,0.004436,0.003711,0.003095,0.002590,0.002194,0.001904,0.001716,0.001624};

//128 params

//const float32_t firCoeffs32[NUM_TAPS] = {
//	-0.000192,-0.000260,-0.000326,-0.000390,-0.000449,-0.000503,-0.000548,-0.000580,-0.000593,-0.000583,-0.000543,-0.000469,-0.000356,-0.000200,0.000000,0.000243,0.000525,0.000841,0.001178,0.001526,0.001866,0.002181,0.002450,0.002651,0.002761,0.002761,0.002630,0.002354,0.001923,0.001331,0.000582,-0.000314,-0.001339,-0.002466,-0.003658,-0.004873,-0.006059,-0.007162,-0.008120,-0.008870,-0.009351,-0.009502,-0.009268,-0.008601,-0.007461,-0.005821,-0.003667,-0.001001,0.002163,0.005790,0.009834,0.014229,0.018900,0.023756,0.028699,0.033621,0.038413,0.042965,0.047167,0.050918,0.054125,0.056708,0.058602,0.059758,0.060146,0.059758,0.058602,0.056708,0.054125,0.050918,0.047167,0.042965,0.038413,0.033621,0.028699,0.023756,0.018900,0.014229,0.009834,0.005790,0.002163,-0.001001,-0.003667,-0.005821,-0.007461,-0.008601,-0.009268,-0.009502,-0.009351,-0.008870,-0.008120,-0.007162,-0.006059,-0.004873,-0.003658,-0.002466,-0.001339,-0.000314,0.000582,0.001331,0.001923,0.002354,0.002630,0.002761,0.002761,0.002651,0.002450,0.002181,0.001866,0.001526,0.001178,0.000841,0.000525,0.000243,0.000000,-0.000200,-0.000356,-0.000469,-0.000543,-0.000583,-0.000593,-0.000580,-0.000548,-0.000503,-0.000449,-0.000390,-0.000326,-0.000260,-0.000192};

	void pidSPinit()
{
	pidSP[0].Kp_auto=35;
	pidSP[0].Ki_auto=0.005;
	pidSP[0].Kd_auto=44243;
	pidSP[0].SetPoint=100;


	pidSP[1].Kp_auto=27.8;
	pidSP[1].Ki_auto=0.0067;
	pidSP[1].Kd_auto=23884;
	pidSP[1].SetPoint=200;

	
	pidSP[2].Kp_auto=38.5;
	pidSP[2].Ki_auto=0.01;
	pidSP[2].Kd_auto=39697;
	pidSP[2].SetPoint=300;
}
 int PIDInit(float kp,float ki, float kd,float sp)
{

	PID.Kp=kp;
	PID.Ki=ki;
	PID.Kd=kd;

//	PID.Kp=35;
//	PID.Ki=0.0058;
//	PID.Kd=44243;
	
	arm_pid_init_f32(&PID, 1);
	errorSum=0;
	errorLast=0;
	memset(temperbuff,0,T_BUFFLEN);
	memset(&autoTuningParam,0,sizeof(autoTuningParam));
	autoTuningParam.SetPoint=sp;
	if(sp<120)
	{
		autoTuningParam.kpadj=0.85;
	}
	else if(sp>=120&&sp<220)
	{
		autoTuningParam.kpadj=0.85;
	}
	else
	{
		autoTuningParam.kpadj=0.85;
	}
	autoTuningParam.kiadj=0.5;
	autoTuningParam.kdadj=0.7;
	if(debuginfo)
	{
		printf("Kp:%5f, Ki:%5f, Kd:%5f\n",PID.Kp,PID.Ki,PID.Kd);
	}
	arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], 32);
	f_errorreset=1;
	return 1;
}

int compare(const void *a,const void *b)
{
	return *(float*)a>*(float*)b?1:-1;
}
float medbuff[MEDBUFFLEN];
float sortbuff[MEDBUFFLEN];
float medfilt()
{
	
	memcpy(sortbuff,medbuff,sizeof(medbuff[0])*MEDBUFFLEN);
	qsort(medbuff,MEDBUFFLEN,sizeof(medbuff[0]),compare);
	return medbuff[MEDBUFFLEN/2];
}

uint16_t pidCalc(float e)
{
	float errorNow=e;
	
	float  duty=0;
	float outKp,outKi,outKd;
	float derror=0,derror_ft;
	int error_threashold=autoTuningParam.SetPoint*0.5;
	if(autoTuningParam.SetPoint<100)
	{
		error_threashold=5;
		if(f_errorreset&&errorNow<1)
		{
			//errorSum=0;
			f_errorreset=0;
			
		}
	}
	else if(autoTuningParam.SetPoint<150)
	{
		error_threashold=10;
	}
	else if(autoTuningParam.SetPoint<220)
	{
		error_threashold=20;
	}
	else
	{
		error_threashold=25;
	}
	if(errorNow>error_threashold)
	{
		errorSum=0;
	}
	else
	{
		errorSum=errorSum+errorNow;
	}
	derror=errorNow-errorLast;
	
	if(tb_idx>MEDBUFFLEN-1)
		tb_idx=0;
	medbuff[tb_idx]=derror;
	derror_ft=medfilt();
	tb_idx++;
		
	outKp=PID.Kp*errorNow;
	outKi=PID.Ki*errorSum;
	outKd=PID.Kd*derror_ft;
	duty=outKp+outKi+outKd;
	errorLast=errorNow;
	//duty=arm_pid_f32(&PID, errorNow);//use arm lib
	//printf("%f,%f,%f\n",e,derror,derror_ft);
	if(debuginfo)
	{
		printf("errorNow: %f, errorSum: %f, derror:%f \n",errorNow,errorSum,derror);
		printf("outKp: %5f, outKi: %5f,outKd: %5f,Out: %f \n",outKp,outKi,outKd,duty);
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
	
	//printf("%f,%f,%f\n",in,outtemp1,outtemp2);
	return outtemp2;
}

float xhat=-2,xhat_last=0;
float P=0.00031,P_last=0;
float	K=0;
float R=0.1;
float Q=0.001;
float getSuprsTemper(float in)
{
	float out=in;
	float error=dev_info.setTemp-in;
	if(error<0&&f_ReachedSP==0)
	{
		f_ReachedSP=1;
	}
	if(f_ReachedSP==1)
	{
		if(error>-1.5&&error<1.5)
		{
			error=error*0.3;
			out=dev_info.setTemp-error;
		}
		else
		{
			out=in;
			f_ReachedSP=0;
		}
	}
	else
	{
		out=in;
	}
	return out;
}

float adj_display(float in_temp)
{
	//time update
	float out;
	float surptemp;
	float kalmantemp;
//	if(dev_info.useKalman==1)
//	{
//		kalmantemp=kalmanFilter(in_temp);
//	}
//	else
//	{
//		kalmanFilter(in_temp);
//		kalmantemp=in_temp;
//	}
	surptemp=getSuprsTemper(in_temp);
	out=surptemp;
	printf("%f,%f\n",in_temp,surptemp);//output 1s
	return out;
}

uint16_t autoTuning(float errornow,int * pwm_out,struct AutoTuningParamStruct* ats)
{
	
	uint16_t out=0;
	int i=1;	
	int pc_auto=0;
	float calctemp;
	int f_autoTuning=ats->f_autoTuning;
	int sp=ats->SetPoint;
	int switchPoint=10;
	if(!f_autoTuning)
		return 0;
	autoTuningParam.elapse_time++;
	if(sp<120)
	{
		switchPoint=13;
	}
	else if(sp<220)
	{
		switchPoint=10;
	}
	else if(sp<320)
	{
		switchPoint=7;
	}
	
	if(errornow<switchPoint)
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
			autoTuningParam.kpadj=0.85;
			autoTuningParam.kiadj=0.5;
			autoTuningParam.kdadj=0.7;
			autoTuningParam.Kp_auto=autoTuningParam.kc_auto*autoTuningParam.kpadj;
			autoTuningParam.Ki_auto=autoTuningParam.Kp_auto*Ts/autoTuningParam.Ti_auto*autoTuningParam.kiadj;
			autoTuningParam.Kd_auto=autoTuningParam.Kp_auto*autoTuningParam.Td_auto/Ts*autoTuningParam.kdadj;
			
			f_autoTuning=0;
			autoTuningParam.f_autoTuning=0;
			autoTuningParam.f_autoTuningDone=1;
			autoTuningParam.AutoTuneStatus=1;
			autoTuningParam.AutoTuneOutput=AutoTuneOutput;
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
			
	}//if(f_autoTuning&&errornow<switchPoint)

	 if(errornow>=switchPoint)
	{
		out=1000;
		
		if(AutoTuneStatus==1&&errornow>switchPoint+2)
		{
			if(tbuffidx<4)
			{
				AutoTuneStatus=-2;
				AutoTuneOutput=AutoTuneOutput+100;
				if(AutoTuneOutput>900)
					AutoTuneOutput=900;
			}
			else
			{
				AutoTuneStatus=-3;
				AutoTuneOutput=AutoTuneOutput-100;
				if(AutoTuneOutput<300)
					AutoTuneOutput=300;
			}
		}
	}
	
	//didn't reach 3rd hit point in 1 hour
	//this situation exist in theory, but never tested
	//1.check every hour, check tbuffidx==tbuffidx_last
	//2.if true, add output
	/*
	autoTuningParam.timestamp=autoTuningParam.elapse_time;
	if(autoTuningParam.timestamp>Hour1\
		&&AutoTuneStatus==1\
	&&(autoTuningParam.m_tbuffidx==tbuffidx))
	{
		autoTuningParam.timestamp=0;
		autoTuningParam.m_tbuffidx=tbuffidx;
		AutoTuneOutput=AutoTuneOutput+100;
		AutoTuneStatus=-5;
		
	}
	*/
	
	if(autoTuningParam.elapse_time>AUTOTUNE_TIMEOUT)
	{//time out, 5 hours
		AutoTuneStatus=-4;
		autoTuningParam.f_autoTuningDone=1;
		autoTuningParam.f_autoTuning=0;
		f_autoTuning=0;
		printf("autotune -4\n");
		autoTuningParam.elapse_time=0;
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
//float kalmanFilter(float in_temp)
//{
//	if(xhat<-1)
//	{
//		xhat=in_temp;
//		xhat_last=xhat;
//		return xhat;
//	}
//	P_last=P_last+Q;
//	//measurement update
//	K=P_last/(P_last+R);
//	xhat=xhat_last+K*(in_temp-xhat_last);
//	P=(1-K)*P_last;
//	//params update;
//	xhat_last=xhat;
//	P_last=P;
//	return xhat;
//}