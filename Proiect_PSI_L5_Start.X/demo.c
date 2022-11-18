#include "demo.h"

s_SM demoSM;
void LIGHTS_DemoState1();
void LIGHTS_DemoState2();
void LIGHTS_DemoEventPoll();

void LIGHTS_DemoRun()
{
    if(0 != demoSM._currentState)
    {
        (*demoSM._currentState)();
    }
    if(0 != demoSM._pollEvents)
    {
        (*demoSM._pollEvents)();
    }
}

void LIGHTS_DemoState1()
{
    if(demoSM.firstEntry == 1)
    {
        IOC_vSetOutputPort(IOC_FD, 1);
        demoSM.firstEntry = 0;
    }

    IOC_vSetOutputPort(IOC_FS, 1);
    
    if(1 == demoSM.toggle)
    {
        demoSM._currentState = &LIGHTS_DemoState2;
        demoSM.firstEntry = 1;
        IOC_vSetOutputPort(IOC_FS, 0);
        IOC_vSetOutputPort(IOC_FD, 0);
    }
}

void LIGHTS_DemoState2()
{
    if(demoSM.firstEntry == 1)
    {
        IOC_vSetOutputPort(IOC_SSD, 1);
        demoSM.firstEntry = 0;
    }
    
    IOC_vSetOutputPort(IOC_SSS, 1);
    
    if(0 == demoSM.toggle)
    {
        demoSM._currentState = &LIGHTS_DemoState1;
        demoSM.firstEntry = 1;
        IOC_vSetOutputPort(IOC_SSD, 0);
        IOC_vSetOutputPort(IOC_SSS, 0);
    }
}

void LIGHTS_DemoEventPoll()
{
    demoSM.toggle = IOC_T16GetInputPort(IOC_FRANA);
}

void LIGHTS_DemoInit()
{
    demoSM._currentState = &LIGHTS_DemoState1;
    demoSM._pollEvents = &LIGHTS_DemoEventPoll;
    demoSM.firstEntry = 1;
    demoSM.toggle = 0;
}