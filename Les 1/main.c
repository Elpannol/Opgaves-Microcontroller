/*
 * Code les 1.c
 *
 * Created: 2-2-2017 12:20:45
 * Author : gijsb
 */ 
 #define F_CPU 8000000
 #define BIT(x) (1 << (x)) // bit position

#include <avr/io.h>
#include <util/delay.h>

typedef struct{
	unsigned char light;
	unsigned int delay;
} pattern;

pattern patternStruct[] = {
 {0x00, 100}, {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100}, {0x80, 100},
 {0x00, 100},
 {0xAA,  50}, {0x55,  50},
 {0xAA,  50}, {0x55,  50},
 {0xAA,  50}, {0x55,  50},
 {0x00, 100},
 {0x81, 100}, {0x42, 100}, {0x24, 100}, {0x18, 100}, {0x0F, 200}, {0xF0, 200}, {0x0F, 200}, {0xF0, 200},
 {0x00, 0x00}
};

void wait(int ms)
{
	for(int i=0; i<ms; i++){
		_delay_ms(1);
	}
}


void b2(){

	DDRD = 0b00000110;

	while(1){
		PORTD = 0x04;
		wait(500);
		PORTD = 0x0a;
		wait(500);
	}
}

void b3(){

	DDRD = 0b01111111;
	PORTC = 0x10;

	while(1)
	{
		if(PINC & 0x01)
		{
			PORTD = 0x01;
			wait(100);
			PORTD = 0x00;
			wait(1000);
		}
		else
		{
			PORTD = 0x00;
		}
	}
}

void b4(){
	DDRD = 0b11111111;
	
	while(1)
	{
		for (int i = 0; i < 8; i++)
		{
			wait(100);

			PORTD &= BIT(i);
			if(i != 0){			
				PORTD |= BIT(i - 1);
			}
			else{
				PORTD |= BIT(7);
			}
		}
	}
}

void b5(){
	DDRD = 0b11111111;

	while(1)
	{
		int i = 0;
		while(patternStruct[i].delay != 0){
			PORTD = patternStruct[i].light;
			wait(patternStruct[i].delay);
			i++;
		}
	}

}

void b6(){
	DDRD = 0b11111111;
	PORTC = 0x10;
	int delayInt = 500;

	while(1)
	{
		int pushed = 0;
		if(PINC & 0x01 & !pushed)
		{
			if(delayInt == 500)
			delayInt = 125;
			else
			delayInt = 500;
		}

		PORTD = 0x10;
		wait(delayInt);
		PORTD = 0x00;
		wait(delayInt);
	}
}

int main(void)
{
    /* Replace with your application code */
	b6();   
	return 0;
}






