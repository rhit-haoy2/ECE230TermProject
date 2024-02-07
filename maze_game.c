#include "maze_game.h"



void printGame(enum pixel_t board[GAME_ROW][GAME_COL]){

    int i, j;

    printf("\r\n Game board:");
    for (i = 0; i < GAME_ROW; i++){
        printf("\r\n");
        for (j=0; j < GAME_COL; j++){
            switch(board[i][j]){
            case NULL_PIXEL: printf(" "); break;
            case WALL: printf("="); break;
            case PATH: printf("."); break;
            default: printf(" ");
            }

        }

    }
    printf("\r\n");

}
