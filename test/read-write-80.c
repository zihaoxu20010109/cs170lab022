/*
 * read-write-80.c -- A simple cat program to test Linux semantics
 */

#include <stdio.h>

main()
{
	char ch[80];
	int n;
	n = read(0, ch, 80);
	write(1, &ch[0], n);
	if (n < 0) {
		perror("cat");
	}
}
