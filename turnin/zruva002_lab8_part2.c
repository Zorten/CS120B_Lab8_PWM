/*	Author: Zergio Ruvalcaba (zruva002@ucr.edu)
 *	Lab Section:23
 *	Assignment: Lab #8  Exercise #2
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/FKYjNUPRKhA
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "../header/timer.h"

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

enum SM2_States {SM2_Init, SM2_Off, SM2_On, SM2_Inc, SM2_Dec} SM2_State;
unsigned char i = 0;

void TickFct_Scale(){
	unsigned char buttons = ~PINA;
	double C4_freq = 261.63;
	double D_freq = 293.66;
	double E_freq = 329.63;
	double F_freq = 349.23;
	double G_freq = 392.00;
	double A_freq = 440.00;
	double B_freq = 493.88;
	double C5_freq = 523.25;
	double off = 0;

	double notes[] = {off, C4_freq, D_freq, E_freq, F_freq, G_freq, A_freq, B_freq, C5_freq};

	switch (SM2_State){
		case SM2_Init: //transitions
			PORTB = 0;
			i = 0;
			SM2_State = SM2_Off;
			break;

		case SM2_Off:
			if (buttons == 0x00){
				SM2_State = SM2_Off;
			}

			if (buttons == 0x01){
				SM2_State = SM2_On;
				if (i < 8){
                                	i = i + 1;
                        	}
			}


			break;

		
		case SM2_On:
			if (buttons == 0x00){
				SM2_State = SM2_On;
			}

			if (buttons == 0x01){
				SM2_State = SM2_Off;
			}

			if (buttons == 0x02){
				SM2_State = SM2_Inc;
			}

			if (buttons == 0x04){
				SM2_State = SM2_Dec;
			}
			break;


		case SM2_Inc:
			SM2_State = SM2_On;
			break;

		case SM2_Dec:
			SM2_State = SM2_On;
			break;

		default:
			SM2_State = SM2_Init;
			break;
	}

	switch (SM2_State){ //actions
		case SM2_Init:
			break;

		case SM2_Off:
			i = 0;
			break;

		case SM2_Inc:
			if (i < 8){
				i = i + 1;
			}
			break;

		case SM2_Dec:
			if (i > 1){
				i = i - 1;
			}
			break;

		case SM2_On:
			break;

		default:
			break;
	
	}

	set_PWM(notes[i]);
	PORTB = i;


}


int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;


	/* Insert your solution below */
	TimerSet(100);
	TimerOn();
	SM2_State = SM2_Init;
	PWM_on();
	while (1) {
		TickFct_Scale();
		while (!TimerFlag){}
		TimerFlag = 0;
	}
	return 1;
}
