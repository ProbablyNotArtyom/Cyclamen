//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <stdbool.h>
	#include <std.h>

//---------------------------------------------------

extern int gmon(void);

//---------------------------------------------------

//int main(int argc, char **argv) {
int main(void) {
	tty_init();
	while (true) gmon();
	return 0;
}
