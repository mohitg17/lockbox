// DAC.c
// Amy Lee
// 03/06/21
// Contains DAC function implementations
// Lab 5
// Blake Margolis
// 03/11/21

#include <stdint.h>
#include "../inc/ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"

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
void Port_D_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x08; // activate port D
	while((SYSCTL_PRGPIO_R & 0x08)==0){}; // allow time for clock to stabilize
		
	// --------------- Initialize PD7 as U2TX, PD6 as U2RX -----------------------
	GPIO_PORTD_LOCK_R = 0x4C4F434B; // unlock REQUIRED for PD7
	GPIO_PORTD_CR_R |= 0xC0; // commit PD6, PD7

	GPIO_PORTD_AMSEL_R &= ~0xC0; // disable analog functionality on PD6, PD7
	GPIO_PORTD_AFSEL_R |= 0xC0; // enable alternate function on PD6, PD7
	GPIO_PORTD_DEN_R |= 0xC0; // enable digital on PD6, PD7
	// (PD6 is U2RX, PD7 is U2TX)
	GPIO_PORTD_PCTL_R =(GPIO_PORTD_PCTL_R& 0x00FFFFFF)| 0x11000000; // configure PD6, PD7 as UART
		
	// --------------- Initialize PD2 as AIN5 ----------------------------------
	GPIO_PORTD_DIR_R &= ~0x04; // make PD2 input
	GPIO_PORTD_AFSEL_R |= 0x04; // enable alternate function on PD2
	GPIO_PORTD_DEN_R &= ~0x04; // disable digital I/O on PD2
	GPIO_PORTD_AMSEL_R |= 0x04; // enable analog functionality on PD2
		
	// --------------- Initialize PD3,1,0 as SSI1 MOSI, FS & SCK ----------------
	GPIO_PORTD_AMSEL_R &= ~0x0B; // disable analog functionality on PD
	GPIO_PORTD_AFSEL_R |= 0x0B; // enable alt funct on PD3,1,0
	GPIO_PORTD_DEN_R |= 0x0B; // enable digital I/O on PD3,1,0
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0xFFFF0F00) + 0x00002022;
}

//==========================
// DAC_Init
//==========================
// Initialize TLV5616 12-bit DAC
// assumes bus clock is 80 MHz
// inputs: initial voltage output (0 to 4095)
// outputs: none
//--------------------------
void DAC_Init(uint16_t data) {
	SYSCTL_RCGCSSI_R |= 0x02; // activate SSI1
	while((SYSCTL_RCGCSSI_R & 0x0002) == 0){}; // ready?

	SSI1_CR1_R = 0x00000000; // disable SSI, master mode
	SSI1_CPSR_R = 0x08; // 80MHz/8 = 10 MHz SSIClk (should work up to 20 MHz)
	SSI1_CR0_R &= ~(0x0000FFF0); // SCR = 0, SPH = 0, SPO = 1 Freescale Mode
	SSI1_CR0_R += 0x40; // SPO = 1
	SSI1_CR0_R |= 0x0F; // DSS = 16-bit data
	SSI1_DR_R = data; // load 'data' into transmit FIFO
	SSI1_CR1_R |= 0x00000002; // enable SSI
}

//==========================
// DAC_Out 
//==========================
// Send data to TLV5616 12-bit DAC
// inputs: voltage output (0 to 4095)
// outputs: return parameter from SSI (not meaningful)
//--------------------------
void DAC_Out(uint16_t code){
	while((SSI1_SR_R & 0x00000002)==0){}; // wait until room in FIFO
	//SSI1_DR_R = (3.3/3) * code; // data out
	SSI1_DR_R = code; // data out
	//while((SSI1_SR_R & 0x00000004)==0){}; // wait until response	 
} 
