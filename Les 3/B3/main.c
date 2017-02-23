/*
 * B3.c
 *
 * Created: 23-2-2017 11:20:50
 * Author : gijsb
 */ 

#include <avr/io.h>
#include "B3.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000
#define BIT(x) (1 << (x)) // bit position
#define arrSize(array) (sizeof(array)/sizeof(array[0]))

int msCount = 0; 
int isAan = 0;

ISR(TIMER2_COMP_vect)
{
	msCount++; // Increment ms counter
	if ( msCount == 8 & !isAan)
	{
		PORTD ^= BIT(7); // Toggle bit 0 van PORTC
		msCount = 0; // Reset ms_count value
		isAan = 1;
	}
	else if(msCount == 12)
	{
		PORTD ^= BIT(7); // Toggle bit 0 van PORTC
		msCount = 0; // Reset ms_count value
		isAan = 0;
	}


}

void wait(int ms)
{
	for(int i = 0; i < ms; i++) _delay_ms(1);
}

void main(void)
{
	// Init I/O
	DDRC = 0xFF;			// PORTD(7) output, PORTD(6:0) input
	DDRD = 0xFF;
	// Init LCD
	timer2Init();

	// Loop forever
	while (1)
	{
	}
}

void timer2Init()
{
	OCR2 = 15;			// Compare value of counter 2
	TIMSK |= BIT(7);	// T2 compare match interrupt enable
	sei();				// turn_on intr all
	TCCR2 = 0b00011101;		// compare output disconnected,CTC,RUN
}

