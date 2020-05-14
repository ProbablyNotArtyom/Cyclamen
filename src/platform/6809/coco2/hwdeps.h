//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_COCO2
#define	CONFIG_COCO2

#define CPU_6809

//---------------------------------------------------

#define DEV_VIRTSCREEN
#define SCREEN			0x400
#define SCREEN_WIDTH	32
#define SCREEN_HEIGHT	16

#define DEV_PLAT_COCO2

#define peek_def 		dev_plat_coco2_peek
#define putc_def 		dev_virtscreen_putc
#define getc_def 		dev_plat_coco2_getc
#define tty_init_def 	dev_virtscreen_init

//---------------------------------------------------

#endif
