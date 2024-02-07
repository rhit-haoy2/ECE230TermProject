/*
 * Author: Yejia Hao
 * 02/07/2024
 */

#include "msp.h"
#include "GY521MPU6050Gyroscope.h"
#include "csLFXT.h"
#include "csHFXT.h"
#include <math.h>
#include <stdio.h>

extern volatile int16_t accel_x, accel_y, accel_z;
double accel_x_g, accel_y_g, accel_z_g, fullscale1g;

enum
{
    g2, g4, g8, g16
} gFullScale = g8;  //2g, 4g, 8g and 16g mapped t0 0, 1,2,3

#define One_G_Full_Scale    (pow(2,14-gFullScale))  //1g = 2^14 = 16384

long i;
float roll_angle, pitch_angle;

/*
 * Input: float accel_x, float accel_y, float accel_z
 * The unit and scale of three-axis acceleration value doesn't matter, whether they
 * are in g-scale or raw data from GY521, as long as the unit or scale of the three
 * axis of acceleration are the same
 *
 * Output: float roll_angle
 * The roll angle in unit of degrees
 */
float calculateRoll(float accel_x, float accel_y, float accel_z){
    float roll_angle = atan2(accel_y, sqrt(accel_x*accel_x + accel_z*accel_z));

    roll_angle = roll_angle * (180/M_PI);

    return roll_angle;
}


/*
 * Input: float accel_x, float accel_y, float accel_z
 * The unit and scale of three-axis acceleration value doesn't matter, whether they
 * are in g-scale or raw data from GY521, as long as the unit or scale of the three
 * axis of acceleration are the same
 *
 * Output: float pitch_angle
 * The pitch angle in unit of degrees
 */
float calculatePitch(float accel_x, float accel_y, float accel_z){
    float pitch_angle = atan2(accel_x, sqrt(accel_y*accel_y + accel_z*accel_z));

    pitch_angle = pitch_angle * (180/M_PI);

    return pitch_angle;
}
/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    configHFXT();
    configLFXT();

    ConfigureGY521MPU6050();

    fullscale1g =  (pow(2,14-gFullScale));

    while (1){
        for (i=0; i<1000000; i++){

        }

        StartAccelReading();


        accel_x_g = ((float) accel_x) / fullscale1g;
        accel_y_g = ((float) accel_y) / fullscale1g;
        accel_z_g = ((float) accel_z) / fullscale1g;
        printf("\r\n   accel_x = %d", accel_x); //print on Console
        printf("   accel_y = %d", accel_y);
        printf("   accel_z = %d", accel_z);
        printf("\r\n   accel_x_g =  %.4lf", accel_x_g);
        printf("   accel_y_g =  %.4lf", accel_y_g);
        printf("   accel_z_g =  %.4lf", accel_z_g);

        roll_angle = calculateRoll(accel_x_g, accel_y_g, accel_z_g);
        pitch_angle = calculatePitch(accel_x_g, accel_y_g, accel_z_g);

        printf ("\r\n  roll = %.4lf", roll_angle);
        printf ("  pitch = %.4lf", pitch_angle);


    }


}
