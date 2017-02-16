/*
 * B2.c
 *
 * Created: 16-2-2017 10:51:03
 * Author : gijsb
 */ 

#include <avr/io.h>
#include "B2.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_E 	3
#define LCD_RS	2

#define F_CPU 8000000
#define BIT(x) (1 << (x)) // bit position
#define arrSize(array) (sizeof(array)/sizeof(array[0]))

int firstTime = 0;

void lcd_strobe_lcd_e(void);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);

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

void main(void)
{
	// Init I/O
	DDRD = 0xFF;			// PORTD(7) output, PORTD(6:0) input

	// Init LCD
	init();

	int index = 0;
	int isPushed = 0;

	display_text("Niggah");
	// Loop forever
	while (1)
	{
		wait(10000);
		display_text("Gijs");
	}
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
	clear();
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void clear()
{
	if(firstTime) {
		lcd_write_command(0x01);
		lcd_write_command(0x02);
		PORTC = 0x00;   // Entry mode set
		lcd_strobe_lcd_e();
		PORTC = 0x60;
		lcd_strobe_lcd_e();
	}
	else firstTime = 1;
}

void set_cursor(int position){
	
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

