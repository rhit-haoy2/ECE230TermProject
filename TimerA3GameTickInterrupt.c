//File Name: TimerA3GameTickInterrupt.c
// Generate interrupt for global game tick


#include <msp.h>
#include "TimerA3GameTickInterrupt.h"
#include "ece230TermProject.h"

#define CLK_FREQ 32000   //ACLK at 32kHz
#define GAME_TICK_PERIOD 500 // In unit of ms, the interval between updates of game board
#define LED1port    P1
#define LED1pin BIT0

void initTimerA3GameTickInterrupt(void) {
    //blinking LED
    LED1port->DIR |= LED1pin;
    LED1port->SEL0 &= ~LED1pin;
    LED1port->SEL0 &= ~LED1pin;
    LED1port->OUT |=LED1pin;
    /* Configure Timer_A3 and CCR0 */
    // Set period of Timer_A3 in CCR0 register for Up Mode
    TIMER_A3->CCR[0] = GAME_TICK_PERIOD*CLK_FREQ/1000;
    // TODO configure CCR0 for Compare mode with interrupt enabled (no output mode - 0)

    // Configure CCR0 for Compare mode with interrupt enabled (no output mode - 0)
     // TODO configure CCR0
     TIMER_A3->CCTL[0]=0x0010;  //enable CCR0 compare interrupt
    // Configure Timer_A3 in UP Mode, with ACLK, prescale 1:1, and
    //  interrupt disabled
     TIMER_A3->CTL=0b00000000100010100; //0x0114 bits9-8=0b01 ACLK, bits5-4=0b01 UP MODE, NO INTERRUPT

    /* Configure global interrupts and NVIC */
    // Enable TA3CCR0 compare interrupt by setting IRQ bit in NVIC ISER0 register
         NVIC->ISER[0] |= (1)<<TA3_0_IRQn;
}

void haltGameTickTimer(void){
    TIMER_A3->CTL &= ~(BIT4 | BIT5); // Timer A3 in stop mode
}

void restartGameTickTimer(void){
    TIMER_A3->CTL |= (BIT4 ); // Timer A3 in up mode
    TIMER_A3->R = 0x00; // Clear timer A3

}
// Timer A3 CCR0 interrupt service routine
void TA3_0_IRQHandler(void)
{
    /* Not necessary to check which flag is set because only one IRQ
     *  mapped to this interrupt vector     */
    if(TIMER_A3->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        LED1port->OUT ^=LED1pin;

        game_update_flag = true;
    // TODO clear timer compare flag in TA3CCTL0
        TIMER_A3->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  //clear interrupt flag
    }
}
