#include "pcs.h"

void PCS_vInit(void)
{
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;

    TRISCbits.RC0 = 1;
    TRISCbits.RC1 = 1;
    TRISCbits.RC2 = 1;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;

    TRISD = 0x00;

    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
};

void PCS_vSetOutputPortD(const T_U8 u8BitNumber, const T_U8 u8Value)
{
    T_U8 u8BitValue = u8Value>0 ? 1 : 0; //bool value
    T_U8 u8BitNumberValue = u8BitNumber<=7 ? u8BitNumber : 7; // Limit range of bit

    if(1 == u8BitValue)
    {
        PORTD |= (0x01<<u8BitNumberValue);
    }
    else
    {
        PORTD &= ~(0x01<<u8BitNumberValue);
    }
}

void PCS_vSetOutputPortE(const T_U8 u8BitNumber, const T_U8 u8Value)
{
    T_U8 u8BitValue = u8Value>0 ? 1 : 0; // bool value
    T_U8 u8BitNumberValue = u8BitNumber<=1 ? u8BitNumber : 1; // Limit range of bit

    if(1 == u8BitValue)
    {
        PORTE |= (0x01<<u8BitNumberValue);
    }
    else
    {
        PORTE &= ~(0x01<<u8BitNumberValue);
    }
}

T_U16 PCS_T16GetInputPortC(const T_U8 u8BitNumber)
{
    T_U8 u8BitNumberValue = u8BitNumber<=4 ? u8BitNumber : 4; // Limit range of bit
    return (PORTC & 0x01<<u8BitNumberValue)>>u8BitNumberValue;
}
