#ifndef LIGHTS_H_
#define LIGHTS_H_
#include "ioc.h"

typedef struct s_HeadLightStateMachine
{
    void (*_currentState)(void);
    void (*_pollEvents)(void);
    T_BOOL firstEntry;
    T_U8 faruri;
    T_U16 lightLevel;
} s_HeadLightSM;

void LIGHTS_BrakeInit();
void LIGHTS_BrakeRun();

void LIGHTS_HeadLampInit();
void LIGHTS_HeadLampRun();

#endif