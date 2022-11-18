#ifndef IOC_H_
#define IOC_H_
#include "global.h"

typedef enum s_Port
{
    IOC_PORTC = 0x0000,
    IOC_PORTD = 0x0100,
    IOC_PORTE = 0x0200,
    IOC_PORTA = 0x0300
}IOC_ePort;

typedef enum s_OutputPort
{
    IOC_SFD = IOC_PORTD | 0x0000,
    IOC_FD = IOC_PORTD | 0x0001,
    IOC_FS = IOC_PORTD | 0x0002,
    IOC_SFS = IOC_PORTD | 0x0003,
    IOC_SSD = IOC_PORTD | 0x0004,
    IOC_LSF = IOC_PORTD | 0x0005,
    IOC_LS = IOC_PORTD | 0x0006,
    IOC_SSS = IOC_PORTD | 0x0007,

    IOC_IE = IOC_PORTE | 0x0000,
    IOC_IFS = IOC_PORTE | 0x0001
}IOC_eOutputChannelPort;

typedef enum s_InputPort
{
    IOC_ADC0 = IOC_PORTA | 0x0000,
    IOC_ADC1 = IOC_PORTA | 0x0001,

    IOC_FRANA = IOC_PORTC | 0x0000,
    IOC_FARURI = IOC_PORTC | 0x0001,
    IOC_AVARII = IOC_PORTC | 0x0002,
    IOC_SEM_D = IOC_PORTC | 0x0003,
    IOC_SEM_S = IOC_PORTC | 0x0004
    
}IOC_eInputChannelPort;

//    IOC_ADC0 = 0x0300 | 0x0000,
//    IOC_ADC1 = 0x0300 | 0x0001

/**
 * Name: IOC_vSetOutputPort
 * Description: Sets a pin value for a channel port
 * Parameters: u16ChannelSelect[in] - channel that
 * represents the port and pin number. To select one,
 * pass a IOC_eChannelPort with operator | for pin number.
 * Example: IOC_CH_PORTD|7
 * Parameters: u8Value[in] - value of bit [0-1]
 * Return: void
 */
void IOC_vSetOutputPort(const IOC_eOutputChannelPort u16ChannelSelect, const T_U8 u8Value);

/**
 * Name: IOC_T16GetInputPort
 * Description: Get a pin value from a channel port
 * Parameters: u16ChannelSelect[in] - channel that
 * represents the port and pin number. To select one,
 * pass a IOC_eChannelPort with operator | for pin number.
 * Example: IOC_CH_PORTC|7
 * Return: value of bit [0-1]
 */
T_U16 IOC_T16GetInputPort(const IOC_eInputChannelPort u16ChannelSelect);

#endif
