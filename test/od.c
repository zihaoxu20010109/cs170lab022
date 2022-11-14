/*
 * this program dumps the stack until the end of argv
 *
 * usage: od arg1 arg2 arg3
 */

#include <stdio.h>
#include <ctype.h>

void od(char *start, char *end)
{
	unsigned int addr;
	unsigned char *curr;
	int i;
	char buf[1024];

	/*
 	 * back up to nearest 4-byte boundary
 	 */
	addr = (unsigned int)start;
	while((addr % 4) != 0) {
		addr--;
	}

	while(addr <= (unsigned int)end) {
		curr = (unsigned char *)addr;
		sprintf(buf,"%10d: %10u | ",
			addr,
			*((unsigned int *)curr));
		write(1,buf,strlen(buf));
		for(i=0; i < 4; i++) {
			if(isprint(curr[i])) {
				sprintf(buf,"%3c ",curr[i]);
			} else {
				sprintf(buf,"%3u ",curr[i]);
			}
			write(1,buf,strlen(buf));
		}
		write(1,"\n",strlen("\n"));
		addr += 4;
	}

	return;
}



main(int argc, char **argv, char **envp)
{
	/*
 	 * dumps the stack (decimal and ascii) from the parameter location
 	 * to the last character in argv
 	 */
	unsigned char *top_addr;
	int i;
	int j;

	top_addr = 0;
	for(i=0; i < argc; i++) {
		for(j=0; j < strlen(argv[i])+1; j++) {
			if(top_addr < &(argv[i][j])) {
				top_addr = &(argv[i][j]);
			}
		}
	}
		
	od(&argc,top_addr);
	exit(0);
}
