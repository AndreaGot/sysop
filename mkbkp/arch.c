#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <stdbool.h>
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


void show_file (const char *filename, FILE *out) {
    int c;
    FILE *file;
    //fprintf(out, "%s(%s) BEGIN\n", __func__, filename);
    file = fopen(filename, "r");
    if (file) {
        while ((c = fgetc(file)) != EOF) {
            fputc(c, out);
        }
        fclose(file);
    } else {
        fprintf(out, "%s: failed to open file '%s' (%d)\n", __func__, filename, errno);
    }
    //fprintf(out, "%s(%s) END\n", __func__, filename);
}


int main(int argc, char * argv[])
{
	char * nome;
	nome = argv[1];
	
	
	archivio = scrivi(getcwd(NULL, 0), nome);
    arch = fopen(archivio, "w");
	
	int i=3;
	
	
	fprintf(arch, "%s", LIST);
	fprintf(arch, "%s", " ");
	
	while(i<=argc)
	{
		copy = strdup(argv[i-1]);
		filename = basename(argv[i-1]);
		puts(filename);	
		fprintf(arch, "%s", filename);
		fprintf(arch, "%s", " ");
		i++;
	}
	
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", LIST);
	fprintf(arch, "%s", "\n");
	
	
	fprintf(arch, "%s", DIRS);
	fprintf(arch, "%s", " ");
	
	//INSERIRE UN WHILE CHE INSERISCE LE EVENTUALI CARTELLE FORMANTI IL PERCORSO
	
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", DIRS);
	fprintf(arch, "%s", "\n");
	i=3;
	while(i<=argc)
	{
		fprintf(arch, "%s", CONTENT);
		fprintf(arch, "%s", " ");
		show_file(argv[i-1], arch);
		fprintf(arch, "%s", " ");
		fprintf(arch, "%s", CONTENT);
		
		fprintf(arch, "%s", "\n");
		
		i++;
		
		
	}
	
	return 0;
}
