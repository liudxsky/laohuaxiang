#ifndef  __MAIN_H
#define  __MAIN_H


#include "stm32f4xx.h"

#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
//输入寄存器起始地址
#define 	REG_INPUT_START 		0x0000
//输入寄存器数量
#define 	REG_INPUT_NREGS 		8




//保持寄存器起始地址
#define 	REG_HOLDING_START 		0x0000
//保持寄存器数量
#define 	REG_HOLDING_NREGS 		158


//线圈寄存器起始地址
#define 	REG_COILS_START 		0x0000
//线圈寄存器数量
#define 	REG_COILS_SIZE 			8


//开关输入寄存器起始地址
#define 	REG_DISCRETE_START  	0x0000
//开关输入寄存器数量
#define 	REG_DISCRETE_SIZE  		8
/* --------------------------------------------------------------------------*/



void System_Init(void);

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );

eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode );

eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );

eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );


#endif

