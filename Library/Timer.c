#include "Timer.h"
uint32_t leftPressed = 0;
uint32_t rightPressed = 0;
uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint32_t capturedTime = 0;

void Timer_Init() {// initialize timer
	//Give the Correct Function Values to IOCON_OUT
	uint32_t value=IOCON_OUT;
	value |=1<<0;
	value |=1<<1;
	value &=~(1<<2);
	IOCON_OUT=value;
	//Enable Timer3
	PCONP |=1<<23;
	//Change the mode of Timer3 to Timer Mode.
	uint32_t value1=TIMER3->CTCR;
	value1 &=~(1<<0 | 1<<1);
	TIMER3->CTCR=value1;
	//Disable Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR &=~(1<<0);
	//Reset Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR |=1<<1;
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = (PERIPHERAL_CLOCK_FREQUENCY/1000000) -1;
	//Capture Rising and Falling Edge on CAP1
	TIMER3->CCR|= 3;
	//uint32_t value2=TIMER3->CCR;
	//value2 &=~(1<<1);
	//value2 |= (1<<0);
	//TIMER3->CTCR=value2;
	//Remove the reset on counters of Timer3.
	TIMER3->TCR &=~(1<<1);
	//Enable Timer3 Counter and Prescale Counter for counting.
	TIMER3->TCR |=1<<0;	
}
