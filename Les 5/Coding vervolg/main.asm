;
; Coding.asm
;
; Created: 23-3-2017 09:14:27
; Author : gijsb
;


; Replace with your application code
rjmp e

a: 
	ldi xh, $05 ;high x is 05
	ldi	xl, $40 ;low x is 40
	; Together it's 0x0540

	ldi r16, $48 ;48x is the low end that's keeps being checked
	ldi r17, $55 ;the value that has to be stored in every register between 5400 and 5480

loopa:
	st x+, r17 ;store the value in x and increment x
	cp xl, r16 ;compare r16 with xl
	brne loopa ;if r16 is not 00, repeat loop
	rjmp end ;jump to the end

b:
	ldi xh, $06 ;high x is 06
	ldi	xl, $00 ;low x is 00
	; Together it's 0x060
	ldi r16, $00;

	loopb:
		ld r17, x+ ;store x in r17 and increment x 
		com r17 ;takes one's complement from r16
		cp xl, r16 ;compare r16 with xl
		brne loopb ;if r16 is not 00, repeat loop
	rjmp end

c:
	ldi xh, $06 ;high x is 06
	ldi	xl, $60 ;low x is 60
	; Together it's 0x0660

	ldi yh, $08 ;high y is 08
	ldi yl, $b0 ;low y is b0
	; Together it's 0x08b0

	ldi r16, $70 ;for comparing xl
	ldi r18, $55 ;testing value 
	loopc:
		st x, r18; store r18 in x for testing

		ld r17, x+ ;store x in r17 and increment x 
		st y+, r17 ;store r17 in y and increment y

		cp xl, r16 ;compare r16 with xl
		brne loopc ;if r16 is not 00, repeat loop
	rjmp end

d:
	ldi yh, $04 ;
	ldi yl, $00 ;

	ldd R8, Y+0 ; Haal waarden op uit data memory
	ldd R9, Y+1 ;

	ldd R6, Y+2 ;
	ldd R7, Y+3 ;	
	ldd R10, Y+6 ;
	ldd R11, Y+7 ;	ldd R12, y+4 ;	ldd R13, Y+5 ;


	; 32 bits optelling: R5/R4 = R9/R8 + R7/R6 (23 ab + 44 98 = 68 43)
	; Stap 1: tel de LSB’s op: R4 = R6+R8
		MOV R2, R8 ; R2 <- R8
		ADD R2, R6 ; R2 <- R2+R6 misschien Carry gezet

		MOV R4, R12 ; R4 <- R12
		Add R4, R10 ; R4 <- R12+R10 misschien Carry gezet
	; Stap 2: tel de MSB’s op met
	; de (eventuele) carry uit LSB’s:
	; R3 = R5+R6+C
		MOV R3, R9 ; R3 <- R9
		ADC R3, R7 ; R3 <- R9+R7+C

		MOV R5, R13; R5 <- R13
		ADC R5, R11 ; R5 <- R12+R20+C
	;
	; Opslag resultaat
	;
		STD Y+8,R2; Sla resultaat op in data memory
		STD Y+9,R3;
		STD Y+10,R2 ;
		STD Y+11,R5 ;
	rjmp end
e:
	ldi xh, $00;
	ldi xl, $07;

	ldi r16, $07;
	ldi r17, $00;

	sbi $1b, $07;

	frontLoop:
		sbr $1b, r26;
		ld r17, x+ 

		ldi r19,0x0 ; load 256 in r19 (counter 1)
		clc
		fwait1: ldi r18,0x0 ; load 256 in r18 (counter 2)
		fwait2: ldi r17,0x8 ; load 8 in r17 (counter 3)
		fwait3: nop ; inner circle
			dec r17 ;
			brne fwait3 ; branch if r17 not zero
			;
			 dec r18 ; intermediate circle
			brne fwait2 ; branch if r18 not zero
			;
			dec r19 ; upper circle
			brne fwait1 ; branch if r19 not zero

			
		inc r26;
		cp xl, r16;compare r16 with x
		brne backLoop;
		jmp frontLoop;
	backLoop:
		cbr $1b, r26;
		ld r17, x-

		ldi r19,0x0 ; load 256 in r19 (counter 1)
		clc;
		wait1: ldi r18,0x0 ; load 256 in r18 (counter 2)
		wait2: ldi r17,0x8 ; load 8 in r17 (counter 3)
		wait3: nop ; inner circle
			dec r17 ;
			brne wait3 ; branch if r17 not zero
			;
			 dec r18 ; intermediate circle
			brne wait2 ; branch if r18 not zero
			;
			dec r19 ; upper circle
			brne wait1 ; branch if r19 not zero

			
		dec r26;
		cp xl, r17 ;compare r17 with xl
		brne frontLoop;
		jmp backLoop;

end:
rjmp end