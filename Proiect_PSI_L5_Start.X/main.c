/*
 * File:   main.c
 * Author: Boghean Stefan
 */
#include "ioc.h"
#include "sched.h"
#include "RS232.h"

void main(void)
{
    RS232_vInit();
    RS232_vSendMessage("Init system lumini!\n\r");
    SCHED_vInitSystem();
    SCHED_vStartScheduler();
}
