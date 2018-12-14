/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"
#include "./usart/usart.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "./gpio/gpio.h"

extern uint8_t  Rs485RX,Rs485TX;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
     if(xRxEnable)
	{
		USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);
		RS485_RX_EN();
	}
	else
	{
		USART_ITConfig(RS485_USART, USART_IT_RXNE, DISABLE);
		RS485_TX_EN();
	}

	if(xTxEnable)
	{
		USART_ITConfig(RS485_USART, USART_IT_TXE, ENABLE);
		RS485_TX_EN();
	}
	else
	{
	   USART_ITConfig(RS485_USART, USART_IT_TXE, DISABLE);
	   RS485_RX_EN();
	}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	RS485_USART_Config(ulBaudRate);
    return TRUE;
}

BOOL
xMBPortSerialPutByte( UCHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
     RS485_TX_EN();
     USART_SendData(RS485_USART, ucByte);
	  while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TC) == RESET)/*等待发送完成*/
	  {
	  
	  }	
	  RS485_RX_EN(); 
    return TRUE;
}

BOOL
xMBPortSerialGetByte( UCHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
     *pucByte = USART_ReceiveData(RS485_USART);
	Rs485RX = 1;
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR( void )
{
	RS485_TX_EN();
    pxMBFrameCBTransmitterEmpty(  );
	RS485_RX_EN();
	Rs485TX = 1;
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );

}
