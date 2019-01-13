#include "LPC407x_8x_177x_8x.h"
#include "Library/GPIO.h"
#include "Library/PWM.h"
#include "Library/Joystick.h"
#include "Library/Timer.h" 
#include "Parameters.h"
#include "Library/Ulltrasonic.h"
#include "Library/LED.h"
#include "Library/Wait.h"
#include "Library/ADC.h"
#include "Library/External.h"

 
void init() {
	// initialize joystick pwm and timer
	Joystick_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	ADC_Init();
	External_Init();
	//ADC_Init3();
	
	
	//burayi sonradan sil
	/*LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
	
	LED1_Off();
	LED2_Off();
	LED3_Off();
	LED4_Off();*/
	//buraya kadar
	
	//Initialize All Direction Pins That We Use In This Project
	PORT1->DIR |= (1<<24 | 1<<23 | 1<<3 | 1<<20 );
	PORT1->DIR |= (1<<20);
	PORT0->DIR |= (1<<0 | 1<<21 | 1<<1| 1<<7 | 1<<8);
	
	PWM_Init();

	PWM_Write(0);
	Timer_Init();
}

//When Joystick Left button is pressed, Motor rotation direction will be changed to forward.
//When Joystick Up button is pressed, Motor Speed will be increased.
//When Joystick Down button is pressed, Motor Speed will be decreased.
//When Joystick Center button is pressed, Motor will stop .
//When Joystick Right button is pressed, Motor rotation direction will be changed to backward.
//When Joystick no button is pressed, Motor will continue to perform the last action.
uint32_t counter=0;//this counter is for 70 degree rotation counter
uint32_t dumb =0;
uint32_t i=0;
uint32_t LDRDataValue;
uint32_t LDRDataValue3;
int turnBoolR=0;
int turnBoolL=0;
int ar=0;

void leftForward(){//left motors go forward
		uint32_t value= PORT1->PIN;
		value |= (1<<24);
		value &= ~(1<<23);
		PORT1->PIN = value;
		PWM_Write(ROBOT_SPEED);

}
void leftForwardSlow(){//left motors go forward
		uint32_t value= PORT1->PIN;
		value |= (1<<24);
		value &= ~(1<<23);
		PORT1->PIN = value;
		PWM_Write(ROBOT_SPEED/2);

}
void rightForwardSlow(){//right motors go forward
		uint32_t value= PORT0->PIN;
		value |= (1<<7);
		value &= ~(1<<8);
		PORT0->PIN = value;	
		PWM_Write_Right(ROBOT_SPEED/2);

}
void rightForward(){//right motors go forward
		uint32_t value= PORT0->PIN;
		value |= (1<<7);
		value &= ~(1<<8);
		PORT0->PIN = value;	
		PWM_Write_Right(ROBOT_SPEED);

}
void leftBackward(){//left motors turn backward
		uint32_t value= PORT1->PIN;
		value |= (1<<23);
		value &= ~(1<<24);
		PORT1->PIN = value;
		PWM_Write(ROBOT_SPEED);

}
void rightBackward(){//right motors turn backward
		uint32_t value= PORT0->PIN;
		value |= (1<<8);
		value &= ~(1<<7);
		PORT0->PIN = value;	
		PWM_Write_Right(ROBOT_SPEED);
}
void stop(){//stops the motor functions and closes all the leds
		uint32_t value2= PORT0->PIN;
		value2 |= (1<<8 | 1<<7);
		value2 &=~(1<<0);
		value2 &=~(1<<1);
		value2 &=~(1<<21);
		PORT0->PIN = value2;
	
		uint32_t value= PORT1->PIN;
		value |= (1<<23 | 1<<24);
		value &=~(1<<20);
		PORT1->PIN = value;
		rightPressed=0;
		leftPressed = 0;
}

