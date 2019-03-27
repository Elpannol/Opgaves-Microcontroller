#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms(1);			// library function (max 30 ms at 8MHz)
	}
}

void adcInit( void )
{
	ADMUX = 0b11100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}

int main( void )
{
	DDRD = 0xFF;
	DDRF = 0;	//Taking portF as input.
	adcInit();
	ADCSRA |= (1<<ADSC);  //starting the ADC conversion
	float i = 0;

	while (1)
	{
		i = ADC;

		if(i > 54400){
			PORTD ^= (1<<7);	// Toggle PORTD.7
			wait(100);
			//Do something when it is too loud
			//TODO make this wotk with the buzzer and figure out what the best value is for the sound recognition
		}
	}
}