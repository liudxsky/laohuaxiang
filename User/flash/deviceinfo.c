#include "stm32f4xx.h"
#include "stdio.h"
#include "./delaytime/delaytime.h"
#include "./flash/deviceinfo.h"
#include "./flash/flash.h"
#include "./usart/usart.h"


uint8_t ChipUniqueID[12] = {0};
uint32_t * const  p = (uint32_t *)(0x0800F000); 
#define INVALID_INFO		0xFFFF
dev_info_t dev_info;
extern uint8_t SCREENSIZE;

void DeviceInfo_Init(void)
{
	uint8_t i;
	FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info.valid_flag,4);
	//��ȡ�洢��־λ
	if(dev_info.valid_flag==INVALID_INFO)
	{
		dev_info.valid_flag = 1;
		dev_info.pwmscope = 1000;
		dev_info.pwmvalue = 0;
		dev_info.airvalveangle = 0;
		dev_info.biglanguagestatus = 1;
		for (i = 0; i < 19; ++i)
		{
			dev_info.change_air_time[i] = 0;
		}
		/*����ֵ*/
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*���ڴ�ӡ��Ϣ*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.airvalveangle);
		if(SCREENSIZE)
		{
			printf("\r\n device current language is selected %d (1 is chinese ,0 is english)\r\n",dev_info.biglanguagestatus);
		}
		else
		{
			printf("\r\n device current language is selected %d (1 is chinese ,0 is english)\r\n",dev_info.biglanguagestatus);
		}
	}
	else
	{
		//todo
		FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*���ڴ�ӡ��Ϣ	*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.airvalveangle);
		printf("\r\n device current Pid value is : \r\nP :%f\r\nI :%f\r\n D : %f\r\n",dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D);
		printf("\r\n device current change air times is :\r\n");
		for (i = 0; i < 19; ++i)
		{
			printf("%d : %d \r\n",90 - 5*i,dev_info.change_air_time[i]);
		}
		printf("\r\n device current set testtime is :%f\r\n",dev_info.testtime);		
		printf("\r\n device current set tempvalue is :%f\r\n",dev_info.testtemp);
	}

}


uint32_t password1 = 0;
uint32_t password2 = 0;
u32 Fml_Constant = 5000000; // ���뵽��ʽ�ĳ��� 


void  encryp_password(void)
{
	FLASH_Write_Nbytes((uint8_t *)p,(uint8_t *)&Fml_Constant,32);
	delay_ms(2);
	FLASH_Read_Nbytes((uint8_t *)p,(uint8_t *)&Fml_Constant,32);
	password1 = Fml_Constant%912031;
	
	password2 = Fml_Constant%998527;
	printf("password1 is %ld:\n",password1);
	printf("password2 is %ld:\n",password2);
}


void STM32_Read_ID(void)
{	
//     ��ַ��С����,�ȷŵ��ֽڣ��ٷŸ��ֽڣ�С��ģʽ
//     ��ַ��С����,�ȷŸ��ֽڣ��ٷŵ��ֽڣ����ģʽ
	ChipUniqueID[11] = *(__IO u32*)(0X1FFF7A10);  // ���ֽ�
	ChipUniqueID[10] = *(__IO u32 *)(0X1FFF7A11); // 
	ChipUniqueID[9] = *(__IO u32 *)(0X1FFF7A12); // 
	ChipUniqueID[8] = *(__IO u32 *)(0X1FFF7A13); // 
	ChipUniqueID[7] = *(__IO u32 *)(0X1FFF7A14); // 
	ChipUniqueID[6] = *(__IO u32 *)(0X1FFF7A15); // 
	ChipUniqueID[5] = *(__IO u32 *)(0X1FFF7A16); // 
	ChipUniqueID[4] = *(__IO u32 *)(0X1FFF7A17); // 
	ChipUniqueID[3] = *(__IO u32 *)(0X1FFF7A18); // 
	ChipUniqueID[3] = *(__IO u32 *)(0X1FFF7A19); // 
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFF7A1A); // 
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFF7A1B); // ���ֽ�
	printf(" оƬ��ΨһIDΪ: %.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x \r\n",
		ChipUniqueID[0],ChipUniqueID[1],
		ChipUniqueID[2],ChipUniqueID[3],
		ChipUniqueID[4],ChipUniqueID[5],
		ChipUniqueID[6],ChipUniqueID[7],
		ChipUniqueID[8],ChipUniqueID[9],
		ChipUniqueID[10],ChipUniqueID[11]);

}


#if 0

uint16_t encry_password1(void)
{
	uint16_t password1 = 0;
	*p = *ChipUniqueID;
	password1 = *p>>8 &0xfff0;
	return password1;
}


uint16_t encry_password2(void)
{
	uint16_t password2 = 0;
	*p = *ChipUniqueID;
	password2 = *p>>16 &0x0fff;
	return password2;
}

//CRC16����

u32 Fml_Constant; // ���뵽��ʽ�ĳ��� 
u8 *C= (u8*)&Fml_Constant;//�ѳ���ת��Ϊ���� 
u16 Fml_CRC16; 
u16 Formula_CRC16(u8 *p,u8 len)
{ 
  u8 i; 
  while(len--) 
  { 
    for(i=0x80; i!=0; i>>=1) 
    { 
      if((Fml_CRC16 & 0x8000) != 0) 
      { 
        Fml_CRC16 <<= 1; 
        Fml_CRC16 ^= 0x1021; 
      } 
      else 
      { 
        Fml_CRC16 <<= 1; 
      } 
      if((*p&i)!=0) 
      { 
        Fml_CRC16 ^= 0x1021; 
      } 
    } 
    p++; 
  } 
  return Fml_CRC16; 
}


//��ʽ     DΪ��ʽ���������飬ResultΪ��ʽ���������
void Formula_74(u8 *D,u8 *Result)
{
	Result[0] = C[0] | D[0] | D[1] ; 
  	Result[1] = C[1] + D[0] + D[6] + D[7] - D[8] + D[9] - D[10] + D[11] ; 
  	Result[2] = C[2] | D[0] | D[1] ^ D[2] & D[3] ^ D[4]; 
  	Result[3] = C[3] | D[0] | D[1] ^ D[2] ^ D[3] ^ D[4] ^ D[5] | D[6] ^ D[7] ^ D[8] ^ D[9] ^ D[10] ^ D[11] ; 
}

#endif

