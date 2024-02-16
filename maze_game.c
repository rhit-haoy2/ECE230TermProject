#include "maze_game.h"


void initGame(void){


    start_position.x = 0;
    start_position.y = 7;

    end_position.x = 3;
    end_position.y = 4;

    ball_position.x = start_position.x;
    ball_position.y = start_position.y;

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

void generateGame(void){
    ball_position.x = start_position.x;
    ball_position.y = start_position.y;

    int i,j;
    // Example game board for test
    for (i = 0; i < GAME_ROW; i++) {
        for (j = 0; j < GAME_COL; j++) {
            if ((i == 3 && j == 3) || \
                    (i == 3 && j == 4) || \
                    (i == 5 && j == 3) || \
                    (i == 6 && j == 3) || \
                    (i == 7 && j == 3) || \
                    (i == 7 && j == 2) || \
                    (i == 7 && j == 1)) {
                game_board[i][j] = PATH;
            }
            else if (i == start_position.y && j == start_position.x){
                game_board[i][j] = START;
            }
            else if (i == end_position.y && j == end_position.x){
                game_board[i][j] = END;
            }
            else {
                game_board[i][j] = WALL;
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
                case START: printf("S"); break;
                case END: printf("E"); break;
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


            if (i == ball_position.y && j == ball_position.x){
                setLEDColor(p, 0x3F, 0x00, 0x3F);
                }
            else{
                switch(game_board[i][j]){
                case NULL_PIXEL: setLEDColor(p, 0x0, 0x00, 0x00);  break;
                case WALL: setLEDColor(p, 0x3F, 0x00, 0x00);  break;
                case PATH: setLEDColor(p, 0x00, 0x3F, 0x00);  break;
                case START: setLEDColor(p, 0x00, 0x00, 0x3F);  break;
                case END: setLEDColor(p, 0x00, 0x00, 0x3F);  break;
                default: setLEDColor(p, 0x00, 0x00, 0x00);
                }

            }
            p++;

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
    else if (ball_position.x >= GAME_COL){
        ball_position.x = ball_position.x % GAME_COL;
    }

    if (ball_position.y < 0){
        ball_position.y = GAME_ROW + ball_position.y;
    }
    else if (ball_position.y >= GAME_ROW){
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

enum collision_result_t collisionHandler(void){
    enum collision_result_t result;

    switch(game_board[ball_position.y][ball_position.x]){
    case PATH:
        result = ON_PATH;
        break;
    case WALL:
        result = ON_WALL;
        break;
    case START:
        result = ON_PATH;
        break;
    case END:
        result = ON_END;
        break;
    default:
        result = ON_PATH;
        break;
    }

    return result;

}

static short r = 0x05;
static short g = 0x23;
static short b = 0x45;
static u_int count = 0;


void playStartEffect(void){
    enum pixel_t board[GAME_ROW][GAME_COL];

    if (count < 60){
        b--;
        g++;
    }
    else{
        b++;
        g--;
    }

    count++;
    count = count % 120;

    fillStrip(r,g/4,b/4);

}

void playWinEffect(void){
    gradualFill(GAME_COL*GAME_ROW, 0x00,0x23,0x23);

}

void playLoseEffect(void){
    gradualFill(GAME_COL*GAME_ROW, 0x55,0x10,0x00);

}

