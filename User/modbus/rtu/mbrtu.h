/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu.h,v 1.9 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_RTU_H
#define _MB_RTU_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif





typedef struct{

	uint8_t 	address_code;		//��ַ��
	uint8_t 	function_code;		//������
	uint16_t 	Reg_address;		//�Ĵ�����ַ
	uint8_t		Reg_number;			//�Ĵ�������
	uint16_t	Rev_data;			//��������
	uint16_t	Crc_verify;			//CRCУ��
}RevFramestruct;


eMBErrorCode eMBRTUInit( UCHAR slaveAddress, UCHAR ucPort, ULONG ulBaudRate,
                             eMBParity eParity );
void            eMBRTUStart( void );
void            eMBRTUStop( void );
eMBErrorCode    eMBRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength );
eMBErrorCode    eMBRTUSend( UCHAR slaveAddress, const UCHAR * pucFrame, USHORT usLength );
BOOL            xMBRTUReceiveFSM( void );
BOOL            xMBRTUTransmitFSM( void );
BOOL            xMBRTUTimerT15Expired( void );
BOOL            xMBRTUTimerT35Expired( void );
#if 0

//0x01:��ȡ��Ȧ״̬ȡ��һ���߼���Ȧ�ĵ�ǰ״̬��ON/OFF)  
int modbus_read_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest);

//0x02:��ȡ����״̬ ȡ��һ�鿪������ĵ�ǰ״̬��ON/OFF)  
int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest);

//0x03:��ȡ���ּĴ��� ��һ���������ּĴ�����ȡ�õ�ǰ�Ķ�����ֵ  
int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);

//0x04:��ȡ����Ĵ��� ��һ����������Ĵ�����ȡ�õ�ǰ�Ķ�����ֵ  
int modbus_read_input_registers(modbus_t *ctx, int addr, int nb,
                                uint16_t *dest);

//0x05:ǿ�õ���Ȧ ǿ��һ���߼���Ȧ��ͨ��״̬
int modbus_write_bit(modbus_t *ctx, int addr, int status);

//0x06:Ԥ�õ��Ĵ��� �Ѿ������ֵװ��һ�����ּĴ���
int modbus_write_register(modbus_t *ctx, int addr, int value);

//0x0F:ǿ�ö���Ȧ ǿ��һ�������߼���Ȧ��ͨ��
int modbus_write_bits(modbus_t *ctx, int addr, int nb, const uint8_t *src);

//0x10:��ѯ��ֻ����484�� ��ʹ������һ̨����ִ�г���������ӻ�ͨ�ţ�̽ѯ�ôӻ��Ƿ��������������񣬽��ں��й�����9�ı��ķ��ͺ󣬱�������ŷ���
int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *src);

//0x17:����ӻ���ʶ ��ʹ�����жϱ�ַ�ӻ������ͼ��ôӻ�����ָʾ�Ƶ�״̬
int modbus_write_and_read_registers(modbus_t *ctx,
                                    int write_addr, int write_nb, const uint16_t *src,
                                    int read_addr, int read_nb, uint16_t *dest)

#endif
#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
