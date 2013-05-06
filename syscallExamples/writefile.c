/* Program to create a file and write some data the file */
#include <stdio.h>
#include <string.h>
main( int argc, char * argv[])
{
     FILE *fp;
	 FILE *sp;
     char stuff[512];
     int index;
     fp = fopen(argv[1],"r"); /* open for reading */
     strcpy(stuff,argv[2]);
     for (index = 1; index <= 10; index++)
     	fprintf(fp,"%s",stuff);
     fclose(fp); /* close the file before ending program */
}