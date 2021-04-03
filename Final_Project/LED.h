// LED.h
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/06/21
// LED initializations and data function prototypes 
// Final Project
// Blake Margolis
// 03/11/21


#include <stdint.h>


// 2 LEDs on PE1,0
#define PE0 (*((volatile uint32_t *)0x40024004))
#define PE1 (*((volatile uint32_t *)0x40024008))


void LED_Init(void);
void SetGreenLED(uint8_t value);
void SetRedLED(uint8_t value);

