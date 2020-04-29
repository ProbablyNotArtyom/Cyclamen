#ifndef _STRING_H
#define _STRING_H

	#include <std.h>

int strcmp(const char *s1, const char *s2);
int stricmp(const char *s1, const char *s2);
int memcmp(const void *s1, const void *s2, size_t n);
int memicmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *strchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);
char *strlwr(char *s);
char *strupr(char *s);

#endif
