#include "lights.h"

s_BlinkerSM blinkerSM;

void LIGHTS_BlinkPasiveState();
void LIGHTS_BlinkSwitchOnState();
void LIGHTS_BlinkSwitchOffState();
void LIGHTS_BlinkEventPoll();


void LIGHTS_toggleAllHazardLights(T_U8* toggle)
{
    IOC_vSetOutputPort(IOC_SFD, *toggle);
    IOC_vSetOutputPort(IOC_SFS, *toggle);
    IOC_vSetOutputPort(IOC_SSD, *toggle);
    IOC_vSetOutputPort(IOC_SSS, *toggle);
    *toggle = *toggle ^ 1;
}

void LIGHTS_toggleSideHazardLights(T_U8* toggle, T_U8 side)
{
    if(0 == side)
    {
        IOC_vSetOutputPort(IOC_SFS, *toggle);
        IOC_vSetOutputPort(IOC_SSS, *toggle);
    }
    else
    {
        IOC_vSetOutputPort(IOC_SFD, *toggle);
        IOC_vSetOutputPort(IOC_SSD, *toggle);
    }
    *toggle = *toggle ^ 1;
}

void LIGHTS_setAllHazardLights(T_U8 value)
{
    IOC_vSetOutputPort(IOC_SFD, value);
    IOC_vSetOutputPort(IOC_SFS, value);
    IOC_vSetOutputPort(IOC_SSD, value);
    IOC_vSetOutputPort(IOC_SSS, value); 
}

void LIGHTS_setSideHazardLights(T_U8 value, T_U8 side)
{
    if(0 == side)
    {
        IOC_vSetOutputPort(IOC_SFS, value);
        IOC_vSetOutputPort(IOC_SSS, value);
    }
    else
    {
        IOC_vSetOutputPort(IOC_SFD, value);
        IOC_vSetOutputPort(IOC_SSD, value);
    }
}



void LIGHTS_BlinkPasiveState()
{
    static T_U16 counter = 0;
    static T_U8 firstEntry = 1;
    static T_U8 toggleLights = 0;

    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        LIGHTS_setAllHazardLights(0);
        blinkerSM.firstEntry = 0;
    }
    
    // State run
    if(1 == blinkerSM.avarie)
    {
        if(1 == firstEntry)
        {
            LIGHTS_setAllHazardLights(1);
            firstEntry = 0;
        }
        if(500 == counter)
        {
            LIGHTS_toggleAllHazardLights(&toggleLights);
            counter = 0;
        }
        else
        {
            counter++;
        }
    }
    else
    {
        LIGHTS_setAllHazardLights(0);
        counter = 0;
        firstEntry = 1;
        toggleLights = 0;
    }
    
    // Transition check
    if( ((1 == blinkerSM.leftSwitch) || (1 == blinkerSM.rightSwitch)) && 0 == blinkerSM.avarie )
    {
        blinkerSM._currentState = LIGHTS_BlinkSwitchOnState;
        blinkerSM.firstEntry = 1;
        counter = 0;
        toggleLights = 0;
        LIGHTS_setAllHazardLights(0);
    }
    
}

void LIGHTS_BlinkSwitchOnState()
{
    static T_U8 side = 0;
    static T_U16 counter = 0;
    static T_U8 firstEntry = 1;
    static T_U8 toggleLights = 0;
    
    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        // right side is off, that means left is on in this state
        side = blinkerSM.rightSwitch;
        blinkerSM.firstEntry = 0;
    }
    
    // State run
    if(1 == firstEntry)
    {
        LIGHTS_setSideHazardLights(1, side);
        firstEntry = 0;
    }
    if(500 == counter)
    {
        LIGHTS_toggleSideHazardLights(&toggleLights, side);
        counter = 0;
    }
    else
    {
        counter++;
    }
    
    // Transition check
    if( 1 == blinkerSM.avarie )
    {
        blinkerSM._currentState = LIGHTS_BlinkPasiveState;
        blinkerSM.firstEntry = 1;
        counter = 0;
        toggleLights = 0;
        firstEntry = 1;
        LIGHTS_setAllHazardLights(0);
    }else if( (0 == blinkerSM.avarie) && (0 == blinkerSM.leftSwitch) && (0 == blinkerSM.rightSwitch) )
    {
        blinkerSM._currentState = LIGHTS_BlinkSwitchOffState;
        blinkerSM.firstEntry = 1;
        counter = 0;
        toggleLights = 0;
        firstEntry = 1;
        LIGHTS_setAllHazardLights(0);
    }

}

void LIGHTS_BlinkSwitchOffState()
{
    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        blinkerSM.firstEntry = 0;
        LIGHTS_setAllHazardLights(0);
    }
    
    // State run
    
    // Transition check
    if( 1 == blinkerSM.avarie )
    {
        blinkerSM._currentState = LIGHTS_BlinkPasiveState;
        blinkerSM.firstEntry = 1;
    } else if( (1 == blinkerSM.leftSwitch) || (1 == blinkerSM.rightSwitch) )
    {
        blinkerSM._currentState = LIGHTS_BlinkSwitchOnState;
        blinkerSM.firstEntry = 1;
    }
}

void LIGHTS_BlinkEventPoll()
{
    blinkerSM.avarie = IOC_T16GetInputPort(IOC_AVARII);
    blinkerSM.leftSwitch = IOC_T16GetInputPort(IOC_SEM_S);
    blinkerSM.rightSwitch = IOC_T16GetInputPort(IOC_SEM_D);
}

void LIGHTS_BlinkersInit()
{
    blinkerSM._currentState = LIGHTS_BlinkPasiveState;
    blinkerSM._pollEvents = LIGHTS_BlinkEventPoll;
    blinkerSM.avarie = 0;
    blinkerSM.firstEntry = 1;
    blinkerSM.leftSwitch = 0;
    blinkerSM.rightSwitch = 0;
}
void LIGHTS_BlinkersRun()
{
    if(0 != blinkerSM._currentState)
    {
        (*blinkerSM._currentState)();
    }
    if(0 != blinkerSM._pollEvents)
    {
        (*blinkerSM._pollEvents)();
    }
}
