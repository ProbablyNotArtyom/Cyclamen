
	#include <std.h>
	#include <hwdeps.h>
	#include <io.h>

//---------------------------------------------------

int puts(const char *str) {
	int i = 0;
	while (str[i] != '\0') putc(str[i++]);
	return 0;
}
