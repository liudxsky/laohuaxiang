/*******************
device status is hold by dev_info
in this file,dev_info push device main status to other device
1. IO relay, IOStatus
2. screen status (some text and icon is judged by dev_status)
3. modbus coil status

***********************/

#include "./status/status.h"
#include "./gpio/gpio.h"
#include "hmi_driver.h"
#include "./dataprocess/dataprocess.h"
#include "./screen/screen.h"
#include "./delaytime/delaytime.h"
#include "./pwm/pwm.h"
#include "./flash/deviceinfo.h"
#include "./dac/dac.h"

extern BIG_SCREEN_ID_TAB bigchinese_screen ;
extern BIG_SCREEN_ID_TAB bigenglish_screen; 
extern BIG_SCREEN_ID_TAB biglanguage_screen;


extern uint16_t current_screen_id;
extern TextValueTab  textvalue;					//�ı��ؼ�����ֵ

extern uint8_t cmd_buffer[CMD_MAX_SIZE];
extern dev_info_t dev_info;
extern uint8_t  soft_ver[10];

extern struct mainIconStruct mainIcon;
extern struct mainTextStruct mainPageText;
extern struct argSetErrorStruct argSetErrorIcon;
extern struct IOStatusStruct IOStatus;


uint8_t  pwmgpiostatus = 0;
uint16_t gpiostatus = 0;
uint32_t door_openstatus = 0,door_closestatus = 0;
uint16_t Modbus_rate = 9600;



ThermalLag heattime_log;						//���ͺ�ʱ���¼�ṹ��
uint8_t  control_mode[2][8] = {"4-20mA","2-10V"};
uint8_t  Rs485TX = 0,Rs485RX = 0;				//0��ʾͨ��������1��ʾͨ���쳣��2��ʾ485ͨ��δ����






//���̵�����������״̬�仯
uint16_t Get_GPIO_Status(void)
{
	
	
	uint8_t i;
	uint16_t status = 0;
	uint8_t readgpio[GPIO_TABLE_SIZE] = {0},gpioback[GPIO_TABLE_SIZE] = {0};
	
	readgpio[0] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	gpioback[0] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN1);
	
	readgpio[1] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	gpioback[1] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN2);
	
	readgpio[2] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	gpioback[2] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN3);
	
	readgpio[3] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	gpioback[3] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN4);
	
	readgpio[4] = GPIO_ReadOutputDataBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	gpioback[4] = GPIO_ReadInputDataBit(BACK_GPIO_PORT,BACK_GPIO_PIN5);
	
	for(i = 0;i < GPIO_TABLE_SIZE;i++)
	{
		status |= ((readgpio[i]^(!gpioback[i]))<<i);
	}	
	return status;
}



//�ϵ��Լ���ʾ���������ͨ���������˸LED��
extern uint8_t screen_flag;
//�����Ļ����
void check_screen_connect(void)
{
	if(screen_flag)
	{
		if(current_screen_id == biglanguage_screen.BIG_MAIN_SHOW_SCREEN)
		{
			
			MySetScreen(biglanguage_screen.BIG_MAIN_SHOW_SCREEN);
		}
	}
	else
	{
		led_blink(3);
		screen_flag = 0;
	}
}


//����Ƿ񵽶�ʱ�ϵ�ʱ��
void check_nopowertime(void)
{
	//????
	//after this day, it will work
	if((dev_info.timenow.Year == dev_info.autonopowertime.year)&&(dev_info.timenow.Mon == dev_info.autonopowertime.month)&&(dev_info.timenow.Day == dev_info.autonopowertime.day))
	{
		MySetScreen(biglanguage_screen.BIG_AUTO_NOPOWER_RECOVER);
		RCC_AHB1PeriphClockCmd(DRIVER_GPIO_CLK|BACK_GPIO_CLK|BOX_DOOR_GPIO_CLK, DISABLE);
	}
}

//������ͨ��
void check_pwmicon(void)
{
	if(dev_info.pwmvalue != 0)
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,SHOW);
	}
	else
	{
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_HEAT_OUTPUT_ID,HIDE);
	}
}


