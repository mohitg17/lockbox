#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"
#include "../inc/ST7735.h"
#include "../inc/TExaS.h" 

#include "Music.h"
#include "DAC.h"

const uint16_t waveTest[64] = {  
  1024, 1122, 1219, 1314, 1407, 1495, 1580, 1658, 1731, 1797, 1855, 1906,1948,1981,2005,2019,2024,2019,2005,1981,1948,1906,
  1855, 1797, 1731, 1658, 1580, 1495, 1407, 1314, 1219, 1122, 1024, 926, 829, 734, 641, 553, 468, 390, 317, 251, 193, 142,
  100, 67, 43, 29, 24, 29, 43, 67, 100, 142, 193, 251, 317, 390, 468, 553, 641, 734, 829, 926
}; 
//=======================================
// TESTING SPEAKER CODE
//=======================================
uint8_t i = 0;					// Alarm1 idx

//===================
// Test_Speaker_Init
//===================
void Test_Speaker_Init(void) {
	SysTick_Init(4000);
	SysTick_Start();
}

void SysTick_Handler2(void) {
	DAC_Out(waveTest[i]);					// outputs every value in wave array
	i = ((i + 1) % 32);				// ensures index doesn't go out of bounds when note is played and it
}
