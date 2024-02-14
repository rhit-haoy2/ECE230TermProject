/*
 * Author: Yejia Hao
 * 02/13/2024
 */

#include "msp.h"
#include "maze_game.h"
#include "GY521MPU6050Gyroscope.h"
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

    float roll_angle, pitch_angle;

    enum
    {
        g2, g4, g8, g16
    } gFullScale = g8;  //2g, 4g, 8g and 16g mapped t0 0, 1,2,3

    configHFXT();
    configLFXT();

    initGame();
    generateGame();
    initStrip();


    ConfigureGY521MPU6050();




    while (1){
        StartAccelReading();




        roll_angle = calculateRoll(accel_x_g, accel_y_g, accel_z_g);
        pitch_angle = calculatePitch(accel_x_g, accel_y_g, accel_z_g);

        printf ("\r\n  roll = %.4lf", roll_angle);
        printf ("  pitch = %.4lf", pitch_angle);

        if (roll_angle > -90 && roll_angle < -1*TILT_THRESHOLD){
            moveBallLeft();
        }
        else if (roll_angle > TILT_THRESHOLD && roll_angle < 90){
            moveBallRight();
        }

        if (pitch_angle > -90 && pitch_angle < -1*TILT_THRESHOLD){
            moveBallUp();
        }
        else if (pitch_angle > TILT_THRESHOLD && pitch_angle < 90){
            moveBallDown();
        }

        printGame();
        renderGame();
    }


}
