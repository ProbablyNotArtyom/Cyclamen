//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <std.h>
	#include <hwdeps.h>
	#include <io.h>

//---------------------------------------------------

int iter;
int puts(const char *str) {
	iter = 0;
	while (str[iter] != '\0') putc(str[iter++]);
	return 0;
}
