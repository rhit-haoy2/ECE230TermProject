//File Name: spi_ws2812.h

#ifndef SPI_WS2812_H_
#define SPI_WS2812_H_

#define NUM_LEDS    (50)    // NUMBER OF LEDS IN YOUR STRIP

// Useful typedefs
typedef unsigned char u_char;	// 8 bit
typedef unsigned int u_int;     // 16 bit

// Transmit codes
//at 1.4us SPI period for logic 1 of 0.7us and logic 0 of 0.35us
//#define HIGH_CODE   (0b11110000)      // 0.7us high and 0.7us low for Logic 1
//#define LOW_CODE    (0b11000000)      // 0.35us high and 1.05us low for Logic 0

//after calibration with software delay, the SPI period is 0.8us
//at 0.8us SPI period for logic 1 of 0.7us and logic 0 of 0.35us
#define HIGH_CODE   (0b11111110)      // 0.7us high and 0.1us low for Logic 1
#define LOW_CODE    (0b11100000)      // 0.35us high and 1.05us low for Logic 0

//#define HIGH_CODE   (0b11111100)      // 0.7us high and 0.1us low for Logic 1
//#define LOW_CODE    (0b11000000)      // 0.35us high and 1.05us low for Logic 0

// Configure processor to output to data strip
void initStrip(void);

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip(void);

// Set the color of a certain LED
void setLEDColor(u_int p, u_char r, u_char g, u_char b);

// Clear the color of all LEDs (make them black/off)
void clearStrip(void);

// Fill the strip with a solid color. This will update the strip.
void fillStrip(u_char r, u_char g, u_char b);

void gradualFill(u_int n, u_char r, u_char g, u_char b);

//#define UPDATE_INTERVAL 1000000 //1,000,000 = ~1 second
#define UPDATE_INTERVAL 100000 //1,000,000 = ~0.5 second
void delay_cycles(unsigned int count);

#endif //SPI_WS2812_H_
