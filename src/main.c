//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <stdbool.h>
	#include <std.h>

	#include "gmon.h"

//---------------------------------------------------

extern int gmon(void);
char mon_exit_status;
char testInput[20];

//---------------------------------------------------

//int main(int argc, char **argv) {
int main(void) {
	tty_init();
	puts("\n### cyclamen " CYCLAMEN_VERSION " ###\n");
	puts("### notartyom, rev (" CYCLAMEN_DATE ") ###\n\n");

	puts("starting g'mon...\n");
	mon_exit_status = gmon();
	puts("\n### exited with value: ");
	printByte(mon_exit_status);
	puts(" ###\n");
	while (true) {}
	return 0;
}
