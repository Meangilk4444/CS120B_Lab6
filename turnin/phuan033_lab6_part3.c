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


enum States{START, INIT, IN, DE, WAIT_IN, WAIT_DE, RESET } state;
unsigned char button;
unsigned cnt = 0x00;
unsigned light = 0x00;
void Tick()
{
//	button = (~PINA & 0x03);

	switch(state)
	{
		case START:
			PORTB = 7;
			cnt = 0x00;
			state = INIT;
			break;

		case INIT:
			if(button == 0x01)
			{
				state = IN;
			}
			else if(button == 0x02)
			{
				state = DE;
			}
			else if(button == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = INIT;
			}

			break;

		case IN:
			state = WAIT_IN;
			break;

		case DE: 
			state = WAIT_DE;
			break;

		case WAIT_IN:
			if(button == 0x01)
			{ 
				if(cnt >= 10 && PORTB < 9)
				{
					PORTB = PORTB + 1;
					cnt = 0;
				}
				cnt++;
			}
			else
			{
				state = INIT;
			}
			
			break;

		case WAIT_DE:
			if(button == 0x02)
			{
				if(cnt >= 10 && PORTB > 0)
                                {
                                        PORTB = PORTB - 1;
                                        cnt = 0;
                                }
                                cnt++;


			}
			else
			{
				state = INIT;
			}
			break;

		case RESET:
			state = START;


		default:
			state = START;
			break;
	}

	switch(state)
	{
		case START:
			break;

		case IN:
			if(PORTB < 9)
			{
				PORTB = PORTB + 1;
			}
			break;

		case DE:
			if(PORTC > 0)
			{
				PORTB =PORTB - 1;
			}
			break;

		case RESET:
			PORTB = 7;
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
	    button = ~PINA & 0x03;
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}

