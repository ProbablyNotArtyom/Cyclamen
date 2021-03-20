//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_PCXT
#define	CONFIG_PCXT

	#include <i86.h>
	#include <bios.h>

//---------------------------------------------------

//#define DEV_PLAT_XT
#define DEV_DUMMY

#define peek_def 		dev_dummy_peek
#define putc_def 		dev_dummy_putc
#define getc_def 		dev_dummy_getc
#define tty_init_def 	dev_dummy_init

//---------------------------------------------------

#endif
