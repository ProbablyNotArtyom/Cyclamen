//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <std.h>

//---------------------------------------------------

char *gets(char *buff, int len) {
	char i = 0;
	char key;
	while (i < len) {
        key = getc();
		if (key == 0x7F && i > 0) {
			puts("\b \b");
			i--;
			buff[i] = ' ';
		} else {
			buff[i] = key;
			putc(key);
			if (key == '\n') break;
            i++;
        }
	}
	buff[i] = '\0';
	return buff;
}
