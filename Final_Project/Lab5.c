// Lab5.c
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/06/21
// Contains main program that inits music player
// Final Project
// Blake Margolis
// 03/29/21

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

#include "TestMotor.h"
#include "TestSpeaker.h"

//==========================
// Global variables
//==========================

#define LOCKED 0
#define UNLOCKED 1
#define SET_CODE 2
uint8_t state = LOCKED;

char secretCode[] = {'1','2','3','4'};
char newCode[] = {'1','2','3','4'};
char enteredCode[] = {'1','2','3','4'};
uint8_t totalDigits = 4;
uint8_t digitIndex = 0;

uint8_t updateDisplayNeeded = 1;
uint8_t clearScreenNeeded = 1;

char key;


//==========================
// Helper functions
//==========================

// Called by scan()
void recordKey(char k){
	key = k;
	updateDisplayNeeded = 1;
}

void processKey(char enteredKey){
	
}

void resetCode(char *c){
	char *localCode = c;
	for(int i=0; i<totalDigits; i++){
		localCode[i] = '*';
	}
}

void Lockbox_Init(void){
	resetCode(newCode);
	resetCode(enteredCode);
}


//==========================
// Graphics functions
//==========================

void clearScreen(void){
	ST7735_FillScreen(0);
	clearScreenNeeded = 0;
}

void drawTitle(void){
	ST7735_DrawString(0,1," ------------------- ",ST7735_YELLOW);
	ST7735_DrawString(0,2," |  PANDORA'S BOX  | ",ST7735_YELLOW);
	ST7735_DrawString(0,3," ------------------- ",ST7735_YELLOW);
}


void drawCode(char *c){
	char *localCode = c;
	
	ST7735_SetCursor(7,7);

	for(int i=0; i<totalDigits; i++){
		ST7735_OutChar(localCode[i]);
		ST7735_OutString(" ");
	}
	
	ST7735_DrawString(7,8,"- - - -",ST7735_YELLOW);

}

void drawLockScreen(){
	drawTitle();

	ST7735_DrawString(6,5,"Enter Code:",ST7735_YELLOW);
	drawCode(enteredCode);
}

void drawSetCodeScreen(){
	
}

void drawUnlockScreen(){
	
}




//==========================
// Main control loop
//==========================
int main(void) { 
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	TExaS_Init(SCOPE_PD2,80000000); 	

	ST7735_InitR(INITR_REDTAB); 		 // init LCD
	GPIOPortD_Init(); 							 // init DAC pin
	DAC_Init(0x07FF); 							 // init DAC
	Music_Init();
	
	//Test_Speaker_Init(); 							// test speaker
	//MotorTest();
	
	Lockbox_Init();
	
  EnableInterrupts();

  while(1){
    if(clearScreenNeeded){
			clearScreen();
		}
		
		else if(updateDisplayNeeded){
			if(state == LOCKED){
				drawLockScreen();
			}
			else if(state == UNLOCKED){
				drawUnlockScreen();
			}
			else if(state == SET_CODE){
				drawSetCodeScreen();
			}
			updateDisplayNeeded = 0;
		}
		
		else{
			char enteredKey = scanKeypad();	//blocking
			processKey(enteredKey);
		}
		
  }
}
