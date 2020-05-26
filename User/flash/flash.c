#include "stm32f4xx.h"
#include "./flash/flash.h"  
#include "stdio.h"
#include "./flash/deviceinfo.h"
extern dev_info_t dev_info;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;      //Flash操作状态变量
static uint32_t GetSector(uint32_t Address);
int scanFlash()
{
	uint32_t readaddr=FLASH_START_ADDR;
	uint32_t tempcnt=0;
	uint32_t validaddress=0;
	while(readaddr!=(FLASH_END_ADDR))
	{
		FLASH_Read_Nbytes((uint8_t *)readaddr,(uint8_t *)&dev_info,sizeof(dev_info_t));
		if(dev_info.flash_setvalue.flash_write_cnt!=0xFFFFFFFF&&dev_info.flash_setvalue.flash_write_cnt>tempcnt)
		{
			tempcnt=dev_info.flash_setvalue.flash_write_cnt;
			validaddress=readaddr;
		}
		
		readaddr=readaddr+FLASH_BLOCK_SIZE;
	}
	if(validaddress>=FLASH_START_ADDR&&validaddress<FLASH_END_ADDR)
	{
		__disable_irq(); 
		FLASH_Read_Nbytes((uint8_t *)validaddress,(uint8_t *)&dev_info,sizeof(dev_info_t));
		__enable_irq();
		delay_ms(1000);
		//printf("valid addr:%x, got addr:%x\n", validaddress,dev_info.flash_setvalue.flash_this_address);
		return 1;
	}
	else 
	{
		  
		return -1;
	}
}
void readFlash()
{
	
	__disable_irq(); 
	FLASH_Read_Nbytes((uint8_t *)dev_info.flash_setvalue.flash_this_address,(uint8_t *)&dev_info,sizeof(dev_info_t));
	__enable_irq();
	printf("read addr:%x\n", dev_info.flash_setvalue.flash_this_address);
}
void writeFlash()
{
	if(dev_info.flash_setvalue.flash_this_address>FLASH_END_ADDR||dev_info.flash_setvalue.flash_this_address<FLASH_START_ADDR)
	{
		if(scanFlash()<0)
		{//all flash corrupted
			dev_info.flash_setvalue.flash_this_address=FLASH_START_ADDR;
			dev_info.flash_setvalue.flash_write_cnt=0;
		}
	}
	__disable_irq(); 

		dev_info.flash_setvalue.flash_this_address=dev_info.flash_setvalue.flash_this_address+FLASH_BLOCK_SIZE;
		dev_info.flash_setvalue.flash_write_cnt++;
	if(dev_info.flash_setvalue.flash_this_address==FLASH_END_ADDR)
	{
		dev_info.flash_setvalue.flash_this_address=FLASH_START_ADDR;
	}
	
	FLASH_Write_Nbytes(dev_info.flash_setvalue.flash_this_address,(uint8_t *)&dev_info,sizeof(dev_info_t));	
	printf("write flash %x\n",dev_info.flash_setvalue.flash_this_address);
	__enable_irq();  
}
void FLASH_Read_Nbytes(uint8_t *ReadAddress, uint8_t *ReadBuf, uint16_t Len) 
{
	uint16_t i = 0;
	for (i=0; i<Len; i++)
	{
		//4bytes read
		*ReadBuf++ = *ReadAddress;
		ReadAddress+=4;
	}
}

int8_t FLASH_Write_Nbytes(uint32_t WriteAddress,uint8_t *WriteBuf,uint16_t Len)
{
	uint16_t i = 0;
	
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
	uint32_t FLASH_USER_START_ADDR=WriteAddress;
	uint32_t FLASH_USER_END_ADDR=WriteAddress+FLASH_BLOCK_SIZE;

	FLASH_Unlock();	
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	uwSectorCounter = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);
	while (uwSectorCounter <= uwEndSector) 
	{
		if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
		{
			return -1;
		}
		if (uwSectorCounter == uwEndSector) 
		{
			uwSectorCounter += 8;
		} 
		else 
		{
			uwSectorCounter += 8;
		}
	}
	

	uwAddress = (uint32_t)WriteAddress;
	
	while (uwAddress < FLASH_USER_END_ADDR) 
	{
		for(i=0;i<Len;i++)
		{
			if (FLASH_ProgramWord(uwAddress, (uint32_t )*WriteBuf) == FLASH_COMPLETE)
			{
				uwAddress = uwAddress + 4;
				WriteBuf++;
			} 
			else 
			{
				return -1;
			}
		}
		break;
	}
	FLASH_Lock();
	/* 从FLASH中读取出数据进行校验***************************************/
	  /*  MemoryProgramStatus = 0: 写入的数据正确
		  MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数 */
	  uwAddress = FLASH_START_ADDR;
	  uwMemoryProgramStatus = 0;
	  
	  while (uwAddress < FLASH_USER_END_ADDR)
	  {
		uwData32 = *(__IO uint32_t*)uwAddress;

		if (uwData32 != (uint32_t )*WriteBuf)
		{
		  uwMemoryProgramStatus++;	
		}

		uwAddress = uwAddress + 4;
	  }  
	  /* 数据校验不正确 */
	  if(uwMemoryProgramStatus)
	  {    
			return -1;
	  }
	  else /*数据校验正确*/
	  { 
			return 0;	
	  }

} 


/**
  * @brief  根据输入的地址给出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else /*((Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }


  return sector;
}

