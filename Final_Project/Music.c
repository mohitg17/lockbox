// Music.c
// Amy Lee
// 03/08/21
// Contains music function implementations and song definitions
// Lab 5
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "DAC.h"
#include "Music.h"

#define NUM_INSTRUMENT 4
#define LEN_WAVE 64
#define NUM_MELODY_NOTES 163
#define NUM_HARMONY_NOTES 123

//====================
// global variables
//====================
uint8_t IsMusicPlaying;
static uint8_t isSongEnd;
static uint16_t melodyNoteIdx, harmonyNoteIdx; 				// track which note program is on
static uint16_t melodyInstrOut, harmonyInstrOut;			// for DAC output (combining sine waves)
static uint32_t melodyNoteLen, harmonyNoteLen;				// length of current note
static uint32_t melodyBeats, harmonyBeats; 						// track how long program has been on a note
static uint16_t curInstrument, harmonyInstrIdx, melodyInstrIdx; 			// instrument selection; sine wave output

//========================
// Instrument waveforms
//========================
const uint16_t wave[LEN_WAVE] = {  
  1024, 1122, 1219, 1314, 1407, 1495, 1580, 1658, 1731, 1797, 1855, 1906,1948,1981,2005,2019,2024,2019,2005,1981,1948,1906,
  1855, 1797, 1731, 1658, 1580, 1495, 1407, 1314, 1219, 1122, 1024, 926, 829, 734, 641, 553, 468, 390, 317, 251, 193, 142,
  100, 67, 43, 29, 24, 29, 43, 67, 100, 142, 193, 251, 317, 390, 468, 553, 641, 734, 829, 926
}; 

const uint16_t trumpet[LEN_WAVE] = {
	987, 1049, 1090, 1110, 1134, 1160, 1139, 1092, 1070, 1042, 1035, 1029, 1008, 1066, 1150, 1170, 1087, 915, 679, 372, 151, 
	558, 1014, 1245, 1260, 1145, 1063, 984, 934, 960, 1027, 1077, 1081, 1074, 1064, 1042, 1010, 974, 968, 974, 994, 1039, 
	1094, 1129, 1125, 1092, 1056, 1056, 1082, 1059, 1046, 1058, 1061, 1045, 1034, 1050, 1094, 1112, 1092, 1063, 1053, 1065, 1052, 992
};

const uint16_t oboe[LEN_WAVE] = {
	1024, 1024, 1014, 1008, 1022, 1065, 1093, 1006, 858, 711, 612, 596, 672, 806, 952, 1074, 1154, 1191, 
	1202, 1216, 1236, 1255, 1272, 1302, 1318, 1299, 1238, 1140, 1022, 910, 827, 779, 758, 757, 782, 856, 
	972, 1088, 1177, 1226, 1232, 1203, 1157, 1110, 1067, 1028, 993, 958, 929, 905, 892, 900, 940, 1022, 
	1125, 1157, 1087, 965, 836, 783, 816, 895, 971, 1017
};

const uint16_t bassoon[LEN_WAVE] = {
	1068, 1169, 1175, 1161, 1130, 1113, 1102, 1076, 1032, 985, 963, 987, 1082, 1343, 1737, 1863, 
	1575, 1031, 538, 309, 330, 472, 626, 807, 1038, 1270, 1420, 1461, 1375, 1201, 1005, 819, 658, 
	532, 496, 594, 804, 1055, 1248, 1323, 1233, 1049, 895, 826, 826, 850, 862, 861, 899, 961, 1006, 
	1023, 1046, 1092, 1177, 1224, 1186, 1133, 1098, 1102, 1109, 1076, 1027, 1003
};

static const uint16_t *instruments[NUM_INSTRUMENT] = {trumpet, oboe, bassoon, wave};		// array of ptrs to instrument waveform arrays

