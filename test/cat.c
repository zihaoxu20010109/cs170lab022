/*
 * cat.c -- A simple cat program. You may want to change the arguments
 *          to bogus values and see if you jos can handle it properly.
 */

#include <stdio.h>

main()
{
	char ch;
	int n;
	while ( (n = read(0, &ch, 1)) > 0) {
		write(1, &ch, n);
	}
	if (n < 0) {
		perror("cat");
	}
}
