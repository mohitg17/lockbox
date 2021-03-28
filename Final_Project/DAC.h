// DAC.h
// Amy Lee
// 03/06/21
// Contains DAC function declarations/prototypes
// Lab 5
// Blake Margolis
// 03/06/21

#include <stdint.h>

#ifndef __DAC_H
#define __DAC_H

//==========================
// PORT D Initialization
//==========================
// PD7 = U2TX
// PD6 = U2RX
// PD3 = SSI1_MOSI (to TLV5616)
// PD2 = AIN5 (Monitor DAC_OUT signal from TLV5616)
// PD1 = SSI1_FS/CS (to TLV5616)
// PD0 = SSI1_SCK (to TLV5616)
//--------------------------
void Port_D_Init(void);

//==========================
// DAC_Init
//==========================
// Initialize TLV5616 12-bit DAC
// assumes bus clock is 80 MHz
// inputs: initial voltage output (0 to 4095)
// outputs: none
//--------------------------
void DAC_Init(uint16_t data);

//==========================
// DAC_Out 
//==========================
// Send data to TLV5616 12-bit DAC
// inputs: voltage output (0 to 4095)
// outputs: return parameter from SSI (not meaningful)
//--------------------------
void DAC_Out(uint16_t code);

#endif
