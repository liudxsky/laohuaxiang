#include "stm32f4xx.h"
#include "./flash/flash.h"  
#include "stdio.h"

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;      //Flash����״̬����
static uint32_t GetSector(uint32_t Address);

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

int8_t FLASH_Write_Nbytes(uint8_t *WriteAddress,uint8_t *WriteBuf,uint16_t Len)
{
	uint16_t i = 0;
	
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;


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
	/* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
	  /*  MemoryProgramStatus = 0: д���������ȷ
		  MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
	  uwAddress = FLASH_USER_START_ADDR;
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
	  /* ����У�鲻��ȷ */
	  if(uwMemoryProgramStatus)
	  {    
			return -1;
	  }
	  else /*����У����ȷ*/
	  { 
			return 0;	
	  }

} 


/**
  * @brief  ��������ĵ�ַ���������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
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

