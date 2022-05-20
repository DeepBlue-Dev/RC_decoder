/*
 * RC_decoder.c
 *
 * Created: 19/05/2022 14:25:19
 * Author : Jelle en Arthur <3
 */ 

#define F_CPU 3686400L
//	margins used for determining if the controller says right or left 
#define CH1_RIGHT_MARGIN 3317	//	0.9 ms
#define CH1_LEFT_MARGIN 7372	//	2 ms
#define CH2_BACK_MARGIN 3317	//	0.9 ms
#define CH2_FORWARD_MARGIN 7372	//	2 ms
#define NEUTRAL_LOWER_MARGIN 5160	//	1.4 ms
#define NEUTRAL_UPPER_MARGIN 5898	//	1.6 ms
#define TRUE 1
#define FALSE 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//	function prototypes
void INT0_setup(void);
void INT1_setup(void);
void TIMER1_setup(void);
uint16_t calculate_time_interval(uint16_t start_time, uint16_t end_time);
void PORTC_clear(void);

uint16_t ch1_end_time;
uint16_t ch2_start_time;
uint16_t ch2_end_time;

uint8_t ch1_time_available = FALSE;
uint8_t ch2_time_available = FALSE;


int main(void)
{
	INT0_setup();
	INT1_setup();
	TIMER1_setup();
	DDRC = 0xFF;
	
    while (TRUE) 
    {
		if(ch1_time_available){
			if(ch1_end_time > NEUTRAL_LOWER_MARGIN && ch1_end_time < NEUTRAL_UPPER_MARGIN){
				PORTC |= ((1 << PORTC6));
			} else if(ch1_end_time > CH1_LEFT_MARGIN){
				PORTC_clear();
				PORTC |= ((1 << PORTC5));
			} else if (ch1_end_time > CH1_RIGHT_MARGIN)
			{
				PORTC_clear();
				PORTC |= ((1 << PORTC7));
			} 
			ch1_time_available = FALSE;
		}

		if(ch2_time_available){
			if(ch2_end_time > NEUTRAL_LOWER_MARGIN && ch2_end_time < NEUTRAL_UPPER_MARGIN){
				PORTC_clear();
				PORTC |= (1 << PORTC1);
			} else if(ch2_end_time > CH2_FORWARD_MARGIN){
				PORTC_clear();
				PORTC |= (1 << PORTC0);
			} else if(ch2_end_time > CH2_BACK_MARGIN){
				PORTC_clear();
				PORTC |= (1 << PORTC2);
			}
			ch2_time_available = FALSE;
		}
    }
}

ISR(INT0_vect){
	if(PIND & (1 << PIND2)){	//	if CH1 is HIGH
		TCNT1 = 0;	//	reset timer back to 0
	} else {
		ch2_time_available = TRUE;
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

void PORTC_clear(void){
	PORTC = 0x00;
}
