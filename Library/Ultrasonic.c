#include "Ulltrasonic.h"

void Ultrasonic_Trigger_Timer_Init() {
	//Give the Correct Function Values to IOCON_TRIGGER
	uint32_t val= IOCON_TRIGGER;
	val |= (1<<0|1<<1);
	val &= ~(1<<2);
	IOCON_TRIGGER=val;
	//Enable Timer2.
	PCONP |=(1<<22);
	
	//Change the mode of Timer2 to Timer Mode.
	TIMER2->CTCR &= ~(1<<0|1<<1);
	
	//Reset Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |=(1<<1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER2->PR = (PERIPHERAL_CLOCK_FREQUENCY/1000000)-1;
	
	//Remove the reset on counters of Timer2.
	TIMER2->TCR &=~(1<<1);
	
}

void Ultrasonic_Capture_Timer_Init() {
	//Give the Correct Function Values to IOCON_ECHO
	uint32_t val= IOCON_ECHO;
	val |= (1<<0|1<<1);
	val &= ~(1<<2);
	IOCON_ECHO=val;
	
	PCONP |= 1 << 23;
	
	TIMER3->CTCR = 0x00;
	
	TIMER3->TCR &= ~(1 << 0);
	
	TIMER3->TCR |= (1 << 1);
	
	TIMER3->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000000 - 1;
	
	//Write the Correct Value to CCR register for getting interrupt when Rising Edge Occur
	val=TIMER3->CCR;
	val |= ((1<<3) | (1<<5));
	val &= ~(1<<4);
	TIMER3->CCR = val;
	
	TIMER3->TCR &= ~(1 << 1);
	
	TIMER3->TCR |= (1 << 0);
	
	//Clear pendings for Timer3.
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
	
	//Set Priority Timer3 IRQ as 5.
	NVIC_SetPriority(TIMER3_IRQn,5);
	
	//Enable TIMER3_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER3_IRQn);
}

void Ultrasonic_Start_Trigger() {
	//Give HIGH value to T2_MAT_3 pin
	TIMER2->EMR|=(1<<3);
	
	//Calculate the MR3 register value for giving 10 microsecond HIGH value
	TIMER2->MR3=10;
	
	//Write to correct values to Timer2 EMR register for making LOW output value of Trigger Pin when match occurs.
	
	uint32_t val=TIMER2->EMR;
	val |= (1<<10);
	val &= ~(1<<11);
	TIMER2->EMR = val;
	
	//Reset TC and Stop (TC and PC), if MR3 register matches the TC.
	val=TIMER2->MCR;
	val |= (1<<11) | (1<<10);
	val &= ~(1<<9);
	TIMER2->MCR = val;
	
	//Enable Timer2 Counter and Prescale Counter for counting.
	TIMER2->TCR |= (1<<0);
	
	ultrasonicSensorEdgeCount = 0;
}

uint8_t ultrasonicSensorEdgeCount = 0;

uint32_t ultrasonicSensorRisingTime = 0;
uint32_t ultrasonicSensorFallingTime = 0;
uint32_t ultrasonicSensorDuration = 0;
uint32_t ultrasonicSensorDistance = 0;

void TIMER3_IRQHandler() {
	if(ultrasonicSensorEdgeCount == 0) {
		
		//Store the rising time into ultrasonicSensorRisingTime variable
		ultrasonicSensorRisingTime = TIMER3->CR1;
		
		//Change the CCR register value for getting interrupt when falling edge event is occured.
		uint32_t val= TIMER3->CCR;
		val = ((1<<4) | (1<<5));
		val &= ~(1<<3);
		TIMER3->CCR =val;
		
		ultrasonicSensorEdgeCount = 1;
		
		NVIC_ClearPendingIRQ(TIMER3_IRQn);
	}
	else if(ultrasonicSensorEdgeCount == 1){
		
		//Store the falling time into ultrasonicSensorFallingTime variable
		ultrasonicSensorFallingTime = TIMER3->CR1;
		
		//Change the CCR register value for getting interrupt when rising edge event is occured.
		uint32_t val= TIMER3->CCR;
		val = ((1<<3) | (1<<5));
		val &= ~(1<<4);
		TIMER3->CCR =val;
		
		ultrasonicSensorEdgeCount = 2;
		
		NVIC_ClearPendingIRQ(TIMER3_IRQn);
	}
	
	//Clear the interrupt flag for capture channel 1 event
	TIMER3->IR|=(1<<5);
}
