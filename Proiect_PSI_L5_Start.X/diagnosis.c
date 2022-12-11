#include "diagnosis.h"
#include "ioc.h"

T_U16 g_lightStateFrana = 0;
T_U16 g_lightStateLumini = 0;
T_U16 g_sglStareSemnal = 0;

void DIAGNOSIS_ActivateError()
{
    T_U16 sensorValue = IOC_T16GetInputPort(IOC_ADC1);
    
    if(g_lightStateFrana || g_lightStateLumini || g_sglStareSemnal)
    {
        if(sensorValue > (4.5*ADC_SCALLING_FACTOR))
        {
            IOC_vSetOutputPort(IOC_IE, 1);
        }
        else
        {
            IOC_vSetOutputPort(IOC_IE, 0);
        }
    }
    else
    {
        IOC_vSetOutputPort(IOC_IE, 0);
    }
}
