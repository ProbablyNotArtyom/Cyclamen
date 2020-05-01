//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard exit codes for use with G'DOS shell

#ifndef HEADER_GSHELL
#define HEADER_GSHELL

	#include <stdint.h>
	#include <std.h>
	#include <ctype.h>
	#include <io.h>

//---------------------------------------------------

	#define GMON_VERSION	"1.2"
	#define	BUFFLEN			0xFF

	#define	ADDRSIZE	uint16_t
	#define NUM_FUNCS		6

//---------------------------------------------------

#define isEOI()		(*skipBlank() == '\0')

#define ifEOI(err)	if (*skipBlank() == '\0') return err;

#define getArg(var)						\
			if (*skipBlank() == '\0') return errNOARGS;		\
			if (isVar()) var = (void*)getMonVar(*parse++); \
			else var = (void*)strToHEX();

#define queryBreak()	if (getc() == 0x09) return errBREAK;

static enum errList {
		errNONE,
		errSYNTAX,
		errUNDEF,
		errNOARGS,
		errEND,
		errHEX,
		errBADRANGE,
		errBREAK,
		errDOEXIT
};

extern const char funcKeys[];
extern const char* const errors[];
extern const char hexTable[];

extern bool isCurrentVar;
extern char *parse;
extern char *current_addr;
extern char *end_addr;
extern char *cmdStart;

int vprintf(const char *format, ...);

void print_prompt(void);
bool isRange(void);
bool isVar(void);
bool isArg(void);
void evalScript(void);
bool setCurrents(void);
bool isAddr(void);
char* skipBlank(void);
char* skipToken(void);
char* skipHex(void);
bool funcCmp(const char s1, const char s2);
ADDRSIZE strToHEX(void);
enum errList throw(uint8_t index);
uint32_t *getMonVar(char var);
void setMonVar(char var, ADDRSIZE val);
bool getRange(char **lower, char **upper);
void printHex(char num);
void printByte(char num);
void printWord(uint16_t num);
void printLong(uint32_t num);

//---------------------------------------------------

#endif
