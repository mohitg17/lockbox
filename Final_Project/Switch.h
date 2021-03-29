// Button.h
// Amy Lee
// 03/06/21
// Switch initializations and data function prototypes 
// PC4 --> UP switch (on lab board)
// PF0,4 --> SW1,2 (on microcontroller) 
// Lab 5
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#ifndef __SWITCH_H
#define __SWITCH_H

//==========================
// Port data bit definitions
//==========================
#define PF0				(*((volatile uint32_t *)0x40025004))
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4       (*((volatile uint32_t *)0x40025040))

#define PC4				(*((volatile uint32_t *)0x40006040))

//===============================
// Timer0Arm
//-------------------------------
// Timer for de-bouncing switches
// inputs: none
// outputs: none
//===============================
static void Timer0Arm(void);

//==========================
// PORT C Initialization 
//==========================
// PC4 = UP switch
// PC5 = RIGHT switch
// PC6 = LEFT switch
// PC7 = DOWN switch
//--------------------------
void Port_C_Init(void);

//==========================================
// GPIOPortC_Handler
//------------------------------------------
// Port C ISR (PC4 and PC7)
// inputs: none
// outputs: none
//==========================================
void GPIOPortC_Handler(void);

//=====================================
// GPIOCArm
//-------------------------------------
// Arm GPIO (Port C) edge-interrupts
// inputs: none
// outputs: none
//=====================================
static void GPIOCArm(void);

//==========================
// PORT F Initialization 
//==========================
// PF0 = SW1
// PF4 = SW2
//--------------------------
void Port_F_Init(void);

//=====================================
// GPIOFArm
//-------------------------------------
// Arm GPIO (Port F) edge-interrupts
// inputs: none
// outputs: none
//=====================================
static void GPIOFArm(void);

//==========================================
// GPIOPortF_Handler
//------------------------------------------
// Port F ISR (PF4 and PF0)
// inputs: none
// outputs: none
//==========================================
void GPIOPortF_Handler(void);








#endif 
