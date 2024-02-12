#include "msp.h"
#include "csHFXT.h"
#include "GY521MPU6050Gyroscope.h"
#include "spi_ws2812.h"
#include "maze_game.h"
#include "TimerA3GameTickInterrupt.h"

enum game_status_t {GAME_START, GAME_PLAYING, GAME_WIN, GAME_LOSE};

enum game_status_t global_game_status;

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	enum game_status_t global_game_status = GAME_START;

	while (1){
	        switch (global_game_status){
	        case GAME_START:
	            // Do something ...
	            break;
	        case GAME_PLAYING:
	            // Do something ...
	            break;
	        case GAME_WIN:
	            // Do something ...
	            break;
	        case GAME_LOSE:
	            // Do something ...
	            break;
	        default:
	            // Do something ...
	            break;
	        }
	    }
}
