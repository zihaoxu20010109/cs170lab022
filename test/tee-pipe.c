/*
 * simple program to replicate stdin among downtream processes
 *
 * example:
 * tee-pipe prog1 prog2 prog3
 *
 * forks prog1, prog2, and prog3 with pipes and stdin and copies
 * stdin to each 
 *
 * the downtream processes do not get an argv
 *
 * the program assumes an 8K pipe buffer
 *
 * -p option (which must be first arg) runs the downstream processes in parallel
 *
 * example:
 * tee-pipe -p prog1 prog2 prog3
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int i;
	int j;
	int pid;
	char *child_argv[2];
	int sent;
	int bytes;
	int *pd;
	int in_parallel;
	int prog_count;
	int err;
	char in_buf[1024*8];

	in_parallel = 0;
	child_argv[1] = NULL;

	if(argc < 2) {
		fprintf(stderr,
			"must specify at least one downstream program\n"); 
		fprintf(stderr,
			"Usage: tee-pipe [-p] prog1 prog2 prog3\n");
		exit(1);
	}

	if((argc < 3) && (strcmp(argv[1],"-p") == 0)) {
		fprintf(stderr,"must specify a program with -p option\n");
		fprintf(stderr,
			"Usage: tee-pipe [-p] prog1 prog2 prog3\n");
		exit(1);
	}

	if(argc > 8) {
		fprintf(stderr,"at best, tee-pipe can run 7 children on kos\n");
		fprintf(stderr,
			"Usage: tee-pipe [-p] prog1 prog2 prog3\n");
		exit(1);
	}

	if(strcmp(argv[1],"-p") == 0) {
		in_parallel = 1;
		prog_count = argc - 2;
	} else {
		prog_count = argc - 1;
	}


	/*
 	 * get space for pipe descriptors for each process
 	 */
	pd = (int *)malloc(prog_count * 2);
	if(pd == NULL) {
		exit(1);
	}

	/*
 	 * create all of the pipes first
 	 */
	for(i=0; i < prog_count; i++) {
		err = pipe(&pd[i*2]);
		if(err < 0) {
			fprintf(stderr,"ERROR: could not create pipe for prog %d\n",
				i);
			exit(1);
		}
	}

	/*
 	 * fork, dup, exec
 	 */
	for(i=0; i < prog_count; i++) {
		pid = fork();
		if(pid < 0) {
			fprintf(stderr,"ERROR: could not fork prog %d\n",
				i);
			exit(1);
		}
		if(pid == 0) { /* I am the child */
			/*
 			 * close everything I'm not using
 			 */
			for(j=0; j < prog_count; j++) {
				if(j != i) {
					close(pd[j*2+0]);
					close(pd[j*2+1]);
				}
			}
			/*
 			 * close stdin
 			 */
			close(0);
			/*
 			 * close my unneeded write end of the pipe
 			 */
			close(pd[i*2+1]);
			/*
 			 * dup the read end of the pipe
 			 */
			dup(pd[i*2+0]);
			/*
 			 * close the duplicate now that it is in 0
 			 */
			close(pd[i*2+0]);
			/*
 			 * point child argv at program name
 			 */
			if(in_parallel == 0) {
				child_argv[0] = argv[1];
			} else {
				child_argv[0] = argv[2];
			}
			execve(child_argv[0],child_argv,NULL);
			fprintf(stderr,"ERROR: could not execve %s\n",
				child_argv[0]);
			exit(1);
		}

		/*
 		 * parent closes read end since parent doesn't need it
 		 */
		close(pd[i*2+0]);
	}

	/*
 	 * now write the stdin of the parent into each of the pipes to the
 	 * children
 	 */
	if(in_parallel == 0) { /* write a block at a time */
		bytes = read(0,in_buf,sizeof(in_buf));
		while(bytes > 0) {
			for(i=0; i < prog_count; i++) {
				sent = 0;
				while(sent < bytes) {
					sent = write(pd[i*2+1],&in_buf[sent],bytes - sent);
					if(sent <= 0) {
						fprintf(stderr,"ERROR: write returned %d\n",err);
						exit(1);
					}
					sent = sent + bytes;
				}
			}
			/*
 		 	 * if there is more to send
 		 	 */
			bytes = read(0,in_buf,sizeof(in_buf));
		}
	} else { /* write a character at a time */
		bytes = read(0,&in_buf[0],1);
		while(bytes > 0) {
			for(i=0; i < prog_count; i++) {
				err = write(pd[i*2+1],in_buf,1);
				if(err != 1) {
					fprintf(stderr,"ERROR: single write for prog %d returned %d\n",
						i,err);
					exit(1);
				}
			}
			bytes = read(0,&in_buf[0],1);
		}
	}
	/*
 	 * close the write ends of the pipes
 	 */
	for(i=0; i < prog_count; i++) {
		close(pd[i*2+1]);
	}

	/*
 	 * free the pd memory
 	 */
	free(pd);

	/*
 	 * wait for all of the children to exit
 	 */
	for(i=0; i < prog_count; i++) {
		pid = wait(NULL);
	}

	return(0);
}
