/*
 * This program climbs up the stack printing out values. It goes up
 * 4 bytes at a time printing the integer representation of the 4 bytes
 * and the characters of the 4 bytes. It continues until it eventually
 * seg faults when it runs off the end of the stack.
 */

#include <stdio.h>

char buf[256];

main(int argc, char **argv, char **envp)
{
	int  *i_ptr;
	char *c_ptr;

	sprintf(buf, "Starting at &argc (%u)\n", &argc);
	write(1,buf, strlen(buf));
	i_ptr = &argc;
	while (1) {
		c_ptr = (char *)i_ptr;
		sprintf(buf,"at address %u is %d (%c%c%c%c)\n",i_ptr, *i_ptr,
			c_ptr[0], c_ptr[1], c_ptr[2], c_ptr[3]);
		write(1,buf, strlen(buf));
		read(0,buf,1);
		i_ptr++;
	}
}
