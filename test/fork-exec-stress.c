/*
 * fork.c -- forks a process and has parent and child print something
 */

#include <stdio.h>

int MaxProcs()
{
	int count;
	int pid;
	int i;

	count = 0;
	while(1) {
		pid = fork();
		if(pid > 0) {
			count++;
			if(count == 7) {
				break;
			}
		} else if(pid == 0) {
			for(i=0; i < 1000; i++);
			exit(0);
		} else {
			break;
		}
	}
	while((pid = wait()) > 0);
	return(count);
}

main(int argc, char **argv)
{
	int procs;
	int max;
	int pids[8];
	int pid;
	int status;
	int i;
	int j;
	int k;

	if(argc < 3) {
		fprintf(stderr,"usage: fork-exec-stress numprocs path-to-program\n");
		exit(1);
	}

	procs = atoi(argv[1]);
	if((procs <= 0) || (procs > 7)) {
		fprintf(stderr,"numproces must be > 0 and less than 8\n");
		exit(1);
	}

	/*
 	 * self tune
 	 */
	max = MaxProcs();

	if(max < procs) {
		printf("procs set to %d as max\n",max);
		procs = max;
	} else {
		printf("running with %d procs\n",procs);
	}
	fflush(stdout);


	for(j=0; j < procs; j++) {
		pids[j] = 0;
	}
	for(i=0; i < 10; i++) { // loop 10 times
		printf("starting iteration %d\n",i);
		fflush(stdout);
		for(j = 0; j < procs; j++) { // numprocs
			pids[j] = fork();
			if(pids[j] < 0) {
				fprintf(stderr,"fork failed, iteration %d, proc %d\n",
					i,j);
				exit(1);
			}
			if(pids[j] == 0) { // I am the child
				execve(argv[2],&argv[2]);
				fprintf(stderr,"execve failed, iteration %d, proc %d\n",
					i,j);
				exit(1);
			}
		}
		for(k=0; k < procs; k++) { // reap all of the childern
			pid = wait(&status);
			if(pid <= 0) {
				fprintf(stderr,"wait failed with code %d\n",pid);
				exit(1);
			}
			for(j=0; j < procs; j++) { // find specific child
				if(pids[j] == pid) {
					pids[j] = 0;
					break;
				}
			}
		}
		printf("parent reaping at iteration %d\n",i);
		fflush(stdout);
		for(j=0; j < procs; j++) { // make sure all are reaped
			if(pids[j] != 0) {
				fprintf(stderr,"pid: %d not properly reaped\n",pids[j]);
				exit(1);
			}
		}
		printf("finished iteration %d\n",i);
		fflush(stdout);
	}
			
	exit(0);


}
