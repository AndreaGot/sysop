#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#define PERMS 0666
#define LIST "%LIST%"
#define DIRS "%DIRS%"
#define CONTENT "%CONTENT%"


char *copy, *filename, *archivio;

FILE * arch;
FILE * par;
//------ DATO UN FILE (O UNA LISTA DI ESSI), CREA UN ARCHIVIO DI ESSI


char * scrivi(char * a, char * b)
{
	
	char *targetdir = malloc(2048);
	strcpy(targetdir,a);
	strcat(targetdir,"/");
	strcat(targetdir,b);
	
	printf("%s", targetdir);	
	return targetdir;
}


void read_words (FILE *f) {
    char x[1024];
	printf("ci sono");
    while (fscanf(f, " %s", x) == 1) {
		printf("here i am");
		fprintf(arch," %s", x);
		puts(x);
    }
}



int main(int argc, char * argv[])
{
	char * nome;
	nome = argv[1];
	
	
	archivio = scrivi(getcwd(NULL, 0), nome);
    arch = fopen(archivio, "w");
	
	int i=3;
	
	while(i<=argc)
	{
		copy = strdup(argv[i-1]);
		filename = basename(argv[i-1]);
		puts(filename);	
		i++;
	}
	
	
	par = fopen(argv[2], "r+");
	read_words(par);
	
	fprintf(arch, "%s", LIST);
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", filename);
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", LIST);
	fprintf(arch, "%s", "\n");
	fprintf(arch, "%s", DIRS);
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", DIRS);
	fprintf(arch, "%s", "\n");
	fprintf(arch, "%s", CONTENT);
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", CONTENT);
	return 0;
}

