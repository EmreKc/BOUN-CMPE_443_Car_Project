#include "External.h"
#include "../Parameters.h"

void External_Init() {
	//Change the functionality of the push button as EINT0
	uint32_t val = IOCON_PUSH_BUTTON;
	val |=(1<<0);
	val &=~(1<<2 | 1<<1);
	IOCON_PUSH_BUTTON=val;
	
	
	//Change the External interrupt mode as Edge Sensitive
	EXT->EXTMODE |=(1<<0);
	
	
	//Enable interrupt for EINT0_IRQn.
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_SetPriority(EINT0_IRQn, 3);
	
}

void EINT0_IRQHandler() {
	CONTROL_START_MODE=1;
	EXT->EXTINT|= 1;
	if(START_MODE==0){
		START_MODE=1;
	}else{
		START_MODE=0;
	}
	//Clear interrupt for EINT0
	NVIC_ClearPendingIRQ(EINT0_IRQn);
}
