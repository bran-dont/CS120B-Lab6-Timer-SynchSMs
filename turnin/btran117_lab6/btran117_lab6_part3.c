/*	Author: Brandon Tran
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: (Challenge) (from an earlier lab) Buttons are connected to PA0 and PA1.
 *	Output for PORTB is initially 7. Pressing PA0 increments PORTB once (stopping at 9). Pressing
 *	PA1 decrements PORTB once (stopping at 0). If both buttons are depressed (even if not initially
 *	simultaneously), PORTB resets to 0. Now that we have timing, only check to see if a button has 
 *	been pressed every 100 ms. Additionally, if a button is held, then the count should continue to 
 *	increment (or decrement) at a rate of once per second. 
 *	Video Demonstration: http://youtu.be/D33pn3TcjpM
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *	Demo Link: https://youtu.be/RQXhjKpBiAc
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum State {oo, oi, io, ii} state; // state = (PA1)(PA0)
unsigned char count;
unsigned char button;

void tick() {
	button = ~PINA & 0x03;
    // no state actions
    switch (state) { // state transitions
        case oo:
            // no change for 0x00
            if(button == 0x01) { 
                count = (count < 9 ? count + 1 : count); 
                state = oi; 
            }
            if(button == 0x02) { 
                count = (count > 0 ? count - 1 : count); 
                state = io; 
            }
            if(button == 0x03) { count = 0; state = ii; }
            break;
        case oi:
            if(button == 0x00) { state = oo; }
            if(button == 0x01) { count = (count < 9 ? count + 1 : count); }
            if(button == 0x02) {
                count = (count > 0 ? count - 1 : count); 
                state = io; 
            }
            if(button == 0x03) { count = 0; state = ii; }
            break;
        case io:
            if(button == 0x00) { state = oo; }
            if(button == 0x01) {
                count = (count < 9 ? count + 1 : count); 
                state = oi; 
            }
            if(button == 0x02) { count = (count > 0 ? count - 1 : count); }
            if(button == 0x03) { count = 0; state = ii; }
            break;
        case ii:
            if(button == 0x00) { state = oo; }
            if(button == 0x01) { state = oi; }
            if(button == 0x02) { state = io; }
            // no change for 0x03
            // normally should consider 11 -> 01/10 -> 11/other, but no issue here
            break;
        default:
            count = -1;
            break;
    }
    PORTB = count;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x07; // Configure port B's 8 pins as outputs
                               // Initialize output on PORTB to 0x07

    /* Insert your solution below */
    TimerSet(100);
    TimerOn();
    state = oo;
    count = 7;
    while (1) {
    	if(TimerFlag) { tick(); TimerFlag = 0; }
    }
    return 1;
}
