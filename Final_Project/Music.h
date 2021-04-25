// Music.h
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/08/21
// Contains music function protoypes and music struct definitions
// Final Project
// Blake Margolis
// 03/29/21

#include <stdint.h>
#ifndef __MUSIC_H
#define __MUSIC_H

extern uint8_t isMusicPlaying; 			// for play/pause button
extern uint8_t lockStatus;

//==============================
// Note Frequencies (pitches)
//==============================
#define C_2 11945   // 65.406 Hz			
#define DF_1 11274   // 69.296 Hz			
#define D_1 10641   // 73.416 Hz			
#define EF_1 10044   // 77.782 Hz			
#define E_1 9480   // 82.407 Hz			
#define F_1 8948   // 87.307 Hz			
#define GF_1 8446   // 92.499 Hz			
#define G_1 7972   // 97.999 Hz			
#define AF_1 7525   // 103.826 Hz			
#define A_1 7102   // 110.000 Hz			
#define BF_1 6704   // 116.541 Hz			
#define B_1 6327   // 123.471 Hz			
#define C_1 5972   // 130.813 Hz			
#define DF0 5637   // 138.591 Hz			
#define D0 5321   // 146.832 Hz			
#define EF0 5022   // 155.563 Hz			
#define E0 4740   // 164.814 Hz			
#define F0 4474   // 174.614 Hz			
#define GF0 4223   // 184.997 Hz			
#define G0 3986   // 195.998 Hz			
#define AF0 3762   // 207.652 Hz			
#define A0 3551   // 220.000 Hz			
#define BF0 3352   // 233.082 Hz			
#define B0 3164   // 246.942 Hz			
#define C0 2986   // 261.626 Hz			
#define DF 2819   // 277.183 Hz			
#define D 2660   // 293.665 Hz			
#define EF 2511   // 311.127 Hz			
#define E 2370   // 329.628 Hz			
#define F 2237   // 349.228 Hz			
#define GF 2112   // 369.994 Hz			
#define G 1993   // 391.995 Hz			
#define AF 1881   // 415.305 Hz			
#define A 1776   // 440.000 Hz			
#define BF 1676   // 466.164 Hz			
#define B 1582   // 493.883 Hz			
#define C 1493   // 523.251 Hz			
#define DF1 1409   // 554.365 Hz			
#define D1 1330   // 587.330 Hz			
#define EF1 1256   // 622.254 Hz			
#define E1 1185   // 659.255 Hz			
#define F1 1119   // 698.456 Hz			
#define GF1 1056   // 739.989 Hz			
#define G1 997   // 783.991 Hz			
#define AF1 941   // 830.609 Hz			
#define A1 888   // 880.000 Hz			
#define BF1 838   // 932.328 Hz			
#define B1 791   // 987.767 Hz			
#define C1 747   // 1046.502 Hz			
#define DF2 705   // 1108.731 Hz			
#define D2 665   // 1174.659 Hz			
#define EF2 628   // 1244.508 Hz			
#define E2 593   // 1318.510 Hz			
#define F2 559   // 1396.913 Hz			
#define GF2 528   // 1479.978 Hz			
#define G2 498   // 1567.982 Hz			
#define AF2 470   // 1661.219 Hz			
#define A2 444   // 1760.000 Hz			
#define BF2 419   // 1864.655 Hz			
#define B2 395   // 1975.533 Hz			
#define C2 373   // 2093.005 Hz	
#define R 0		// rest

//=====================================================
// Note Lengths
//=====================================================
#define BEAT_CYCLES 9000000
#define HALF 8
#define QUARTER 4
#define EIGHTH 2
#define BLANK 1							// for enveloping (rectangle)

//====================
// Song data structs
//====================
struct Note {
	uint32_t pitch;
	uint32_t length;
};

struct Song {
	struct Note *melody;								// ptr to array of Note structs
	struct Note *harmony; 							// ptr to array of Note structs
};

//============================================
// Music_Init
//--------------------------------------------
// Initializes music to start @ beginning
// input(s): none
// output(s): none
//============================================
void Music_Init(void); 

//============================================
// Music_Play
//--------------------------------------------
// Plays song: Promenade by Mussorgsky
// input(s): none
// output(s): none
//============================================
void Music_Play(void);

//============================================
// Music_Stop
//--------------------------------------------
// Pauses song
// input(s): none
// output(s): none
//============================================
void Music_Stop(void); 

//============================================
// Music_Rewind
//--------------------------------------------
// Resets song @ the beginning
// input(s): none
// output(s): none
//============================================
void Music_Rewind(void);

//============================================
// SysTick_Init
//--------------------------------------------
// Inits SysTick timer for tempo/song control
// input(s): period
// output(s): none
//============================================
void SysTick_Init(uint32_t period);

//============================================
// SysTick_Start
//--------------------------------------------
// Arms SysTick timer
// input(s): none
// output(s): none
//============================================
void SysTick_Start(void);

//============================================
// SysTick_Stop
//--------------------------------------------
// Disarms SysTick timer
// input(s): none
// output(s): none
//============================================
void SysTick_Stop(void); 

//=====================================================
// Timer1A_Init
//-----------------------------------------------------
// Initialize TIMER1A interrupts to play melody note
// Inputs: period (note freq)
// Outputs: none
//=====================================================
void Timer1A_Init(uint32_t period);

//=====================================================
// Timer1A_Start
//-----------------------------------------------------
// Activate Timer1A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer1A_Start(void);

//=====================================================
// Timer1A_Stop
//-----------------------------------------------------
// De-activate TIMER1A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer1A_Stop(void);

//==========================================
// Timer1A_ChangePeriod
//------------------------------------------
// Changes Timer1A interrupt period to play
// next note in melody
// inputs: note freq
// outputs: none
//==========================================
void Timer1A_ChangePeriod(uint32_t note);

//=====================================================
// Timer2A_Init
//-----------------------------------------------------
// Initialize Timer2A interrupts to play harmony note
// Inputs: period (note freq)
// Outputs: none
//=====================================================
void Timer2A_Init(uint32_t period);

//=====================================================
// Timer2A_Start
//-----------------------------------------------------
// Activate Timer2A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer2A_Start(void);

//=====================================================
// Timer2A_Stop
//-----------------------------------------------------
// De-activate Timer2A interrupts
// Inputs: none
// Outputs: none
//=====================================================
void Timer2A_Stop(void);

//==========================================
// Timer2A_ChangePeriod
//------------------------------------------
// Changes Timer2A interrupt period to play
// next note in harmony
// inputs: note freq
// outputs: none
//==========================================
void Timer2A_ChangePeriod(uint32_t note);

//==========================================
// setLockStatus
//------------------------------------------
// Changes lockStatus
// inputs: status
// outputs: none
//==========================================
void setLockStatus(uint8_t status);


#endif
