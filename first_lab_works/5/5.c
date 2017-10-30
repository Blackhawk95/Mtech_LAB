#include<stdio.h>
#include<sys/syscall.h>

void main(int argc,char* argv[])
{	int c;
	FILE *f;
	f = fopen(argv[1],"r");
	while( (c = getc(f))!= EOF )
	{	putchar(c);	
	}
	fclose(f);
}
