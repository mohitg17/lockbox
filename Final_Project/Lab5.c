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

//#include "TestMotor.h"
//#include "TestSpeaker.h"

//==========================
// Global variables
//==========================

#define LOCKED 0
#define UNLOCKED 1
#define SET_CODE 2
uint8_t state = UNLOCKED;
uint8_t cursor = 7;

char secretCode[] = {'1','2','3','4'};
char newCode[] = {'1','2','3','4'};
char enteredCode[] = {'1','2','3','4'};
const uint8_t totalDigits = 4;
uint8_t digitIndex = 0;

uint8_t updateDisplayNeeded = 1;
uint8_t clearScreenNeeded = 1;

char key;
uint8_t keyReady = 0;

//==========================
// Keypad functions
//==========================

// Called by keypad module, to transfer key to main
void typeKey(char k){
//	key = k;

//	ST7735_SetCursor(cursor,8);
//	ST7735_OutChar(k);
//	
//	cursor += 2;
//	if (cursor == 15) cursor = 7;
	
	recordKey(k);
}

// Store key from keypad into variable in main
void recordKey(char k){
	key = k;
	keyReady = 1;
	updateDisplayNeeded = 1;
}

// Blocking, wait for key from keypad
char scanKeypad(void){
	while(keyReady == 0){}
	keyReady = 0;
	return key;
}

//==========================
// Music functions
//==========================

void playGoodSong(void){
	setLockStatus(1);
	Music_Init();
	Music_Play();
}

void playBadSong(void){
	setLockStatus(2);
	Music_Init();
	Music_Play();
}


//==========================
// Helper functions
//==========================

void setNewCode(char *c1, char *c2) {
	for(uint8_t i = 0; i < 4; i++) {
		c1[i] = c2[i];
	}
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

uint8_t compareCodes(char *c1, char *c2){
	for(int i=0; i<totalDigits; i++){
		if(c1[i] != c2[i]){
			return 0;
		}
	}
	return 1;
}

void processKey(char enteredKey){
	// turn off LEDs upon any key press
	SetGreenLED(0);
	SetRedLED(0);
	
	if(state == LOCKED){
		// Record key
		if(digitIndex < totalDigits){
			enteredCode[digitIndex] = enteredKey;
			digitIndex++;
		}
		
		// Check if finished entering digits
		if(digitIndex == totalDigits){
			uint8_t isEqual = compareCodes(enteredCode, secretCode);
			
			// correct submission
			if(isEqual){
				Motor_Unlock();
				state = UNLOCKED;
				clearScreenNeeded = 1;
				
				// play correct sound/light
				playGoodSong();
				SetGreenLED(1);
			}
			
			// incorrect submission
			else{
				// play incorrect sound/light
				playBadSong();
				SetRedLED(1);
			}
			
			resetCode(enteredCode);
			digitIndex = 0;
		}
	}
	
	else if(state == UNLOCKED){
		// Lock
		if(enteredKey == '#'){
			Motor_Lock();
			state = LOCKED;
			clearScreenNeeded = 1;
		}
		
		// Set mode
		else if(enteredKey == '*'){
			state = SET_CODE;
			resetCode(newCode);
			clearScreenNeeded = 1;
		}
	}
	
	else if(state == SET_CODE){
		// Record key
		if(digitIndex < totalDigits){
			newCode[digitIndex] = enteredKey;
			digitIndex++;
		}
		
		// Check if finished entering keys
		if(digitIndex == totalDigits){
			state = UNLOCKED;
			setNewCode(secretCode, newCode);
			digitIndex = 0;
			clearScreenNeeded = 1;
		}
	}
	
}




//==========================
// Graphics functions
//==========================

void drawTitle(void){
	ST7735_DrawString(0,1," ------------------- ",ST7735_YELLOW);
	ST7735_DrawString(0,2," |  PANDORA'S BOX  | ",ST7735_YELLOW);
	ST7735_DrawString(0,3," ------------------- ",ST7735_YELLOW);
}


void drawCode(char *c){
	char *localCode = c;
	
	ST7735_SetCursor(7,8);

	for(int i=0; i<totalDigits; i++){
		ST7735_OutChar(localCode[i]);
		ST7735_OutString(" ");
	}
	
	ST7735_DrawString(7,9,"- - - -",ST7735_YELLOW);

}

void drawLockScreen(){
	drawTitle();

	ST7735_DrawString(6,6,"Enter Code",ST7735_YELLOW);
	drawCode(enteredCode);
}

void drawSetCodeScreen(){
	drawTitle();

	ST7735_DrawString(5,6,"Set New Code",ST7735_YELLOW);
	drawCode(newCode);
}

void drawUnlockScreen(){
	drawTitle();
	SetGreenLED(1);
	ST7735_DrawString(7,5,"Welcome!",ST7735_YELLOW);
	ST7735_DrawString(4,8,"# = Lock",ST7735_YELLOW);
	ST7735_DrawString(4,9,"* = Set Code",ST7735_YELLOW);
}




//==========================
// Main control loop
//==========================
int main(void) { 
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	TExaS_Init(SCOPE_PD2,80000000); 	

	LaunchPad_Init();
	LED_Init();
	ST7735_InitR(INITR_REDTAB); 		 // init LCD
	GPIOPortD_Init(); 							 // init DAC pin
	DAC_Init(0x07FF); 							 // init DAC
	lockStatus = 1; 								 // default setting: Wii Channel music
	Music_Init();
	Motor_Init();
	
	Lockbox_Init();
	GPIOPortB_Init(&typeKey);
	
  EnableInterrupts();

  while(1){
		// Music_Play(); 								// test music (Wii Channel); comment out to turn off music
		
    if(clearScreenNeeded){
			ST7735_FillScreen(0);
			clearScreenNeeded = 0;
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
			//while(1);		// temporary to simulate blocking
			char enteredKey = scanKeypad();	//blocking
			processKey(enteredKey);
			updateDisplayNeeded = 1;
		}
		
  }
}
