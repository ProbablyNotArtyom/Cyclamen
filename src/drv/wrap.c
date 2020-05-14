//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

    #include <hwdeps.h>
    #include <std.h>
	#include <ctype.h>
    
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

//---------------------------------------------------

int putc_def(char chr);
char getc_def(void);
char peek_def(void);
char tty_init_def(void);

//---------------------------------------------------

int putc(char chr) {
	return putc_def(chr);
}

char getc(void) {
	return (char)getc_def();
}

char peek(void) {
	return peek_def();
}

char tty_init(void) {
	return tty_init_def();
}
