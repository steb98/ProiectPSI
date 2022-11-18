#include "tasks.h"
#include "demo.h"
#include "lights.h"

void TASK_vTaskAppInit()
{
    LIGHTS_BlinkersInit();
    //LIGHTS_DemoInit();
}

void TASK_vTask1ms()
{
    LIGHTS_BlinkersRun();
    //LIGHTS_DemoRun();
}

void TASK_vTask10ms()
{
}

void TASK_vTask100ms()
{
}

void TASK_vTask1s()
{
}