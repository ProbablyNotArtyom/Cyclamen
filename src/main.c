
	#include <stdbool.h>
	#include <std.h>

int main(int argc, char **argv) {
	int i = 0;
	while (true) {
		while (i++ < 20) {
			int x = 0;
			if (i < 10) {
				while (x < i) {
					putc(' ');
					x++;
				}
			} else {
				while (x < (20 - i)) {
					putc(' ');
					x++;
				}
			}
			puts("cyclamen\n");
		}
		i = 0;
	}
	return 0;
}
