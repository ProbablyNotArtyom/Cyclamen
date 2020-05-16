//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <stdbool.h>
	#include <std.h>

//---------------------------------------------------

extern int gmon(void);

//---------------------------------------------------

//int main(int argc, char **argv) {
int main(void) {
	tty_init();
	puts("\n### cyclamen " CYCLAMEN_VERSION " ###\n");
	puts("### notartyom, rev (" CYCLAMEN_DATE ") ###\n\n");
    while (true) {
		puts("starting g'mon...\n");
		gmon();
	}
	return 0;
}
