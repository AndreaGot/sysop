#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERMS 0666
#define LUNG 1024

void scrivi(char * a, char * b);

int main (int argc, char *argv[])
{
	scrivi(argv[1],argv[2]);
}

void scrivi(char * a, char * b)
{
	
	char *targetdir = malloc(2048);
	strcpy(targetdir,a);
	strcat(targetdir,"/");
	strcat(targetdir,b);
	
	printf("%s", targetdir);	
}