//===================================
// Song: "Promenade" by M. Mussorgsky
//===================================
struct Note promenade_melody[] = {
	{E1, (QUARTER-BLANK)},	{R, BLANK}, 
	{D1, (QUARTER-BLANK)},	{R, BLANK}, 
	{G1, (QUARTER-BLANK)},	{R, BLANK},	
	{A1, (EIGHTH-BLANK)},		{R, BLANK}, 
	{D2, (EIGHTH-BLANK)},		{R, BLANK}, 
	{B1, (QUARTER-BLANK)},	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)},		{R, BLANK}, 
	{D2, (EIGHTH-BLANK)},		{R, BLANK}, 
	{B1, (QUARTER-BLANK)},	{R, BLANK}, 
	{G1, (QUARTER-BLANK)},	{R, BLANK}, 
	{A1, (QUARTER-BLANK)},	{R, BLANK}, 
	{E1, (QUARTER-BLANK)},	{R, BLANK}, 
	{D1, (QUARTER-BLANK)},	{R, BLANK}, 
	{E1, (QUARTER-BLANK)},	{R, BLANK}, 
	{D1, (QUARTER-BLANK)},	{R, BLANK}, 
	{G1, (QUARTER-BLANK)},	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)},		{R, BLANK}, 
	{D2, (EIGHTH-BLANK)},		{R, BLANK}, 
	{B1, (QUARTER-BLANK)},	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)},		{R, BLANK}, 
	{D2, (EIGHTH-BLANK)},		{R, BLANK}, 
	{B1, (QUARTER-BLANK)},	{R, BLANK}, 
	{G1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (QUARTER-BLANK)},  {R, BLANK}, 
	{E1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{E1, (QUARTER-BLANK)},	{R, BLANK}, 
	{B, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{E1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{A, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{E1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{GF1, (EIGHTH-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D2, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{B1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{G1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{E1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{B, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{E1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{C, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{A1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{F1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{F2, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D2, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{C1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{BF1, (EIGHTH-BLANK)}, 	{R, BLANK}, 
	{F1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{F1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{F1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{E1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{F1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{D2, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{B1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{D2, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{B1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{E1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{E1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{D1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{D1, (EIGHTH-BLANK)}, 		{R, BLANK}, 
	{B1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{C1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{A1, (QUARTER-BLANK)}, 	{R, BLANK}, 
	{G1, (HALF-BLANK)}, {R, BLANK}
};

struct Note promenade_harmony[] = {
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK},	
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 	
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 	
	{D, ((HALF+QUARTER)-BLANK)},	{R, BLANK}, 
	{A, (HALF-BLANK)},	{R, BLANK},
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 	
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)},	{R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 	
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{C0, (QUARTER-BLANK)}, {R, BLANK}, 
	{C0, (QUARTER-BLANK)}, {R, BLANK}, 
	{E, (QUARTER-BLANK)},	{R, BLANK}, 
	{D, (QUARTER-BLANK)},	{R, BLANK}, 	
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK}, 	
	{R, (QUARTER-BLANK)}, {R, BLANK}, 
	{R, (QUARTER-BLANK)}, {R, BLANK},	
	{F, ((HALF+HALF)-BLANK)},	{R, BLANK}, 	
	{BF, ((HALF+QUARTER)-BLANK)},	{R, BLANK}, 
	{BF, (HALF-BLANK)}, {R, BLANK}, 
	{D, (QUARTER-BLANK)}, {R, BLANK}, 
	{G, (QUARTER-BLANK)}, {R, BLANK}, 
	{A, (EIGHTH-BLANK)}, 	{R, BLANK}, 
	{D1, (EIGHTH-BLANK)},	{R, BLANK}, 
	{B, (QUARTER-BLANK)}, {R, BLANK}, 
	{A, (EIGHTH-BLANK)}, {R, BLANK}, 
	{D1, (EIGHTH-BLANK)}, {R, BLANK}, 
	{B, (QUARTER-BLANK)}, {R, BLANK}, 
	{G, (QUARTER-BLANK)}, {R, BLANK}, 
	{A, (QUARTER-BLANK)}, {R, BLANK}, 
	{E, (QUARTER-BLANK)}, {R, BLANK}, 
	{D, (QUARTER-BLANK)}, {R, BLANK}, 
	{E, (QUARTER-BLANK)}, {R, BLANK}, 
	{D, (QUARTER-BLANK)}, {R, BLANK}, 
	{G, (QUARTER-BLANK)}, {R, BLANK}, 
	{A, (EIGHTH-BLANK)},	{R, BLANK}, 
	{D, (EIGHTH-BLANK)}, {R, BLANK}, 
	{B, (QUARTER-BLANK)}, {R, BLANK}, 
	{G, (QUARTER-BLANK)}, {R, BLANK}, 
	{C, (QUARTER-BLANK)}, {R, BLANK}, 
	{A, (QUARTER-BLANK)}, {R, BLANK}, 
	{G, (HALF-BLANK)}, {R, BLANK}
};

struct Song promenade = {									// default song settings
	trumpet,
	promenade_melody,
	promenade_harmony
};

//============================================
// Music_Init
//--------------------------------------------
// Initializes music to start @ beginning
// input(s): none
// output(s): none
//============================================
void Music_Init(void) {
	IsMusicPlaying = 0; 
	isSongEnd = 0; 
	melodyInstrIdx = 0;											// idx to instrument waveform for DAC_Out (melody)
	harmonyInstrIdx = 0; 										// idx to instrument waveform for DAC_Out (harmony)
	curInstrument = 0;											// 0: trumpet; 1: oboe; 2: bassoon; 3: wave
	
	SysTick_Init(BEAT_CYCLES); 							// controller for song and tempo

	melodyNoteIdx = 0; 											// idx to melody portion of song
	melodyBeats = 0;												// tracks how long note has been held in melody portion
	melodyNoteLen = promenade.melody[melodyNoteIdx].length;		// get first melody note's duration
	Timer1A_Init(promenade.melody[melodyNoteIdx].pitch);			// get first melody note's pitch
	
	harmonyNoteIdx = 0;
	harmonyBeats = 0;
	harmonyNoteLen = promenade.harmony[harmonyNoteIdx].length;	// get first harmony note's duration
	Timer2A_Init(promenade.harmony[harmonyNoteIdx].pitch);			// get first harmony note's pitch
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
	
	IsMusicPlaying = 1;
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
	
	IsMusicPlaying = 0; 
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
	IsMusicPlaying = 0; 
	
	Music_Init();
}

//============================================
// Change_Instrument
//--------------------------------------------
// Changes instrument
// input(s): instrument (enumerated)
// output(s): none
//============================================
void Change_Instrument(void) { 
	curInstrument = (curInstrument + 1) % NUM_INSTRUMENT;
	promenade.instrument = instruments[curInstrument];		// trumpet -> oboe -> bassoon -> wave ->...
	
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0,2); 
	switch (curInstrument) {		
		case 0:
			ST7735_OutString("Trumpet");
			break;
		case 1:
			ST7735_OutString("Oboe");
			break;
		case 2:
			ST7735_OutString("Bassoon");
			break;
		case 3:
			ST7735_OutString("Wave");
			break;
	}
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
	PF1 = 2; 
	
	melodyBeats++;													// keeps track of how long current note has been held
	harmonyBeats++;
	
	if ((melodyNoteIdx >= NUM_MELODY_NOTES) || (harmonyNoteIdx >= NUM_HARMONY_NOTES)) {	
		melodyInstrOut = 0;										// stop playing sound when song ends
		harmonyInstrOut = 0;
		isSongEnd = 1; 
	}
		
	if (melodyBeats >= melodyNoteLen) {
		melodyNoteIdx += 1;										// get next note in song
		melodyInstrIdx = 0;
		melodyBeats = 0;											// prep for new note
		melodyNoteLen = promenade.melody[melodyNoteIdx].length;					// change to next note length
		Timer1A_ChangePeriod(promenade.melody[melodyNoteIdx].pitch); 		// change to next note pitch
	}
	
	if (harmonyBeats >= harmonyNoteLen) {			// need to proceed to next note
		harmonyNoteIdx += 1;										// get next note in song
		harmonyInstrIdx = 0;
		harmonyBeats = 0;												// prep for new note
		harmonyNoteLen = promenade.harmony[harmonyNoteIdx].length;				// change to next note length
		Timer2A_ChangePeriod(promenade.harmony[harmonyNoteIdx].pitch); 		// change to next note pitch
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
	PF2 = 4;

	TIMER1_ICR_R = TIMER_ICR_TATOCINT;			// acknowledge TIMER1A timeout
	uint16_t output; 
	
	if (!isSongEnd) {																				// only play sound when song hasn't ended
		melodyInstrOut = promenade.instrument[melodyInstrIdx];
		melodyInstrIdx = (melodyInstrIdx + 1) % LEN_WAVE;			// play instrument voice
		output = (melodyInstrOut + harmonyInstrOut) / 2;			// combine melody + harmony waveforms (divide to avoid clipping) 
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
	PF3 = 8;

  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout	
	uint16_t output; 
		
	if (!isSongEnd) {																				// only play sound if song hasn't ended
		harmonyInstrOut = promenade.instrument[harmonyInstrIdx];
		harmonyInstrIdx = (harmonyInstrIdx + 1) % LEN_WAVE;		// play instrument voice
		output = (melodyInstrOut + harmonyInstrOut) / 2;			// combine melody + harmony waveforms (divide to avoid clipping)
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
