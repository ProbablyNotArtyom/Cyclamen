//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <std.h>
	#include <stdbool.h>
	#include <hwdeps.h>
	#include <io.h>

//---------------------------------------------------

char i, key;
char *gets(char *buff, int len) {
    i = 0;
	while (i < len) {
        key = getc();
		if (key == 0x7F && i > 0) {
			puts("\b \b");
			i--;
			buff[i] = ' ';
		} else {
			buff[i] = key;
			putc(key);
			if (key == '\r') break;
            i++;
        }
	}
	buff[i] = '\0';
	return buff;
}
