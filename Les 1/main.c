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
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	unsigned char light;
	unsigned int delay;
} pattern;


void s1(void);
void s2(void);
void s3(void);
void end(void);
void start(void);

/* Define fsm (states,events) */
typedef enum { D7, D6, D5 } ENUM_EVENTS;
typedef enum { START, STATE_1, STATE_2, STATE_3, END } ENUM_STATES;
typedef enum { Nott, S1, S2, S3, Start, End} ENUM_FUNC;

/* Define fsm transition */
typedef struct {
	ENUM_FUNC finit;
	ENUM_FUNC fbody;
	ENUM_FUNC fexit;
	ENUM_STATES nextState;
} STATE_TRANSITION_STRUCT;

// Sample fsm definition:
//
// 		| EV_D7    EV_D6		EV_D5
// ---------------------------------
// START| START     S1          S2
// S1   | START     S1        	S2
// S2   | START     S1          S3
// S3   | START     END         END
// END  | START     END         END

STATE_TRANSITION_STRUCT fsm[5][3] = {
	{ {S1,   S2,  Nott, START}, 	{Nott, S1, Nott, STATE_1},		{Start, Nott, Nott, STATE_2} },
	{ {Nott, Start, Nott, START},	{S3, Nott, S2, STATE_1},	{Nott, S2, Nott, STATE_2} },
	{ {Nott, Start, Nott, START}, 	{Nott, S1, Nott, STATE_1}, 		{Nott, S3, Nott, START} },
	{ {Nott, Nott,  Nott, START},	{Nott, End, Nott, END}, 		{Nott, End, Nott, END} },
	{ {Nott, Start, Nott, START},	{Nott, End, Nott, END}, 		{Nott, End, Nott, END} }
};

// State holder
ENUM_STATES state = START;


void runFunction(ENUM_FUNC function){
	switch(function){
		case S1:
		s1();
		break;
		case S2:
		s2();
		break;
		case S3:
		s3();
		break;
		case Start:
		start();
		break;
		case End:
		end();
		break;
		case Nott:
		break;
	}
}

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


void handleEvent(ENUM_EVENTS event)
/* 
short:			Change state in fsm. State change depens on event
inputs:			event, this determines the next state in the fsm
outputs:	
notes:			Needs extensive testing. !! Please review execution 
                order !!
Version :    	DMK, Initial code
*******************************************************************/
{

	// Call EXIT function old state
	runFunction(fsm[state][event].fexit) ;

	// Set new state, -1 means 
	state = fsm[state][event].nextState;

	runFunction(fsm[state][event].finit);
	runFunction(fsm[state][event].fbody);
	
}

void wait(int ms)
{
	for(int i=0; i<ms; i++){
		_delay_ms(1);
	}
}


//
// State S1
//
void s1(void){
	PORTD = 0b00000111;
	wait(500);
}

//
// State S2
//
void s2(void){
	PORTD = 0b00000011;
	wait(500);
}

//
// State S3
//
void s3(void){
	PORTD = 0b00000001;
	wait(500);
}

//
// State Start
//
void start(void){
	PORTD = 0b00001111;
	wait(500);
}

//
// State Stop
//
void end(void){
	PORTD = 0b00000000;
}


void b7a(){
	DDRD = 0b00001111;			// Pins PORTD<7:4> input, PORTD<3:0)
	// output
	
	handleEvent(D7);
	handleEvent(D5);
	handleEvent(D5);
	handleEvent(D5);

	// wait forever
	while (1)
	{
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

	DDRD = 0b11111111;

	while(1)
	{
		if(PINC & 0b00000001)
		{
			PORTD = 0x01;
			wait(100);
			PORTD = 0x00;
			wait(100);
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
	int delayInt = 500;
	int pushed = 0;

	while(1)
	{
		PORTD = 0x10;
		wait(delayInt);
		PORTD = 0x00;
		wait(delayInt);

		if(PINC & 0x01)
		{
			if(delayInt == 500 && pushed == 0)
				delayInt = 125;
			else if(delayInt == 125 && pushed == 0)
				delayInt = 500;
				
			pushed = 1;
		}
		else{
			pushed = 0;
		}
	}
}


int main(void)
{
    /* Replace with your application code */
	b7a();   
	return 0;
}






