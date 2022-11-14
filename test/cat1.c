/*
 * cat1.c -- A simple cat program. Reads and writes 1 char at a time.
 */

#include <stdio.h>

main()
{
	char ch;

         while((ch = getchar()) != EOF) putchar(ch);
}
