/*
 * Opgaves les 2.c
 *
 * Created: 9-2-2017 10:20:09
 * Author : gijsb
 */ 

#define F_CPU 8000000
#define BIT(x) (1 << (x)) // bit position
#define arrSize(array)(sizeof(array)/sizeof(array[0]))
#define ARRAY_SIZE(array)(sizeof(array)/sizeof(array[0]))

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char LOOKUP[16] =
{
	0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF, 0xF7, 0xFF, 0xB9, 0xBF, 0xF9, 0xC7
};

typedef struct
{
	unsigned char data;
	unsigned int delay;
}Pattern;


Pattern LOOKUP[] =
{
	{0x01, 1000}, {0x02, 1000}, {0x04, 1000}, {0x08, 1000}, {0x10, 1000}, {0x20, 1000}, {0x01, 1000},
	{0x00, 2000},
	{0x01, 1000}, {0x02, 1000}, {0x40, 1000}, {0x10, 1000}, {0x08, 1000}, {0x04, 1000}, {0x40, 1000}, {0x20, 1000}, {0x01, 1000},
	{0x00, 1000},
	{0x01, 1000}, {0x40, 1000}, {0x08, 1000}, {0x20, 1000}, {0x02, 1000}, {0x10, 1000}, {0x04, 1000},
	{0x00, 1000}
};


/******************************************************************/
void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

void display(int digit)
{
	if(digit >= 16)digit = 14;
	PORTD = LOOKUP[digit];
}

/******************************************************************/
ISR( INT1_vect )
/* 
short:			ISR INT0
inputs:			
outputs:	
notes:			Set PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
{
	for(int i = 0; i < 8; i++)
	{
		PORTC |= BIT(i);	
		wait(100);			
	}
}

/******************************************************************/
ISR( INT2_vect )
/* 
short:			ISR INT1
inputs:			
outputs:	
notes:			Clear PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
{
	for(int i = 0; i < 8; i++)
	{
		 PORTC &= ~BIT(i);
		 wait(100);
	}
}

/******************************************************************/
int main( void )
/* 
short:			main() loop, entry point of executable
inputs:			
outputs:	
notes:			Slow background task after init ISR
Version :    	DMK, Initial code
*******************************************************************/
{
	b3();
	return 1;
}

void b1()
{
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input

	// Init Interrupt hardware
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x03;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();

	while (1)
	{
		PORTD ^= (1<<7);	// Toggle PORTD.7
		wait( 500 );
	}

}

void b2(){
	// Init I/O
	DDRD = 0b11000000;
	DDRC = 0b11111111;
	
	// Init Interrupt hardware
	EICRA |= 0b00101100;			// INT2 falling edge, INT1 rising edge
	EIMSK |= 0b00000110;			// Enable INT1 & INT2

	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();

	while(1)
	{
		;
	}	

}

void b3(){
  DDRD = 0xFF; //Sets all of PORT D to output
  int index = 0;

  while (1)
  {
	  PORTD = 0x00;
	  wait(10);
	  display(index);
	  wait(3000);
	  index++;

	  if(index > 15)index = 0;
	  //if(index == 20)index = 0;
  }
}

void b4(){
	DDRD = 0xFF; //Sets all of PORT D to output
	int step = 0;

	while (1)
	{
		display(step);
		step++;

		if(step == ARRAY_SIZE(LOOKUP))step = 0;
	}
}