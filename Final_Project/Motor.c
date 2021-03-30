// Motor.c
// Amy Lee
// 03/06/21
// Motor initializations and data function implementations 
// Final Project
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "Motor.h"

// PC4 square wave


void GPIOPortC_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x04; // Activate clock for Port C
	while((SYSCTL_PRGPIO_R & 0x04) != 0x04){}; // Allow time for clock to start

	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFFF0FFFF);		//regular GPIO, clear wanted bits
	GPIO_PORTC_AMSEL_R &= ~0x10; // disable analog function 
	GPIO_PORTC_DIR_R |= 0x10; // inputs on PC7-PC4
	GPIO_PORTC_AFSEL_R &= ~0x10; // regular port function
	GPIO_PORTC_DEN_R |= 0x10; // enable digital port 
}

void Motor_Init(void){
	GPIOPortC_Init();
}

void Motor_Run(void){
	
}

void Motor_Stop(void){
	
}

