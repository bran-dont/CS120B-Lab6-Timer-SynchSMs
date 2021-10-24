/*	Author: Brandon Tran
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: Create a synchSM to blink three LEDs connected to PB0, 
 *		PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the 
 *		method defined in class. In addition to demoing your program, you will need to show 
 *		that your code adheres entirely to the method with no variations.
 *	
 *		To clarify and match the video, the lights should turn on in the following sequence:
 *	
 *		PB0,PB1,PB2,PB0,PB1,PB2,PB0,PB1,PB2…
 *	
 *		Video Demonstration: http://youtu.be/ZS1Op26WiBM
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.
unsigned char i, output, input, direction; // direction: 0 = forward, 1 = backward
enum States { Seq_P, Seq_R, Stay_P, Stay_R } state;

void TimerISR() {
	TimerFlag = 1;
}

void Tick() {
	input = PORTA & 0x01;
	
	switch (state) {
		case Seq_P:
			if(i == 0) { 
				output = 0x01; 
				direction = 0;
				i++;
			}
			else if(i == 1) { 
				output = 0x02; 
				i = (direction == 0 ? i + 1 : i - 1);
			}
			else if(i == 2) { 
				output = 0x04; 
				direction = 1;
				i--;
			}
			state = (input == 1 ? Seq_P : Seq_R);
		break;
		
		case Seq_R:
			if(i == 0) { 
				output = 0x01; 
				direction = 0;
				i++;
			}
			else if(i == 1) { 
				output = 0x02; 
				i = (direction == 0 ? i + 1 : i - 1);
			}
			else if(i == 2) { 
				output = 0x04; 
				direction = 1;
				i--;
			}
			state = (input == 1 ? Stay_P : Seq_R);
		break;
		
		case Stay_P:
			if(i == 0) { output = 0x01; }
			else if(i == 1) { output = 0x02; }
			else if(i == 2) { output = 0x04; }
			state = (input == 1 ? Stay_P : Stay_R);
		break;
		
		case Stay_R:
			if(i == 0) { output = 0x01; }
			else if(i == 1) { output = 0x02; }
			else if(i == 2) { output = 0x04; }
			if(input == 1) {
				state = Seq_P;
				i = 0;
				direction = 0;
			}
			else { state = Stay_R; }
		break;
		
		PORTB = output;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(300);
    TimerOn();
    i = 0;
    output = 0;
    state = Seq_R;
    while (1) {
		if(TimerFlag == 1) { Tick(); }
    }
    return 1;
}
