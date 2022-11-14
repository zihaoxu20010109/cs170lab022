/*
 * instrerr.c -- This program tries to perform an illegal instruction.
 */

main(int argc)
{
	int *p = 0;
	while(1)
		*(p++) =  0x0000000d;
}
