// Keypad.c
// Amy Lee
// 03/06/21
// Keypad initializations and data function implementations 
// PB6 - col2
// PB5 - row1
// PB4 - col1
// PB3 - row4
// PB2 - col3
// PB1 - row3
// PB0 - row2
// Final Project
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"

void GPIOArm(void){
  GPIO_PORTB_ICR_R = 0x7F;      // (e) clear flag
  GPIO_PORTB_IM_R |= 0x7F;      // (f) arm interrupts
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFF00FFF)|0x00002000; // (g) priority 1
  NVIC_EN0_R = 0x00000002;      // (h) enable interrupt 1 in NVIC	
}

void GPIOPortB_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x00000002;
	while((SYSCTL_RCGCGPIO_R & 0x00000002) == 0) {};

	GPIO_PORTB_CR_R = 0x7F;				// allow changes to PB6-0
	GPIO_PORTB_AMSEL_R &= ~0x7F;	// disable analog functionality	
	GPIO_PORTB_DIR_R = 0x0;			// make PB6-0 inputs
	GPIO_PORTB_DEN_R = 0x7F;			// enable digital I/O on PB7-3
	GPIO_PORTB_PCTL_R &= ~0xF0000000; // configure PB6-0 as GPIO
  GPIO_PORTB_IS_R &= ~0x7F;     // (d) PB6-0 is edge-sensitive
  GPIO_PORTB_IBE_R &= ~0x7F;    //     PB6-0 is not both edges
  GPIO_PORTB_IEV_R |= 0x7F;     //     PB6-0 rising edge event
  GPIO_PORTB_ICR_R = 0x7F;      // (e) clear flag6-0
	GPIO_PORTB_IM_R |= 0x7F;      // (f) arm interrupt on PB6-0

	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFF00FFF)|0x00002000; // (g) priority 1
  NVIC_EN0_R = 0x00000002;      // (h) enable interrupt 1 in NVIC	
}

//===============================
// Timer0A_Init
//-------------------------------
// Timer for de-bouncing switches
// inputs: none
// outputs: none
//===============================
void Timer0A_Init(void){
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x0000001;    // 3) 1-SHOT mode
  TIMER0_TAILR_R = 16000000;      // 4) 10ms reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
}

//==========================================
// Timer0A_Handler
//------------------------------------------
// Interrupt 10 ms after rising edge of
// switches (PF0, PF4, PC4)
// inputs: none
// outputs: none
//==========================================
void Timer0A_Handler(void){
  TIMER0_IMR_R = 0x00000000; 	
  GPIOArm(); 
}

char scan(void) {
	if(GPIO_PORTB_RIS_R & 0x80) {    // if SW5 pressed, change mode
		GPIO_PORTB_IM_R &= ~0x80;
	}
	if(GPIO_PORTB_RIS_R & 0x04) {    // if SW4 pressed, set alarm
		GPIO_PORTB_IM_R &= ~0x04;
	}
	if(GPIO_PORTB_RIS_R & 0x20) {    // if SW3 pressed, increment minute
		GPIO_PORTB_IM_R &= ~0x20;
	}
	if(GPIO_PORTB_RIS_R & 0x10) {    // if SW2 pressed, increment hour
		GPIO_PORTB_IM_R &= ~0x10;
	}
	if(GPIO_PORTB_RIS_R & 0x08) {    // if SW1 pressed, turn off alarm or start stopwatch
		GPIO_PORTB_IM_R &= ~0x08;
	}
	                // one-shot timer initialization for debouncing
}

void GPIOPortB_Handler(void) {
	GPIO_PORTB_IM_R &= ~0x7F;
	scan();
	Timer0A_Init();
}