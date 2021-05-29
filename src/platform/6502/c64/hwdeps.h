//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_C64
#define	CONFIG_C64

	#include <c64.h>

//---------------------------------------------------

#define DEV_VIRTSCREEN
#define SCREEN			0x400
#define SCREEN_WIDTH	40
#define SCREEN_HEIGHT	25

#define DEV_PLAT_C64
#define DEV_DUMMY

#define peek_def 		dev_dummy_peek
#define putc_def 		dev_virtscreen_putc
#define getc_def 		dev_plat_c64_getc
#define tty_init_def 	dev_virtscreen_init

#define PLATFORM_SCROLL	plat_c64_scroll
extern void plat_c64_scroll(void);

//---------------------------------------------------

#endif
