// Lab5.c
// Amy Lee
// 03/06/21
// Contains main program that inits music player
// Lab 5
// Blake Margolis
// 03/08/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"
#include "../inc/ST7735.h"
#include "../inc/TExaS.h" 

#include "DAC.h"
#include "Music.h" 
#include "Keypad.h"
#include "Lab5.h"
#include "LED.h"
#include "Motor.h"

#include "TestSpeaker.h"

char key;
uint8_t updateDisplayNeeded = 0;

void recordKey(char k){
	key = k;
	updateDisplayNeeded = 1;
}


int main(void) { 
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	TExaS_Init(SCOPE_PD2,80000000); 	

	ST7735_InitR(INITR_REDTAB); 		 // init LCD
	Port_D_Init(); 									 // init DAC pin
	DAC_Init(0x07FF); 							 // init DAC
	Music_Init();
	
	Test_Speaker_Init(); 							// test speaker
	
  EnableInterrupts();

  while(1){
    if(updateDisplayNeeded){
			ST7735_SetCursor(0,0);
			ST7735_OutChar(key);
			updateDisplayNeeded = 0;
		}
		
  }
}