void update() {
	if(CONTROL_START_MODE){
		stop();
		CONTROL_START_MODE=0;
	}
	ROBOT_SPEED=ADC_ReadTrim();
	if(!START_MODE){
		currentTime = TIMER3->TC;//find current totime to compare timer variables
		dumb = TIMER3->CR0;
		if(rightPressed!=0){//if right pressed it blinks
			if((currentTime-rightPressed)%500000<250000){
			PORT1->PIN |=(1<<20);
			PORT0->PIN |=(1<<21);
			}
				else{
					PORT1->PIN &=~(1<<20);
					PORT0->PIN &=~(1<<21);
				}
		}
			if(TURN_LEFT_FLAG!=0 || TURN_RIGHT_FLAG!=0){//if right or left pressed and then nothing is pressed increment counter
				if(((TIMER3->CR0)>previousTime)&&(counter<ROTATION_NUMBER_FOR_90_DEGREE)){
					counter++;
					previousTime=TIMER3->CR0;
				}
				else if(((TIMER3->CR0)==previousTime)&&(counter>=ROTATION_NUMBER_FOR_90_DEGREE)) {
					stop();
					counter=0;
				}
				previousTime=TIMER3->CR0;
			}
		if(leftPressed!=0){//if left pressed it blinks
			if((currentTime-leftPressed)%500000<250000){
			PORT0->PIN |=(1<<0|1<<1);
			}
				else{
					PORT0->PIN &=~(3);
				}
		}

		if(Joystick_Left_Pressed()){//if left pressed right now 
			leftBackward();//left turns backward
			rightForward();//right goes forward
			leftPressed =TIMER3->TC;//update left pressed time
			PORT1->PIN &=~(1<<20);//change led pins
			uint32_t value1=PORT0->PIN;
			value1 &=~(1<<21);
			value1 |=1<<0;
			value1 |=(1<<1);
			PORT0->PIN=value1;
			rightPressed =0;
			TURN_LEFT_FLAG = 1;//update flags
			TURN_RIGHT_FLAG = 0;
			FORWARD_FLAG = 0;
			BACKWARD_FLAG = 0;
			counter=0;
		}
		else if(Joystick_Up_Pressed()) {//if up pressed right now 
			
			uint32_t value=PORT0->PIN;//change led pins
			value |=1<<21 | 1<<0;
			value &=~(1<<1);
			PORT0->PIN=value;
			leftForward();//left and right go forward
			rightForward();
			PORT1->PIN &=~(1<<20);
			leftPressed =0;
			rightPressed =0;
			TURN_LEFT_FLAG = 0;//update flag
			TURN_RIGHT_FLAG = 0;
			FORWARD_FLAG = 1;
			BACKWARD_FLAG = 0;
			counter=0;
		}
		else if(Joystick_Down_Pressed()){// if down pressed right now
			
			uint32_t value=PORT0->PIN;//change the led pins
			value &=~(1<<0);
			value |=1<<1;
			value &=~(1<<21);
			
			PORT0->PIN=value;
			leftBackward();//left and right go backward
			rightBackward();
			PORT1->PIN |=(1<<20);
			leftPressed =0;
			rightPressed =0;
			TURN_LEFT_FLAG = 0;//update turn flag
			TURN_RIGHT_FLAG = 0;
			FORWARD_FLAG = 0;
			BACKWARD_FLAG = 1;
			counter=0;
		}
		else if(Joystick_Center_Pressed()){//if center pressed right now
			stop();// stop the motor
			TURN_LEFT_FLAG = 0;// update flag
			TURN_RIGHT_FLAG = 0;
			FORWARD_FLAG = 0;
			BACKWARD_FLAG = 0;
			
			leftPressed =0;
			rightPressed =0;
			counter=0;
		}
		else if (Joystick_Right_Pressed()){// if right pressed right now
			/*uint32_t value= PORT1->PIN;
			value |= (1<<23);
			value &= ~(1<<24);
			PORT1->PIN = value;*/
			
			leftForward();//left goes forward right goes backward
			rightBackward();
			rightPressed=TIMER3->TC;//update right pressed value
			PORT1->PIN |=(1<<20);
			uint32_t value1=PORT0->PIN;// change led pins
			value1 &=~(1<<0);
			value1 |=1<<21;
			value1 &=~(1<<1);
			PORT0->PIN=value1;
			leftPressed =0;
			TURN_LEFT_FLAG = 0;// update flag
			TURN_RIGHT_FLAG = 1;
			FORWARD_FLAG = 0;
			BACKWARD_FLAG = 0;
			counter=0;
		}
		
		/*LDRDataValue=ADC_Read();
		if(LDRDataValue>950){
			if(FORWARD_FLAG&&turnBoolL){
				leftForward();
				turnBoolL=0;
			}
		}else if(LDRDataValue<770){
			//turn left
			leftForwardSlow();
			turnBoolL=1;
			

		}
		LDRDataValue3=ADC_Read3();
		
		if(LDRDataValue3>950){
			if(FORWARD_FLAG&&turnBoolR){
				rightForward();
				turnBoolR=0;
			}
		}else if(LDRDataValue3<770){
			//turn right
			rightForwardSlow();

			turnBoolR=1;
		}*/
	}else if(START_MODE){
		if(Joystick_Up_Pressed()) {//if up pressed right now 	
			uint32_t value=PORT0->PIN;//change led pins
			value |=1<<21 | 1<<0;
			value &=~(1<<1);
			PORT0->PIN=value;
			leftForward();//left and right go forward
			rightForward();
			PORT1->PIN &=~(1<<20);
			leftPressed =0;
			rightPressed =0;
			TURN_LEFT_FLAG = 0;//update flag
			TURN_RIGHT_FLAG = 0;
			FORWARD_FLAG = 1;
			BACKWARD_FLAG = 0;
			counter=0;
		}
		
				LDRDataValue=ADC_Read();
		if(LDRDataValue>950){
			if(FORWARD_FLAG&&turnBoolL){
				leftForward();
				turnBoolL=0;
			}
		}else if(LDRDataValue<770){
			//turn left
			leftForwardSlow();
			turnBoolL=1;
			/*LED1_On();
			LED2_On();
			LED3_Off();
			LED4_Off();*/

		}
		LDRDataValue3=ADC_Read3();
		
		if(LDRDataValue3>950){
			if(FORWARD_FLAG&&turnBoolR){
				rightForward();
				turnBoolR=0;
			}
		}else if(LDRDataValue3<770){
			//turn right
			rightForwardSlow();
			/*LED3_On();
			LED4_On();
			LED1_Off();
			LED2_Off();*/
			turnBoolR=1;
		}

		
		
	}
	if(FORWARD_FLAG){
		Ultrasonic_Start_Trigger();
		ultrasonicSensorDistance=(ultrasonicSensorFallingTime-ultrasonicSensorRisingTime)/58;
		wait(200);
		
		if(ultrasonicSensorDistance<15){
			leftBackward();
			rightBackward();
			i=1;
			//LED_ON();
		}else if(ultrasonicSensorDistance>30&&i==1){
			i=0;
			leftForward();
			rightForward();
			//LED3_Off();
			//LED4_Off();
		}
	
	}
	
}
 
int main() {
    init();
 
    while(1) {
        update();
    }
}
