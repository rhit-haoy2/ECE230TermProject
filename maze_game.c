#include "maze_game.h"


void initGame(void){
    ball_position.x = 0;
    ball_position.y = 0;

    int i,j;
    // Example game board for test
    for (i = 0; i < GAME_ROW; i++) {
        for (j = 0; j < GAME_COL; j++) {
            if (i == 3 && j == 3) {
                game_board[i][j] = PATH; // Example: Set the ball's initial position
            } else {
                game_board[i][j] = WALL; // Set other positions to WALL as an example
            }
        }
    }

}

void printGame(void){

    int i, j;

    printf("\r\n Game board:");
    for (i = 0; i < GAME_ROW; i++){
        printf("\r\n");

        for (j=0; j < GAME_COL; j++){
            if (i == ball_position.y && j == ball_position.x){
                    printf("O");
                }
            else{
                switch(game_board[i][j]){
                case NULL_PIXEL: printf(" "); break;
                case WALL: printf("="); break;
                case PATH: printf("."); break;
                default: printf(" ");
                }
            }

        }

    }
    printf("\r\n");

}

void renderGame(void){
    int i,j;
    u_int p = 0;

    for (i = 0; i < GAME_ROW; i++){
        for (j = 0; j < GAME_COL; j++){
            p++;

            if (i == ball_position.y && j == ball_position.x){
                setLEDColor(p, 0xFF, 0x00, 0xFF);
                }
            else{
                switch(game_board[i][j]){
                case NULL_PIXEL: setLEDColor(p, 0x0, 0x00, 0x00);  break;
                case WALL: setLEDColor(p, 0xFF, 0x00, 0x00);  break;
                case PATH: setLEDColor(p, 0x00, 0xFF, 0x00);  break;
                default: setLEDColor(p, 0x00, 0x00, 0x00);
                }
            }

        }
    }

    showStrip();
}

void moveBall(enum direction_t direction){
    switch (direction){
    case STAY:
        break;
    case LEFT:
        ball_position.x = ball_position.x-1;
        break;
    case RIGHT:
        ball_position.x = ball_position.x+1;
        break;
    case UP:
        ball_position.y = ball_position.y-1;
        break;
    case DOWN:
        ball_position.y = ball_position.y+1;
        break;
    }

    // Boundary check
    if (ball_position.x < 0){
        ball_position.x =  GAME_COL + ball_position.x;
    }
    else if (ball_position.x > GAME_COL){
        ball_position.x = ball_position.x % GAME_COL;
    }

    if (ball_position.y < 0){
        ball_position.y = GAME_ROW + ball_position.y;
    }
    else if (ball_position.y > GAME_ROW){
        ball_position.y = ball_position.y % GAME_COL;
    }
}

void moveBallUp(void){
    moveBall(UP);
}

void moveBallDown(void){
    moveBall(DOWN);
}

void moveBallLeft(void){
    moveBall(LEFT);
}
void moveBallRight(void){
    moveBall(RIGHT);
}

