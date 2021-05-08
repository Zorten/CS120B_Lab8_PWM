/*	Author: Zergio Ruvalcaba (zruva002@ucr.edu)
 *	Lab Section:23
 *	Assignment: Lab #8  Exercise #3
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/HYYHTtvcqlI
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "../header/timer.h"
#include "stdbool.h"

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

enum SM3_States {SM3_Init, SM3_Wait, SM3_Play, SM3_Hold} SM3_State;
unsigned char i = 0;
bool mid = false;
unsigned char j = 0;

void TickFct_Melody(){
	unsigned char buttons = ~PINA;
	double D4_freq = 293.66;
	double G4_freq = 392.00;
	double A4_freq = 440.00;
	double A4s_freq = 466.16;
	double C5_freq = 523.25;
	double D5_freq = 587.33;
	double G5_freq = 783.99;
	double A5_freq = 880.00;
	double F5_freq = 698.46;
	double off = 0;

	double merry[] = {off, D4_freq, G4_freq, A4s_freq, D5_freq, D5_freq, C5_freq, A4s_freq, A4_freq, A4s_freq, G4_freq, A4s_freq, D5_freq, G5_freq, G5_freq, A5_freq, A4s_freq, F5_freq};

	switch (SM3_State){
		case SM3_Init: //transitions
			PORTB = 0;
			i = 0;
			j = 0;
			mid = false;
			SM3_State = SM3_Wait;
			break;

		case SM3_Wait:
			if (buttons == 0x00){
				SM3_State = SM3_Wait;
			}

			if (buttons == 0x01){
				SM3_State = SM3_Hold;
			}


			break;

		case SM3_Hold:
			if (buttons == 0x01){
				SM3_State = SM3_Hold;
			}
			else{
				if (!mid){
					SM3_State = SM3_Play;
				}
				else{
					SM3_State = SM3_Wait;
					mid = false;
					j = 0;
				}
			}
			break;

		
		case SM3_Play:
			SM3_State = SM3_Play;
			if (buttons == 0x01){
				j = j + 1;
				if (j >= 6){
					mid = true;
				}
			}
			break;

		default:
			SM3_State = SM3_Init;
			break;
	}

	switch (SM3_State){ //actions
		case SM3_Init:
			break;

		case SM3_Wait:
			break;

		case SM3_Play:
			if (i < 18){
				i = i + 1;
			}
			else{
				i = 0;
				SM3_State = SM3_Wait;
			}
			break;


		default:
			break;
	
	}

	set_PWM(merry[i]);
	PORTB = i;


}


int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;


	/* Insert your solution below */
	TimerSet(650);
	TimerOn();
	SM3_State = SM3_Init;
	PWM_on();
	while (1) {
		TickFct_Melody();
		while (!TimerFlag){}
		TimerFlag = 0;
	}
	return 1;
}
