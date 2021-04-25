// Music.c
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/08/21
// Contains music function implementations and song definitions
// Final Project
// Blake Margolis
// 03/29/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "DAC.h"
#include "Music.h"

#define LEN_WAVE 64

#define NUM_MELODY_NOTES 106
#define NUM_HARMONY_NOTES 106

#define NUM_MELODY_BAD_NOTES 8
#define NUM_HARMONY_BAD_NOTES 8

//====================
// global variables
//====================
uint8_t isMusicPlaying, lockStatus;
static uint8_t isSongEnd;
static uint16_t melodyNoteIdx, harmonyNoteIdx; 				// track which note program is on
static uint16_t melodyInstrOut, harmonyInstrOut;			// for DAC output (combining sine waves)
static uint32_t melodyNoteLen, harmonyNoteLen;				// length of current note
static uint32_t melodyBeats, harmonyBeats; 						// track how long program has been on a note
static uint16_t harmonyInstrIdx, melodyInstrIdx; 			// instrument selection; sine wave output

//========================
// Instrument waveform
//========================
const uint16_t trumpet[LEN_WAVE] = {
	987, 1049, 1090, 1110, 1134, 1160, 1139, 1092, 1070, 1042, 1035, 1029, 1008, 1066, 1150, 1170, 1087, 915, 679, 372, 151, 
	558, 1014, 1245, 1260, 1145, 1063, 984, 934, 960, 1027, 1077, 1081, 1074, 1064, 1042, 1010, 974, 968, 974, 994, 1039, 
	1094, 1129, 1125, 1092, 1056, 1056, 1082, 1059, 1046, 1058, 1061, 1045, 1034, 1050, 1094, 1112, 1092, 1063, 1053, 1065, 1052, 992
};

