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

#ifdef DEV_PLAT_COCO2
#define DEV_PLAT_COCO2

//---------------------------------------------------

char last_keypress;
char dev_plat_coco2_getc(void) {
	while (true);
	return 'A';
}

char dev_plat_coco2_peek(void) {
    return '\0';
}

//---------------------------------------------------

#endif
