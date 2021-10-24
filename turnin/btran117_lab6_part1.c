/*	Author: Brandon Tran
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the method defined in class. In addition to demoing your program, you will need to show that your code adheres entirely to the method with no variations.

    To clarify and match the video, the lights should turn on in the following sequence:

	PB0,PB1,PB2,PB0,PB1,PB2,PB0,PB1,PB2…

    Video Demonstration: http://youtu.be/ZS1Op26WiBM
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
unsigned char i;
unsigned char output;

void TimerISR() {
	TimerFlag = 1;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(1000);
    TimerOn();
    i = 0;
    output = 0;
    while (1) {
    	if(TimerFlag) {
			output = (i == 0 ? 0x01 : output);
			output = (i == 1 ? 0x02 : output);
			output = (i == 2 ? 0x04 : output);
		}
		PORTB = output;
    }
    return 1;
}
