#include "RC_lib_aj.h"
void INT0_setup(void) {

  //	INT0, trigger on rising and falling flank, interrupt enabled
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
}

void INT1_setup(void) {

  //	INT1, trigger on rising and falling flank, interrupt enabled
  EICRA |= (1 << ISC10);
  EICRA &= ~(1 << ISC11);
  EIMSK |= (1 << INT1);
}

void TIMER1_setup(void) {

  //	timer1 mode 0, no prescaler, no interrupts
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS12));
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
  TCCR1B |= (1 << CS10);
}

uint16_t calculate_time_interval(uint16_t start_time, uint16_t end_time) {
  return (end_time -
          start_time); //	calculates difference between two timestamps
}

void PORTC_clear(void) {
  PORTC = 0x00; //	set all outputs on PORTC low
}