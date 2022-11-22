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

void SGL_setAllHazardLights(T_U8* toggle)
{
    IOC_vSetOutputPort(IOC_SFD, *toggle);
    IOC_vSetOutputPort(IOC_SFS, *toggle);
    IOC_vSetOutputPort(IOC_SSD, *toggle);
    IOC_vSetOutputPort(IOC_SSS, *toggle); 
    *toggle = *toggle ^ 1;
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
    static T_U8 firstEntry = 1;

    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        blinkerSM.firstEntry = 0;
    }
    
    // State run
    if(1 == blinkerSM.pollAvarie)
    {
        if(1 == firstEntry)
        {
            SGL_setAllHazardLights(blinkerSM.globalToggle);
            firstEntry = 0;
        }
        if(500 == blinkerSM.globalCounter)
        {
            SGL_toggleAllHazardLights(&blinkerSM.globalToggle);
            blinkerSM.globalCounter = 0;
        }
        else
        {
            blinkerSM.globalCounter++;
        }
    }
    else
    {
        if((firstEntry == 0) && blinkerSM.globalToggle == 0)
        {
            SGL_setAllHazardLights(&blinkerSM.globalToggle);
        }
        firstEntry = 1;
    }
    
    // Transition check
    if( ((1 == blinkerSM.pollLeftSwitch) || (1 == blinkerSM.pollRightSwitch)) && 0 == blinkerSM.pollAvarie )
    {
        blinkerSM._currentState = SGL_BlinkSwitchOnState;
        blinkerSM.firstEntry = 1;
        SGL_setAllHazardLights(0);
    }
    
}

void SGL_BlinkSwitchOnState()
{
    static T_U8 side = 0;
    static T_U8 firstEntry = 1;
    
    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        // right side is off, that means left is on in this state
        side = blinkerSM.pollRightSwitch;
        blinkerSM.holdSwitch = side; // prepare this variable for OffState
        blinkerSM.firstEntry = 0;
    }
    
    // State run
    if(1 == firstEntry)
    {
        SGL_setSideHazardLights(blinkerSM.globalToggle, side);
        firstEntry = 0;
    }
    if(500 == blinkerSM.globalCounter)
    {
        SGL_toggleSideHazardLights(&blinkerSM.globalToggle, side);
        blinkerSM.globalCounter = 0;
    }
    else
    {
        blinkerSM.globalCounter++;
    }
    
    // Transition check
    if( 1 == blinkerSM.pollAvarie )
    {
        blinkerSM._currentState = SGL_BlinkPasiveState;
        blinkerSM.firstEntry = 1;
        firstEntry = 1;
        SGL_setAllHazardLights(0);
    }else if( (0 == blinkerSM.pollAvarie) && (0 == blinkerSM.pollLeftSwitch) && (0 == blinkerSM.pollRightSwitch) )
    {
        blinkerSM._currentState = SGL_BlinkSwitchOffState;
        blinkerSM.firstEntry = 1;
        firstEntry = 1;
        //SGL_setAllHazardLights(0);
    }

}

void SGL_BlinkSwitchOffState()
{
    static T_U8 counterRuns = 0;
    static T_U8 extraToggle = 0;

    // State entry
    if(blinkerSM.firstEntry == 1)
    {
        blinkerSM.firstEntry = 0;
        if(blinkerSM.globalToggle == 0)
        {
            extraToggle = 1;
        }
    }

    
    // State run
    if(counterRuns <= (9+extraToggle))
    {
        if(blinkerSM.globalCounter == 500)
        {
            SGL_toggleSideHazardLights(&blinkerSM.globalToggle, blinkerSM.holdSwitch);
            blinkerSM.globalCounter = 0;
            counterRuns++;
        }
        else
        {
            blinkerSM.globalCounter++;
        }
    }
    else
    {
        blinkerSM.globalCounter = 0;
    }
    
    // Transition check
    if( 1 == blinkerSM.pollAvarie )
    {
        blinkerSM._currentState = SGL_BlinkPasiveState;
        blinkerSM.firstEntry = 1;
        counterRuns = 0;
        extraToggle = 0;
    } else if( (1 == blinkerSM.pollLeftSwitch) || (1 == blinkerSM.pollRightSwitch) )
    {
        blinkerSM._currentState = SGL_BlinkSwitchOnState;
        blinkerSM.firstEntry = 1;
        counterRuns = 0;
        extraToggle = 0;
    }
}

void SGL_BlinkEventPoll()
{
    blinkerSM.pollAvarie = IOC_T16GetInputPort(IOC_AVARII);
    blinkerSM.pollLeftSwitch = IOC_T16GetInputPort(IOC_SEM_S);
    blinkerSM.pollRightSwitch = IOC_T16GetInputPort(IOC_SEM_D);
}

void SGL_BlinkersInit()
{
    blinkerSM._currentState = SGL_BlinkPasiveState;
    blinkerSM._pollEvents = SGL_BlinkEventPoll;
    blinkerSM.pollAvarie = 0;
    blinkerSM.firstEntry = 1;
    blinkerSM.pollLeftSwitch = 0;
    blinkerSM.pollRightSwitch = 0;
    blinkerSM.holdSwitch = 0;
    blinkerSM.globalToggle = 1;
    blinkerSM.globalCounter = 0;
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
