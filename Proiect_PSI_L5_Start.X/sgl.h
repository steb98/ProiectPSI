#ifndef SGL_H_
#define SGL_H_
#include "ioc.h"

typedef struct s_blinkStateMachine
{
    void (*_currentState)(void);
    void (*_pollEvents)(void);
    T_BOOL firstEntry;
    T_U8 pollAvarie;
    T_U8 pollLeftSwitch;
    T_U8 pollRightSwitch;
    T_U8 holdSwitch;
    T_U8 globalToggle;
    T_U16 globalCounter;
} s_BlinkerSM;

void SGL_BlinkersInit();
void SGL_BlinkersRun();

#endif