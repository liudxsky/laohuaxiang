#include "dataprocess.h"
#include "stdlib.h"
#include "./gpio/gpio.h"
#include "main.h"
#include "./status/status.h"
#include "hmi_driver.h"
#include "./screen/screen.h"









extern uint16_t     usRegInputBuf[REG_INPUT_NREGS];				//����Ĵ���      �����룺04
extern uint16_t		usRegHoldingBuf[REG_HOLDING_NREGS];			//���ּĴ���	�����룺03,06,16
extern uint16_t		ucRegCoilsBuf[REG_COILS_SIZE];					//��Ȧ�Ĵ���	�����룺01,05,15
extern uint16_t		ucRegDiscreteBuf[REG_DISCRETE_SIZE];		//��������Ĵ���	�����룺02


extern MainShowTextValue showtextvalue;									//��ҳ���ı��ؼ�����ֵ
extern uint8_t cmd_buffer[CMD_MAX_SIZE];								//ָ���
extern BIG_SCREEN_ID_TAB biglanguage_screen;;							//��������ѡ��
CoilSaveValue  coilsavevalue;											//���ּĴ����洢ֵ



extern ThermalLag heattime_log;										//���ͺ�ʱ���¼�ṹ��
extern RtcTime changetime;												//״̬�ı�ʱ���

PCTRL_MSG tempmessage;



uint8_t temp[COILNUM] = {0};



#define READ_GPIO_VALUE_TABLE(_i, _port, _pin)   do { temp[(_i)] = GPIO_ReadOutputDataBit((_port),(_pin)); } while(0)


GpioTableIndex gpiotable[GPIO_TABLE_SIZE] = {0};



//����Ȧ״̬
void  read_coil(void)
{
	
	READ_GPIO_VALUE_TABLE(0,DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);					//����
	READ_GPIO_VALUE_TABLE(1,DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);			//��ת��
	READ_GPIO_VALUE_TABLE(2,DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);		//���
	READ_GPIO_VALUE_TABLE(3,DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);				//�¶ȱ���1
	READ_GPIO_VALUE_TABLE(4,DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);				//�¶ȱ���2
	READ_GPIO_VALUE_TABLE(5,BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);					//����
	
	ucRegCoilsBuf[0] = temp[0]|temp[1]<<1|temp[2]<<2|temp[3]<<3|temp[4]<<4|temp[5]<<5;
}


//д��Ȧ״̬
void  write_coil(void)
{
	GPIO_WriteBit(DRIVER_GPIO_PORT,HEAT_CONTROL_PIN,ucRegCoilsBuf[0] & 0x01);
	GPIO_WriteBit(DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN,ucRegCoilsBuf[0] & 0x02);
	GPIO_WriteBit(DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN,ucRegCoilsBuf[0] & 0x04);
	GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN,ucRegCoilsBuf[0] & 0x08);
	GPIO_WriteBit(DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN,ucRegCoilsBuf[0] & 0x10);
	GPIO_WriteBit(DRIVER_GPIO_PORT,BOX_DOOR_PIN,ucRegCoilsBuf[0] & 0x20);
}

extern uint8_t thermalbuff[38];

//������Ĵ���
void read_input_register(void)
{
	uint8_t i = 0;
	//��ʱ�¶�
	usRegInputBuf[0] = 	showtextvalue.current_temp_vlaue;
	for(i = 0;i < TIMERECORDNUM;i++)
	{
//		usRegInputBuf[38*i] = thermalbuff[i];
	}
	
//	usRegInputBuf[381] = heattime_log.heattime.Year<<8|heattime_log.heattime.Mon;
//	usRegInputBuf[382] = heattime_log.heattime.Day<<8|heattime_log.heattime.Hour;
//	usRegInputBuf[383] = heattime_log.heattime.Min<<8|heattime_log.heattime.Sec;
}




//�����ּĴ���ֵ
void read_coilregister(void)
{
	usRegHoldingBuf[0] = coilsavevalue.test_duration;
	usRegHoldingBuf[1] = coilsavevalue.test_duration;
	usRegHoldingBuf[2] = coilsavevalue.test_temp;
	usRegHoldingBuf[3] = coilsavevalue.warning1_up;
	usRegHoldingBuf[4] = coilsavevalue.warning1_down;
	usRegHoldingBuf[5] = coilsavevalue.warning2_up;
	usRegHoldingBuf[6] = coilsavevalue.warning2_down;
	usRegHoldingBuf[8] = coilsavevalue.menu_password&0x0000ffff;
	usRegHoldingBuf[9] = coilsavevalue.menu_password>>16;
	usRegHoldingBuf[10] = coilsavevalue.secondtime_password&0x0000ffff;
	usRegHoldingBuf[11] = coilsavevalue.secondtime_password>>16;
	usRegHoldingBuf[12] = coilsavevalue.change_air_time;
	usRegHoldingBuf[13] = coilsavevalue.menu_language;
}





