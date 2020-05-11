/*	Author: lab
 *  Partner(s) Name: karen
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//demo: https://drive.google.com/file/d/1ivoCSHfNrOSHg5OGllc1eRTAGEMu1TcJ/view?usp=sharing
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum States{START, INIT, PBO, PBT} state;

void Tick()
{
	switch(state)
	{
		case START:
			state = INIT;
			PORTB = 0x00;
			break;

		case INIT:
			state = PBO;
			break;

		case PBO:
			state = PBT;
			break;

		case PBT:
			state = INIT;
			break;

	}

	switch(state)
	{
		case START:
			break;

		case INIT:
			PORTB = 0x01;
			break;

		case PBO:
			PORTB = 0x02;
			break;

		case PBT:
			PORTB = 0x04;
			break;

	}
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();

    /* Insert your solution below */
    while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
