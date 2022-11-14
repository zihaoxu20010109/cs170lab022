/*
 * cat80.c -- A simple cat program. Buffering of 80 chars.
 */

#include <stdio.h>

main()
{
	char ch[80];
	int n;
	while ( (n = read(0, ch, 70)) > 0) {
		write(1, &ch[0], n);
	}
	if (n < 0) {
		perror("cat");
	}
}
