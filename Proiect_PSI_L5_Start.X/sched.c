#include "sched.h"
#include "adc.h"
#include "pcs.h"
#include "int.h"
#include "tim.h"
#include "tasks.h"

T_U8 sched_gTick10ms;
T_U8 sched_gTick100ms;
T_U16 sched_gTick1s;
T_BOOL sched_bTicked;

void SCHED_vInitSystem()
{
    PCS_vInit();
    INT_vInit();
    ADC_vInit();
    TIM_vInit();
}

void SCHED_vTickFunctionCall()
{
    ++sched_gTick10ms;
    ++sched_gTick100ms;
    ++sched_gTick1s;
    sched_bTicked = TRUE;
}

void SCHED_vStartScheduler()
{
    sched_gTick10ms = 0;
    sched_gTick100ms = 0;
    sched_gTick1s = 0;
    sched_bTicked = FALSE;

    TASK_vTaskAppInit();
    
    while(1)
    {
        if(sched_bTicked)
        {
            sched_bTicked = FALSE;
            TASK_vTask1ms();
            
            if(sched_gTick10ms == 10)
            {
                sched_gTick10ms = 0;
                TASK_vTask10ms();
            }
            if(sched_gTick100ms == 100)
            {
                sched_gTick100ms = 0;
                TASK_vTask100ms();
            }
            if(sched_gTick1s == 1000)
            {
                sched_gTick1s = 0;
                TASK_vTask1s();
            }
        }
    }
}
