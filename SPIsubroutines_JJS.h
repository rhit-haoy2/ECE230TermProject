//SPIsubroutines_JJS.h
//Jianjian Song
//Feb. 10, 2023
/******************************************************************************
 * MSP432 SPI - 3-wire Master Incremented Data
 *
 * This example shows how SPI master talks to SPI slave using 3-wire mode.
 *
 * Note that in this example, EUSCIA1 is used for the SPI port. If the user
 * wants to use EUSCIA for SPI operation, they are able to with the same APIs
 * with the EUSCI_AX parameters.
 *
 * ACLK = ~32.768kHz, MCLK = SMCLK = 48MHz
 * Use with SPI Slave Data Echo code example.
 *
 *                MSP432P4111
 *              -----------------
 *             |                 |
 *             |                 |
 *             |                 |
 *             |             P2.3|-> Data Out (UCA1SIMO)
 *             |                 |
 *             |             P2.2|<- Data In (UCA1SOMI)
 *             |                 |
 *             |             P2.1|-> Serial Clock Out (UCA1CLK)
 *             |                 |
 *             |             P2.0|-> STE indicate slave on this unit is ready
 *             |                 |
 *******************************************************************************/
#include <msp.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "csHFXT.h"
#include "csLFXT.h"

//![Simple SPI Configuration
/* SPI Master Configuration Parameter with USCI_A1 */
#define SPI_PORT    P2      //eUSCI_A1
#define STE_PIN    BIT0     //input to indicate slave on this unit is ready
#define CLK_PIN    BIT1     //P2.1 is clock
#define SOMI_PIN    BIT2    //P2.2 is slave out master in
#define MOSI_PIN    BIT3    //P2.3 is master out slave in

#define SPI_MODULE_CLOCK 48000000.0    //48MHz
////5,714,285 Hz or 1.4us period for neopixel period, this is without software delay
//#define SPI_BIT_CLOCK 5714285.0    //5,714,285 Hz = 5.7MHz
//#define SPI_BIT_CLOCK 5714285.0    //5,714,285 Hz = 5.7MHz

//calibrated from software delay, the SPI period should be x+0.6=1.4us, x=1.4-0.6=0.8us
//SPI frequency is therefore 1/0.8us=12,000,000Hz
//logic 1 = 0.7us or 0b11111100
//logic 0 = 0.35us high or 0b11100000
#define SPI_BIT_CLOCK 12000000.0    //12,000,000 Hz = 12MHz

//baud rate bit clock frequency = SPI module clock/USBRx value
#define USBR_VALUE (float) SPI_MODULE_CLOCK/ (float) SPI_BIT_CLOCK

void ConfigureSPI_USCI_A1(void);

//blocking call
void SPIwriteByte_USCI_A1(char WriteData);

uint8_t SPIreadByteSPI_USCI_A1(void);

//Use External 48MHz oscillator
//Set MCLK at 48 MHz for CPU
//Set SMCLK at 48 MHz
//Set ACLK at 32kHz
void SetClocks(void);
