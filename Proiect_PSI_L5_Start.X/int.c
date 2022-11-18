#include "int.h"
#include "adc.h"
#include "pcs.h"
#include "sched.h"

void INT_vInit()
{
    // Enable ADC int
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;

    // Enable TMR2 int
    PIR1bits.TMR2IF = 0b0;
    IPR1bits.TMR2IP = 0b1;
    PIE1bits.TMR2IE = 0b1;

    // Enable periferic and global int
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
};

void interrupt INT_vInterruptHandler()
{
    if(PIR1bits.ADIF && ADCON0bits.ADON)
    {
        ADC_vConversionCompleted();
    }

    if(PIR1bits.TMR2IF && T2CONbits.TMR2ON)
    {
        SCHED_vTickFunctionCall();
        PIR1bits.TMR2IF = 0b0;
    }
};

