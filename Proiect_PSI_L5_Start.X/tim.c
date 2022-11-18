#include "tim.h"

void TIM_vInit()
{
    T2CONbits.TMR2ON = 0b0;
    TMR2 = 0x00;
    T2CON = 0x00;
    PR2 = 0x00;

    T2CONbits.TOUTPS = 0b0100;  // PostScaler 5
    T2CONbits.T2CKPS = 0b01;    // Prescaler 4
    PR2 = 125;                  // Value to compare

    PIR1bits.TMR2IF = 0b0;
    IPR1bits.TMR2IP = 0b1;
    PIE1bits.TMR2IE = 0b1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    T2CONbits.TMR2ON = 0b1;
};
