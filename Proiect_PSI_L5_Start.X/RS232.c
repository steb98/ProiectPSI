/*
 * File:   RS232.c
 * Author: PSI
 */

#include "global.h"
#include "RS232.h"

/*******************************************************************************
 *  Function name    : RS232_vInit()
 *
 *  Description      : 
 *
 *  List of arguments: none
 *
 *  Return value     : void
 *
 ******************************************************************************/
void RS232_vInit(void)
{
    /* Set Baud Rate */
    T_U16 u16BaudRate = ((GetInstructionClock() / UART_BD_RATE) / 64) - 1;
    SPBRGH = (u16BaudRate >> 8);
    SPBRG = (T_U8) u16BaudRate;

    TXSTAbits.TXEN = 1; /* Enable transmit */

    RCSTAbits.CREN = 1; /* Enable receiver */

    RCSTAbits.SPEN = 1; /* Enable serial port */
}

/*******************************************************************************
 *  Function name    : RS232_vSendDataByte()
 *
 *  Description      : Send one data byte
 *
 *  List of arguments: T_U8 u8DataByte
 *
 *  Return value     : void
 *
 ******************************************************************************/
void RS232_vSendDataByte(T_U8 u8DataByte)
{
    /* Wait for the TX register to be empty */
    while (!TXSTAbits.TRMT);
    /* Load the data byte */
    TXREG = u8DataByte;
}

/*******************************************************************************
 *  Function name    : RS232_vSendMessage()
 *
 *  Description      : 
 *
 *  List of arguments: const char u8Message[]
 *
 *  Return value     : void
 *
 ******************************************************************************/
void RS232_vSendMessage(const char u8Message[])
{
    T_U8 u8Count = 0;

    /* Until end of string */
    while (u8Message[u8Count] != '\0')
    {
        RS232_vSendDataByte(u8Message[u8Count]);
        u8Count++;
    }
    
    /* New line characters */
    RS232_vSendDataByte('\r');
    RS232_vSendDataByte('\n');
}
