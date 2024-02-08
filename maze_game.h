/*
 * maze_game.h
 *
 *  Created on: Feb 7, 2024
 *      Author: Yejia Hao
 */

#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_

#include <stdio.h>
#include <math.h>


#define GAME_ROW 10
#define GAME_COL 5

enum pixel_t {NULL_PIXEL, WALL, PATH, BALL};

enum pixel_t game_board[GAME_ROW][GAME_COL];

enum direction_t {STAY, LEFT, RIGHT, UP, DOWN};

struct ball_pos_t{
    int x;
    int y;
};

struct ball_pos_t ball_position;

void initGame(void);

void printGame(void);
void moveBall(enum direction_t direction);
void moveBallUp(void);
void moveBallDown(void);
void moveBallLeft(void);
void moveBallRight(void);




#endif /* MAZE_GAME_H_ */
