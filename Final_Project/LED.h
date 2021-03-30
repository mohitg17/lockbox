// LED.h
// Amy Lee
// 03/06/21
// LED initializations and data function prototypes 
// Final Project
// Blake Margolis
// 03/11/21

// 2 LEDs on PE1,0
#define PE0 (*((volatile uint32_t *)0x40024004))
#define PE1 (*((volatile uint32_t *)0x40024008))

void LED_Init(void);



