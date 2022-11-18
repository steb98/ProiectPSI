#ifndef PCS_H_
#define PCS_H_

#include "global.h"

/**
 * Name: PCS_vInit
 * Description: Initialize ports used in this project
 * Parameters: None
 * Return: void
 */
void PCS_vInit(void);

/**
 * Name: PCS_vSetOutputPortD
 * Description: Sets a pin value for port D
 * Parameters: u8BitNumber[in] - selected bit [0-7]
 * Parameters: u8Value[in] - value of bit [0-1]
 * Return: void
 */
void PCS_vSetOutputPortD(const T_U8 u8BitNumber, const T_U8 u8Value);

/**
 * Name: PCS_vSetOutputPortE
 * Description: Sets a pin value for port E
 * Parameters: u8BitNumber[in] - selected bit [0-1]
 * Parameters: u8Value[in] - value of bit [0-1]
 * Return: void
 */
void PCS_vSetOutputPortE(const T_U8 u8BitNumber,const T_U8 u8Value);

/**
 * Name: PCS_T16GetInputPortC
 * Description: Get the value of a pin from port C
 * Parameters: u8BitNumber[in] - selected bit [0-4]
 * Return: T_16 - Bit value [0-1]
 */
T_U16 PCS_T16GetInputPortC(const T_U8 u8BitNumber);

#endif
