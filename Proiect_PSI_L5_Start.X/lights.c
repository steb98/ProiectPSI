#include "lights.h"

#define ADC_SCALLING_FACTOR 204

void LIGHTS_BrakeInit()
{
    IOC_vSetOutputPort(IOC_LSF, 0);
}

void LIGHTS_BrakeRun()
{
    static T_U8 counter = 0;
    
    if(4 == counter)
    {
        IOC_vSetOutputPort(IOC_LSF, IOC_T16GetInputPort(IOC_FRANA));
        counter = 0;
    }
    ++counter;
}


s_HeadLightSM headLightSM;

void SGL_HeadLightEventPoll();
void SGL_HeadLightPasiveState();
void SGL_HeadLightActiveState();
T_U8 SGL_isNight();

T_U8 SGL_isNight()
{
    static T_U8 prevState = 0; // 0 meaning day
    T_U16 currentLightLevel = headLightSM.lightLevel;
    
    if(prevState == 0 && currentLightLevel < (2.2*ADC_SCALLING_FACTOR))
    {
        prevState = 1;
        return 1;
    }
    
    if(prevState == 1 && currentLightLevel > (2.4*ADC_SCALLING_FACTOR))
    {
        prevState = 0;
        return 1;
    }
    
    return prevState;
}

void SGL_HeadLightPasiveState()
{
    // State entry
    if(headLightSM.firstEntry == 1)
    {
        IOC_vSetOutputPort(IOC_IFS, 0);
        IOC_vSetOutputPort(IOC_LS, 0);
        headLightSM.firstEntry = 0;
    }
    
    // State run
    if(SGL_isNight())
    {
        IOC_vSetOutputPort(IOC_IFS, 1);
    }
    else
    {
        IOC_vSetOutputPort(IOC_IFS, 0);
    }
    
    // State transit
    if(1 == headLightSM.faruri)
    {
        headLightSM.firstEntry = 1;
        headLightSM._currentState = SGL_HeadLightActiveState;
        IOC_vSetOutputPort(IOC_IFS, 0);
    }
}

void SGL_HeadLightActiveState()
{
    // State entry
    if(headLightSM.firstEntry == 1)
    {
        IOC_vSetOutputPort(IOC_LS, 1);
        IOC_vSetOutputPort(IOC_FS, 1);
        IOC_vSetOutputPort(IOC_FD, 1);
        headLightSM.firstEntry = 0;
    }
    
    // State run
    
    //State transit
    if(0 == headLightSM.faruri)
    {
        headLightSM._currentState = SGL_HeadLightPasiveState;
        headLightSM.firstEntry = 1;
        IOC_vSetOutputPort(IOC_LS, 0);
        IOC_vSetOutputPort(IOC_FS, 0);
        IOC_vSetOutputPort(IOC_FD, 0);
    }
    
}

void SGL_HeadLightEventPoll()
{
    headLightSM.faruri = IOC_T16GetInputPort(IOC_FARURI);
    headLightSM.lightLevel = IOC_T16GetInputPort(IOC_ADC0);
}

void LIGHTS_HeadLampInit()
{
    headLightSM._currentState = SGL_HeadLightPasiveState;
    headLightSM._pollEvents = SGL_HeadLightEventPoll;
    headLightSM.firstEntry = 1;
    headLightSM.faruri = 0;
    headLightSM.lightLevel = 0;
}

void LIGHTS_HeadLampRun()
{
    if(0 != headLightSM._currentState)
    {
        (*headLightSM._currentState)();
    }
    if(0 != headLightSM._pollEvents)
    {
        (*headLightSM._pollEvents)();
    }
}
