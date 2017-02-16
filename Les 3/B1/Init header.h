
 #ifdef LCDHeader
 #define LCDHeader
 
 #define F_CPU 8000000 
 #define BIT(x) (1 << (x)) // bit position
 #define arrSize(array) (sizeof(array)/sizeof(array[0]))

 #include <stdio.h>
 #include <avr/io.h>

 void init();
 void display_text(char *str);
 void set_cursor(int position);

 #endif