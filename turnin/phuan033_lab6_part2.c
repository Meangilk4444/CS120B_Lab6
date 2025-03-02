/*	Author: lab
 *  Partner(s) Name: karen
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//demo: https://drive.google.com/open?id=1isTZWNl5idhiwb9nmJRwENmz68rFNf19
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


enum States{START, SEQUENCE,Wait_S, WAIT, WAITING_STATE, WAIT2 } state;
unsigned char button;
unsigned cnt = 0x00;
unsigned light = 0x00;
void Tick()
{
//	button = (~PINA & 0x01);

	switch(state)
	{
		case START:
			state = SEQUENCE;
		//	PORTB = 0x00;
		//	cnt = 0x00;
			break;

		case SEQUENCE:
			if(button)
			{
				state = WAIT;
			}
			else
			{
				if(cnt == 12)
				{	cnt = 0x00;
					state = Wait_S;
				}
				else
				{
					state = SEQUENCE;
				}
			}

		//	state = Wait_S;
			break;

		case Wait_S:
			state = SEQUENCE;
			break;

		case WAIT:
			if(button)
			{
				state = WAIT;
			}
			else
			{
				state = WAITING_STATE;
			}
			break;

		case WAITING_STATE:
			if(button)
			{
				state = WAIT2;
			}
			else
			{
				state = WAITING_STATE;
			}

			break;

		case WAIT2:
			if(button)
			{
				state = START;
			}
			else
			{
				state = WAIT2;
			}
			break;

		default:
			state = START;
			break;
	}

	switch(state)
	{
		case START:
			PORTB = 0x00;
			cnt = 0x00;
			break;

		case SEQUENCE:
			if(cnt >= 0 && cnt < 3)
			{
				PORTB = 0x01;
			//	cnt++;
			}
			else if(cnt >= 3 && cnt <= 5)
			{
				PORTB = 0x02;
			//	cnt++;
			}
			else if(cnt >= 6 && cnt <= 8)
			{
				PORTB = 0x04;
				
			}
			else if(cnt >= 9 && cnt <= 11)
			{
				PORTB = 0x02;
			}

			cnt++;

	
			
			break;	

		case WAITING_STATE:
			cnt = 0x00;
			break;


	}
}



int main(void) {
    /* Insert DDR and PORT initialization
     * s */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();

    /* Insert your solution below */
    while (1) {
	    button = ~PINA & 0x01;
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
