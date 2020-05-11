
	#include <stdbool.h>
	#include <std.h>

extern int gmon(void);

int main(int argc, char **argv) {
	while (true) gmon();
	return 0;
}
