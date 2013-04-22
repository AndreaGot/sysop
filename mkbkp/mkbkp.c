#include <getopt.h>
#include <stdio.h>

void usage();

int main(int argc, char * argv[])
{
	int f,c,x,t,i;
while ( (i = getopt(argc, argv, "fcxt")) != -1)
	{
		switch (i) 
		{
			/*
			 * Handling options
			 */
			case 'f':
				f=1;
				break;
			case 'c':
				c = 1;
				break;
			case 'x':
				x = 1;
				break;
			case 't':
				t = 1;
				break;
			case '?':   /* unrecognized options */
				printf("Unrecognized options -%c\n",optopt);
			default:    /* should not reached */
				usage();
		}
	}
	
	if(f==1)
		printf("crea/estraggo archivio specificato qui \n");
	
	if(c==1)
		printf("creo archivio \n");
	
	if(x==1)
		printf("estraggo archivio \n");
	
	if(t==1)
		printf("elenco archivio \n");
}

void usage()
{
	printf("possibili comandi:\n");
	printf("-f			crea/ estrae archivio\n");
	printf("-x			creo archivio\n");
	printf("-c			estrae archivio\n");
	printf("-t			elenco archivio\n");
}
