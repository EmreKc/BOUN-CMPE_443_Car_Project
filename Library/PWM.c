#include "PWM.h"
//#define IOCON_LED_ADDRESS	0x4002C088
//#define IOCON_LED1	*((volatile uint32_t*)(IOCON_LED_ADDRESS))

void PWM_Init() {// initialize pwm
	//Change the function of the pin in here:
	uint32_t value = IOCON_MOTOR_SPEED ;
	value |= (1<<0 | 1<<1);
	value &= ~(1<<2);
	IOCON_MOTOR_SPEED = value;
	
	uint32_t value1 = IOCON_MOTOR_SPEED_RIGHT ;
	value1 |= (1<<0 | 1<<1);
	value1 &= ~(1<<2);
	IOCON_MOTOR_SPEED_RIGHT = value1;
	
	PCONP |= 1 << 5;
	
	//Enable PWM output for corresponding pin.
	PWM0 -> PCR  |= (1<<10);
	PWM0 -> PCR  |= (1<<9);
	//Reset The PWM Timer Counter and The PWM Prescale Counter on the Next Positive Edge of PCLK
	PWM0 -> TCR |= (1<<1);
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20 * 1000;
	
	//Reset TC, when MR0 matches TC.
	PWM0 -> MCR |= (1<<1);
	//Enable PWM Match 0 Latch.
	PWM0-> LER |= (1<<0); //uyari
	//Enable Counter and PWM and Clear Reset on the PWM
	PWM0-> TCR |=(1<<0|1<<3);
	PWM0 -> TCR &= ~(1<<1);
	PWM_Write(0);
}
void PWM_Cycle_Rate(uint32_t period_In_Cycles) { // change cycle rate with given number
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * period_In_Cycles * 1000;
	
	//Enable PWM Match 0 Latch.
	PWM0 -> LER |= (1<<0);

}

void PWM_Write(uint32_t T_ON) {	// changes speed of left motor
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWM0->MR0) * T_ON) / 100);
	
	if (T_ON == PWM0->MR0) {
		T_ON++;
	}
	PWM0->MR2 = T_ON;
	
	//Enable PWM Match Register Latch.
	PWM0 -> LER |= (1<<2);
}
void PWM_Write_Right(uint32_t T_ON) {	// changes speed of right motor
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWM0->MR0) * T_ON) / 100);
	
	if (T_ON == PWM0->MR0) {
		T_ON++;
	}
	PWM0->MR1 = T_ON;
	
	//Enable PWM Match Register Latch.
	PWM0 -> LER |= (1<<1);
}
























