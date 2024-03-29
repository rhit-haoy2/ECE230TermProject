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

    enum collision_result_t collision;

    configHFXT();
    configLFXT();

    initGame();
    generateGame();
    initStrip();


    printGame();
    renderGame();

    moveBallDown();
    moveBallRight();


    moveBallDown();
    printGame();
    renderGame();

    moveBallDown();
    printGame();
    renderGame();

    moveBallDown();
    printGame();
    renderGame();

    collision = collisionHandler();
    printf("/r/n Collision result: %d",collision);

    moveBallLeft();
    printGame();
    renderGame();

    moveBallLeft();
    printGame();
    renderGame();

    moveBallLeft();
    printGame();
    renderGame();

    moveBallLeft();
    printGame();
    renderGame();


    while (1){

    }


}
