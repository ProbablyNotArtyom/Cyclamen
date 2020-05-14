//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------
    
	#include <hwdeps.h>
	#include <std.h>
	#include <stdbool.h>
	#include <io.h>

#ifdef DEV_DUMMY
#define DEV_DUMMY
	
    extern int dev_dummy_putc(char chr);
    extern char dev_dummy_getc(void);
    extern char dev_dummy_peek(void);
    extern char dev_dummy_init(void);

//---------------------------------------------------

int dev_dummy_putc(char chr) {
	return (int)chr;
}

char dev_dummy_getc(void) {
	while (true);
    return (char)NULL;
}

char dev_dummy_peek(void) {
	return (char)NULL;
}

char dev_dummy_init(void) {
    return (char)NULL;
}

#endif
