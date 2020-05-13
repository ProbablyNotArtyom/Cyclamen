//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

#ifndef _STD_H
#define _STD_H

	#include <stdlib.h>
	#include <stdint.h>
	#include <stdarg.h>

#ifdef CPU_6809
#define const	_CMOC_CONST_
#endif

#ifdef CPU_6309
#define const	_CMOC_CONST_
#endif

//---------------------------------------------------

int putc(char chr);
int puts(const char *str);
char getc(void);
char *gets(char *buff, int len);

char peek(void);
char tty_init(void);

//---------------------------------------------------

#endif
