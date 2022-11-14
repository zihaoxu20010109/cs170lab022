#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
/*
 *  * simple program implementing cat
 *   */
void Memset(unsigned char *buffer, unsigned char c, int size)
{
	int i;
	for(i=0; i < size; i++) {
		buffer[i] = c;
	}
	return;
}


int main(int argc, char **argv)
{
	pid_t my_id;
	char buffer[4096];

	my_id = getpid();
	fprintf(stderr,"pid: %d -- I am my-cat and I have started\n",my_id);
	Memset(buffer,0,sizeof(buffer));
	while(read(0,buffer,sizeof(buffer)) > 0) {
		fprintf(stderr,"pid: %d read some data\n",getpid());
		buffer[4095] = 0; /* safety first */
		write(1,buffer,strlen(buffer));
		fprintf(stderr,"pid: %d wrote some data\n",getpid());
		Memset(buffer,0,sizeof(buffer));
	}
	fprintf(stderr,"pid: %d -- I am my-cat and I am exiting\n",my_id);
	exit(0);
}


