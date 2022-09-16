#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/delay.h>
unsigned char secs = 0;    //seconds of the system
unsigned char mins = 0;   // minutes of the system
unsigned char hours = 0; // hours of the system
unsigned char tick=0;   // the tick  of timer
ISR(INT0_vect) {
	secs = 0;
	mins = 0;
	hours = 0;
	tick = 0;
	// reset the7  segment
}
void INT0_INIT() {
	GICR |= (1 << INT0);
	MCUCR |= (1 << ISC01);
	sei();
}
ISR(INT1_vect) {
	TCCR1A = 0;
	TCCR1B = 0;
	// disable the clock of the timer
}
void INT1_INIT() {
	GICR |= (1 << INT1);
	MCUCR = (1 << ISC11) | (1 << ISC10);
	sei();
}
ISR(INT2_vect) {
	TCCR1A = (1 << FOC1A) | (1 << WGM12);
	TCCR1B |= (1 << CS10);
	// enable the clock of the timer
}
void INT2_INIT() {
	GICR |= (1 << INT2);
	sei();
}
void increaseNum() {
	if (secs == 59) {
		secs = 0;
		if (mins == 59) {
			mins = 0;
			if (hours == 23) {
				secs = 0;
				mins = 0;
				hours = 0;
			} else {
				hours++;
			}
		} else
			mins++;
	} else {
		secs++;
	}
}
void TIMER1_COMPA_INIT() {
	TCNT1 = 0;
	OCR1A = 50000;
	TCCR1A = (1 << FOC1A) | (1 << WGM12);
	TCCR1B |= (1 << CS10);
	TIMSK |= (1 << OCIE1A);
	sei();
}
ISR(TIMER1_COMPA_vect) {
	if (tick == 20) {
		increaseNum();
		tick = 0;
	} else
		tick++;
}
void DisplaySegment() {
	// the value must be done first to avoid rubbish values
	PORTC = (PORTC & 0xf0) | (secs % 10 & 0x0f);
	PORTA |= 0x01;
	_delay_ms(2);
	PORTA = 0;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
	PORTC = (PORTC & 0xf0) | ((int) secs / 10 & 0x0f);
	PORTA |= 0x02;
	_delay_ms(2);
	PORTA = 0;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
	PORTC = (PORTC & 0xf0) | (mins % 10 & 0x0f);
	PORTA |= 0x04;
	_delay_ms(2);
	PORTA = 0;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
	PORTC = (PORTC & 0xf0) | ((int) mins / 10 & 0x0f);
	PORTA |= 0x08;
	_delay_ms(2);
	PORTA = 0;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
	PORTC = (PORTC & 0xf0) | (hours % 10 & 0x0f);
	PORTA |= 0x10;
	_delay_ms(2);
	PORTA = 0;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
	PORTC = (PORTC & 0xf0) | ((int) hours / 10 & 0x0f);
	PORTA |= 0x20;
	_delay_ms(2);
	PORTA = 0;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// the delay enough is 2 ms for every operation
}
void REG_INIT() {
	DDRA |= 0x03; // output pins
	DDRB &= ~(1 << 2);
	DDRC = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	DDRD &= 0xF3;
	PORTA = 0;
	PORTB |= (1 << 2); // to open the internal resistor(important)
	PORTC &= 0xf0; //(initial value to 7 segment)
	PORTD |= (1 << 2); // to open the internal resistor(important)
}
void INT_INIT() {
	INT0_INIT();
	INT1_INIT();
	INT2_INIT();
	TIMER1_COMPA_INIT();
}
int main() {
	REG_INIT();
	INT_INIT();
	while (1) {
		DisplaySegment();
	}
}
