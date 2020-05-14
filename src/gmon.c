//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

	#include <std.h>
	#include <ctype.h>
	#include "gmon.h"

//-----------------------------------------------------------------------------

static bool gsh_do_cmd(uint8_t num);

enum errList gsh_version(void);
enum errList gsh_help(void);
enum errList gsh_exit(void);
enum errList gsh_echo(void);
enum errList gsh_execute(void);
enum errList gsh_deposit(void);
enum errList gsh_view(void);
enum errList gsh_copy(void);
enum errList gsh_move(void);
enum errList gsh_fill(void);

//-----------------------------------Tables------------------------------------

const char const hexTable[] = "0123456789abcdef";

enum errList (*const funcTable[])(void) = {
	&gsh_version,
	&gsh_help,
	&gsh_exit,
	&gsh_execute,
	&gsh_deposit,
	&gsh_view,
	&gsh_echo,
	&gsh_copy,
	&gsh_move,
	&gsh_fill,
	NULL
};

const char const funcKeys[] = {
	'v',
	'h',
	'q',
	'g',
	':',
	'r',
	'p',
	'z',
	'm',
	'x',
	NULL
};

const char *const helpText[] = {
	"-=-= commands -=-=-=-=-=-=-=-=-\n",
	"r <$/%>     dump memory\n",
	": (#)       deposit bytes\n",
	"g <$>       jump to asm\n",
	"p ...       print string\n",
	"v           display version\n",
	"h           print help\n",
	"z <%>, ($)  copy range to addr\n",
	"m <%>, ($)  move range to addr\n",
	"x <%>, ($)  fill range with byte\n\n",
	NULL
};

const char *const errors[] = {
	"",
	"(?) bad syntax\n",
	"(?) invalid function\n",
	"(!) unexpected args\n",
	"(?) expected args\n",
	"(?) invalid hex\n",
	"(?) invalid range\n",
	"(!) break\n"
};

//-----------------------------------------------------------------------------

char *parse;
char *current_addr;
char *end_addr;
char *cmdStart;

static bool doExit;
static char *tmp;
static uint8_t numCMDs;
static uint16_t numLoops;
static char inBuffer[BUFFLEN];						// Our input buffer

//-----------------------------------------------------------------------------

int gmon(void) {
	doExit = false;
	end_addr = 0x00;
	current_addr = 0x00;
	puts("g'mon version " GMON_VERSION "\r");
	while (!doExit) {
		print_prompt();
		parse = inBuffer;							// Set the parse pointer to the beginning of the buffer
		gets(inBuffer, BUFFLEN);					// Get user input
		skipBlank();								// Skip and leading spaces
		putc('\r');
		if (!noArgs()) {
			numLoops = 1;
			numCMDs = 0x01;
			skipBlank();
			if (*parse == '/') {
				cmdStart = parse++;
				while (*cmdStart != '/') cmdStart++;
				*cmdStart = ' ';
				numLoops = (uint16_t)strToHEX();
				parse++;
			}
			cmdStart = parse;
			for (tmp = parse; *tmp != '\0'; tmp++) if (*tmp == ';') numCMDs += 1;
			while (numLoops > 0) {
				gsh_do_cmd(numCMDs);
                if (isBreak()) {
					throw(errBREAK);
					numLoops = 0;
				} else {
					numLoops--;
					parse = cmdStart;
				}
			}
		} else throw(errUNDEF);
	}
	return true;
}

