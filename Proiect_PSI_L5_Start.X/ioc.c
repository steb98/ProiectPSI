#include "ioc.h"
#include "pcs.h"
#include "adc.h"

void IOC_vSetOutputPort(const IOC_eOutputChannelPort u16ChannelSelect, const T_U8 u8Value)
{
    T_U16 u16ChannelMask = (u16ChannelSelect&0xFF00);

    if(u16ChannelMask == IOC_PORTD)
    {
        PCS_vSetOutputPortD(u16ChannelSelect, u8Value);
    }
    else if(u16ChannelMask == IOC_PORTE)
    {
        PCS_vSetOutputPortE(u16ChannelSelect, u8Value);
    }
}

T_U16 IOC_T16GetInputPort(const IOC_eInputChannelPort u16ChannelSelect)
{
    T_U16 pinValue = 0;
    T_U16 u16ChannelMask = (u16ChannelSelect&0xFF00);

    if(u16ChannelMask == IOC_PORTC)
    {
        pinValue = PCS_T16GetInputPortC(u16ChannelSelect);
    }
    else if(u16ChannelMask == IOC_PORTA)
    {
        pinValue = ADC_u16ReadAdcValue(u16ChannelSelect);
    }

    return pinValue;
}