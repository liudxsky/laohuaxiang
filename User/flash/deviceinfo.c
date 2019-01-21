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
extern uint8_t Modbus_rate;

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
		dev_info.flash_setvalue.air_door_angle = 0;
		for (i = 0; i < 21; ++i)
		{
			dev_info.change_air_time[i] = 0;
		}
		dev_info.flash_setvalue.warning1_up = 1000;
		dev_info.flash_setvalue.warning2_up = 1000;
		dev_info.Modbus_address = 1;					
		dev_info.biglanguagestatus = 1;
		dev_info.modbus_tran_rate_flag = 3;
		dev_info.flash_adjusttemp = 0;
		dev_info.compensatetemp = 0;
		dev_info.addup_testtime = 0;
		/*����ֵ*/
		FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*���ڴ�ӡ��Ϣ*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.flash_setvalue.air_door_angle);
		
	}
	else
	{
		//todo
		FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*���ڴ�ӡ��Ϣ	*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.flash_setvalue.air_door_angle);
		printf("\r\n device current Pid value is : \r\nP :%f\r\nI :%f\r\n D : %f\r\n",dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D);
		printf("\r\n device current change air times is :\r\n");
		for (i = 0; i < CHANGE_AIR_SIZE; ++i)
		{
			printf("%d : %d \r\n",100 - 5*i,dev_info.change_air_time[i]);
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


void STM32_Read_ID(void)					//С��ģʽ
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
uint32_t Fml_Constant; // ���뵽��ʽ�ĳ��� 
uint8_t *C= (uint8_t*)&Fml_Constant;//�ѳ���ת��Ϊ���� 
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


//��ʽ     DΪ��ʽ���������飬ResultΪ��ʽ���������
void Formula_74(uint8_t *D,uint8_t *Result)
{
	Result[0] = C[0] | D[0] | D[1] ; 
  	Result[1] = C[1] + D[0] + D[6] + D[7] - D[8] + D[9] - D[10] + D[11] ; 
  	Result[2] = C[2] | D[0] | D[1] ^ D[2] & D[3] ^ D[4]; 
  	Result[3] = C[3] | D[0] | D[1] ^ D[2] ^ D[3] ^ D[4] ^ D[5] | D[6] ^ D[7] ^ D[8] ^ D[9] ^ D[10] ^ D[11] ; 
}

#endif








uint8_t InputMessage[] =  "Huangloong";
/* string length only, without '\0' end of string marker */
uint32_t InputLength = (sizeof(InputMessage) - 1);

uint8_t HMAC_Key[] =
  {
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
   	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x30, 0x31, 0x32, 0x33
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

static const char *ALPHA_BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
char *encode(const char *buf, const long size, char *base64Char) {
    int a = 0,i = 0;
	int int63 = 0x3F; //  00111111
    int int255 = 0xFF; // 11111111
    char b0,b1,b2;
    while (i < size) {
        b0 = buf[i++];
        b1 = (i < size) ? buf[i++] : 0;
        b2 = (i < size) ? buf[i++] : 0;
         
        base64Char[a++] = ALPHA_BASE[(b0 >> 2) & int63];
        base64Char[a++] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
        base64Char[a++] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
        base64Char[a++] = ALPHA_BASE[b2 & int63];
    }
    switch (size % 3) {
        case 1:
            base64Char[--a] = '=';
        case 2:
            base64Char[--a] = '=';
    }
    return base64Char;
}
 
char *decode(const char *base64Char, const long base64CharSize, char *originChar, long originCharSize) {
    int toInt[128] = {-1};
	int i;
	int int255 = 0xFF;
    int index = 0;
	int c0,c1,c2,c3;
    for (i = 0; i < 64; i++) {
        toInt[ALPHA_BASE[i]] = i;
    }
  
    for ( i = 0; i < base64CharSize; i += 4) {
        c0 = toInt[base64Char[i]];
        c1 = toInt[base64Char[i + 1]];
        originChar[index++] = (((c0 << 2) | (c1 >> 4)) & int255);
        if (index >= originCharSize) {
            return originChar;
        }
        c2 = toInt[base64Char[i + 2]];
        originChar[index++] = (((c1 << 4) | (c2 >> 2)) & int255);
        if (index >= originCharSize) {
            return originChar;
        }
        c3 = toInt[base64Char[i + 3]];
        originChar[index++] = (((c2 << 6) | c3) & int255);
    }
    return originChar;
}

void testpassword(void)
{
	int32_t status = HASH_SUCCESS;
	uint8_t i;
	char * basedata = (char *)malloc(sizeof(unsigned char)*KeyLength+1); 
//	memcpy(HMAC_Key,ChipUniqueID,sizeof(ChipUniqueID));
	
	printf("\r\n*************************************************************** :\r\n");
	for (i = 0; i < KeyLength; ++i)
		{
		printf("%.2X ",HMAC_Key[i]);
		}
	printf("\r\n*************************************************************** :\r\n");
	status = STM32_SHA1_HMAC_Compute((uint8_t*)InputMessage,
                             InputLength,
                             HMAC_Key,
                             KeyLength,
                             (uint8_t*)MAC,
                             &MACLength);
	if(status == HASH_SUCCESS)
	{
		printf("out buff is :");
		printf("\r\n*************************************************************** :\r\n");
		for(i = 0;i<MACLength;i++)
		{
			printf("%.2x  ",MAC[i]);
		}
		
		printf("\r\n*************************************************************** :\r\n");
		encode(MAC,MACLength,basedata);
		printf("base data is \n%s\r\n",basedata);
	}
	free(basedata);
}

