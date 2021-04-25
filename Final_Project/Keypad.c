// Keypad.c
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
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
//#include "Keypad.h"
//#include "Lab5.h"

#define PB0 (*((volatile uint32_t *)0x40005004))
#define PB1 (*((volatile uint32_t *)0x40005008))
#define PB2 (*((volatile uint32_t *)0x40005010))
#define PB3 (*((volatile uint32_t *)0x40005020))
#define PB4 (*((volatile uint32_t *)0x40005040))
#define PB5 (*((volatile uint32_t *)0x40005080))
#define PB6 (*((volatile uint32_t *)0x40005100))	

//====================
// global variables
//====================
uint8_t row;
uint8_t character = 0;
uint8_t lastPressed = 0;
uint8_t pressed = 0;

void (*TypeKey)(char);

const char characterpadMatrix[4][3] = {
    {'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
    {'*', '0', '#'}
};


//void GPIOArm(void){
//  GPIO_PORTB_ICR_R = 0x7F;      // (e) clear flag
//  GPIO_PORTB_IM_R |= 0x7F;      // (f) arm interrupts
//  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFF00FFF)|0x00002000; // (g) priority 1
//  NVIC_EN0_R = 0x00000002;      // (h) enable interrupt 1 in NVIC	
//}


//===============================
// Timer0A_Init
//-------------------------------
// Iterating through each row
// inputs: none
// outputs: none
//===============================
void Timer0A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x01;      // 0) activate timer0
  TIMER0_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER0_CFG_R = 0x00000000;       // 2) configure for 32-bit timer mode
  TIMER0_TAMR_R = 0x00000002;      // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = 4999999;        // 4) reload value
  TIMER0_TAPR_R = 0;               // 5) 12.5ns timer0A
  TIMER0_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER0_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|(3<<29); 
  NVIC_EN0_R = 0x00080000;     // 9) enable interrupt 19 in NVIC
  TIMER0_CTL_R |= 0x00000001;      // 10) enable timer0A
}

void GPIOPortB_Init(void(*typeKey)(char)) {
	row = 0;
	SYSCTL_RCGCGPIO_R |= 0x00000002;
	while((SYSCTL_RCGCGPIO_R & 0x00000002) == 0) {};
		
	TypeKey = typeKey;	

	GPIO_PORTB_CR_R = 0x7F;				// allow changes to PB6-0
	GPIO_PORTB_AMSEL_R &= ~0x7F;	// disable analog functionality	
	GPIO_PORTB_DIR_R = 0x2B;			// make PB0,PB1,PB3,PB5, outputs for rows and PB2,PB4,PB6 inputs for columns
	GPIO_PORTB_DEN_R = 0x7F;			// enable digital I/O on PB6-0
	GPIO_PORTB_PCTL_R &= ~0xF0000000; // configure PB6-0 as GPIO
	
	Timer0A_Init();
}

//==========================================
// Timer0A_Handler
//------------------------------------------
// Checks each row if a button is pressed
// inputs: none
// outputs: none
//==========================================
void Timer0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
  GPIO_PORTF_DATA_R ^= 0x04;	
	
//	PB3 = 0x08;
//	
//	if (PB4 & 0x10){
//		TypeKey('1');
//	} else if (PB6 & 0x40) {
//		TypeKey('2');
//	} else if (PB2 & 0x04) {
//		TypeKey('3');
//	}
//	
//	PB0 = 0;
//	PB1 = 0;
//	PB3 = 0;
//	PB5 = 0;
	
	switch (row){
		case 0:
			PB5 = 0x20;
		  PB0 = 0;
		  PB1 = 0;
		  PB3 = 0;
		
			if (PB4 & 0x10) {
				character = '1';
				pressed = 1;
			} else if (PB6 & 0x40) {
				character = '2';
				pressed = 1;
			}
		  else if (PB2 & 0x04) {
				character = '3';
				pressed = 1;
			} else {
				pressed = 0;
			}
			
		  break;
		case 1:
			PB0 = 0x01;
		  PB1 = 0;
		  PB3 = 0;
		  PB5 = 0;
		
			if (PB4 & 0x10) {
				character = '4';
				pressed = 1;
			} else if (PB6 & 0x40) {
				character = '5';
				pressed = 1;
			}
		  else if (PB2 & 0x04) {
				character = '6';
				pressed = 1;
			} else {
				pressed = 0;
			}
			
		  break;
		case 2:
			PB1 = 0x02;
		  PB0 = 0;
		  PB3 = 0;
		  PB5 = 0;
		
			if (PB4 & 0x10) {
				character = '7';
				pressed = 1;
			} else if (PB6 & 0x40) {
				character = '8';
				pressed = 1;
			}
		  else if (PB2 & 0x04) {
				character = '9';
				pressed = 1;
			} else {
				pressed = 0;
			}
			
		  break;
		case 3:
			PB3 = 0x08;
		  PB0 = 0;
		  PB1 = 0;
		  PB5 = 0;
		
			if (PB4 & 0x10) {
				character = '*';
				pressed = 1;
			} else if (PB6 & 0x40) {
				character = '0';
				pressed = 1;
			}
		  else if (PB2 & 0x04) {
				character = '#';
				pressed = 1;
			} else {
				pressed = 0;
			}
			
		  break;
	}
	
	if (pressed == 0 ){ 
	  row = (row + 1)%5;
		lastPressed = 0;
	} else if (pressed == 1 && lastPressed == 0){
		TypeKey(character);
		lastPressed = 1;
	} 
}

//char scanColumns(int row){
//	// Col 1
//	if(GPIO_PORTB_RIS_R & 0x10) { // PB4
//		return characterpadMatrix[row-1][0];
//	}
//	// Col 2
//	else if(GPIO_PORTB_RIS_R & 0x40) { // PB6
//		return characterpadMatrix[row-1][1];
//	}
//	// Col 3
//	else if(GPIO_PORTB_RIS_R & 0x04) { // PB2
//		return characterpadMatrix[row-1][2];
//	}
//	return '\0';
//}

//char scan(void) {
//	// Row 1
//	if(GPIO_PORTB_RIS_R & 0x20) { // PB5
//		return scanColumns(1);
//	}
//	// Row 2
//	else if(GPIO_PORTB_RIS_R & 0x01) { // PB0
//		return scanColumns(2);
//	}
//	// Row 3
//	else if(GPIO_PORTB_RIS_R & 0x02) { // PB1
//		return scanColumns(3);
//	}
//	// Row 4
//	else if(GPIO_PORTB_RIS_R & 0x08) { // PB3
//		return scanColumns(4);
//	}
//	return '\0';
//}

//void GPIOPortB_Handler(void) {
//	GPIO_PORTB_IM_R &= ~0x7F;
//	recordKey(scan());
//	Timer0A_Init();
//}


//char scanKeypad(void){
//	return '\0';
//}



