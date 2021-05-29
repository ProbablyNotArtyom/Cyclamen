//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

    #include <hwdeps.h>
	#include <std.h>
	#include <hwdeps.h>
	#include <io.h>

#ifdef DEV_VIRTSCREEN
#define DEV_VIRTSCREEN

//---------------------------------------------------

    char xpos, ypos;
    const uint8_t scr_width = SCREEN_WIDTH;

    static void scroll_screen(void);
    int dev_virtscreen_putc(char chr);
    char dev_virtscreen_init(void);

//---------------------------------------------------

#ifndef PLATFORM_SCROLL				// Allow platforms to supply their own optimized scrolling routine
	static void scroll_screen(void) {
		char i, x;
		char *scr;
		for (i = 0; i < (SCREEN_HEIGHT-1); ++i) {
			scr = (char *)(SCREEN+(SCREEN_WIDTH*i));
			for (x = 0; x < SCREEN_WIDTH; ++x) scr[x] = scr[SCREEN_WIDTH + x];
		}
		scr = (char *)(SCREEN + (SCREEN_WIDTH*(SCREEN_HEIGHT-1)));
		for (i = 0; i < SCREEN_WIDTH; ++i) scr[i] = ' ';
	}
#else
	static void scroll_screen(void) {
		PLATFORM_SCROLL();
	}
#endif

int dev_virtscreen_putc(char chr) {
	if (xpos >= SCREEN_WIDTH || chr == '\n') {
		xpos = 0;
		++ypos;
		if (ypos >= SCREEN_HEIGHT) {
			--ypos;
			scroll_screen();
		}
	}

	if (chr != '\n' && chr != '\r') {
		*(char *)(SCREEN + (SCREEN_WIDTH*ypos) + xpos) = chr;
		++xpos;
	}
	return 0;
}

char dev_virtscreen_init(void) {
	char x, y;
	char *scr = (char *)SCREEN;
	for (y = 0; y < SCREEN_HEIGHT; ++y) {
		scr = (char *)(SCREEN+(SCREEN_WIDTH*y));
		for (x = 0; x < SCREEN_WIDTH; ++x) scr[x] = ' ';
	}

	// Statically init globals
	xpos = 0;
	ypos = 0;
	puts("virtscreen driver v0.5 (notartyom)\n");
	return 0;
}

#endif
