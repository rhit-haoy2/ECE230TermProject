//SPIsubroutines_JJS.c
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
#include "SPIsubroutines_JJS.h"

void ConfigureSPI_USCI_A1(void)
{
//define pins for primary mode function
    SPI_PORT->DIR |= CLK_PIN;   //output
    SPI_PORT->DIR |= MOSI_PIN;   //output
    SPI_PORT->DIR &= ~STE_PIN;   //input
    SPI_PORT->SEL0 |= CLK_PIN | MOSI_PIN | STE_PIN;   //primary mode
    SPI_PORT->SEL1 &=~(CLK_PIN | MOSI_PIN | STE_PIN);   //primary mode

//    EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
//    48000000,                                   // SMCLK = DCO = 48MHZ
//    5,714,285,                                    // SPICLK = 5,714,285Hz
//    EUSCI_B_SPI_MSB_FIRST,                     // MSB First
//    EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
//    EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
//    EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
    /* Configuring SPI in 3wire master mode */

    EUSCI_A1->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset to configure eUSCI
//bit 15 = 0 capture on 2nd edge; bit14=0 inactive low ; bit13=1 for MSB first;
//bit12=0 for 8-bit mode; bit11=1 for MASTER mode; bits 10-9=00 for 3-pin SPI
//bit8=1 synchronous mode; bits7-6 = 0b10 or 11 for SMCLK; bit1=0 STE used to prevent conflict
    EUSCI_A1->CTLW0 |= 0b0010100110000000; //0x2980

    EUSCI_A1->BRW=USBR_VALUE;
    /* Enable SPI module */
    EUSCI_A1->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
}

//blocking call
void SPIwriteByte_USCI_A1(char WriteData) {
    /* Polling to see if the TX buffer is ready */
    while (EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG != EUSCI_A_IFG_TXIFG);
    EUSCI_A1->TXBUF = WriteData;
} //end SPIwriteBytes


uint8_t SPIreadByte_USCI_A1(void) {
    uint8_t RXdata;
    /* Polling to see if the RX buffer is ready */
    /* Polling to see if the TX buffer is ready */
    while (EUSCI_A1->IFG & EUSCI_A_IFG_RXIFG != EUSCI_A_IFG_TXIFG);
    RXdata = EUSCI_A1->RXBUF;
    return RXdata;
}

//Use External 48MHz oscillator
//Set MCLK at 48 MHz for CPU
//Set SMCLK at 48 MHz
//Set ACLK at 32kHz
void SetClocks(void) {
    configHFXT();
    configLFXT();
}
