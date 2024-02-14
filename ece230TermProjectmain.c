
#include "ece230TermProject.h"


enum game_status_t global_game_status;

bool game_update_flag = false;

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	enum game_status_t global_game_status = GAME_START;

	enum collision_result_t collision;

	    float roll_angle, pitch_angle;

//	    enum
//	    {
//	        g2, g4, g8, g16
//	    } gFullScale = g8;  //2g, 4g, 8g and 16g mapped t0 0, 1,2,3

	    configHFXT();
	    configLFXT();

	    initGame();
	    generateGame();
	    initStrip();

	    initTimerA3GameTickInterrupt();

	    ConfigureGY521MPU6050();





	while (1){
	        switch (global_game_status){
	        case GAME_START:
	            // Do something ...

	            StartAccelReading();
//	            accel_x_g = ((float) accel_x) / fullscale1g;
//                accel_y_g = ((float) accel_y) / fullscale1g;
//                accel_z_g = ((float) accel_z) / fullscale1g;
	            calculateGAccel();


	            printf("\r\n  Accel Meg = %.4lf", calculateGAccelMagnitude());

	            if (calculateGAccelMagnitude() > 1.5){
	                global_game_status = GAME_PLAYING;
	            }
	            else{
	                playStartEffect();
	            }




	            break;
	        case GAME_PLAYING:
	            // Do something ...

	            while (! game_update_flag);

	            StartAccelReading();


//                accel_x_g = ((float) accel_x) / fullscale1g;
//                accel_y_g = ((float) accel_y) / fullscale1g;
//                accel_z_g = ((float) accel_z) / fullscale1g;

//                roll_angle = calculateRoll(accel_x_g, accel_y_g, accel_z_g);
//                pitch_angle = calculatePitch(accel_x_g, accel_y_g, accel_z_g);


	            calculateGAccel();

	            if (calculateGAccelMagnitude() > ACCEL_THRESHOLD){
	                break;
	            }
                roll_angle = calculateRoll();
                pitch_angle = calculatePitch();

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

                //printGame();
                renderGame();
                game_update_flag = false;

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
