
#ifndef _STD_H
#define _STD_H

	#include <stdlib.h>
	#include <stdint.h>
	#include <stdarg.h>

#ifdef CPU_6809
#define const	_CMOC_CONST_
#endif

int putc(char chr);
int puts(const char *str);

#endif