//���485ͨ��״̬
void Check_Rs485(void)
{
	if(Rs485TX&Rs485RX)
	{
		dev_info.Rs485Status = 0;
	}
	else if(Rs485TX|Rs485RX)
	{
		dev_info.Rs485Status = 1;
	}
	else
	{
		dev_info.Rs485Status = 2;
	}
	switch (dev_info.Rs485Status)
	{
		case 0:
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,0);
			break;
		case 1:
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,1);
			delay_ms(300);
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,0);
			delay_ms(300);
			break;
		case 2:
			AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_RS485_COMMU_ID,2);
			break;
		default:
			break;
	}
}


//��������Ƿ��е�ͨ���ж��ϴζϵ�ʱ���Ƿ�С��2001���ж�
void check_powertime(void)
{
	if(dev_info.timenow.Year == 0)
	{
		MySetScreen(biglanguage_screen.BIG_CONTROL_TIME_SET);
	}		
}

//����ѡ��
void check_language_select(void)
{
	if(dev_info.biglanguagestatus == 1)
	{
		biglanguage_screen = bigchinese_screen; 		//Chinese
		current_screen_id = 11;
	}
	else
	{
		biglanguage_screen = bigenglish_screen; 		//English
		current_screen_id = 22;
	}
}

//modbus��������
void check_modbuss_rate(void)
{
	switch(dev_info.modbus_tran_rate_flag)
	{
		case 0:
			Modbus_rate = 1200;
			break;
		case 1:
			Modbus_rate = 2400;
			break;
		case 2:
			Modbus_rate = 4800;
			break;
		case 3:
			Modbus_rate = 9600;
			break;
		default:
			Modbus_rate = 9600;
	}
}


//���PWM���
void check_pwm(void)
{
	pwmgpiostatus = GPIO_ReadInputDataBit(PWM_GPIO_PORT,BACK_PWM_PIN);
}

//��ⱨ��
void check_warning(void)
{
	if(showtextvalue.current_temp_vlaue - dev_info.testtemp > dev_info.flash_setvalue.warning1_up)
	{
		dev_info.temp_warnning1=1;
	}
	else if(showtextvalue.current_temp_vlaue < (dev_info.testtemp + dev_info.flash_setvalue.warning1_up - dev_info.temp_backdiff))
	{
		dev_info.temp_warnning1=0;
		
	}
	if( showtextvalue.current_temp_vlaue - dev_info.testtemp >= dev_info.flash_setvalue.warning2_up)
	{
			dev_info.temp_warnning2=1;
	}
	else if(showtextvalue.current_temp_vlaue < (dev_info.testtemp + dev_info.flash_setvalue.warning2_up - dev_info.temp_backdiff))
	{
		dev_info.temp_warnning2=0;

		
	}
}

 
//�¶Ȳ���
void temp_detection(float dispTemper)
{
	dev_info.currentTemp = dev_info.flash_adjusttemp + dispTemper+0.4;
	if(dev_info.thermocouple_flag)
	{
		SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CURRENT_TEMP_ID, 999);
		SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CURRENT_TEMP_DECIMAL_ID, 9);
	}
	else
	{	
		SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CURRENT_TEMP_ID,(uint16_t)showtextvalue.current_temp_vlaue/1);
		SetTextValueInt32(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_CURRENT_TEMP_DECIMAL_ID,(uint16_t)(showtextvalue.current_temp_vlaue*10)%10);
	}
}

void Check_All_Status(void)
{
//	check_pwm();//pwm hardware self check
	check_pwmicon();//heat output icon
	check_pidstatus();//auto turn icon
	Check_Rs485();
	check_powertime();
	check_screen_connect();
	check_nopowertime();
	check_warning();
	door_open_status();
}


uint8_t thermalbuff[38] = {0};

