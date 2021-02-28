//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

#ifndef _STD_H
#define _STD_H

	#include <hwdeps.h>
	#include <stdlib.h>
	#include <stdint.h>
	#include <stdarg.h>

// #ifdef CPU_6809
// #define const	_CMOC_CONST_
// #endif
//
// #ifdef CPU_6309
// #define const	_CMOC_CONST_
// #endif

#define CYCLAMEN_VERSION	"v0.3"
#define CYCLAMEN_DATE		"15-5-20"

//---------------------------------------------------

int putc(char chr);
int puts(const char *str);
char getc(void);
char *gets(char *buff, int len);

char peek(void);
char tty_init(void);

//---------------------------------------------------

// Macro expansion
#define STR(s)		# s
#define XSTR(s)		STR(s)
#define EXPAND(x)	x

// Macro overloading support
#define VA_MACRO(M, ...)			_VA_MACRO(M, _NARGS(__VA_ARGS__)) (__VA_ARGS__)
#define _VA_MACRO(name, nargs) 		_VA_MACRO_(name, nargs)
#define _VA_MACRO_(name, nargs)		name##nargs
#define _NARGS(...)  				_NARGS_(__VA_ARGS__, 9,8,7,6,5,4,3,2,1)
#define _NARGS_(_1,_2,_3,_4,	\
				_5,_6,_7,_8,	\
				_9,N,...) N

//---------------------------------------------------

#endif
