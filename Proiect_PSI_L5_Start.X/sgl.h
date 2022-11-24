#ifndef SGL_H_
#define SGL_H_
#include "ioc.h"

typedef struct s_blinkStateMachine
{
    void (*_currentState)(void);
    void (*_pollEvents)(void);
    T_BOOL firstEntry;
    T_U8 avarie;
    T_U8 leftSwitch;
    T_U8 rightSwitch;
    T_U8 lastSide;
    T_U8 lastToggle;
    T_U8 carryCounter;
} s_BlinkerSM;

void SGL_BlinkersInit();
void SGL_BlinkersRun();

#endif