#ifndef ADC_H_
#define ADC_H_
#include "global.h"

/**
 * Name: ADC_vInit
 * Description: Initialize ADC used in this project
 * Parameters: None
 * Return: void
 */
void ADC_vInit(void);

void ADC_vConversionCompleted();

/**
 * Name: ADC_u16ReadAdcValue
 * Description: Returns the last value of ADC conversion
 * from internal buffer
 * Parameters: U8Channel[in] - ADC channel to read from [0-1]
 * Return: T_U16 - Value of ADC conversion
 */
T_U16 ADC_u16ReadAdcValue(const T_U8 U8Channel);

#endif
