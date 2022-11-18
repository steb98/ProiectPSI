#ifndef LIGHTS_H_
#define LIGHTS_H_
#include "ioc.h"

typedef struct s_blinkStateMachine
{
    void (*_currentState)(void);
    void (*_pollEvents)(void);
    T_BOOL firstEntry;
    T_U8 avarie;
    T_U8 leftSwitch;
    T_U8 rightSwitch;
} s_BlinkerSM;

void LIGHTS_BlinkersInit();
void LIGHTS_BlinkersRun();

#endif