//===================================
// Unlock song (Wii Channel Theme)
//===================================
struct Note unlock_melody[] = {
	{E1, (QUARTER-BLANK)},	{R, BLANK}, 
	{A, (EIGHTH-BLANK)},	{R, BLANK}, 
	{DF1, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{A, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{GF, (EIGHTH-BLANK)},	{R, BLANK},
	
	{D, (EIGHTH-BLANK)},	{R, BLANK},
	{D, (EIGHTH-BLANK)},	{R, BLANK},
	{D, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{C0, (EIGHTH-BLANK)},	{R, BLANK},
	
	{D, (EIGHTH-BLANK)},	{R, BLANK},
	{GF, (EIGHTH-BLANK)},	{R, BLANK},
	{A, (EIGHTH-BLANK)},	{R, BLANK},
	{DF1, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{A, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{GF, (EIGHTH-BLANK)},	{R, BLANK},
	
	{E1, ((QUARTER+EIGHTH)-BLANK)},	{R, BLANK},
	{EF1, (EIGHTH-BLANK)},	{R, BLANK},
	{D1, (QUARTER-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	
	{AF, (QUARTER-BLANK)},	{R, BLANK}, 
	{DF1, (EIGHTH-BLANK)},	{R, BLANK}, 
	{GF, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{DF1, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{AF, (EIGHTH-BLANK)},	{R, BLANK},
	
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{DF1, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{G, (EIGHTH-BLANK)},	{R, BLANK},
	{GF, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{E, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	
	{E, (EIGHTH-BLANK)},	{R, BLANK},
	{E, (EIGHTH-BLANK)},	{R, BLANK},
	{E, (EIGHTH-BLANK)},	{R, BLANK},	
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{E, (EIGHTH-BLANK)},	{R, BLANK},	
	{E, (EIGHTH-BLANK)},	{R, BLANK},	

	{E, (EIGHTH-BLANK)},	{R, BLANK},	
	{R, (EIGHTH-BLANK)},	{R, BLANK},	
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{EF, (QUARTER-BLANK)},	{R, BLANK},
	{D, (QUARTER-BLANK)},	{R, BLANK},

	
	{R, (QUARTER-BLANK)},	{R, BLANK},
};

struct Note unlock_harmony[] = {
	{E0, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	
	{E_1, (EIGHTH-BLANK)},	{R, BLANK},
	{E_1, (EIGHTH-BLANK)},	{R, BLANK},
	{E_1, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	
	{R, ((QUARTER+EIGHTH)-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK}, 
	{R, (EIGHTH-BLANK)},	{R, BLANK},

	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	
	{C0, (EIGHTH-BLANK)},	{R, BLANK},
	{C0, (EIGHTH-BLANK)},	{R, BLANK},
	{C0, (EIGHTH-BLANK)},	{R, BLANK},	
	{R, (EIGHTH-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{C0, (EIGHTH-BLANK)},	{R, BLANK},	
	{C0, (EIGHTH-BLANK)},	{R, BLANK},	
	
	{C0, (EIGHTH-BLANK)},	{R, BLANK},	
	{R, (EIGHTH-BLANK)},	{R, BLANK},	
	{R, (QUARTER-BLANK)},	{R, BLANK},
	{B0, (QUARTER-BLANK)},	{R, BLANK},
	{BF0, (QUARTER-BLANK)},	{R, BLANK},
	
	{R, (QUARTER-BLANK)},	{R, BLANK},
};

struct Song unlock = {									// default song settings
	unlock_melody,
	unlock_harmony
};

//===================================
// Incorrect song
//===================================
struct Note bad_melody[] = {
	{C0, (QUARTER-BLANK)},	{R, BLANK}, 
	{B0, (QUARTER-BLANK)},	{R, BLANK}, 
	{BF0, (QUARTER-BLANK)},	{R, BLANK}, 
	{A0, (QUARTER-BLANK)},	{R, BLANK},
	
	{R, (QUARTER-BLANK)},	{R, BLANK},
};

struct Note bad_harmony[] = {
	{E0, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	
	{R, (QUARTER-BLANK)},	{R, BLANK},
};
struct Song bad_passcode = {
	bad_melody,
	bad_harmony,
};

//============================================
// Music_Init
//--------------------------------------------
// Initializes music to start @ beginning
// input(s): none
// output(s): none
//============================================
void Music_Init(void) {
	isMusicPlaying = 0; 
	isSongEnd = 0; 
	melodyInstrIdx = 0;											// idx to instrument waveform for DAC_Out (melody)
	harmonyInstrIdx = 0; 										// idx to instrument waveform for DAC_Out (harmony)
	
	melodyNoteIdx = 0; 											// idx to melody portion of song
	melodyBeats = 0;												// tracks how long note has been held in melody portion
	harmonyNoteIdx = 0;
	harmonyBeats = 0;
	
	if (lockStatus == 1) {
		SysTick_Init(BEAT_CYCLES); 							// controller for song and tempo
		melodyNoteLen = unlock.melody[melodyNoteIdx].length;		// get first melody note's duration
		Timer1A_Init(unlock.melody[melodyNoteIdx].pitch);			// get first melody note's pitch

		harmonyNoteLen = unlock.harmony[harmonyNoteIdx].length;	// get first harmony note's duration
		Timer2A_Init(unlock.harmony[harmonyNoteIdx].pitch);			// get first harmony note's pitch
	}
	
	else if (lockStatus == 2) {
		SysTick_Init(BEAT_CYCLES); 							// controller for song and tempo
		melodyNoteLen = bad_passcode.melody[melodyNoteIdx].length;		// get first melody note's duration
		Timer1A_Init(bad_passcode.melody[melodyNoteIdx].pitch);			// get first melody note's pitch

		harmonyNoteLen = bad_passcode.harmony[harmonyNoteIdx].length;	// get first harmony note's duration
		Timer2A_Init(bad_passcode.harmony[harmonyNoteIdx].pitch);			// get first harmony note's pitch
	}
}

//============================================
// Music_Play
//--------------------------------------------
// Plays song: Promenade by Mussorgsky
// input(s): none
// output(s): none
//============================================
void Music_Play(void) {
	SysTick_Start(); 											// song + tempo controller
	Timer1A_Start();											// melody interrupt
	Timer2A_Start();											// harmony interrupt
	
	isMusicPlaying = 1;
}

//============================================
// Music_Stop
//--------------------------------------------
// Pauses song
// input(s): none
// output(s): none
//============================================
void Music_Stop(void) {
	SysTick_Stop(); 											// song + tempo controller
	Timer1A_Stop();												// melody interrupt
	Timer2A_Stop(); 											// harmony interrupt
	
	isMusicPlaying = 0; 
} 

//============================================
// Music_Rewind
//--------------------------------------------
// Resets song @ the beginning
// input(s): none
// output(s): none
//============================================
void Music_Rewind(void) {
	Music_Stop(); 
	isMusicPlaying = 0; 
	
	Music_Init();
}


//============================================
// SysTick_Init
//--------------------------------------------
// Inits SysTick timer for tempo/song control
// input(s): period
// output(s): none
//============================================
void SysTick_Init(uint32_t period) {
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x80000000; // priority 2                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x05;
}

//==========================================
// SysTick_Handler
//------------------------------------------
// Controls song indices for melody 
// and harmony
// inputs: none
// outputs: none
//==========================================
void SysTick_Handler(void){
	melodyBeats++;													// keeps track of how long current note has been held
	harmonyBeats++;
	
	if (lockStatus == 1) {
		if ((melodyNoteIdx >= NUM_MELODY_NOTES) || (harmonyNoteIdx >= NUM_HARMONY_NOTES)) {	
			melodyInstrOut = 0;										// stop playing sound when song ends
			harmonyInstrOut = 0;
			isSongEnd = 1; 
		}
	}
	
	else if (lockStatus == 2) {
		if ((melodyNoteIdx >= NUM_MELODY_BAD_NOTES) || (harmonyNoteIdx >= NUM_HARMONY_BAD_NOTES)) {	
			melodyInstrOut = 0;										// stop playing sound when song ends
			harmonyInstrOut = 0;
			isSongEnd = 1; 
		}
	}
		
	if (melodyBeats >= melodyNoteLen) {
		melodyNoteIdx += 1;										// get next note in song
		melodyInstrIdx = 0;
		melodyBeats = 0;											// prep for new note
		
		if (lockStatus == 1) {
			melodyNoteLen = unlock.melody[melodyNoteIdx].length;					// change to next note length
			Timer1A_ChangePeriod(unlock.melody[melodyNoteIdx].pitch); 		// change to next note pitch
		}
		
		else if (lockStatus == 2) {
			melodyNoteLen = bad_passcode.melody[melodyNoteIdx].length;					// change to next note length
			Timer1A_ChangePeriod(bad_passcode.melody[melodyNoteIdx].pitch); 		// change to next note pitch
		}
	}
	
	if (harmonyBeats >= harmonyNoteLen) {			// need to proceed to next note
		harmonyNoteIdx += 1;										// get next note in song
		harmonyInstrIdx = 0;
		harmonyBeats = 0;												// prep for new note
		
		if (lockStatus == 1) {
			harmonyNoteLen = unlock.harmony[harmonyNoteIdx].length;				// change to next note length
			Timer2A_ChangePeriod(unlock.harmony[harmonyNoteIdx].pitch); 		// change to next note pitch
		}
		
		else if (lockStatus == 2) {
			harmonyNoteLen = bad_passcode.harmony[harmonyNoteIdx].length;				// change to next note length
			Timer2A_ChangePeriod(bad_passcode.harmony[harmonyNoteIdx].pitch); 		// change to next note pitch
		}
	}
}

//============================================
// SysTick_Start
//--------------------------------------------
// Arms SysTick timer
// input(s): none
// output(s): none
//============================================
void SysTick_Start(void) {
	NVIC_ST_CTRL_R = 0x00000007;				// enable SysTick (start Alarm1)
}

//============================================
// SysTick_Stop
//--------------------------------------------
// Disarms SysTick timer
// input(s): none
// output(s): none
//============================================
void SysTick_Stop(void) {
	NVIC_ST_CTRL_R = 0x00000005;       // disable SysTick (stop Alarm2) 
}

//=====================================================
// Timer1A_Init
//-----------------------------------------------------
// Initialize TIMER1A interrupts to play melody note
// Inputs: period (note freq)
// Outputs: none
//=====================================================
void Timer1A_Init(uint32_t period){
	volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
	delay = SYSCTL_RCGCTIMER_R;
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|(0x80000000); // priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
}

//=====================================================
// Timer1A_Handler
//-----------------------------------------------------
// Melody note player
// Inputs: none
// Outputs: none
//=====================================================
void Timer1A_Handler(void) {
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;			// acknowledge TIMER1A timeout
	uint16_t output; 
	
	if (!isSongEnd) {																				// only play sound when song hasn't ended
		melodyInstrOut = trumpet[melodyInstrIdx];
		melodyInstrIdx = (melodyInstrIdx + 1) % LEN_WAVE;			// play instrument voice
		output = (melodyInstrOut + harmonyInstrOut) / 2;			// combine melody + harmony waveforms (divide by 2 --> no clipping)
		DAC_Out(output);
	}
}


//=====================================================
// Timer1A_Start
//-----------------------------------------------------
// Activate Timer1A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer1A_Start(void) {
	TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

//=====================================================
// Timer1A_Stop
//-----------------------------------------------------
// De-activate TIMER1A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer1A_Stop(void) {
	TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
}

//==========================================
// Timer1A_ChangePeriod
//------------------------------------------
// Changes Timer1A interrupt period to play
// next note in melody
// inputs: note freq
// outputs: none
//==========================================
void Timer1A_ChangePeriod(uint32_t note) {
	TIMER1_TAILR_R = note;
}

//=====================================================
// Timer2A_Init
//-----------------------------------------------------
// Initialize Timer2A interrupts to play harmony note
// Inputs: period (note freq)
// Outputs: none
//=====================================================
void Timer2A_Init(uint32_t period){
	volatile uint32_t delay; 
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
	delay = SYSCTL_RCGCTIMER_R;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|(0x80000000); // priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
}

//=====================================================
// Timer2A_Handler
//-----------------------------------------------------
// Harmony note player
// Inputs: none
// Outputs: none
//=====================================================
void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout	
	uint16_t output; 
		
	if (!isSongEnd) {																				// only play sound if song hasn't ended
		harmonyInstrOut = trumpet[harmonyInstrIdx];
		harmonyInstrIdx = (harmonyInstrIdx + 1) % LEN_WAVE;		// play instrument voice
		output = (melodyInstrOut + harmonyInstrOut) / 2;			// combine melody + harmony waveforms (divide by 2 --> no clipping) 
		DAC_Out(output);
	}
}

//=====================================================
// Timer2A_Start
//-----------------------------------------------------
// Activate Timer2A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer2A_Start(void) {
	TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

//=====================================================
// Timer2A_Stop
//-----------------------------------------------------
// De-activate Timer2A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer2A_Stop(void){
  TIMER2_CTL_R = 0x00000000;  // 10) disable timer2A
}

//==========================================
// Timer2A_ChangePeriod
//------------------------------------------
// Changes Timer2A interrupt period to play
// next note in harmony
// inputs: note freq
// outputs: none
//==========================================
void Timer2A_ChangePeriod(uint32_t note) {
	TIMER2_TAILR_R = note; 
}

//==========================================
// setLockStatus
//------------------------------------------
// Changes lockStatus
// inputs: status
// outputs: none
//==========================================
void setLockStatus(uint8_t status){
	lockStatus = status;
}

