/*
 * RC_decoder.c
 *
 * Created: 19/05/2022 14:25:19
 * Author : Jelle en Arthur <3
 */ 

#define F_CPU 3686400L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void INT0_setup(void);
void INT1_setup(void);
void TIMER1_setup(void);
uint16_t calculate_time_interval(uint16_t start_time, uint16_t end_time);

uint16_t ch1_end_time;
uint16_t ch2_start_time;
uint16_t ch2_end_time;


int main(void)
{
	INT0_setup();
	INT1_setup();
	TIMER1_setup();
	
    while (1) 
    {
		
    }
}

ISR(INT0_vect){
	if(PIND & (1 << PIND2)){	//	if CH1 is HIGH
		TCNT1 = 0;	//	reset timer back to 0
	} else {
		ch1_end_time = TCNT1;	//	store end time
	}
}

ISR(INT1_vect){
	if(PIND & (1 << PIND3)){	//	if CH2 is HIGH
		ch2_start_time = TCNT1;	//	store time CH2 went HIGH
	} else {
		ch2_end_time = TCNT1;	//	store end time
	}
}

void INT0_setup(void){
		
	//	INT0, trigger on rising and falling flank, interrupt enabled
	EICRA |= (1 << ISC00);
	EICRA &= ~(1 << ISC01);
	EIMSK |= (1 << INT0);
}

void INT1_setup(void){
	
	//	INT1, trigger on rising and falling flank, interrupt enabled
	EICRA |= (1 << ISC10);
	EICRA &= ~(1 << ISC11);
	EIMSK |= (1 << INT1);
}

void TIMER1_setup(void){
	
	//	timer1 mode 0, no prescaler, no interrupts
	TCCR1B &= ~((1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS12));
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
	TCCR1B |= (1 << CS10);
}

uint16_t calculate_time_interval(uint16_t start_time, uint16_t end_time){
	return (end_time - start_time);	//	calculates difference between two timestamps
}