//д�뱣�ּĴ���ֵ
void  writer_register(void)
{
	uint16_t cmdvalue16;
	uint32_t cmdvalue32;

	cmdvalue16 = PTR2U16(tempmessage->param);				//�ӻ�������ȡ16λ����
	cmdvalue32 = PTR2U32(tempmessage->param);				//�ӻ�������ȡ32λ����
	
//	//����ʱ��
	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == TEST_TIME_VALUE && tempmessage.ctrl_msg == 0x11 )
	{
		usRegHoldingBuf[0] = cmdvalue16;
	}

//	//�����¶�
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == TEST_TEMP_VALUE && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[1] = cmdvalue16;
//	}

//	//����1����
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == WARNING1_UP_VALUE && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[2] = cmdvalue16;
//	}

//	//����1����
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == WARNING1_DOWN_VALUE && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[3] = cmdvalue16;
//	}

//	//����2����
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == WARNING2_UP_VALUE && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[4] = cmdvalue16;
//	}

//	//����2����
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == WARNING2_DOWN_VALUE && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[5] = cmdvalue16;
//	}

//	//ģ���ź����
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == ANALOG_OUTPUT && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[6] = cmdvalue16;
//	}

//	//�˵�������
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == NEW_PASSWORD && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[7] = cmdvalue32&0x00ff;
//		usRegHoldingBuf[8] = (cmdvalue32&0xff00)>>16;
//	}

//	//�˵��������������
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == SECOND_INPUT_PASSWORD && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[9] = cmdvalue32&0x00ff;
//		usRegHoldingBuf[10] = (cmdvalue32&0xff00)>>16;
//	}

//	//���������趨
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == CHANGE_AIR_TIME_SET && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[11] = cmdvalue16;
//	}

//	//�˵������趨
//	if((tempmessage.screen_id == language_screen.SCREAT_PROTECT_SCREEN) && tempmessage.control_id == MENU_LENGUAGE && tempmessage.ctrl_msg == 0x11 )
//	{
//		usRegHoldingBuf[12] = cmdvalue16;
//	}
			
}




//��ȡGPIO״ֵ̬
void readgpiostatus(void)
{
	
	READ_GPIO_VALUE_TABLE(0,DRIVER_GPIO_PORT,HEAT_CONTROL_PIN);
	READ_GPIO_VALUE_TABLE(1,DRIVER_GPIO_PORT,SPINNER_RACK_CONTROL_PIN);
	READ_GPIO_VALUE_TABLE(2,DRIVER_GPIO_PORT,CIRCULATING_FUN_CONTROL_PIN);
	READ_GPIO_VALUE_TABLE(3,DRIVER_GPIO_PORT,ALARM_CONTROL1_PIN);
	READ_GPIO_VALUE_TABLE(4,DRIVER_GPIO_PORT,ALARM_CONTROL2_PIN);
	READ_GPIO_VALUE_TABLE(5,BOX_DOOR_GPIO_PORT,BOX_DOOR_PIN);

//	if(temp[1] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,SAMPLE_FRAME_MOTOR_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,RR_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,SAMPLE_FRAME_MOTOR_ID,HIDE); 
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,RR_WORK_STATUS_ID,SHOW);	
//	}

//	if(temp[2] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FAN_OPERATION_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FR_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FAN_OPERATION_ID,HIDE);	
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,FR_WORK_STATUS_ID,SHOW);	
//	}
//	
//	if(temp[3] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING1_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR1_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR1_WORK_STATUS_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING1_ID,HIDE);	
//	}
//	if(temp[4] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING2_ID,SHOW);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR2_WORK_STATUS_ID,HIDE);
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,TEMP_WARNING2_ID,HIDE);
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,AR2_WORK_STATUS_ID,SHOW);	
//	}
//	if(temp[5] == Bit_SET)
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,DOOR_OPEN_ID,SHOW);
//		//��¼����ʱ�䣬�¶�
//	}
//	else
//	{
//		AnimationPlayFrame(language_screen.MAIN_SHOW_SCREEN,DOOR_OPEN_ID,HIDE);
//		//��¼����ʱ�䣬�¶ȣ����洢��Ϣ
//	}
}


































