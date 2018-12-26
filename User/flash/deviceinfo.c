#include "stm32f4xx.h"
#include "stdio.h"
#include "./delaytime/delaytime.h"
#include "./flash/deviceinfo.h"
#include "./flash/flash.h"
#include "./usart/usart.h"
#include "crypto.h"


uint8_t ChipUniqueID[12] = {0};
uint32_t * const  p = (uint32_t *)(0x08070000); 
#define INVALID_INFO		0xFFFF
dev_info_t dev_info;
extern uint8_t SCREENLANGUAGE;

void DeviceInfo_Init(void)
{
	uint8_t i;
	FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info.valid_flag,4);
	//读取存储标志位
	if(dev_info.valid_flag==INVALID_INFO)
	{
		dev_info.valid_flag = 1;
		dev_info.pwmscope = 1000;
		dev_info.pwmvalue = 0;
		dev_info.flash_setvalue.air_door_angle = 0;
		dev_info.biglanguagestatus = 1;
		for (i = 0; i < 19; ++i)
		{
			dev_info.change_air_time[i] = 0;
		}
		/*赋初值*/
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*串口打印信息*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.flash_setvalue.air_door_angle);
		if(SCREENLANGUAGE)
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
		/*串口打印信息	*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.flash_setvalue.air_door_angle);
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
u32 Fml_Constant = 5000000; // 输入到公式的常数 


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


void STM32_Read_ID(void)					//小端模式
{	
//     地址从小到大,先放低字节，再放高字节：小端模式
//     地址从小到大,先放高字节，再放低字节：大端模式
	ChipUniqueID[11] = *(__IO u32*)(0X1FFF7A10);  // 低字节
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
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFF7A1B); // 高字节
	printf(" 芯片的唯一ID为: %.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x-%.2x \r\n",
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

//CRC16运算
uint32_t Fml_Constant; // 输入到公式的常数 
uint8_t *C= (uint8_t*)&Fml_Constant;//把常量转换为数组 
uint16_t Fml_CRC16; 
uint16_t Formula_CRC16(uint8_t *p,uint8_t len)
{ 
  uint8_t i; 
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


//公式     D为公式的输入数组，Result为公式计算结果输出
void Formula_74(uint8_t *D,uint8_t *Result)
{
	Result[0] = C[0] | D[0] | D[1] ; 
  	Result[1] = C[1] + D[0] + D[6] + D[7] - D[8] + D[9] - D[10] + D[11] ; 
  	Result[2] = C[2] | D[0] | D[1] ^ D[2] & D[3] ^ D[4]; 
  	Result[3] = C[3] | D[0] | D[1] ^ D[2] ^ D[3] ^ D[4] ^ D[5] | D[6] ^ D[7] ^ D[8] ^ D[9] ^ D[10] ^ D[11] ; 
}

#endif







uint8_t InputMessage[] =  "Sample #1";
/* string length only, without '\0' end of string marker */
uint32_t InputLength = (sizeof(InputMessage) - 1);

uint8_t HMAC_Key[] =
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
    0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
    0x3c, 0x3d, 0x3e, 0x3f
  };

/* string length only, without '\0' end of string marker */
uint32_t KeyLength = (sizeof(HMAC_Key));


uint8_t MAC[CRL_SHA1_SIZE];
int32_t MACLength = 0;

const uint8_t Expected_OutputMessage[HMAC_LENGTH] =
{
    0x4f, 0x4c, 0xa3, 0xd5, 0xd6,
    0x8b, 0xa7, 0xcc, 0x0a, 0x12,
    0x08, 0xc9, 0xc6, 0x1e, 0x9c,
    0x5d, 0xa0, 0x40, 0x3c, 0x0a
};










/**
  * @brief  SHA1 HMAC compute example.
  * @param  InputMessage: pointer to input message to be hashed.
  * @param  InputMessageLength: input data message length in byte.
  * @param  HMAC_key: pointer to key used in the HMAC computation
  * @param  HMAC_keyLength: HMAC key length in byte.
  * @param  MessageDigest: pointer to output parameter that will handle message digest
  * @param  MessageDigestLength: pointer to output digest length.
  * @retval error status: can be HASH_SUCCESS if success or one of
  *         HASH_ERR_BAD_PARAMETER, HASH_ERR_BAD_CONTEXT,
  *         HASH_ERR_BAD_OPERATION if error occured.
  */
int32_t STM32_SHA1_HMAC_Compute(uint8_t* InputMessage,
                          uint32_t InputMessageLength,
                          uint8_t *HMAC_key,
                          uint32_t HMAC_keyLength,
                          uint8_t *MessageDigest,
                          int32_t* MessageDigestLength)
{
  HMAC_SHA1ctx_stt HMAC_SHA1ctx;
  uint32_t error_status = HASH_SUCCESS;

  /* Set the size of the desired MAC*/
  HMAC_SHA1ctx.mTagSize = CRL_SHA1_SIZE;

  /* Set flag field to default value */
  HMAC_SHA1ctx.mFlags = E_HASH_DEFAULT;

  /* Set the key pointer in the context*/
  HMAC_SHA1ctx.pmKey = HMAC_key;

  /* Set the size of the key */
  HMAC_SHA1ctx.mKeySize = HMAC_keyLength;

  /* Initialize the context */
  error_status = HMAC_SHA1_Init(&HMAC_SHA1ctx);

  /* check for initialization errors */
  if (error_status == HASH_SUCCESS)
  {
    /* Add data to be hashed */
    error_status = HMAC_SHA1_Append(&HMAC_SHA1ctx,
                                    InputMessage,
                                    InputMessageLength);

    if (error_status == HASH_SUCCESS)
    {
      /* retrieve */
      error_status = HMAC_SHA1_Finish(&HMAC_SHA1ctx, MessageDigest, MessageDigestLength);
    }
  }

  return error_status;
}


/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}


void testpassword(void)
{
	int32_t status = HASH_SUCCESS;
	uint8_t i;
	status = STM32_SHA1_HMAC_Compute((uint8_t*)InputMessage,
                             InputLength,
                             HMAC_Key,
                             KeyLength,
                             (uint8_t*)MAC,
                             &MACLength);
	if(status == HASH_SUCCESS)
	{
		for(i = 0;i<MACLength;i++)
		{
			printf("MAC[%hhd] is %x\n",i,MAC[i]);
		}
	}

}

