#include "sgl.h"

/**Private variables**/

s_BlinkerSM blinkerSM;

/**Private functions**/

void SGL_BlinkPasiveState();
void SGL_BlinkSwitchOnState();
void SGL_BlinkSwitchOffState();
void SGL_BlinkEventPoll();

/**Utility Functions**/

void SGL_toggleAllHazardLights(T_U8* toggle)
{
    IOC_vSetOutputPort(IOC_SFD, *toggle);
    IOC_vSetOutputPort(IOC_SFS, *toggle);
    IOC_vSetOutputPort(IOC_SSD, *toggle);
    IOC_vSetOutputPort(IOC_SSS, *toggle);
    *toggle = *toggle ^ 1;
}

void SGL_toggleSideHazardLights(T_U8* toggle, T_U8 side)
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

void SGL_setAllHazardLights(T_U8 value)
{
    IOC_vSetOutputPort(IOC_SFD, value);
    IOC_vSetOutputPort(IOC_SFS, value);
    IOC_vSetOutputPort(IOC_SSD, value);
    IOC_vSetOutputPort(IOC_SSS, value); 
}

void SGL_setSideHazardLights(T_U8 value, T_U8 side)
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


/**State machine related functions**/

void SGL_BlinkPasiveState()
{
    static T_U16 counter = 0;
    static T_U8 firstEntry = 1;
    static T_U8 toggleLights = 0;

    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        SGL_setAllHazardLights(0);
        blinkerSM.firstEntry = 0;
    }
    
    // State run
    if(1 == blinkerSM.avarie)
    {
        if(1 == firstEntry)
        {
            SGL_setAllHazardLights(1);
            firstEntry = 0;
        }
        if(500 == counter)
        {
            SGL_toggleAllHazardLights(&toggleLights);
            counter = 0;
        }
        else
        {
            counter++;
        }
    }
    else
    {
        SGL_setAllHazardLights(0);
        counter = 0;
        firstEntry = 1;
        toggleLights = 0;
    }
    
    // Transition check
    if( ((1 == blinkerSM.leftSwitch) || (1 == blinkerSM.rightSwitch)) && 0 == blinkerSM.avarie )
    {
        blinkerSM._currentState = SGL_BlinkSwitchOnState;
        blinkerSM.firstEntry = 1;
        counter = 0;
        toggleLights = 0;
        SGL_setAllHazardLights(0);
    }
    
}

void SGL_BlinkSwitchOnState()
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
        blinkerSM.lastSide = side;
        blinkerSM.firstEntry = 0;
        counter = blinkerSM.carryCounter;
    }
    
    // State run
    if(1 == firstEntry)
    {
        SGL_setSideHazardLights(1, side);
        firstEntry = 0;
    }
    if(500 == counter)
    {
        SGL_toggleSideHazardLights(&toggleLights, side);
        counter = 0;
    }
    else
    {
        counter++;
    }
    
    // Transition check
    if( 1 == blinkerSM.avarie )
    {
        blinkerSM._currentState = SGL_BlinkPasiveState;
        blinkerSM.firstEntry = 1;
        counter = 0;
        toggleLights = 0;
        firstEntry = 1;
        SGL_setAllHazardLights(0);
    }else if( (0 == blinkerSM.avarie) && (0 == blinkerSM.leftSwitch) && (0 == blinkerSM.rightSwitch) )
    {
        blinkerSM._currentState = SGL_BlinkSwitchOffState;
        blinkerSM.firstEntry = 1;
        blinkerSM.lastToggle = toggleLights;
        blinkerSM.carryCounter = counter;
        counter = 0;
        toggleLights = 0;
        firstEntry = 1;
        //SGL_setAllHazardLights(0);
    }

}

void SGL_BlinkSwitchOffState()
{
    static T_U16 counter = 0;
    static T_U16 side = 0;
    static T_U8 toggleLights = 0;
    static T_U8 cycles = 0;
    
    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        blinkerSM.firstEntry = 0;
        //SGL_setAllHazardLights(0);
        toggleLights = blinkerSM.lastToggle;
        side = blinkerSM.lastSide;
        counter = blinkerSM.carryCounter;
    }
    
    // State run
    if(500 == counter && (5 != (cycles+blinkerSM.lastToggle)))
    {
        SGL_toggleSideHazardLights(&toggleLights, side);
        cycles++;
        counter = 0;
    }
    else
    {
        if((5 != (cycles+blinkerSM.lastToggle)))
        {
            counter++;   
        }
    }
    
    // Transition check
    if( 1 == blinkerSM.avarie )
    {
        blinkerSM._currentState = SGL_BlinkPasiveState;
        blinkerSM.firstEntry = 1;
        blinkerSM.carryCounter = counter;
        cycles = 0;
        counter = 0;
    } else if( (1 == blinkerSM.leftSwitch) || (1 == blinkerSM.rightSwitch) )
    {
        blinkerSM._currentState = SGL_BlinkSwitchOnState;
        blinkerSM.firstEntry = 1;
        blinkerSM.carryCounter = counter;
        cycles = 0;
        counter = 0;
    }
}

void SGL_BlinkEventPoll()
{
    blinkerSM.avarie = IOC_T16GetInputPort(IOC_AVARII);
    blinkerSM.leftSwitch = IOC_T16GetInputPort(IOC_SEM_S);
    blinkerSM.rightSwitch = IOC_T16GetInputPort(IOC_SEM_D);
}

void SGL_BlinkersInit()
{
    blinkerSM._currentState = SGL_BlinkPasiveState;
    blinkerSM._pollEvents = SGL_BlinkEventPoll;
    blinkerSM.avarie = 0;
    blinkerSM.firstEntry = 1;
    blinkerSM.leftSwitch = 0;
    blinkerSM.rightSwitch = 0;
    blinkerSM.lastSide = 0;
    blinkerSM.lastToggle = 0;
    blinkerSM.carryCounter = 0;
}

void SGL_BlinkersRun()
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
