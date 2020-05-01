//---------------------------------------------------
//
//	Cyclamen v0.0
//	NotArtyom
//	04/28/20
//
//---------------------------------------------------

	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

	#include <std.h>
	#include <ctype.h>
	#include "gmon.h"

//---------------------------------------------------

extern bool doExit;
extern char* parse;
extern char *current_addr;
extern char *end_addr;

extern uint32_t mon_vars[];

uint32_t mon_vars[6];
bool isCurrentVar;

//---------------------------------------------------

void print_prompt(void) {
	putc('(');
	printWord((uint16_t)current_addr);
	if (end_addr != 0x00) {
		putc('.');
		printWord((uint16_t)end_addr);
	}
	puts(")\n");
	puts(" $ ");
}

bool isRange(void) {
	char *tmpParse = parse;
	while (*tmpParse != '.' && *tmpParse != ',' && *tmpParse != '\0') tmpParse++;
	if (*tmpParse != '.' && *tmpParse != ',') return false;
	return true;
}

bool isVar(void) {
	if (isLower(*parse) && (*(parse+1) == ' ' || !isAlnum(*(parse+1)))) return true;
	return false;
}

bool isArg(void) {
	return (isVar() || isAddr());
}

bool setCurrents(void) {
	skipBlank();
	if (isVar()) isCurrentVar = true;
	else if (isAddr()) isCurrentVar = false;

	return getRange(&current_addr, &end_addr);
}

char* skipBlank(void) {
	while (*parse == ' ') parse++;
	return parse;
}

char* skipToken(void) {
	while (*parse != ' ' && *parse != '\0') parse++;
	return parse;
}

char* skipHex(void) {
	uint8_t i;
	while (true){
		for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return parse;
		parse++;
	}
}

bool isAddr(void) {
	uint8_t i;
	for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
	return (hexTable[i] != '\0');
}

bool funcCmp(const char s1, const char s2){
	if (s1 == s2) return true;
	return false;
}

ADDRSIZE strToHEX(void) {
	uint8_t i;
	ADDRSIZE val = 0;
	val = strtoul(parse, NULL, 16);
	return val;
}

enum errList throw(uint8_t index){
	if (index == errNONE) return errNONE;
	else if (index == errDOEXIT) return errDOEXIT;
	//vprintf("\n%s\n", errors[index]);
	return errNONE;
}

void evalScript(void) {


}

uint32_t *getMonVar(char var) {
	if (isLower(var)){
		return &mon_vars[ (var - 0x60) ];
	} else {
		return 0x00000000;
	}
}

void setMonVar(char var, ADDRSIZE val){
	if (isAscii(var) && isLower(var)){
		mon_vars[ var & 0x00011111 ] = val;
	} else {
		return;
	}
}

bool getRange(char **lower, char **upper){
	skipBlank();
	if (isAddr()){
		*(ADDRSIZE*)lower = strToHEX();
	} else if (isVar()){
		*lower = (char*)getMonVar(*parse);
	} else if (*parse == '.' || *parse == ','){
		*lower = current_addr;
	} else {
		return false;
	}
	if (isCurrentVar == false && (*parse == '.' || *parse == ',')){
		char tmp = *parse;
		parse++;
		skipBlank();
		ifEOI(errNOARGS);
		if (!isAddr()) return errSYNTAX;
		if (tmp == '.')
			*(ADDRSIZE*)upper = strToHEX();
		else
			*(ADDRSIZE*)upper = (strToHEX() + (uint32_t)*lower);
	} else {
		if (*parse != ' ' || *parse != '\0'){
			skipToken();
		}
		*upper = NULL;
	}
	return true;
}

void printHex(char num){
	putc(hexTable[(num & 0x0F)]);
	return;
}

void printByte(char num){
	putc(hexTable[(num & 0xF0) >> 4]);
	putc(hexTable[(num & 0x0F)]);
	return;
}

void printWord(uint16_t num){
	putc(hexTable[(num & 0xF000) >> 12]);
	putc(hexTable[(num & 0x0F00) >> 8]);
	putc(hexTable[(num & 0x00F0) >> 4]);
	putc(hexTable[(num & 0x000F)]);
	return;
}

void printLong(uint32_t num){
	printWord((num & 0xFFFF0000) >> 16);
	printWord(num & 0x0000FFFF);
	return;
}
