/*	Author: Zergio Ruvalcaba (zruva002@ucr.edu)
 *	Lab Section:23
 *	Assignment: Lab #8  Exercise #1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/8QkCdTbstG0
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if(!frequency)
            TCCR3B &= 0x08;
        else
            TCCR3B |= 0x03;

        if(frequency < 0.954)
            OCR3A = 0xFFFF;
        else if (frequency > 31250)
            OCR3A = 0x0000;
        else
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

enum SM1_States {SM1_Init, SM1_Wait, SM1_C, SM1_D, SM1_E} SM1_State;

void TickFctThreeTones(){
	unsigned char buttons = ~PINA;
	double C_freq = 261.63;
	double D_freq = 293.66;
	double E_freq = 329.63;

	switch (SM1_State){
		case SM1_Init: //transitions
			PORTB = 0;
			SM1_State = SM1_Wait;
			break;

		case SM1_Wait:
			if (buttons == 0x00){
				SM1_State = SM1_Wait;
			}

			if (buttons == 0x01){
				SM1_State = SM1_C;
			}

			if (buttons == 0x02){
				SM1_State = SM1_D;
			}

			if (buttons == 0x04){
				SM1_State = SM1_E;
			}

			break;

		case SM1_C:
			if (buttons == 0x01){
				SM1_State = SM1_C;
			}

			else{
				SM1_State = SM1_Wait;
			}
			break;

		case SM1_D:
			if (buttons == 0x02){
				SM1_State = SM1_D;
			}

			else{
				SM1_State = SM1_Wait;
			}
			break;

		case SM1_E:
			if (buttons == 0x04){
				SM1_State = SM1_E;
			}
			
			else{
				SM1_State = SM1_Wait;
			}
			break;

		default:
			SM1_State = SM1_Wait;
			break;
	}

	switch (SM1_State){ //actions
		case SM1_Init:
			break;

		case SM1_Wait:
			set_PWM(0);
			break;

		case SM1_C:
			set_PWM(C_freq);
			break;

		case SM1_D:
			set_PWM(D_freq);
			break;

		case SM1_E:
			set_PWM(E_freq);
			break;

		default:
			break;
	
	}


}


int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	/* Insert your solution below */
	PWM_on();
	while (1) {
		TickFctThreeTones();
	}
	return 1;
}
