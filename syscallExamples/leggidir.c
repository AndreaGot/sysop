#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

struct dirent *readdir(DIR *dirp);

char * percorso;
DIR *cartella;
struct dirent *elemento;

void scorriFolder(char* nome);

char * scrivi(char * a, char * b)
{
	
	char *targetdir = malloc(2048);
	strcpy(targetdir,a);
	strcat(targetdir,"/");
	strcat(targetdir,b);
	
	printf("%s \n", targetdir);	
	return targetdir;
}


main(int argc, char * argv[]) {
	scorriFolder(argv[1]);
}



void scorriFolder(char* nome)
{
	if ((cartella = opendir(nome)) == NULL)			
		perror("opendir() error");						
	else {
		//printf("contents of root: \n");
		while ((elemento = readdir(cartella)) != NULL)
		{				
			if ( strcmp(elemento->d_name, ".")  == 0)
			{
				continue;
			}
			if ( strcmp(elemento->d_name, "..")  == 0)
			{
				continue;
			}
				if(elemento->d_type == DT_DIR)
				{
				
					{
						percorso = scrivi(nome, elemento->d_name); //quando esce dalla cartella il percorso rimane lungo
						scorriFolder(percorso);
					}
				}
				else 
				{
					printf("  %s\n", elemento->d_name);	
				}
			}
			
	}
				
				
														
		closedir(cartella);								
	}


