/*
 * Author: Yejia Hao
 * 02/07/2024
 */

#include "msp.h"
#include "maze_game.h"
#include "csHFXT.h"
#include "csLFXT.h"
#include <stdio.h>



/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    configHFXT();
    configLFXT();

    initGame();
    printGame();

    moveBallDown();
    moveBallRight();


    moveBallDown();
    printGame();

    moveBallDown();
    printGame();

    moveBallDown();
    printGame();

    moveBallLeft();
    printGame();

    moveBallLeft();
    printGame();

    moveBallLeft();
    printGame();

    moveBallLeft();
    printGame();

    while (1){

    }


}
