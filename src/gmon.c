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

//-----------------------------------------------------------------------------

bool gsh_do_cmd(uint8_t num);

const enum errList gsh_help(void);
const enum errList gsh_exit(void);
const enum errList gsh_echo(void);
const enum errList gsh_execute(void);
const enum errList gsh_deposit(void);
const enum errList gsh_view(void);

const enum errList read_range(char *ptr, char *end);

//-----------------------------------Tables------------------------------------

const enum errList (* funcTable[])(void) = {
	&gsh_help,
	&gsh_exit,
	&gsh_execute,
	&gsh_deposit,
	&gsh_view,
	NULL
};

const char funcKeys[] = {
	'W',
	'Q',
	'G',
	':',
	'R',
	0
};

const char* const errors[] = {
	"",
	"SYNTAX ERROR",
	"UNDEFINED FUNCTION",
	"UNEXPECTED ARGUMENTS",
	"UNEXPECTED END OF INPUT",
	"INVALID HEX",
	"INVALID RANGE",
	"BREAK"
};

const char hexTable[] = "0123456789ABCDEF";
const char *helpText[] = {
	"G'MON VERSION " GMON_VERSION,
	"-=-=- COMMANDS -=-=-=-=-=-",
	"R (<$|%>) DUMP MEMORY",
	": (#)     DEPOSIT BYTES",
	"G <$|%>   JUMP TO ASM",
	"^ ...     PRINT FORMATTED",
	"?         PRINT HELP",
	"\0"
};

//-----------------------------------------------------------------------------

bool doExit;
char *parse;
char *current_addr;
char *end_addr;
char *cmdStart;
char *tmp;

uint8_t numCMDs;
uint32_t numLoops;
char inBuffer[BUFFLEN];					// Our input buffer

int gmon(void) {
	puts("G'MON VERSION " GMON_VERSION "\n");
	doExit = false;
	current_addr = 0x00;
	end_addr = 0x00;
	while (doExit == false){
		print_prompt();
		parse = inBuffer;							// Set the parse pointer to the beginning of the buffer
		gets(inBuffer, BUFFLEN);					// Get user input
		skipBlank();								// Skip and leading spaces
		if (!isEOI()){
			puts("");
			numLoops = 1;
			numCMDs = 0x01;
			skipBlank();
			cmdStart = parse;
			if (*cmdStart == '{'){
				while (*cmdStart != '}') cmdStart++;
				numLoops = strtoul(parse+1, NULL, 10);
				cmdStart++;
			}
			parse = cmdStart;
			for (tmp = parse; *tmp != '\0'; tmp++) if (*tmp == ';') numCMDs += 1;
			while (numLoops > 0){
				gsh_do_cmd(numCMDs);
				// Break if escape is pressed
				if (getc() == 0x09) {
					throw(errBREAK);
					numLoops = 0;
				} else {
					numLoops--;
					parse = cmdStart;
				}
			}
		} else puts("\n");
	}

	return 0;
}

bool gsh_do_cmd(uint8_t num){
	uint8_t i;
	char *tmp;
	for (tmp = parse; *tmp != '\0'; tmp++)
		if (*tmp == ';') *tmp = '\0';

	while(num > 0){
		skipBlank();
		for (i = 0; funcCmp(*parse, funcKeys[i]) == false && i < NUM_FUNCS-1; i++);	// Identify what function it is
		if (i == NUM_FUNCS-1) {
			if (setCurrents() == false){
				throw(errUNDEF);			// If none matches, complain
				return false;
			}
		} else {
			skipToken();							// Skip over the function name itself
			if (throw((*funcTable[i])()) != errNONE) return false;
		}
		while (*parse != '\0') parse++;
		parse++;
		num--;
	}
	return true;
}

//----------------------------------Builtins-----------------------------------

/* Exits the monitor */
const enum errList gsh_exit(void) {
	doExit = true;
	return errNONE;
}

/* Starts executing code from a place in memory */
const enum errList gsh_execute(void) {
	void (*ptr)(void) = (void*)current_addr;
	if (!isEOI() && isAddr()) ptr = (void*)strToHEX();

	(*ptr)();			// Call that function
	return errNONE;		// Return error free, assuming that whatever we call actually returns (good chance it wont)
}

const enum errList gsh_help(void) {
	int i;
	for (i = 0; helpText[i] != NULL; i++) {
		puts(helpText[i]);
		if (i % 2) while ((getc()) != '\r');	// Require a keypress to print another 2 lines
	}
	return errNONE;
}

/* Writes bytes to memory */
const enum errList gsh_deposit(void) {
	uint8_t *end, *ptr = (uint8_t*)current_addr;	// Create pointers for the start and end of the section
	uint8_t val;

	skipBlank();
	if (isCurrentVar){
		ptr = (uint8_t*)strToHEX();
	} else {
		if (*skipBlank() == '\0') return errNOARGS;
		while (!isEOI()) {
			val = (uint8_t)strToHEX();
			*(uint8_t*)(ptr++) = (uint8_t)val;
			skipHex();
		}
	}
	return errNONE;
}

const enum errList read_range(char *ptr,char *end) {
	if (end != NULL){									// If we hit a range identifier...
		uint8_t column;									// Create something to track how many columns have been printed so far
		char *addrBuff;
		column = 0;
		while (ptr <= end){								// Continue until we've reached the end of the range
			int i;
			if (ptr <= end){
				putc('\n');								// Then set up a new line
				column = 0;								// And print out the location header
				printLong((uint32_t)ptr);
				puts(" | ");
			}
			while (column < 4 && ptr <= end){
				printByte(*ptr++);						// Print data byte at this address
				putc(' ');
				column++;								// Increase our column number
				queryBreak();
			}
		}
	} else {
		putc('\n');									// Then set up a new line
		printLong((uint32_t)ptr);
		puts(" | ");
		printByte(*ptr);
	}
	return errNONE;
}

/* Handles viewing of memory */
const enum errList gsh_view(void) {
	char *ptr, *end;									// Create start and end pointers
	if (!isEOI()){
		while(*parse != '\0'){
			skipBlank();
			if (isVar()){
				read_range((char*)getMonVar(*parse), (char*)0x00000000);
				parse++;
			} else {
				if (!getRange(&ptr, &end)) return errSYNTAX;
				read_range(ptr, end);
			}
		}
	} else {
		if (isCurrentVar) read_range(current_addr, end_addr);
		else read_range(current_addr, end_addr);
	}
	puts("");
	return errNONE;										// Return error free
}
