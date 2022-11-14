main()
{
  int len;
  int i;
  char s[255];
  char incr;
  char c;

  len = read(0, s, 255);

  incr = 'a' - 'A';

  while(len > 0) {
	for(i=0; i < len; i++)
	{
		if((s[i] >= 'a') && (s[i] <= 'z'))
		{
			c = s[i] - incr;
		}else if((s[i] >= 'A') && (s[i] <= 'Z'))
		{
			c = s[i] + incr;
		}
		else
		{
			c = s[i];
		}

		write(1,&c,1);
	}
	len = read(0, s, 255);
  }


}

