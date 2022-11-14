/*
 * This program writes out the the strings in the argv array.
 * It uses the &argc as a baseline so that addresses are relative to the stack
 * pointer and not absolute addresses (since those will differ depending on
 * the placement of global variables that the compile chooses)
 */

#include <stdio.h>

void Memcpy(unsigned char *dst, unsigned char *src, unsigned int size)
{
	int i;

	for(i=0; i < size; i++) {
		dst[i] = src[i];
	}
}

main(int argc, char **argv, char **envp)
{
	int i;
	int j;
	int temp_i;
	char buf[256];
	unsigned int base;
	char *s;
	unsigned int curr;
	unsigned int min = 0xFFFFFFFF;

	/*
 	 * find the lowest address used by any string in argv[] and use that
 	 * as a reference point
 	 */
	for(i=0; i < argc; i++) {
		if((unsigned int)argv[i] < min) {
			min = (unsigned int)argv[i];
		}
	}
	

	base = min;

	while((base %4) != 0) {
		base--;
	}
	/*
 	 * back up by the number of pointers in argv[] + 1 for the NULL
 	 */
	base -= ((argc+1) * sizeof(int));
	Memcpy((char *)&temp_i,(char *)base,sizeof(int));

	/*
 	 * back up 6 more words -- the format is
 	 *
 	 * 0
 	 * 0
 	 * 0
 	 * argc
 	 * &argv
 	 * &envp (which should be zero)
 	 */
	base -= (6 * sizeof(int));

	/*
 	 * now print them out -- first three should be zero
 	 */
	Memcpy((char *)&temp_i,(char *)base+0,sizeof(temp_i));
#ifdef PRINTADDR
	sprintf(buf,"addr\t%d:\t%d\n",base + 0,temp_i);
#else
	sprintf(buf,"offset\t%d:\t%d\n",0,temp_i);
#endif
	write(1,buf,strlen(buf));

	Memcpy((char *)&temp_i,(char *)base+4,sizeof(temp_i));
#ifdef PRINTADDR
	sprintf(buf,"addr\t%d:\t%d\n",base + 4,temp_i);
#else
	sprintf(buf,"offet\t%d:\t%d\n",4,temp_i);
#endif
	write(1,buf,strlen(buf));

	Memcpy((char *)&temp_i,(char *)base+8,sizeof(temp_i));
#ifdef PRINTADDR
	sprintf(buf,"addr\t%d:\t%d\n",base + 8,temp_i);
#else
	sprintf(buf,"offset\t%d:\t%d\n",8,temp_i);
#endif
	write(1,buf,strlen(buf));

	/*
 	 * argc is next
 	 */
	Memcpy((char *)&temp_i,(char *)base+12,sizeof(temp_i));
#ifdef PRINTADDR
	sprintf(buf,"addr\t%d:\t%d\t-- argc\n",base + 12, temp_i);
#else
	sprintf(buf,"offset\t%d:\t%d\t-- argc\n",12,temp_i);
#endif
	write(1,buf,strlen(buf));

	/*
 	 * then &argv
 	 */
	Memcpy((char *)&temp_i,(char *)base+16,sizeof(temp_i));
#ifdef PRINTADDR
	sprintf(buf,"addr\t%d:\t%d\t-- &argv[0]\n",base + 16,temp_i);
#else
	sprintf(buf,"offset\t%d:\t%d\t-- &argv[0]\n",16,temp_i - base);
#endif
	write(1,buf,strlen(buf));

	/*
 	 * then &envp (which should be zero)
 	 */
	Memcpy((char *)&temp_i,(char *)base+20,sizeof(temp_i));
#ifdef PRINTADDR
	sprintf(buf,"%d:\t%d\t-- &envp\n",base + 20,temp_i);
#else
	sprintf(buf,"offset\t%d:\t%d\t-- &envp\n",20,temp_i);
#endif
	write(1,buf,strlen(buf));

	/*
 	 * then the strings pointed to by argv[] in order
 	 */
	for(i=0; i <= argc; i++) {
		Memcpy((char *)&temp_i,(char *)base + 24 + (i*sizeof(int)),sizeof(int));
		s = (char *)temp_i;
		sprintf(buf,"argv[%d]: %s\n",
			i,
			s);
		write(1,buf,strlen(buf));
	}

	exit(0);
}
