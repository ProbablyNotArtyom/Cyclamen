//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

	#include <std.h>
	#include <ctype.h>
	#include <hwdeps.h>

//---------------------------------------------------

extern int putc_def(char chr);
extern char getc_def(void);
extern char peek_def(void);
extern char tty_init_def(void);

//---------------------------------------------------

int putc(char chr) {
	__asm__("");						// Prevent the parent function symbol from being optimized away
	return putc_def(chr);
}

char getc(void) {
	__asm__("");						// Prevent the parent function symbol from being optimized away
	return (int)getc_def();
}

char peek(void) {
	__asm__("");						// Prevent the parent function symbol from being optimized away
	return peek_def();
}

char tty_init(void) {
	__asm__("");						// Prevent the parent function symbol from being optimized away
	return tty_init_def();
}
