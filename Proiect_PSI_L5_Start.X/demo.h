#ifndef DEMO_H_
#define DEMO_H_
#include "ioc.h"

typedef struct s_stateMachine
{
    void (*_currentState)(void);
    void (*_pollEvents)(void);
    T_BOOL firstEntry;
    T_U16 toggle;
} s_SM;

void LIGHTS_DemoInit();
void LIGHTS_DemoRun();

#endif