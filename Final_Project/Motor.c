// Motor.c
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/06/21
// Motor initializations and data function implementations 
// Final Project
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "Motor.h"

uint32_t Timer4Period = 199999;

void GPIOPortC_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x04; // Activate clock for Port C
	while((SYSCTL_PRGPIO_R & 0x04) != 0x04){}; // Allow time for clock to start

	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFFF0FFFF);		//regular GPIO, clear wanted bits
	GPIO_PORTC_AMSEL_R &= ~0x10; // disable analog function 
	GPIO_PORTC_DIR_R |= 0x10; // inputs on PC7-PC4
	GPIO_PORTC_AFSEL_R &= ~0x10; // regular port function
	GPIO_PORTC_DEN_R |= 0x10; // enable digital port 
}

//=======================================
// Timer3 Init
//=======================================
void Timer3A_Init(){
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = 1599999;    // 4) reload value 20ms
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|(3<<29); // priority  
// interrupts enabled in the main program after all devices initialized
// vector number 51, interrupt number 35
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER3A
}

void Timer4A_Init(uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER4
  TIMER4_CTL_R = 0x00000000;    // 1) disable TIMER4A during setup
  TIMER4_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER4_TAMR_R = 0x00000001;   // 3) one-shot mode
  TIMER4_TAILR_R = period;    // 4) reload value (199999, 39999)
  TIMER4_TAPR_R = 0;            // 5) bus clock resolution
  TIMER4_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER4_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI17_R = (NVIC_PRI17_R&0xFF00FFFF)|(3<<21); // priority
// interrupts enabled in the main program after all devices initialized
// vector number 86, interrupt number 70
  NVIC_EN2_R = 0x00000040;      // 9) enable interrupt 70 in NVIC
  TIMER4_CTL_R = 0x00000001;    // 10) enable TIMER4A
}

void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER3A timeout
	GPIO_PORTC_DATA_R ^= 0x10;
	Timer4A_Init(Timer4Period);
}

void Timer4A_Handler(void){
  TIMER4_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER4A timeout
	GPIO_PORTC_DATA_R ^= 0x10;
}

void Motor_Init(void){
	GPIOPortC_Init();
	Timer3A_Init();
}

void Motor_Lock(void){
	Timer4Period = 39999;
}

void Motor_Unlock(void){
	Timer4Period = 199999;
}

