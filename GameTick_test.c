#include "msp.h"
#include "TimerA3GameTickInterrupt.h"

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    initTimerA3GameTickInterrupt();

    while(1){

    }
}
