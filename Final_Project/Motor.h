// Motor.h
// Amy Lee
// 03/06/21
// Motor initializations and data function prototypes 
// Final Project
// Blake Margolis
// 03/11/21

#define PC4 (*((volatile uint32_t *)0x40006040))

void Motor_Init(void);
void Motor_Run(void);
void Motor_Stop(void);



