/*
 * File:   main.c
 * Author: PSI
 */
#include "ioc.h"
#include "sched.h"

//#define DEFAULT_MAIN


void main(void)
{
    SCHED_vInitSystem();
    SCHED_vStartScheduler();
}


#ifdef DEFAULT_MAIN
void main(void)
{
    /* All PORTD as output */
    TRISD = 0x00;

    /* Only pins 0 and 1 of PORTE as output */
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    int index = 0, toggle = 0;

    while (1)
    {

        for(index = 0; index <30000; index++);
        if(toggle == 0)
        {
            toggle = 1;
            LATD = 0xFF;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 1;
        }else if(toggle == 1)
        {
            toggle = 0;
            LATD = 0x00;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
        }
    }
}
#endif