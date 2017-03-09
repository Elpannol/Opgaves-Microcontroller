/*
 * B2.c
 *
 * Created: 9-3-2017 11:56:04
 * Author : gijsb
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x) (1 << (x))
#define F_CPU 8000000

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait(int ms)
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

// Initialize ADC: 10-bits (left justified), free running
void adcInit(void)
{
	ADMUX = 0x63;			// AREF=2,56 V, result left adjusted, channel 3 at pin PF1
	ADCSRA = 0x86;		// ADC-enable, no interrupt, no free running, division by 64
}


// Main program: ADC at PF1
int main(void)
{
	DDRF = 0x00;					// set PORTF for input (ADC)
	DDRA = 0xFF;					// set PORTA for output
	DDRC = 0x01;					// sets led 1 as output
	adcInit();						// initialize ADC

	while (1)
	{
		ADCSRA |= BIT(6);				// Start ADC
		while ( ADCSRA & BIT(6) ) ;		// Wait for completion
		PORTA = ADCH;					// Show MSB (bit 9:2) of ADC
		PORTC ^= 1;						// 1 for not shifting
		wait(2500);						// every 50 ms (busy waiting)

	}
}

