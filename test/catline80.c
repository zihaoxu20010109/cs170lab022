/*
 * catline80.c -- A simple cat program. Print of 80 chars per line
 */

#include <stdio.h>

main()
{
	char ch[81];
	int n;
	while ( (n = read(0, ch, 80)) > 0) {
		ch[n] = '\n';
		write(1, &(ch[0]), n+1);
	}
	if (n < 0) {
		perror("catline80");
	}
}
