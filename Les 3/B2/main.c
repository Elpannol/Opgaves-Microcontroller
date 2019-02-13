/*
 * B2.c
 *
 * Created: 16-2-2017 10:51:03
 * Author : gijsb
 */ 

#include <avr/io.h>
#include "B2.h"
#include <avr/interrupt.h>
#include <string.h>
#include <limits.h>
#include <util/delay.h>

#define LCD_E 	3
#define LCD_RS	2

#define F_CPU 8000000
#define BIT(x) (1 << (x)) // bit position
#define arrSize(array) (sizeof(array)/sizeof(array[0]))

void lcd_strobe_lcd_e(void);
void lcd_write_data(unsigned char byte);
void lcd_write_command(unsigned char byte);

int index = 0;
char toChar[6];

ISR(INT0_vect)
{
	clear();
	wait(100);
	index++;

	sprintf(toChar, "%d", index);

	display_text(toChar);
	memset(toChar, 0 ,6);
	wait(20);
}

/******************************************************************/
void lcd_strobe_lcd_e(void)
/*
short:			Strobe LCD module E pin --__
inputs:
outputs:
notes:			According datasheet HD44780
Version :    	DMK, Initial code
*******************************************************************/
{
	PORTC |= (1<<LCD_E);	// E high
	wait(1);			// nodig
	PORTC &= ~(1<<LCD_E);  	// E low
	wait(1);			// Je weet maar nooit
}

void wait(int ms)
{
	for(int i = 0; i < ms; i++) _delay_ms(1);
}

int main(void)
{
	// Init I/O
	DDRD = 0xF0;			// PORTD(7) output, PORTD(6:0) input


	EICRA |= 0x06;
	EIMSK |= 0x01;

	sei();
	// Init LCD
	init();

	sprintf(toChar, "%d", index);
	display_text(toChar);
	memset(toChar, 0, 6);
	// Loop forever
	while (1)
	{
		PORTD ^= (1<<7);
		wait(500);
	}
	return 1;
}

void init(){
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	// Step 5 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();

}

void display_text(char *str){
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void clear()
{
	lcd_write_command(0x01);
	lcd_write_command(0x02);
}

void set_cursor(int position)
{
	int i;
	if(position > 0)
	{
		for(i = 0; i < position; i++)
		{
			lcd_write_command(0x14);
			wait(100);
		}
	}
}

void lcd_write_data(unsigned char byte)
/*
short:			Writes 8 bits DATA to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_command(unsigned char byte)
/*
short:			Writes 8 bits COMMAND to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

