#ifndef _GCC_WRAP_STDINT_H
#include <stdint.h>
#endif
#ifndef _AVR_IOXXX_H_
#include <avr/io.h>
#endif

//	margins used for determining if the controller says right or left, forward or backward
#define CH1_RIGHT_LOWER_MARGIN 3317	// 0.9 ms
#define CH1_RIGHT_UPPER_MARGIN 3686	//	1 ms
#define CH1_LEFT_LOWER_MARGIN  7273	//	2ms
#define CH1_LEFT_UPPER_MARGIN  7841	//	2.1 ms

#define CH2_BACK_LOWER_MARGIN 3317	//	0.9 ms
#define CH2_BACK_UPPER_MARGIN 3686	//	1 ms
#define CH2_FORWARD_LOWER_MARGIN 7273	//	2 ms
#define CH2_FORWARD_UPPER_MARGIN 7941	//	2.1 ms

#define NEUTRAL_LOWER_MARGIN 5160	//	1.4 ms
#define NEUTRAL_UPPER_MARGIN 5898	//	1.6 ms
#define TRUE 1
#define FALSE 0

void INT0_setup(void);
void INT1_setup(void);
void TIMER1_setup(void);
uint16_t calculate_time_interval(uint16_t start_time, uint16_t end_time);
void PORTC_clear(void);
