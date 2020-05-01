
	#include <std.h>
	#include <stdbool.h>
	#include <hwdeps.h>
	#include <io.h>

//---------------------------------------------------

char *gets(char *buff, int len) {
	char key, i = 0;
	while (true && i < len) {
		key = getc();
		if (key == 0x7F && i > 0) {
			puts("\b \b");
			i--;
			buff[i] = ' ';
		} else {
			buff[i++] = key;
			putc(key);
			if (key == '\r') break;
		}
	}
	buff[i] = '\0';
	return buff;
}
