//File Name: spi_ws2812.c
//Source: https://github.com/mjmeli/MSP430-NeoPixel-WS2812-Library.
//Jianjian Song
//Date: Feb-11-2023

 /* Note that in this example, EUSCI_A1 is used for the SPI port. If the user
 * wants to use EUSCIA1 for SPI operation, they are able to with the same APIs
 * with the EUSCI_AX parameters.
 *
 * ACLK = ~32.768kHz, MCLK = SMCLK = DCO 48MHz
 *
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
 */
#include <msp.h>
#include "spi_ws2812.h"
#include "SPIsubroutines_JJS.h"

// WS2812 takes GRB format
typedef struct {
    u_char green;
    u_char red;
    u_char blue;
} LED;

static LED leds[NUM_LEDS] = { { 0, 0, 0 } };



// Initializes everything needed to use this library. This clears the strip.
void initStrip() {
    ConfigureSPI_USCI_A1();
    clearStrip();           // clear the strip
}

// Sets the color of a certain LED (0 indexed)
void setLEDColor(u_int p, u_char r, u_char g, u_char b) {
	leds[p].green = g/BIRGHTNESS_DEDUCTION;
    leds[p].red = r/BIRGHTNESS_DEDUCTION;
    leds[p].blue = b/BIRGHTNESS_DEDUCTION;
}

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip() {
    __disable_irq();  // disable interrupts

    // send RGB color for every LED
    unsigned int i, j;
    for (i = 0; i < NUM_LEDS; i++) {
        u_char *rgb = (u_char *)&leds[i]; // get GRB color for this LED

        // send green, then red, then blue
        for (j = 0; j < 3; j++) {
            u_char mask = 0x80;    // b1000000

            // check each of the 8 bits
//SPIwriteByte() is a blocking call
            while (mask != 0) {
                if (rgb[j] & mask) {        // most significant bit first
//this is 0.5us faster than calling SPIwriteByte(HIGH_CODE)
                    while (EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG != EUSCI_A_IFG_TXIFG);
                    EUSCI_A1->TXBUF = HIGH_CODE;
//                   SPIwriteByte_USCI_A1(HIGH_CODE);  // send 1
                } else {
//                    SPIwriteByte_USCI_A1(LOW_CODE);;   // send 0
                    while (EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG != EUSCI_A_IFG_TXIFG);
                    EUSCI_A1->TXBUF = LOW_CODE;
                }

                mask >>= 1;  // check next bit
            }
        }
    }

    // send RES code for at least 50 us (800 cycles at 16 MHz)
    _delay_cycles(800);

    __enable_irq();   // enable interrupts
}

// Clear the color of all LEDs (make them black/off)
void clearStrip() {
    fillStrip(0x00, 0x00, 0x00);  // black
}

// Fill the strip with a solid color. This will update the strip.
void fillStrip(u_char r, u_char g, u_char b) {
    int i;
    for (i = 0; i < NUM_LEDS; i++) {
        setLEDColor(i, r, g, b);  // set all LEDs to specified color
    }
    showStrip();  // refresh strip
}

void gradualFill(u_int n, u_char r, u_char g, u_char b){
    int i;
    for (i = 0; i < n; i++){        // n is number of LEDs
        setLEDColor(i, r, g, b);
        showStrip();
        delay_cycles(UPDATE_INTERVAL);       // lazy delay
    }
}

void delay_cycles(unsigned int count) {
unsigned int index;
    for(index=0; index<count; index++) { }
}


