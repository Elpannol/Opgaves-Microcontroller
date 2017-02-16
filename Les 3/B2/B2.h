/*
 * B2.h
 *
 * Created: 16-2-2017 10:53:21
 *  Author: gijsb
 */ 
 #ifdef LCDHeader
 #define LCDHeader

 #include <stdio.h>

 void init();
 void clear();
 void display_text(char *str);
 void set_cursor(int position);

 #endif