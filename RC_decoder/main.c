/*
* RC_decoder.c
*
* Created: 19/05/2022 14:25:19
* Author : Jelle en Arthur <3
*/

#define F_CPU 3686400L

#include "Libraries/RC_lib_aj.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint16_t ch1_end_time = 0;
volatile uint16_t ch2_start_time = 0;
volatile uint16_t ch2_end_time = 0;

volatile uint8_t ch1_time_available = FALSE;
volatile uint8_t ch2_time_available = FALSE;


int main(void)
{
	INT0_setup();
	INT1_setup();
	TIMER1_setup();
	DDRC = 0xFF;
	sei();
	while (TRUE)
	{
		if(ch1_time_available){
			
			if(ch1_end_time > NEUTRAL_LOWER_MARGIN && ch1_end_time < NEUTRAL_UPPER_MARGIN){
				PORTC_clear();
				PORTC |= ((1 << PORTC6));	//	go straight
				} else if(ch1_end_time < NEUTRAL_LOWER_MARGIN && ch1_end_time > CH1_RIGHT_LOWER_MARGIN){
				PORTC_clear();
				PORTC |= ((1 << PORTC5));	//	turn right
			} else if (ch1_end_time > NEUTRAL_UPPER_MARGIN && ch1_end_time < CH1_LEFT_UPPER_MARGIN)
			{
				PORTC_clear();
				PORTC |= ((1 << PORTC7));	//	turn left
			}
			ch1_time_available = FALSE;
		}

		if(ch2_time_available){
			
			volatile uint16_t ch2_time = calculate_time_interval(ch2_start_time, ch2_end_time);
			if(ch2_time > NEUTRAL_LOWER_MARGIN && ch2_time < NEUTRAL_UPPER_MARGIN){
				PORTC_clear();
				PORTC |= (1 << PORTC1);	//	'neutral'
				} else if(ch2_time > NEUTRAL_UPPER_MARGIN && ch2_time < CH2_FORWARD_UPPER_MARGIN){
				PORTC_clear();
				PORTC |= (1 << PORTC0);	//	go forward
				} else if(ch2_time < NEUTRAL_LOWER_MARGIN && ch2_time > CH2_BACK_UPPER_MARGIN){
				PORTC_clear();
				PORTC |= (1 << PORTC2);	//	go backward
			}
			ch2_time_available = FALSE;
		}
	}
}

ISR(INT0_vect){
	if(PIND & (1 << PIND2)){	//	if CH1 is HIGH
		TCNT1 = 0;	//	reset timer back to 0
		} else {
		ch1_time_available = TRUE;
		ch1_end_time = TCNT1;	//	store end time
	}
}

ISR(INT1_vect){
	if(PIND & (1 << PIND3)){	//	if CH2 is HIGH
		ch2_start_time = TCNT1;	//	store time CH2 went HIGH
		} else {
		ch2_time_available = TRUE;
		ch2_end_time = TCNT1;	//	store end time
	}
}
