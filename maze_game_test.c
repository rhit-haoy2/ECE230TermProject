/*
 * Author: Yejia Hao
 * 02/07/2024
 */

#include "msp.h"
#include "maze_game.h"
#include "csHFXT.h"
#include "csLFXT.h"
#include <stdio.h>

enum pixel_t ex_game_board[10][5] = {{WALL, WALL, WALL, WALL, WALL}, \
                                        {WALL, WALL, WALL, WALL, WALL}, \
                                        {WALL, WALL, WALL, WALL, WALL}, \
                                        {WALL, WALL, WALL, PATH, WALL}, \
                                        {WALL, WALL, PATH, PATH, WALL}, \
                                        {WALL, WALL, PATH, WALL, WALL}, \
                                        {WALL, WALL, PATH, WALL, WALL}, \
                                        {WALL, WALL, PATH, WALL, WALL}, \
                                        {WALL, WALL, PATH, WALL, WALL}, \
                                        {WALL, WALL, PATH, WALL, WALL} };

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    configHFXT();
    configLFXT();


    printGame(ex_game_board, 10, 5);

    while (1){

    }


}
