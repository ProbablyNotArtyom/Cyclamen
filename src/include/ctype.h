#ifndef _ctype_h
#define _ctype_h

	#include <std.h>

/* Character classification functions */
int isSpace(int val);
int isLower(int val);
int isUpper(int val);
int isAlpha(int val);
int isDigit(int val);
int toUpper(int val);
int toLower(int val);
int isAscii(int val);
int isAlnum(int val);
int isHex(int val);
int isPrintable(int val);

#endif
