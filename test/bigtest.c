/*
 * should cause excessive paging
 */
#include <stdio.h>

#define BIG (1024*1024*2)


main()
{
	char stack_buf[BIG];
	char *global_buf;
	int i;
	int pid;

	pid = getpid();

	global_buf = malloc(BIG);

	for(i=0; i < BIG; i++)
	{
		stack_buf[i]++;
		global_buf[i]++;

		if((i % 512) == 0)
		{
			printf("%d touching new pages\n",
				pid);
		}
	}
	free(global_buf);
}

