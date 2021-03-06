// LED.c
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/06/21
// LED initializations and data function implementations 
// Final Project
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "LED.h"


void GPIOPortE_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10; // activate port E
	while((SYSCTL_PRGPIO_R & 0x10)==0){};
	
	// --------------- Initialize PE1,0 as GPIO --------------------------------- 
	GPIO_PORTE_DIR_R |= 0x03; // output digital I/O on PE1,0
	GPIO_PORTE_AMSEL_R &= ~0x03; // disable analog functionality on PE3,1,0
	GPIO_PORTE_AFSEL_R &= ~0x03; // disable alt funct on PE3,1,0
	GPIO_PORTE_DEN_R |= 0x03; // enable digital I/O on PE3,1,0

	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFFFFFF00);		//regular GPIO, clear wanted bits
}

void LED_Init(void){
	GPIOPortE_Init();
	PE0 = 0;
	PE1 = 0;
}

// Green = PE0
void SetGreenLED(uint8_t value){
	if(value == 0){
		PE0 = 0;
	}
	else{
		PE0 |= 0x01;
	}
}

// Red = PE1
void SetRedLED(uint8_t value){
	if(value == 0){
		PE1 = 0;
	}
	else{
		PE1 |= 0x02;
	}
}

