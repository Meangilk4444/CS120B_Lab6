/*	Author: lab
 *  Partner(s) Name: karen
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
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


enum States{START, INIT, PBO, PBT, WAIT1, WAIT2, WAIT3} state;
unsigned char button;
unsigned cnt = 0x00;
void Tick()
{
	button = (~PINA & 0x01);

	switch(state)
	{
		case START:
			state = INIT;
			PORTB = 0x00;
			break;

		case INIT:
			if(button)
			{
				cnt++;
				state = WAIT1;
			}
			else
			{
				state = PBO;
			}
			break;

		case PBO:
			if(button)
			{
				cnt++;
				state = WAIT2;
			}
			else
			{
				state = PBT;
			}
			break;

		case PBT:
			if(button)
			{
				cnt++;
				state = WAIT3;
			}
			else
			{	
				state = INIT;
			}
			break;

		case WAIT1:
			if(button)
			{
				state = WAIT1;
			}
			else
			{
				if(cnt == 0x01)
				{
					state = PBO;
				}
				else
				{
					cnt = 0x00;
					state = INIT;
				}
			}
			break;

		case WAIT2:

			if(button)
			{
				state = WAIT2;
			}
			else
			{	if(cnt == 0x01)
				{
					state = PBT;
				}
				else
				{
					cnt = 0x00;
					state = INIT;
				}
			}

			break;

		case WAIT3:

			if(button)
			{
				state = WAIT3;
			}
			else
			{
				if(cnt == 0x01)
				{
					state = INIT;
				}
				else
				{
					cnt = 0x00;
					state = INIT;
				}

			}
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
    /* Insert DDR and PORT initialization
     * s */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);
	TimerOn();

    /* Insert your solution below */
    while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
