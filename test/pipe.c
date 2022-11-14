/*
 * pipe.c -- check whether the pipe operates correctly
 */

#define Syscall(x) exit(x)

main(int argc, char *argv[])
{
	int i, j, rc, num_pipes, *fd;
	char wr_buf[100];
	char rd_buf[100];
	char c;

	if (argc < 2 || (num_pipes=atoi(argv[1])) <= 0) {
		printf("Usage: ./pipe <num_pipes>\n");
		Syscall(0);
	}

	fd = (int *)malloc(num_pipes * 2 * sizeof(int));

	for (i=0; i<num_pipes; ++i) {
		rc = pipe(&fd[i*2]);
		if (rc != 0) {
			perror("pipe() failed");
			Syscall(0);
		}
	}

	for (i=0; i<num_pipes; ++i) {
		sprintf(wr_buf, "Pipe %d, out!", i); 
		write(fd[2*i+1], wr_buf, strlen(wr_buf));
		c = '.';
		for (j=0; j<i; ++j)
			write(fd[2*i+1], &c, 1);
		c = '\0';
		write(fd[2*i+1], &c, 1);
		printf("Wrote to pipe %d\n", i);
	}

	for (i=0; i<num_pipes; ++i) {
		read(fd[2*i+0], rd_buf, strlen(wr_buf));
		for (j=0; j<i; ++j)
			read(fd[2*i+0], rd_buf+strlen(wr_buf)+j, 1);
		read(fd[2*i+0], rd_buf+strlen(wr_buf)+i, 1);
		printf("Read '%s' from pipe %i\n", rd_buf, i);
	}

//	for(i=0; i < 10000; i++);
	Syscall(0);
}
