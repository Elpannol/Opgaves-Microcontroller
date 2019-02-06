#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

int i = 0;
int end = 0;

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );
	}
}

ISR( INT1_vect )
{
	if(i < 7){
		PORTD = PORTD<<1;
		i++;
	}
}


ISR( INT2_vect )
{
	if(i > 0){
		PORTD = PORTD>>1;
		i--;
	}
}

int main( void )

{
	DDRD = 0b11111111;
	PORTD = 0b00000001;
	
	EICRA |= 0b00101100;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0b00000110;			// Enable INT1 & INT2
	
	sei();

	while (1)
	{
	}

	return 1;
}