static bool gsh_do_cmd(uint8_t num) {
	uint8_t i, stat;
	char *tmp;
	for (tmp = parse; *tmp != '\0'; tmp++)
		if (*tmp == ';') *tmp = '\0';

	while(num > 0) {
		skipBlank();
		for (i = 0; !(*parse == funcKeys[i]) && i < NUM_FUNCS-1; i++);	// Identify what function it is
		if (i == NUM_FUNCS-1) {
			stat = setCurrents();
			if (stat != errNONE) {
				throw(stat);				// If none matches, complain
				return false;
			}
		} else {
			skipToken();					// Skip over the function name itself
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
enum errList gsh_exit(void) {
	enforceArgc0();
	doExit = true;
	return errNONE;
}

enum errList gsh_version(void) {
	enforceArgc0();
	puts("g'mon version " GMON_VERSION "\r");
	return errNONE;
}

enum errList gsh_help(void) {
	uint8_t i = 0;
	enforceArgc0();
	while (helpText[i] != NULL) puts(helpText[i++]);
	return errNONE;
}

enum errList gsh_echo(void) {
	requireArg0();

	puts(parse);
	putc('\n');
	return errNONE;
}

/* Starts executing code from a place in memory */
enum errList gsh_execute(void) {
	void (*ptr)(void) = (void*)current_addr;
	getArg1(ptr);

	(*ptr)();			// Call that function
	return errNONE;		// Return error free, assuming that whatever we call actually returns (good chance it wont)
}

/* Writes bytes to memory */
enum errList gsh_deposit(void) {
	uint8_t val, *ptr = (uint8_t*)current_addr;			// Create pointers for the start and end of the section

	requireArg0();
	while (!noArgs()) {
		val = (uint8_t)strToHEX();
		*(uint8_t*)(ptr++) = (uint8_t)val;
		skipHex();
	}

	return errNONE;
}

/* Handles viewing of memory */
enum errList gsh_view(void) {
	char *ptr, *end;									// Create start and end pointers
	uint8_t stat;

	skipBlank();
	if (!noArgs()) {
		while(*parse != '\0') {
			stat = getRange(&ptr, &end);
			if (stat != errNONE) return stat;
			stat = dump_range(ptr, end);
		}
	} else {
		stat = dump_range(current_addr, end_addr);
	}

	return stat;
}

/* Copies a memory range to other memory */
enum errList gsh_copy(void) {
	char *ptr, *end, *dest;								// Create pointers for start, end, and destination of block
	uint8_t stat;

	skipBlank();
	if (isRange()) {
		stat = getRange(&ptr, &end);
		if (stat != errNONE) return stat;
	} else {
		ptr = current_addr;
		end = end_addr;
	}

	if (end == NULL) return errSYNTAX;
	requireArg1(dest);

	if (dest <= ptr) {									// If the destination is below the source in memory,
		while (ptr <= end)
			*(uint8_t*)(dest++) = *ptr++;
	} else {
		dest += end - ptr;								// If the destination is above the start of the source,
		while (end >= ptr) {
			*(uint8_t*)(dest--) = *end--;
			if (end == 0x00) break;
		}
	}													// This is done to avoid overwriting the source before we can copy it

	return errNONE;
}

/* Copies a memory range to other memory, zeroing the source */
enum errList gsh_move(void) {
	uint8_t *ptr, *end, *dest;							// Create pointers for start, end, and destination of block
	uint8_t stat;

	skipBlank();
	if (isRange()) {
		stat = getRange(&ptr, &end);
		if (stat != errNONE) return stat;
	} else {
		ptr = current_addr;
		end = end_addr;
	}

	if (end == NULL) return errSYNTAX;
	requireArg1(dest);

	if (dest <= ptr) {									// If the destination is below the source in memory,
		while (ptr <= end) {
			*(uint8_t*)(dest++) = *ptr;					// Copy it starting at the beginning
			*(uint8_t*)(ptr++) = NULL;
		}
	} else {
		dest += end - ptr;								// If the destination is above the start of the source,
		while (end >= ptr) {
			*(uint8_t*)(dest--) = *end;
			*(uint8_t*)(end--) = NULL;
			if (end == 0x00) break;
		}
	}													// This is done to avoid overwriting the source before we can copy it

	return errNONE;
}

/* Fills a memory range with a byte value */
enum errList gsh_fill(void) {
	uint8_t *ptr, *end, val;
	uint8_t stat;

	skipBlank();
	if (isRange()) {
		stat = getRange(&ptr, &end);
		if (stat != errNONE) return stat;
	} else {
		ptr = current_addr;
		end = end_addr;
	}

	if (end == NULL) return errSYNTAX;
	requireArg2(val, uint8_t);

	while (ptr <= end) *(uint8_t*)(ptr++) = val;		// Set every byte from *ptr to *end to the pattern in val
	return errNONE;
}
