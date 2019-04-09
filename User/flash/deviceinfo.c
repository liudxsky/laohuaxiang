#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "./delaytime/delaytime.h"
#include "./flash/deviceinfo.h"
#include "./flash/flash.h"
#include "./usart/usart.h"
#include "crypto.h"
#include "math.h"
#include "./screen/screen.h"
uint8_t ChipUniqueID[12] = {0};
uint32_t * const  p = (uint32_t *)(0x08070000); 
#define INVALID_INFO		0xFFFF
dev_info_t dev_info;
extern BIG_SCREEN_ID_TAB biglanguage_screen;

void DeviceInfo_Init(void)
{
	uint8_t i;
	//????
	int sf=scanFlash();
	//FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info.valid_flag,4);
	//读取存储标志位
	if(sf<0)
	{
		dev_info.valid_flag = 1;
		dev_info.pwmscope = 1000;
		dev_info.pwmvalue = 0;
		for (i = 0; i < 21; ++i)
		{
			dev_info.change_air_time[i] = 0;
		}
		memset(&dev_info,0,sizeof(dev_info_t));
		dev_info.flash_setvalue.warning1_up = 50;
		dev_info.flash_setvalue.warning2_up = 50;
		dev_info.flash_setvalue.screen_light_value = 100;
		dev_info.flash_setvalue.modbus_address = 1;					
		dev_info.biglanguagestatus = 1;
		dev_info.flash_setvalue.modbus_tran_rate = 9600;
		dev_info.flash_adjusttemp = 0;
		dev_info.compensatetemp = 0;
		dev_info.addup_testtime = 0;
		dev_info.testTime=0;
		dev_info.flash_setvalue.change_air_time=0;
		dev_info.Rs485Status = 2;
		dev_info.thermocouple_flag=0;
		dev_info.runstatus=0;
		dev_info.autonopowertime.Year = 20;  // Prevent direct access to the recovery interface
		dev_info.autonopowertime.Mon = 1;
		dev_info.autonopowertime.Day = 1;
		dev_info.autonopowertimeflag = 0;
		dev_info.flash_setvalue.flash_this_address=FLASH_START_ADDR;
		dev_info.flash_setvalue.flash_write_cnt=0;
		dev_info.pidvalue.PID_P=0;
		dev_info.pidvalue.PID_I=0;
		dev_info.pidvalue.PID_D=0;
		dev_info.setTemp=0;
		autogeneratepassword();
		/*赋初值*/
		writeFlash();
		//FLASH_Write_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*串口打印信息*/
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.airdooropenangle);
		printf("\r\n device current set modbus address is :%d\r\n",dev_info.flash_setvalue.modbus_address);
		
	}
	else
	{
		//todo
		readFlash();
		//FLASH_Read_Nbytes((uint8_t *)FLASH_USER_START_ADDR,(uint8_t *)&dev_info,sizeof(dev_info_t));
		/*串口打印信息	*/
				dev_info.pwmscope = 1000;
		dev_info.pwmvalue = 0;
		printf("\r\n device curren pwm scope is %d \r\n",dev_info.pwmscope);
		printf("\r\n device current pwm value is %d \r\n",dev_info.pwmvalue);
		printf("\r\n device current air valve angle value is %d \r\n",dev_info.airdooropenangle);
		printf("\r\n device current Pid value is : \r\nP :%f\r\nI :%f\r\n D : %f\r\n",dev_info.pidvalue.PID_P,dev_info.pidvalue.PID_I,dev_info.pidvalue.PID_D);
		printf("\r\n device current change air times is :\r\n");
		for (i = 0; i < CHANGE_AIR_SIZE; ++i)
		{
			printf("%d : %d \r\n",100 - 5*i,dev_info.change_air_time[i]);
		}
		printf("\r\n device current set testtime is :%f\r\n",dev_info.testTime);
		printf("\r\n device current set testtime is :%f\r\n",dev_info.testTime);		
		printf("\r\n device current set modbus address is :%d\r\n",dev_info.flash_setvalue.modbus_address);

		dev_info.temp_warnning1=50;
		dev_info.temp_warnning2=50;
		dev_info.dev_status_changed_flag=0;
		dev_info.lefttimeflag=0;
		dev_info.passwordwrongflag=0;
		dev_info.thermocouple_flag=0;

		if(dev_info.runstatus>0)
		{
			//pop up abnormal power down screen
			check_language_select();
			SetTextValueRTC(biglanguage_screen.BIG_ABNORMAL_POWEROFF,BIG_ABNORMAL_TIME,dev_info.timenow);
		}
		dev_info.runstatus=0;
	}

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

void autogeneratepassword(void)
{
	int32_t status = HASH_SUCCESS;
	uint8_t i,j;
	uint8_t demobuff[10] = {0};
	uint32_t hexdata = 0;
	memcpy(HMAC_Key,ChipUniqueID,sizeof(ChipUniqueID));
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
			printf("%x  ",MAC[i]);
		}
		printf("\n");
		for(i = MACLength-1;i>MACLength-4;i--)
		{
			hexdata += MAC[i]*(pow(256,(MACLength -i-1)));
			printf("hexdata is %6x\n",hexdata);
		} 
//		sprintf(demobuff,"%d",hexdata); 
//		strncpy(dev_info.autonopowerpassword,demobuff,PASSWORDLENGTH);
//		printf("destbuff is %6s \n",dev_info.autonopowerpassword);
		sprintf(dev_info.autonopowerpassword,"%06d",111111);
		sprintf(dev_info.flash_setvalue.menu_password,"%06d",666666);
		sprintf(dev_info.flash_setvalue.protect_password,"%06d",888888);
		printf("\r\n*************************************************************** :\r\n");
	}

}

