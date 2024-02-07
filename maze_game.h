/*
 * maze_game.h
 *
 *  Created on: Feb 7, 2024
 *      Author: Yejia Hao
 */

#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_

#include <stdio.h>


#define GAME_ROW 10
#define GAME_COL 5

enum pixel_t {NULL_PIXEL, WALL, PATH, BALL};

enum pixel_t game_board[GAME_ROW][GAME_COL];




void printGame(enum pixel_t board[GAME_ROW][GAME_COL]);




#endif /* MAZE_GAME_H_ */
