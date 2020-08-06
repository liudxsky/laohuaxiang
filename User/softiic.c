#include "softiic.h"
#include "stm32f4xx_gpio.h"

#define M24LC256_WRITE_DELAY delay_ms(15)
 const u32 I2C_CLK_STRETCHING_TIMEOUT = 60000;
 const u32 I2C_WAIT_ACK_TIMEOUT = 30000;
 u32 i2c_clk_stretching_timer = 0;
 u32 i2c_wait_ack_timer = 0;
 #define SCL_PIN GPIO_Pin_12
 #define SDA_PIN GPIO_Pin_13
#define SCL1_H GPIO_SetBits(GPIOB, SCL_PIN)
#define SCL1_L GPIO_ResetBits(GPIOB, SCL_PIN)
#define SDA1_H GPIO_SetBits(GPIOB, GPIO_Pin_13) 
#define SDA1_L GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define SDA1_READ (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
#define SCL1_READ (GPIO_ReadInputDataBit(GPIOB, SCL_PIN))
 void I2C_delay(void)
{
	uint8_t i;
	for (i = 0; i < 200; i++);
}

 u8 I2C_PortInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
#ifdef HW_I2C1	
    //PA8-I2C1_SCL
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C1);
#else
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

#ifdef HW_I2C1 
    //PC9-I2C1_SDA
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C1);
#else 
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 u8 I2C_Start(void)
{
    SDA1_H;
    I2C_delay();   //zyboy
    SCL1_H;
    I2C_delay();
    i2c_clk_stretching_timer = 0;
    while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT);  
    if(!SDA1_READ)
        return 0; //SDA low is busy, exit
    SDA1_L;
    I2C_delay();
    if(SDA1_READ)
        return 0; //SDA high is error, exit
    SCL1_L;
    I2C_delay();
    return 1;
}
 void I2C_Stop(void)
{
    SCL1_L;
    I2C_delay();
    SDA1_L;
    I2C_delay();
    SCL1_H;
    i2c_clk_stretching_timer = 0;
    while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT); 
    I2C_delay();
    SDA1_H;
    I2C_delay();
}
 void I2C_Ack(void)
{
    SCL1_L;
    I2C_delay();
    SDA1_L;
    I2C_delay();
    SCL1_H;
    i2c_clk_stretching_timer = 0;
    //while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT); 
    I2C_delay();
    SCL1_L;
    I2C_delay();
}
 void I2C_NoAck(void)
{
    SCL1_L;
    I2C_delay();
    SDA1_H;
    I2C_delay();
    SCL1_H;
    i2c_clk_stretching_timer = 0;
    //while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT);   
    I2C_delay();
    SCL1_L;
    I2C_delay();
}
 u8 I2C_WaitAck(void) //1 for ack, 0 for no ack
{
    delay_us(10);
    SCL1_L;
    I2C_delay();
    SDA1_H;
    I2C_delay(); 
		//SCL1_H;
	//I2C_delay(); 
	//SCL1_L;
    i2c_wait_ack_timer = 0;
    while((SDA1_READ) && i2c_wait_ack_timer++<=I2C_WAIT_ACK_TIMEOUT);  
    if(!SDA1_READ)
    {
        SCL1_H;
        i2c_clk_stretching_timer = 0;
        while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT);  
        I2C_delay();
        SCL1_L;
			 I2C_delay(); 
			
        return 1;
    }
		 I2C_delay(); 
		SCL1_L;
    return 0;
}
 void I2C_SendByte(u8 SendByte) //MSB
{
    u8 i = 8;
    while(i--)
    {
        SCL1_L;
        I2C_delay();
        if(SendByte&0x80)
            SDA1_H;
        else
            SDA1_L;
        SendByte<<=1;
        I2C_delay();
        SCL1_H;
        i2c_clk_stretching_timer = 0;
        while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT);  
        I2C_delay();
    }
    SCL1_L;
}
 u8 I2C_ReceiveByte(void) //MSB
{
    u8 i = 8;
		u8 m=0;
    u8 ReceiveByte = 0;
					 I2C_delay(); 
				 I2C_delay(); 
				 I2C_delay(); 
    SDA1_H;
    while(i--)
    {
        ReceiveByte <<= 1;
        SCL1_L;
        I2C_delay();
        SCL1_H;
        i2c_clk_stretching_timer = 0;
        //while((!SCL1_READ) && i2c_clk_stretching_timer++<=I2C_CLK_STRETCHING_TIMEOUT); 
        I2C_delay();
        if(SDA1_READ)
        {
            ReceiveByte |= 0x01;
        }
    }
    SCL1_L;
    return ReceiveByte;
}
 u8 Deal_IIC_Data(u8 slave_addr, u8 *sdata, u16 slen, u8 *rdata, u16 rlen)
{
    u16 i = 0;
    
    if(slen > 0)
    {
        if(!I2C_Start())
            return 0;
        I2C_SendByte(slave_addr);
        if(!I2C_WaitAck())
        {
            I2C_Stop();
            return 0;
        }        
        for(i = 0; i < slen; i++)
        {
            I2C_SendByte(sdata[i]);
            if(!I2C_WaitAck())
            {
                I2C_Stop();
                return 0;
            }
        }
    }
        if(rlen > 0)
    {
        if(!I2C_Start())
            return 0;

        I2C_SendByte(slave_addr | 0x01);
        if(!I2C_WaitAck())
        {
            I2C_Stop();
            return 0;
        }
        for(i = 0; i < rlen; i++)
        {
            rdata[i] = I2C_ReceiveByte();

            if(i == (rlen-1))
            {
                I2C_NoAck();
            }
            else
            {
                I2C_Ack();
            }
        }
    }
    I2C_Stop();
    return 1;
}

