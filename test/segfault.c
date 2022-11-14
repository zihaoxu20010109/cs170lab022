/*
 * segfault.c -- This program seg faults
 */

main()
{
	char *p = -1;
	*p = 0;
}
