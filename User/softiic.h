#include "stm32f4xx.h" 
//typedef int            s32;
//typedef long unsigned int   u32;
//typedef short          s16;
//typedef unsigned short u16;
//typedef char           s8;
//typedef unsigned char  u8;


 u8 I2C_PortInit(void);
 u8 I2C_Start(void);
 void I2C_Stop(void);
 void I2C_Ack(void);
 void I2C_NoAck(void);
 u8 I2C_WaitAck(void); //1 for ack, 0 for no ack
 void I2C_SendByte(u8 SendByte);
 u8 I2C_ReceiveByte(void);
 u8 Deal_IIC_Data(u8 slave_addr, u8 *sdata, u16 slen, u8 *rdata, u16 rlen);