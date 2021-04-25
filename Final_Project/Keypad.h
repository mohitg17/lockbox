// Keypad.h
// Amy Lee, Hans Robles, Mohit Gupta, Zander Tedjo
// 03/06/21
// Keypad initializations and data function prototypes 
// Final Project
// Blake Margolis
// 03/11/21

// row 1: 1, 2, 3
// row 2: 4, 5, 6
// row 3: 7, 8, 9
// row 4: *, 0, #

// col 1: 1, 4, 7, *
// col 2: 2, 5, 8, 0
// col 3: 3, 6, 9, #

// PB6 - col2
// PB5 - row1
// PB4 - col1
// PB3 - row4
// PB2 - col3
// PB1 - row3
// PB0 - row2

void GPIOPortB_Init(void(*typeKey)(char));
//void GPIOPortB_Handler(void);
//char scan(void);

//char scanKeypad(void); // blocking call


