
#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>

int mkdir(const char *pathname, mode_t mode); 

int main(int argc, char * argv[])
{
	
	int a;
	mode_t mode;
	mode = (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	// crea una cartella nel percorso passato come parametro
	
	if(argc==1)
		{
		printf("PREGO SPECIFICARE UN PERCORSO DOVE CREARE LA CARTELLA");
		return 0;	
		}
	
	a = mkdir(argv[1], mode);
	
	if(a==0)
	{
		printf("missione compiuta!");
	}
	
}