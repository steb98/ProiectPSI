#include "adc.h"

/**
 * Name: ADC value buffer
 * Type: T_U16
 */
T_U16 au16AdcReadValue[2] = {0, 0};

void ADC_vInit()
{
    ADRESH = 0x00;
    ADRESL = 0x00;

    ADCON0bits.CHS = 0b0000; // AN0

    ADCON1bits.VCFG = 0b00;
    ADCON1bits.PCFG = 0b1101;

    ADCON2bits.ADFM = 1; // Right justified
    ADCON2bits.ACQT = 0b100;
    ADCON2bits.ADCS = 0b001;

    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    ADCON0bits.ADON = 1; // Enable ADC
    __delay_us(40); // Time to charge the cap
    ADCON0bits.GO_nDONE = 1; // Enable convertion
};

/**
 * Name: ADC_vConversionCompleted
 * Description: Function called when ADC conversion
 * is finnished. The result is stored in au16AdcReadValue.
 * Parameters: None
 * Return: void
 */
void ADC_vConversionCompleted()
{
    au16AdcReadValue[ADCON0bits.CHS0] = (ADRESH << 8) | ADRESL;
    ADCON0bits.CHS0 = !ADCON0bits.CHS0;
    PIR1bits.ADIF = 0;
    ADCON0bits.GO_nDONE = 1; // Enable convertion
};

T_U16 ADC_u16ReadAdcValue(const T_U8 U8Channel)
{
    T_U8 U8ChannelValue = U8Channel<=1 ? U8Channel : 1;
    return au16AdcReadValue[U8ChannelValue];
}