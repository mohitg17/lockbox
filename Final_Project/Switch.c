// Button.c
// Amy Lee
// 03/06/21
// Switch initializations and data function implementations 
// PC4 --> UP switch (on lab board)
// PF0,4 --> SW1,2 (on microcontroller) 
// Lab 5
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "Switch.h" 
#include "Music.h"

volatile static unsigned long touchF4; 
volatile static unsigned long lastF4;      // previous F4 status
volatile static unsigned long touchF0; 
volatile static unsigned long lastF0;      // previous F0 status

volatile static unsigned long touchC4; 
volatile static unsigned long lastC4;			 // previous C4 status

volatile static unsigned long curServiceButton;
volatile static unsigned long curServiceButtonC;

//=====================================
// GPIOFArm
//-------------------------------------
// Arm GPIO (Port F) edge-interrupts
// inputs: none
// outputs: none
//=====================================
static void GPIOFArm(void){
  GPIO_PORTF_ICR_R = 0x01;      // (e) clear flag0
  GPIO_PORTF_IM_R |= 0x01;      // (f) arm interrupt on PF0 *** No IME bit as mentioned in Book *** 
	
	GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
	
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
}

//=====================================
// GPIOCArm
//-------------------------------------
// Arm GPIO (Port C) edge-interrupts
// inputs: none
// outputs: none
//=====================================
static void GPIOCArm(void){
  GPIO_PORTC_ICR_R = 0x10;      // (e) clear flag4 (port C)
  GPIO_PORTC_IM_R |= 0x10;      // (f) arm interrupt on PC4 *** No IME bit as mentioned in Book *** 
	
	GPIO_PORTC_ICR_R = 0x80;      // (e) clear flag7 (port C)
  GPIO_PORTC_IM_R |= 0x80;      // (f) arm interrupt on PC7 *** No IME bit as mentioned in Book ***
	
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 4;      					 
}

//==========================
// PORT C Initialization 
//==========================
// PC4 = UP switch
// PC5 = RIGHT switch
// PC6 = LEFT switch
// PC7 = DOWN switch
//--------------------------
void Port_C_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x04;									// Activate clock for Port C
	while((SYSCTL_PRGPIO_R & 0x04) != 0x04){};	// Allow time for clock to start

	GPIO_PORTC_PCTL_R &= ~0xFFFF0000;						// regular GPIO
	GPIO_PORTC_AMSEL_R &= ~0xF0;								// disable analog function
	GPIO_PORTC_DIR_R &= ~0xF0;									// inputs on PC7-PC4
	GPIO_PORTC_AFSEL_R &= ~0xF0;								// regular port function
	GPIO_PORTC_PUR_R = 0xF0;										// enable pull-up on PC7-PC4
	GPIO_PORTC_DEN_R |= 0xF0; 									// enable digital port		
		
	GPIO_PORTC_IS_R &= ~0x90;  
	GPIO_PORTC_IBE_R &= ~0x90;     
  GPIOCArm();
		
	SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
	touchC4 = 0;
	lastC4 = PC4;                 // initial switch state
} 


//==========================
// PORT F Initialization 
//==========================
// PF0 = SW2
// PF4 = SW1
//--------------------------
void Port_F_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x20; 								// activate clock for Port F
	while((SYSCTL_PRGPIO_R & 0x20)==0){}; 		// allow time for clock to stabilize

	GPIO_PORTF_LOCK_R = 0x4C4F434B; 					// unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F; 									// allow changes to PF4-0

	GPIO_PORTF_AMSEL_R = 0x00; 								// disable analog on PF
	GPIO_PORTF_PCTL_R = 0x00000000; 					// PCTL GPIO on PF4-0
	GPIO_PORTF_DIR_R = 0x0E; 									// PF4,PF0 in, PF3-1 out
	GPIO_PORTF_AFSEL_R = 0x00; 								// disable alt funct on PF7-0
	GPIO_PORTF_PUR_R = 0x11; 									// enable pull-up on PF0 and PF4
	GPIO_PORTF_DEN_R = 0x1F; 									// enable digital I/O on PF4-0
		
	GPIO_PORTF_IS_R &= ~0x11;     // (d) PF0,4 is edge-sensitive
	GPIO_PORTF_IBE_R |= 0x11;     //     PF0,4 is both edges
  GPIOFArm();

  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  touchF4 = 0;
	touchF0 = 0;
  lastF4 = PF4;                 // initial switch state
	lastF0 = PF0;
}



//===============================
// Timer0Arm
//-------------------------------
// Timer for de-bouncing switches
// inputs: none
// outputs: none
//===============================
static void Timer0Arm(void){
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
// GPIOPortF_Handler
//------------------------------------------
// Port F ISR (PF4 and PF0)
// inputs: none
// outputs: none
//==========================================
void GPIOPortF_Handler(void){
	curServiceButton = GPIO_PORTF_RIS_R; 
	
	if ((curServiceButton & 0x01) == 0x01) {
		GPIO_PORTF_IM_R &= ~0x01;     // disarm interrupt on PF0
    touchF0 = 1;       						// touch occurred
		
		if (IsMusicPlaying) {
			Music_Stop();
		} 
		
		else {
			Music_Play(); 
		}
	}
	
	else if ((curServiceButton & 0x10) == 0x10) {
		GPIO_PORTF_IM_R &= ~0x10;     // disarm interrupt on PF4
    touchF4 = 1;       						// touch occurred
    Music_Rewind(); 
	}
	
  Timer0Arm(); 										// start one shot
}

//==========================================
// GPIOPortC_Handler
//------------------------------------------
// Port C ISR (PC4)
// inputs: none
// outputs: none
//==========================================
void GPIOPortC_Handler(void) {	
	curServiceButtonC = GPIO_PORTC_RIS_R; 
	
	if ((curServiceButtonC & 0x10) == 0x10) {
		GPIO_PORTC_IM_R &= ~0x10;     // disarm interrupt on PC4
    touchC4 = 1;       						// touch occurred
		Change_Instrument(); 
	}
	
  Timer0Arm(); 										// start one shot
}


