/*
 * GY521MPU6050Gyroscope.h
 *
 *  Created on: January 24, 2024
 *      Author: song
 */

#ifndef GY521MPU6050GYROSCOPE_H_
#define GY521MPU6050GYROSCOPE_H_




//File: GY521MPU6050Gyroscope.c
//Jianjian Song
//Date 1-24-2024
/******************************************************************************
 * MSP432 Lab Exercise 6-3 - eUSCI_B0 I2C Master at 100kbps using BRCLK = 3MHz
 *
 * Description: This example connects MSP432 via the I2C bus to MPU6050.
 *      SMCLK/ DCO is used as a clock source at default 3MHz. The USCI_B0
 *      TX and RX interrupt are used to know when the transmit buffer may be
 *      filled or when new data has been received.
 *
 *      This demonstrates how to implement an I2C master transmitter sending
 *      bytes followed by a repeated start, followed by a read of multiple
 *      bytes. The master sends a register adress and then reads 6 bytes from
 *      the peripheral. The data from the peripheral transmitter begins at
 *      the register adress and increments with each transfer.
 *
 *      This is a common operation for reading register values from I2C
 *      peripheral devices such as sensors. The transaction for the I2C
 *      that is written looks as follows:
 *  ______________________________________________________________________________
 *  |       |         |               |       |         |                 |      |
 *  | Start | Addr  W | <1 Byte Send> | Start | Addr  R |  <6 Byte Read>  | Stop |
 *  |_______|_________|_______________|_______|_________|_________________|______|
 *
 *                                  ___  ___
 *                                   |    |
 *               MSP432P411x        10k  10k     MPU6050
 *             ------------------    |    |    -----------
 *         /|\|     P1.6/UCB0SDA |<--|----|-->| SDA
 *          | |                  |   |        |
 *          --|RST               |   |        |
 *            |     P1.7/UCB0SCL |<--|------->| SCL
 *            |                  |            |
 *
*******************************************************************************/
#define I2Cport P1
#define SCLPIN  BIT7
#define SDAPIN  BIT6

#define NUM_OF_REC_BYTES        6       // number of bytes to receive from sensor read
/* TODO update peripheral address - 0b110100X */
#define GY521_ADDRESS           0x68    //AD0=0v; 0b1101000 I2C address of GY-521 sensor
/* TODO update register addresses   */
#define ACCEL_BASE_ADDR    0x3B         // base address of accelerometer data registers
#define PWR_MGMT_ADDR           0x6B    // address of power management register
#define ACCEL_CONFIG    0x1C

uint8_t RXDataPointer, TXDataPointer;

volatile int16_t accel_x, accel_y, accel_z;

double accel_x_g, accel_y_g, accel_z_g, fullscale1g;



float calculateRoll(float accel_x, float accel_y, float accel_z);
float calculatePitch(float accel_x, float accel_y, float accel_z);

void ConfigureGY521MPU6050(void);

//write to an internal register at RegisterAddress to configure its value with RegisterValue
void ConfigureI2CDeviceRegister(char RegisterAddress, char RegisterValue);

void StartAccelReading(void);

#endif /* GY521MPU6050GYROSCOPE_H_ */
