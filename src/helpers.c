//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

	#include <std.h>
	#include <ctype.h>
	#include "gmon.h"

//---------------------------------------------------

extern bool doExit;
extern char *parse;
extern char *current_addr;
extern char *end_addr;

//---------------------------------------------------

void print_prompt(void) {
	putc('(');
	printWord((ADDRSIZE)current_addr);
	if (end_addr != 0x00) {
		putc('.');
		printWord((ADDRSIZE)end_addr);
	}
	puts(")\n $ ");
}

err_t dump_range(char *ptr, char *end) {
	if (end != NULL) {									// If we hit a range identifier...
		uint8_t column;									// Create something to track how many columns have been printed so far
		while (ptr <= end) {							// Continue until we've reached the end of the range
			if (ptr <= end) {
				column = 0;								// And print out the location header
				printWord((ADDRSIZE)ptr);
				puts(" : ");
			}
			while (column < 8 && ptr <= end) {
				printByte(*ptr++);						// Print data byte at this address
				putc(' ');
				column++;								// Increase our column number
				queryBreak();
			}
			putc('\n');									// Then set up a new line
		}
	} else {
		printWord((ADDRSIZE)ptr);
		puts(" : ");
		printByte(*ptr);
		putc('\n');
	}
	return errNONE;
}

bool isAddr(void) {
	return (char)isHex(*parse);
}

bool isRange(void) {
	char i = 0;
	char *tmpParse = parse;
	while (isHex(tmpParse[i])) i++;
	if (tmpParse[i] != '.' && tmpParse[i] != ',') return false;
	return true;
}

err_t setCurrents(void) {
	skipBlank();
	return getRange(&current_addr, &end_addr);
}

char *skipBlank(void) {
	while (*parse == ' ') parse++;
	return parse;
}

char *skipToken(void) {
	while (*parse != ' ' && *parse != '\0') parse++;
	return parse;
}

char *skipHex(void) {
	while (true) {
		if (!isHex(*parse)) return parse;
		parse++;
	}
}

ADDRSIZE strToHEX() {
	uint8_t i;
	ADDRSIZE val = 0;
	while (*parse != ' ' && *parse != '\0'){
		for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return val;
		val = (val << 4) | i;
		parse++;
	}
	return val;
}

err_t throw(uint8_t index) {
	if (index == errNONE) return errNONE;
	else if (index == errDO_EXIT) return errDO_EXIT;
	if (index == errBREAK) putc('\n');					// Force a newline if we're throwing BREAK
	puts("   "); puts(errors[index]); putc('\n');
	return errNONE;
}

err_t getRange(char **lower, char **upper) {
	if (isAddr()) {
		*lower = (char *)strToHEX();
		while (isHex(*parse)) parse++;
	} else if (*parse == '.' || *parse == ',') {
		*lower = current_addr;
	} else {
		return errUNDEF;
	}

	if (*parse == '.' || *parse == ',') {
		char tmp = *parse;
		parse++;
		if (noArgs()) return errBAD_RANGE;
		if (!isAddr()) return errBAD_HEX;
		if (tmp == '.')
			*upper = (char *)strToHEX();
		else
			*upper = (char *)(strToHEX() + *lower);
	} else {
		skipToken();
		*upper = NULL;
	}
	return errNONE;
}

//-----------------------------------------------------------------------------

void printHex(char num) {
	putc(hexTable[(num & 0x0F)]);
	return;
}

void printByte(char num) {
	putc(hexTable[(num & 0xF0) >> 4]);
	putc(hexTable[(num & 0x0F)]);
	return;
}

void printWord(uint16_t num) {
	putc(hexTable[(num & 0xF000) >> 12]);
	putc(hexTable[(num & 0x0F00) >> 8]);
	putc(hexTable[(num & 0x00F0) >> 4]);
	putc(hexTable[(num & 0x000F)]);
	return;
}

void printLong(uint32_t num) {
	printWord((num & 0xFFFF0000) >> 16);
	printWord(num & 0x0000FFFF);
	return;
}
