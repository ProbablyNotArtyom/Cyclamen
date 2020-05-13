//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

#ifndef HEADER_GSHELL
#define HEADER_GSHELL

	#include <stdint.h>
	#include <std.h>
	#include <ctype.h>
	#include <io.h>

//---------------------------------------------------

	#define GMON_VERSION	"1.2"
	#define	BUFFLEN			0xFF

	#ifndef KEY_BREAK
	#define KEY_BREAK		' '
	#endif

	#define	ADDRSIZE		uint16_t
	#define NUM_FUNCS		11

//---------------------------------------------------

#define queryBreak()	if (peek() == KEY_BREAK) return errBREAK;
#define isBreak()		(peek() == KEY_BREAK)

#define noArgs()		(*skipBlank() == '\0')
#define testArg(err)	if (*skipBlank() == '\0') return err

#define getArg(var)										\
		if (*skipBlank() == '\0') return errNEEDS_ARGS;	\
		else var = (void *)strToHEX();

static enum errList {
		errNONE,
		errSYNTAX,
		errUNDEF,
		errEXTRA_ARGS,
		errNEEDS_ARGS,
		errBAD_HEX,
		errBAD_RANGE,
		errBREAK,
		errDO_EXIT
};

extern const char funcKeys[];
extern const char *const errors[];
extern const char hexTable[];

extern bool isCurrentVar;
extern char *parse;
extern char *current_addr;
extern char *end_addr;
extern char *cmdStart;

void print_prompt(void);
enum errList dump_range(char *ptr, char *end);
bool isRange(void);
bool isArg(void);
enum errList setCurrents(void);
bool isAddr(void);
char* skipBlank(void);
char* skipToken(void);
char* skipHex(void);
ADDRSIZE strToHEX(void);
enum errList throw(uint8_t index);
enum errList getRange(char **lower, char **upper);
void printHex(char num);
void printByte(char num);
void printWord(uint16_t num);
void printLong(uint32_t num);

//---------------------------------------------------

#endif