//��ʽת��
void changestruct(void)
{
	uint8_t j;
	char a = '/',b = ':',c = ';',d = ' ';
	j  = sprintf(thermalbuff,"%02d",heattime_log.heattime.Year);
	j += sprintf(thermalbuff+j,"%c",a);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Mon);
	j += sprintf(thermalbuff+j,"%c",a);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Day);
	
	j += sprintf(thermalbuff+j,"%c",d);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Hour);
	j += sprintf(thermalbuff+j,"%c",b);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Min);
	j += sprintf(thermalbuff+j,"%c",b);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.heattime.Sec);
	j += sprintf(thermalbuff+j,"%c",c);
	
	j += sprintf(thermalbuff+j,"%.1f",heattime_log.set_temp);
	j += sprintf(thermalbuff+j,"%c",c);

	
	j += sprintf(thermalbuff+j,"%.1f",heattime_log.close_temp);
	j += sprintf(thermalbuff+j,"%c",c);

	
	j += sprintf(thermalbuff+j,"%ld",heattime_log.opendoor_duration);
	j += sprintf(thermalbuff+j,"%c",c);
	
	j += sprintf(thermalbuff+j,"%ld",heattime_log.regain_set_temp_time/60);
	j += sprintf(thermalbuff+j,"%c",b);
	j += sprintf(thermalbuff+j,"%02d",heattime_log.regain_set_temp_time%60);
	j += sprintf(thermalbuff+j,"%c",c);
//	printf(" temp is : %s\n",thermalbuff);
	
}

uint8_t savethermalbuff[TIMERECORDNUM][38] = {0},savenum = 0;
//����״̬���,���ͺ����ݼ�¼
void door_open_status(void)
{
	uint8_t doorflag = 0;
	doorflag = GPIO_ReadInputDataBit(BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);
	if(doorflag)			//����״̬
	{
		door_openstatus++;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,SHOW);
		if(door_openstatus == 1)
		{
			door_closestatus = 0;
			heattime_log.heattime = dev_info.timenow;
	//		printf("%d/%d/%d  %02d:%02d:%02d\r\n",heattime_log.heattime.Year,heattime_log.heattime.Mon,heattime_log.heattime.Day,heattime_log.heattime.Hour,heattime_log.heattime.Min,heattime_log.heattime.Sec);
			heattime_log.set_temp = dev_info.setTemp;
	//		printf("\r\nset temp is %f\r\n",heattime_log.set_temp);
		}
	}
	else
	{
		door_closestatus++;
		AnimationPlayFrame(biglanguage_screen.BIG_MAIN_SHOW_SCREEN,BIG_DOOR_OPEN_ID,HIDE);
		if(door_closestatus  == 1)
		{
			door_openstatus = 0;
			heattime_log.closedoor_time = dev_info.timenow;
			heattime_log.close_temp = dev_info.currentTemp;
			heattime_log.opendoor_duration = diff_time(heattime_log.heattime,heattime_log.closedoor_time)/60;
//			if(myabs(showtextvalue.current_temp_vlaue,showtextvalue.setting_temp) < 2)
//			{
				heattime_log.regain_set_temp_time = diff_time(heattime_log.closedoor_time,rtctime);
				changestruct();
				AddDataRecord(biglanguage_screen.BIG_TIME_RECORD_SCREEN,BIG_DATA_RECORD,thermalbuff);
				if(savenum < TIMERECORDNUM)
				{
					memcpy(savethermalbuff[savenum],thermalbuff,sizeof(char)*38);
					savenum++;
				}
				else
				{
					savenum = 0;
				}
//			}
		}
	}
}


//���ſ��Ʒ�ʽѡ��
void control_mode_select(void)
{	
	uint8_t controlflag = GPIO_ReadInputDataBit(DAC_SELECT_GPIO_PORT,DAC_SELECT2_GPIO_PIN);	;
	if(controlflag)
	{
		SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_ANALOG_OUTPUT,control_mode[0]);
	}
	else
	{
		SetTextValue(biglanguage_screen.BIG_PARAM_SET_SCREEN,BIG_ANALOG_OUTPUT,control_mode[1]);
	}
	
}




