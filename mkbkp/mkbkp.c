#include <getopt.h>
#include <stdio.h>

void usage();

int main(int argc, char * argv[])
{
	int a,b,c,d,e,i;
while ( (i = getopt(argc, argv, "abcde")) != -1)
	{
		switch (i) 
		{
			/*
			 * Handling options
			 */
			case 'a':
				a=1;
				break;
			case 'b':
				b = 1;
				break;
			case 'c':
				c = 1;
				break;
			case 'd':
				d = 1;
				break;
			case 'e':
				e = 1;
				break;
			case '?':   /* unrecognized options */
				printf("Unrecognized options -%c\n",optopt);
				usage();
			default:    /* should not reached */
				usage();
		}
	}
	
	if(a==1)
		printf("scrivo a \n");
	
	if(b==1)
		printf("scrivo b \n");
	
	if(c==1)
		printf("scrivo c \n");
	
	if(d==1)
		printf("scrivo d \n");
	
	if(e==1)
		printf("scrivo e \n");
}

void usage()
{
	printf("possibili comandi:\n");
	printf("-a			scrive a\n");
	printf("-b			scrive b\n");
	printf("-c			scrive c\n");
	printf("-d			scrive d\n");
	printf("-e			scrive e\n");
}