#include "ADC.h"

uint32_t ADC_Last = 0;
uint8_t ADC_New_Data_Available = 0;

void ADC_Init() {
	//Change the function value of pin to ADC.
	uint32_t v = ANALOG_PIN_IOCON;
	v |= (1<<0);
	v &= ~((1<<1) | (1<<2));
	ANALOG_PIN_IOCON = v;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	ANALOG_PIN_IOCON &= ~(1<<3|1<<4);
	
	//Change Analog/Digital mode of pin to Analog.
	ANALOG_PIN_IOCON &= ~(1<<7);
	
	
	//Change the function value of pin to ADC.
	uint32_t val = ANALOG_PIN_IOCON3;
	val |= (1<<0);
	val &= ~((1<<1) | (1<<2));
	ANALOG_PIN_IOCON3 = val;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	ANALOG_PIN_IOCONTrim &= ~(1<<3|1<<4);
	
	//Change Analog/Digital mode of pin to Analog.
	ANALOG_PIN_IOCONTrim &= ~(1<<7);
	
		//Change the function value of pin to ADC.
	uint32_t val2 = ANALOG_PIN_IOCONTrim;
	val2 |= (1<<0);
	val2 &= ~((1<<1) | (1<<2));
	ANALOG_PIN_IOCON3 = val2;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	ANALOG_PIN_IOCON3 &= ~(1<<3|1<<4);
	
	//Change Analog/Digital mode of pin to Analog.
	ANALOG_PIN_IOCON3 &= ~(1<<7);
	//Turn on ADC.
	
	//Turn on ADC.
	PCONP |=(1<<12);
	
	//Set the CLKDIV and make the A/D converter operational without Burst mode.
	uint32_t val1 = ADC->CR;
	val1 |= ((1<<14) |(1<<13) |(1<<12) |(1<<9)|(1<<21));
	val1 &= ~((1<<15) |(1<<11) |(1<<10) |(1<<8)|(1<<16));
	ADC->CR = val1;
	
}

void ADC_Start() {
	//Write a code for starting A/D conversion
	uint32_t val=ADC->CR;
	val|=(1<<24);
	val &= ~(1<<25|1<<26);
	ADC->CR=val;
}

void ADC_Stop() {
	//Write a code for stopping A/D conversion
	ADC->CR &=~(1<<24|1<<25|1<<26);
}

uint32_t ADC_Read() {
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	uint32_t val = ADC->CR;
	val |= (1<<2);
	val &= ~((1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7));
	ADC->CR = val;
	
	
	ADC_Start();
	
	//Wait until A/D conversion completed
	
	while(!(ADC->STAT&4)){}
	
	//Convert the data RESULT to 0 - 1000 range and return the ADC data
		uint32_t val1=0;
		val1 |= (ADC_MAX_VALUE<<4);
		data=((ADC->DR[2]& val1)>>4);
		data=(data*1000)/ADC_MAX_VALUE;
	
	ADC_Stop();
	
	return data;
}



uint32_t ADC_Read3() {
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	uint32_t val = ADC->CR;
	val |= (1<<3);
	val &= ~((1<<0) | (1<<1) | (1<<2) | (1<<4) | (1<<5) | (1<<6) | (1<<7));
	ADC->CR = val;
	
	
	ADC_Start();
	
	//Wait until A/D conversion completed
	
	while(!(ADC->STAT&8)){}
	
	//Convert the data RESULT to 0 - 1000 range and return the ADC data
	uint32_t val1=0;
	val1 |= (ADC_MAX_VALUE<<4);
	data=((ADC->DR[3]& val1)>>4);
	data=(data*1000)/ADC_MAX_VALUE;
	
	ADC_Stop();
	
	return data;
}
uint32_t ADC_ReadTrim() {
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	uint32_t val = ADC->CR;
	val |= (1<<0);
	val &= ~((1<<3) | (1<<1) | (1<<2) | (1<<4) | (1<<5) | (1<<6) | (1<<7));
	ADC->CR = val;
	
	
	ADC_Start();
	
	//Wait until A/D conversion completed
	
	while(!(ADC->STAT&1)){}
	
	//Convert the data RESULT to 0 - 100 range and return the ADC trim data
	uint32_t val1=0;
	val1 |= (ADC_MAX_VALUE<<4);
	data=((ADC->DR[0]& val1)>>4);
	data=(data*100)/ADC_MAX_VALUE;
	
	ADC_Stop();
	
	return data;
}

