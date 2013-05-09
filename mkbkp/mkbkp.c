#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <ftw.h>
#include <sys/stat.h>


#define PERMS 0666
#define LIST "%LIST%"
#define DIRS "%DIRS%"
#define CONTENT "%CONTENT%"
char *copy, *filename, *archivio;



FILE * arch;
FILE * par;
char * nome;

char ** globalargv;
int globalargc;
int lung;

void usage();
char * scrivi(char * a, char * b);
void show_file (const char *filename, FILE *out);
void creabkp(int numpar, char * param[], int ind);
void stampa();
int list(const char *name, const struct stat *status, int type);
int listC(const char *name, const struct stat *status, int type);
int listD(const char *name, const struct stat *status, int type);




int main(int argc, char * argv[])
{
	int i;
	int opt = 0;
	bool f = false;													// Setto tutte le variabili a false
	bool c = false;													// mi serviranno per capire che funzione chiamare
	bool x = false;
	bool t = false;
while ( (i = getopt(argc, argv, "fcxt")) != -1)
	{
		switch (i) 
		{
			/*
			 * Handling options
			 */
			case 'f':
				opt++;
				f = true;
				break;
			case 'c':
				opt++;
				c = true;
				break;
			case 'x':
				opt++;
				x = true;
				break;
			case 't':
				opt++;
				t = true;
				break;
			case '?':   /* unrecognized options */
				printf("Unrecognized options -%c\n",optopt);
				opt++;
			default:    /* should not reached */
				usage();
		}
	}
	
	globalargv = argv;														//salvo la lista dei parametri
	globalargc = argc;														//e il numero di essi, mi serviranno poi
	
	if(opt>2)
	{
		printf("You have inserted %d parameters. Maximum is 2", opt);		//se opt>2 significa che sono state inserite troppe opzioni
		exit(1);
	}
	else 
	{
		if(f == false)														// se manca f non si può fare nulla (il nome dell'archivio è fondamentale)
		{
			printf("Non posso operare se non ho il nome dell'archivio.");
			exit(1);
		}
		if(f== true)														// se c'è f....			
		{																	//
			if(c == true)													// ...e c...	
			{																//
				printf("creo un archivio con gli attributi dati");			// ...creo l'archivio...
				creabkp(globalargc, globalargv, opt);						//					
			}																// se al posto di c c'è x...
			if(x == true)													//
			{																//
				printf("estraggo archivio");								// ...estraggo..
			}																//
			if(t == true)													// infine, se ho t...
			{																//	
				printf("scrivo la lista dei file nell'archvio");			// ..pubblico la lista dei file
				nome = argv[opt+1];
				stampa();
				
			}
		}
	}

	
	return 0;
		
}



char * scrivi(char * a, char * b)
{
	
	char *targetdir = malloc(2048);											//alloco 2048 byte di memoria
	strcpy(targetdir,a);													//copio a nella stringa creata poco fa
	strcat(targetdir,"/");													// aggiungo il carattere / (slash)
	strcat(targetdir,b);													//copio anche b nella stringa, concatenandolo			
	
	printf("%s \n", targetdir);	
	return targetdir;														// e il nuovo percorso è fatto
}




void show_file (const char *filename, FILE *out) {							//scrive il contenuto di un file in un altro, carattere per carattere (così si mantengono gli whitespace)
    int c;
    FILE *file;
    //fprintf(out, "%s(%s) BEGIN\n", __func__, filename);
    file = fopen(filename, "r");											//apro il file
    if (file) {																//se è aperto
        while ((c = fgetc(file)) != EOF) {									//e il carattere preso non eccede la fine del file
            fputc(c, out);													//scrivo lo stesso carattere in out (file in uscita)
        }																	//
        fclose(file);														//chiudo il file
    } else {
        fprintf(out, "%s: failed to open file '%s' (%d)\n", __func__, filename, errno);
    }
    //fprintf(out, "%s(%s) END\n", __func__, filename);
}


