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
******************************************************************************/
#include <msp.h>
#include <stdint.h>
#include <math.h>
#include "GY521MPU6050Gyroscope.h"




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

void ConfigureGY521MPU6050(void) {

    /* Configure UART pins */
    I2Cport->SEL0 |= SCLPIN | SDAPIN;                // set I2C pins as secondary function
    I2Cport->SEL1 &= ~(SCLPIN | SDAPIN);
    // Initialize data variable
    RXDataPointer = 0;
    TXDataPointer = 0;

    /* Configure eUSCI_B0 for I2C mode
     *  I2C master mode, synchronous, 7-bit address, SMCLK clock source,
     *  transmit mode, with automatic STOP condition generation
     */
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset mode
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // Master mode
            EUSCI_B_CTLW0_SYNC |            // Sync mode
            EUSCI_B_CTLW0_TR |              // Transmitter mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK at 48MHz default clock

    /* I2C clock calculation
     * Refer to Section 26.3.6 of Technical Reference manual
     * BRCLK = 48MHz, I2C bit clock rate = 100kbps
    */
    // TODO configure eUSCI_B0 bit rate control for 100 kbps
    EUSCI_B0->BRW = 480;  //UCBRx Baud rate value

    /* Configure I2C to communicate with GY-521 */
    EUSCI_B0->I2CSA = GY521_ADDRESS;            // I2C peripheral address
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // Release eUSCI from reset

    /* Initialize GY-521 by writing to Power Management Register
     *
     *  format for Write operations
     *  _________________________________________________________________
     *  |       |          |                 |                  |       |
     *  | Start |  Addr  W | <Register Addr> | <Value to write> | Stop  |
     *  |_______|__________|_________________|__________________|_______|
     */
    // Ensure stop condition not pending
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    do {
        // Send I2C start condition and address frame with W
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
        // wait for TX buffer to be ready
        while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
        // load 1st data byte into TX buffer
        EUSCI_B0->TXBUF = PWR_MGMT_ADDR;            // send register address
        // wait for ACK/NACK after address frame
        while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);
    } while(EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG);   // resend address frame if ACK not received
    // wait for TX buffer to be ready
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    // load 2nd data byte into TX buffer
    EUSCI_B0->TXBUF = 0;                // write value to register
    // wait for 2nd data byte to begin transmit
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    // Send I2C stop condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

    // Ensure stop condition got sent
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    // ensure flags are cleared before enabling interrupts
    EUSCI_B0->IFG &= ~(EUSCI_B_IFG_TXIFG0 | EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_NACKIFG);

    EUSCI_B0->IE |= EUSCI_A_IE_RXIE |       // Enable receive interrupt
            EUSCI_A_IE_TXIE |               // Enable transmit interrupt
            EUSCI_B_IE_NACKIE;              // Enable NACK interrupt

    // Enable eUSCIB0 interrupt in NVIC module
    NVIC->ISER[0] = (1 << EUSCIB0_IRQn);

} //end ConfigureGY521MPU6050


//write to an internal register at RegisterAddress
//to configure its value with RegisterValue
void ConfigureI2CDeviceRegister(char RegisterAddress, char RegisterValue) {

    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    EUSCI_B0->IE &= ~(EUSCI_A_IE_RXIE |       // Disable receive interrupt
    EUSCI_A_IE_TXIE |               // Disable transmit interrupt
    EUSCI_B_IE_NACKIE);              // Disable NACK interrupt

       do {
           // Send I2C start condition and address frame with W
           EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
           // wait for TX buffer to be ready
           while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
           // load 1st data byte into TX buffer
           EUSCI_B0->TXBUF = RegisterAddress;            // send register address
           // wait for ACK/NACK after address frame
           while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);
       } while(EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG);   // resend address frame if ACK not received
       // wait for TX buffer to be ready
       while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
       // load 2nd data byte into TX buffer
       EUSCI_B0->TXBUF = RegisterValue;                // write value to register
       // wait for 2nd data byte to begin transmit
       while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
       // Send I2C stop condition
       EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

       // Ensure stop condition got sent
       while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
       // ensure flags are cleared before enabling interrupts
       EUSCI_B0->IFG &= ~(EUSCI_B_IFG_TXIFG0 | EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_NACKIFG);

    EUSCI_B0->IE |= EUSCI_A_IE_RXIE |       // Enable receive interrupt
            EUSCI_A_IE_TXIE |               // Enable transmit interrupt
            EUSCI_B_IE_NACKIE;              // Enable NACK interrupt
}

//Start the reading process to cause interrupts
void StartAccelReading(void) {
        // Ensure stop condition got sent
        while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);

        /* Read register values from sensor by sending register address and restart
         *
         *  format for Write-Restart-Read operation
         *  _______________________________________________________________________
         *  |       | Periph | <Register |       | Periph |               |       |
         *  | Start |  Addr  |  Address> | Start |  Addr  | <6 Byte Read> | Stop  |
         *  |_______|____W___|___________|_______|____R___|_______________|_______|
         *
         *
         *  Initiated with start condition - completion handled in ISR
         */
        // change to transmitter mode (Write) to start I2C communication interrupt
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;
        // send I2C start condition with address frame and W bit
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
} //end StartAccelReading()

// I2C interrupt service routine
void EUSCIB0_IRQHandler(void)
{
static uint8_t RXData[NUM_OF_REC_BYTES] = {0, 0, 0, 0, 0, 0};
    // Handle if ACK not received for address frame
    if (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG) {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_NACKIFG;

        // resend I2C start condition and address frame
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
        TXDataPointer = 0;
        RXDataPointer = 0;
    }
    // When TX buffer is ready, load next byte or Restart for Read
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0) {
        if (TXDataPointer == 0) {
            // load 1st data byte into TX buffer (writing to buffer clears the flag)
            EUSCI_B0->TXBUF = ACCEL_BASE_ADDR;      // send register address
            TXDataPointer = 1;
        } else {
            // change to receiver mode (Read)
            EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TR;   //read operation after ACCEL_BASE_ADDR is sent
            // send Restart and address frame with R bit
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT; //restart to transmit
            TXDataPointer = 0;
            RXDataPointer = 0;
            // need to clear flag since not writing to buffer
            EUSCI_B0->IFG &= ~ EUSCI_B_IFG_TXIFG0;
        }
    }
    // When new byte is received, read value from RX buffer
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0) {
        // Get RX data
        if (RXDataPointer < NUM_OF_REC_BYTES) {
            // reading the buffer clears the flag
            RXData[RXDataPointer++] = EUSCI_B0->RXBUF;
        }
        else {  // in case of glitch, avoid array out-of-bounds error
            EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG0;
        }

        // check if last byte being received - if so, initiate STOP (and NACK)
        if (RXDataPointer == (NUM_OF_REC_BYTES-1)) {
            // Send I2C stop condition
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

            accel_x = (RXData[0]<<8)+RXData[1];
            accel_y = (RXData[2]<<8)+RXData[3];
            accel_z = (RXData[4]<<8)+RXData[5];

        }
    }
}
