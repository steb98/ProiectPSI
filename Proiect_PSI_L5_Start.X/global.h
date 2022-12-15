#ifndef GLOBAL_H_
#define GLOBAL_H_

/* uC header file */
#include <xc.h>
#include <pic18f6585.h>

/* Type definition */
typedef unsigned char   T_U8;
typedef signed char     T_S8;
typedef unsigned int    T_U16;
typedef signed int      T_S16;
typedef unsigned int    T_BOOL;

typedef int             T_16;
typedef char            T_8;

#define TRUE            1;
#define FALSE           0;

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 10000000UL /* 10Mhz */
#endif

#define GetInstructionClock() _XTAL_FREQ
#define UART_BD_RATE 9600

#endif  /* GLOBAL_H */