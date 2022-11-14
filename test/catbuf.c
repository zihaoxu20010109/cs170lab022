/*
 * cat1.c -- A simple cat program. Reads and writes 1 char at a time.
 */

#include <stdio.h>
#define BUFSIZE (10)

char outbuf[256];

int main()
{
	char buf[BUFSIZE];
	int count;
	int wcount;

	while((count = read(0,buf,sizeof(buf))) != 0) {
		wcount = write(1,buf,count);
		if(wcount != count) {
			sprintf(outbuf,"write error: count: %d, wcount: %d\n",
				count,
				wcount);
			write(1,outbuf,strlen(outbuf));
			exit(1);
		}
	}
	exit(0);
	
}
