#include "tasks.h"
#include "demo.h"
#include "sgl.h"
#include "lights.h"

void TASK_vTaskAppInit()
{
    SGL_BlinkersInit();
    LIGHTS_BrakeInit();
    LIGHTS_HeadLampInit();
    
    //LIGHTS_DemoInit();
}

void TASK_vTask1ms()
{
    SGL_BlinkersRun();
    LIGHTS_BrakeRun();
    //LIGHTS_DemoRun();
}

void TASK_vTask10ms()
{
    LIGHTS_HeadLampRun();
}

void TASK_vTask100ms()
{
}

void TASK_vTask1s()
{
}