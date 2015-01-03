#include <stdio.h>

main (int argc, char **argv)
{
	char byte = 0;
	int column = 0;
	int state = 0;
	int c;
	c = fgetc(stdin);
	while (c != EOF) {
		if (c == ',') {
			state = 1;
		}
		else if (state && c == ' ') {
			if (column > 0 && (column % 8) == 0) {
				fputc((int)byte, stdout);
				byte = 0;
			}
			++column;
		}
		else if (c == '\n') {
			fputc((int)byte, stdout);
			byte = 0;
			column = 0;
			state = 0;
		}
		else if (column) {
			byte *= 2;
			if (c == '1') {
				++byte;
			}
		}
		c = fgetc(stdin);
	}
}

