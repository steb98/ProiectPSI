/*
 * File:   RS232.h
 * Author: PSI
 */

#ifndef RS232_H
#define RS232_H

extern void RS232_vInit(void);
extern void RS232_vSendDataByte(T_U8 u8DataByte);
extern void RS232_vSendMessage(const char u8Message[]);

#endif  /* RS232_H */
