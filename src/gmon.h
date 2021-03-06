//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

#ifndef HEADER_GMON
#define HEADER_GMON

	#include <stdint.h>
	#include <std.h>
	#include <ctype.h>
	#include <io.h>

//---------------------------------------------------

	#define GMON_VERSION	"1.3"
	#define	BUFFLEN			0xFF

	#define	ADDRSIZE		uint16_t
	#define NUM_FUNCS		11

//---------------------------------------------------

#define getArg(...) 			VA_MACRO(getArg, __VA_ARGS__)
#define getArg0()				getArg1(0)
#define getArg1(var)			getArg2(var, void*)
#define getArg2(var, type)		if (!noArgs() && isAddr()) var = (type)strToHEX()

#define requireArg(...) 		VA_MACRO(requireArg, __VA_ARGS__)
#define requireArg0()			if noArgs() return errNEEDS_ARGS
#define requireArg1(var)		requireArg0(); else var = (void*)strToHEX()
#define requireArg2(var, type)	requireArg0(); else var = (type)strToHEX()

#define enforceArgc(...) 		VA_MACRO(enforceArgc, __VA_ARGS__)
#define enforceArgc0()			if (!noArgs()) return errEXTRA_ARGS

#define noArgs()			(*skipBlank() == '\0')
#define queryBreak()		if isBreak() return errBREAK;
#define testArg(err)		if noArgs() return err

#ifndef KEY_BREAK
	#define isBreak()		(0)
#else
	#define isBreak()		(peek() == KEY_BREAK)
#endif

typedef uint8_t err_t;
enum errList {
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

extern const char const funcKeys[];
extern const char *const errors[];
extern const char const hexTable[];

extern bool isCurrentVar;
extern char *parse;
extern char *current_addr;
extern char *end_addr;
extern char *cmdStart;

void print_prompt(void);
err_t dump_range(char *ptr, char *end);
bool isRange(void);
bool isArg(void);
err_t setCurrents(void);
bool isAddr(void);
char* skipBlank(void);
char* skipToken(void);
char* skipHex(void);
ADDRSIZE strToHEX(void);
err_t throw(uint8_t index);
err_t getRange(char **lower, char **upper);
void printHex(char num);
void printByte(char num);
void printWord(uint16_t num);
void printLong(uint32_t num);

//---------------------------------------------------

#endif
