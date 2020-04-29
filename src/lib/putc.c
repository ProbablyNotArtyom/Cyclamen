
	#include <std.h>
	#include <hwdeps.h>
	#include <io.h>

//---------------------------------------------------

static void scroll_screen(void);

static char xpos = 0, ypos = 0;
int putc(char chr) {
	if (xpos >= SCREEN_WIDTH || chr == '\n' || chr == '\r') {
		xpos = 0;
		ypos++;
	}
	if (ypos >= SCREEN_HEIGHT) {
		ypos--;
		scroll_screen();
	}

	if (chr != '\n' && chr != '\r') {
		*(char*)(SCREEN + (SCREEN_WIDTH*ypos) + xpos) = chr;
		xpos++;
	}
	return 0;
}

static void scroll_screen(void) {
	char i, x;
	char *scrptr;
	for (i = 0; i < (SCREEN_HEIGHT-1); i++) {
		scrptr = (char*)(SCREEN+(SCREEN_WIDTH*i));
		for (x = 0; x < SCREEN_WIDTH; x++) scrptr[x] = scrptr[SCREEN_WIDTH + x];
	}
	scrptr = (SCREEN + (SCREEN_WIDTH*(SCREEN_HEIGHT-1)));
	for (i = 0; i < SCREEN_WIDTH; i++) scrptr[i] = ' ';
}
