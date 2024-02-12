/*
 * ece230TermProject.h
 *
 *  Created on: Feb 12, 2024
 *      Author: haoy2
 */

#ifndef ECE230TERMPROJECT_H_
#define ECE230TERMPROJECT_H_


#include "msp.h"
#include "csHFXT.h"
#include "GY521MPU6050.h"
#include "spi_ws2812.h"
#include "maze_game.h"
#include "TimerA3GameTickInterrupt.h"

enum game_status_t {GAME_START, GAME_PLAYING, GAME_WIN, GAME_LOSE};

enum game_status_t global_game_status;

#endif /* ECE230TERMPROJECT_H_ */