void creabkp(int numpar, char * param[], int ind)
{
	char * nome;										
	nome = param[ind+1];
	
	
	archivio = scrivi(getcwd(NULL, 0), nome);
    arch = fopen(archivio, "w+");
	
	
	//-----------------------------------------------------------------------------------------------------------------------------------
	// dai file passati in input viene ricavato un archivio con la seguente struttura:
	//
	//		%LIST% Lista dei file nell'archivio %LIST%
	//		%DIRS% Lista delle cartelle nell'archivio %DIRS%
	//		%CONTENT% Contenuto file 1   %CONTENT%
	//		%CONTENT% Contenuto file ... %CONTENT%
	//		%CONTENT% Contenuto file  N  %CONTENT%
	//
	//------------------------------------------------------------------------------------------------------------------------------------
	
	
	//-----------------------------------------  LISTA DEI FILE CONTENUTI NELL'ARCHIVIO --------------------------------------------------
	
	int i=ind+3;
	
	fprintf(arch, "%s", LIST);
	fprintf(arch, "%s", " ");
	
	while(i<=numpar)
	{
		
		
		if ((opendir(param[i-1])==NULL))									// Se l'operazione di apertura cartella non va a buon fine
		{																	// Allora è un file
			copy = strdup(param[i-1]);										// copio in una stringa il parametro di argv
			filename = basename(param[i-1]);								// estraggo il nome dal percorso
			puts(filename);													//
			fprintf(arch, "/%s", filename);									// e lo stampo nel file
			
		}
		else 
		{
			lung = strlen(param[i-1]);										// se è una cartella
			puts("this... is.. A FOLDEEEEEEEEEER");							//
			ftw(param[i-1], list, 1);										//scorrila tutta	
		}
		fprintf(arch, "%s", " ");
		i++;
	}
	
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", LIST);
	fprintf(arch, "%s", "\n");
	
	
	//-------------------------------------------------------- FINE LISTA ------------------------------------------------------------------
	// -----------------------------------------------------INIZIO DIRECTORY ----------------------------------------------------------------
	i=ind+3;

	fprintf(arch, "%s", DIRS);
	fprintf(arch, "%s", " ");
	while(i<=numpar)
	{
	
		if ((opendir(param[i-1])==NULL))
		{}else{
			lung = strlen(param[i-1]);
			ftw(param[i-1], listD, 1);
		}
		fprintf(arch, "%s", " ");
		i++;

	
	}
	
	fprintf(arch, "%s", " ");
	fprintf(arch, "%s", DIRS);
	fprintf(arch, "%s", "\n");
	
	//---------------------------------------------------------FINE DIRECTORY-------------------------------------------------------------
	//----------------------------------------------------------INIZIO CONTENT------------------------------------------------------------
	
	
	i=ind+3;
	
	
	while(i<=numpar)
	{
		
		
		if ((opendir(param[i-1])==NULL))
		{
			fprintf(arch, "%s", CONTENT);
			fprintf(arch, "%s", " ");
			copy = strdup(param[i-1]);
			show_file(copy, arch);
			fprintf(arch, "%s", " ");
			fprintf(arch, "%s \n", CONTENT);
			
		}
		else 
		{
			lung = strlen(param[i-1])+1;
			puts("this... is.. A FOLDEEEEEEEEEER");
			ftw(param[i-1], listC, 1);
		}
		
		fprintf(arch, "%s", "\n");
		
		i++;
		
		
	}
	
	//-------------------------------------------------------FINE CONTENT-----------------------------------------------------------------------
	fclose(arch);
}

void read_words (FILE *f) {
    char x[1024];
	bool listTrovata;
    while (fscanf(f, " %s", x) == 1) {
		if(strcmp(x, "%LIST%")==0 && listTrovata==false)
		{
			listTrovata= true;
			puts("trovato il primo \n");
			continue;
		}
		else if (strcmp(x, "%LIST%")==0 && listTrovata)
		{
			long position;
			position = ftell(f);
			printf("trovato il secondo \n");
			printf("LIST trovato alla posizione %ld", position);
			fseek(f, 0, SEEK_END);
			continue;
		}
		else
		{
        puts(x);
		}
    }
	printf("esco \n");
	
}






//-------------------------------------------------------INIZIO LIST PER %LIST%-----------------------------------------------------------------------

int list(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0)
	{
		printf("0%3o\t%s\n", status->st_mode&0777, name);
	}
	else if(type == FTW_F)
	{
		printf("0%3o\t%s\n", status->st_mode&0777, name);
		fprintf(arch, "%s ", name + lung);
		printf( "%s ", name + lung);
	}	
	
	return 0;
}

//-------------------------------------------------------FINE LIST-----------------------------------------------------------------------
//-------------------------------------------------------INIZIO LISTC PER %CONTENT%-----------------------------------------------------------------------

int listC(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0)
		printf("0%3o\t%s/\n", status->st_mode&0777, name);
	
	if(type == FTW_F)
	{
		fprintf(arch, "%s", CONTENT);
		fprintf(arch, "%s", " ");
		printf("0%3o\t%s\n", status->st_mode&0777, name);
		show_file(name, arch);
		fprintf(arch, "%s", " ");
		fprintf(arch, "%s", CONTENT);
		fprintf(arch, "\n");
		
	}
	
	
	
	return 0;
}

//-------------------------------------------------------FINE LISTC-----------------------------------------------------------------------
//-------------------------------------------------------INIZIO LISTD PER %DIRS%-----------------------------------------------------------------------

int listD(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0)
	{
		puts("entrato");
		printf("0%3o\t%s\n", status->st_mode&0777, name);
		fprintf(arch, "%s ", name + lung);
		puts("fatto");
		printf( "%s ", name + lung);
	}
	puts("uscito");
	
	return 0;
}

//-------------------------------------------------------FINE LISTD-----------------------------------------------------------------------

void stampa()
{
	FILE * ciao;
	char* daListare;
	daListare = scrivi(getcwd(NULL, 0), nome);
    ciao = fopen(daListare, "r");
	read_words(ciao);
}



void usage()
{
	printf("possibili comandi:\n");
	printf("-f			crea/ estrae archivio\n");
	printf("-x			creo archivio\n");
	printf("-c			estrae archivio\n");
	printf("-t			elenco archivio\n");
}
