/*
 * This program writes out the difference between the address of
 * a local variable and a passed-in variable.
 */

#include <stdio.h>

char buf[256];

main(int argc, char **argv)
{
	int i;

	sprintf(buf,"Address of local %u\n", &i);
	write(1,buf, strlen(buf));
	sprintf(buf,"Address of parameter %u\n", &argc);
	write(1,buf, strlen(buf));
	sprintf(buf,"Difference is %u\n", ((char *)&argc) - ((char *)&i));
	write(1,buf, strlen(buf));
}
