#ifndef _STDLIB_H
#define _STDLIB_H

	#include <std.h>

int atoi(const char *s);
unsigned atoui(const char *s);
long atol(const char *nptr);
unsigned long atoul(const char *nptr);

char *itoa(int val, char *buf, int radix);
char *utoa(unsigned val, char *buf, int radix);
char *ltoa(long val, char *buf, int radix);
char *ultoa(unsigned long val, char *buf, int radix);

long strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);

#endif
