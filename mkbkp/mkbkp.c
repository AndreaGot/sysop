/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : mkbkp.c
 Description   : Archivia i file specificati, li estrae ed elenca il contenuto dell'archivio
 ============================================================================
 */

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
#include <sys/types.h>
#include <fcntl.h>
#include "mkbkp.h"
#include "../managelogs/managelogs.h"

#define PERMS 0777
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
int inizio;
int fine;
int daleggere;



int main(int argc, char * argv[])
{
	crealog(argv[0]);
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
	
	if(opt == 2 && argc == 3 && (x==true||t==true))
	{
		printf("Mancano parametri. Inserisci il nome dell'archivio\n");
		scrivilog("Mancano parametri. Inserisci il nome dell'archivio\n"); 
		exit(1);
	}
	else if(opt == 2 && argc == 3 && c==true)
	{
		printf("Mancano parametri. Inserisci il nome dell'archivio e almeno un percorso valido.\n");
		scrivilog("Mancano parametri. Inserisci il nome dell'archivio e almeno un percorso valido.\n");
		exit(1);
	}
	

	
	globalargv = argv;														//salvo la lista dei parametri
	globalargc = argc;														//e il numero di essi, mi serviranno poi
	
	if(opt>2)
	{
		printf("troppi parametri inseriti. Ne sono stati inseriti %d, il massimo è 2", opt);	//se opt>2 significa che sono state inserite troppe opzioni
		scrivilog("troppi parametri inseriti. Ne sono stati inseriti %d, il massimo è 2", opt);
		exit(1);
	}
	else 
	{
		if(f == false)														// se manca f non si può fare nulla (il nome dell'archivio è fondamentale)
		{
			printf("Non posso operare se non ho il nome dell'archivio.");
			scrivilog("Non posso operare se non ho il nome dell'archivio.");
			exit(1);
		}
		if(f== true)														// se c'è f....			
		{																	//
			if(c == true)													// ...e c...	
			{																//
				printf("creo un archivio con gli attributi dati \n \n");	// ...creo l'archivio...
				scrivilog("creo un archivio con gli attributi dati \n \n");
				creabkp(globalargc, globalargv, opt);						//					
			}																// se al posto di c c'è x...
			if(x == true)													//
			{																//
				
				printf("estraggo archivio \n");								// ...estraggo..
				scrivilog("estraggo archivio \n");	
				nome = argv[opt+1];
				estrai();
			}																//
			if(t == true)													// infine, se ho t...
			{																//	
				printf("scrivo la lista dei file nell'archivio \n\n");			// ..pubblico la lista dei file
				nome = argv[opt+1];
				stampa();
				
			}
		}
	}

	chiudilog();
	return 0;
		
}


//collega due stringhe inserendo un / tra di esse (in questo programma, "xxx", "/", "yyy")
char * collegaSlash(char * a, char * b)									
{
	
	char *targetdir = malloc(2048);											//alloco 2048 byte di memoria
	strcpy(targetdir,a);													//copio a nella stringa creata poco fa
	strcat(targetdir,"/");													// aggiungo il carattere / (slash)
	strcat(targetdir,b);													//copio anche b nella stringa, concatenandolo			
	
	//printf("%s \n", targetdir);	
	return targetdir;														// e il nuovo percorso è fatto
}

// collega due stringhe (in questo programma, "xxx" e "/yyy")
char * collega(char * a, char * b)								
{
	
	char *targetdir = malloc(2048);											//alloco 2048 byte di memoria
	strcpy(targetdir,a);													//copio a nella stringa creata poco fa
	strcat(targetdir,b);													//copio anche b nella stringa, concatenandolo			
	return targetdir;														// e il nuovo percorso è fatto
}



//scrive il contenuto di un file in un altro, carattere per carattere (così si mantengono gli whitespace)
void writeByChar (const char *filename, FILE *out) {							
    int c;
    FILE *file;
    //fprintf(out, "%s(%s) BEGIN\n", __func__, filename);
    file = fopen(filename, "rb");											//apro il file
    if (file) {																//se è aperto
        while ((c = fgetc(file)) != EOF) {									//e il carattere preso non eccede la fine del file
            fputc(c, out);													//scrivo lo stesso carattere in out (file in uscita)
        }																	//
        fclose(file);														//chiudo il file
    } else {
        fprintf(out, "%s: failed to open file '%s' (%d)\n", __func__, filename, errno);
		scrivilog("%s: failed to open file '%s' (%d)\n", __func__, filename, errno);
    }
    
}				//copia un file lettera per lettera


void creabkp(int numpar, char * param[], int ind)					// crea il backup
{
	char * nome;										
	nome = param[ind+1];
	
	
	archivio = collegaSlash(getcwd(NULL, 0), nome);
    	arch = fopen(archivio, "w+b");
	
	
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
	int index = 0;
	char * position;
	while(i<=numpar)
	{
		
		
		if ((opendir(param[i-1])==NULL))									// Se l'operazione di apertura cartella non va a buon fine
		{																	// Allora è un file
			copy = strdup(param[i-1]);										// copio in una stringa il parametro di argv
			filename = basename(param[i-1]);								// estraggo il nome dal percorso

			
			position = strchr(filename, ' ');
			while(position!=NULL)
			{
				index = position-filename;
				filename[index] = '-';
				position = strchr(filename, ' ');
			}

			printf( "archivio file \t/%s", filename);	
			scrivilog("archivio file \t%s \n",filename);	
			fprintf(arch, "/%s", filename);									// e lo stampo nel file
			
		}
		else 
		{
			lung = strlen(param[i-1]);										// se è una cartella
			//puts("thisis A FOLDER");							//
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
			writeByChar(copy, arch);
			fprintf(arch, "%s", " ");
			fprintf(arch, "%s \n", CONTENT);
			
		}
		else 
		{
			lung = strlen(param[i-1])+1;
			//puts("this... is.. A FOLDEEEEEEEEEER");
			ftw(param[i-1], listC, 1);
			puts("eccomi");
		}
		
		fprintf(arch, "%s", "\n");
		
		i++;
		
		
	}
	
	//-------------------------------------------------------FINE CONTENT-----------------------------------------------------------------------

	fclose(arch);
	puts("creazione terminata!\n");
	scrivilog("creazione terminata!\n");
}


//legge un file parola per parola 
void read_words (FILE *f) {
    char x[8192];
	bool listTrovata = false;
    while (fscanf(f, " %s", x) == 1) {
		if(strcmp(x, "%LIST%")==0 && listTrovata==false)
		{
			listTrovata= true;
			//puts("trovato il primo \n");
			continue;
		}
		else if (strcmp(x, "%LIST%")==0 && listTrovata)
		{
			//long position;
			//position = ftell(f);
			//printf("trovato il secondo \n");
			//printf("LIST trovato alla posizione %ld", position);
			fseek(f, 0, SEEK_END);
			continue;
		}
		else if (listTrovata)
		{
        		puts(x);
			scrivilog("%s\n", x);
		}
    }
	//printf("esco \n");
	
}										// lista i file contenuti nella sezione %LIST%



// dato l'archivio, legge l'elenco delle cartelle
void read_dirs (FILE *f) {
    char x[8192];
	bool dirsTrovata = false;
	//long position;
	mode_t mode;
	mode = (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	
    while (fscanf(f, " %s", x) == 1) {
		if(strcmp(x, DIRS)==0 && dirsTrovata==false)
		{
			dirsTrovata= true;
			//position = ftell(f);
			//puts("trovato il primo \n");
			//printf("DIRS trovato alla posizione %ld", position);
			continue;
		}
		else if (strcmp(x, DIRS)==0 && dirsTrovata)
		{
			
			//position = ftell(f);
			//printf("trovato il secondo \n");
			//printf("DIRS trovato alla posizione %ld", position);
			fseek(f, 0, SEEK_END);
			continue;
		}
		else if(dirsTrovata)
		{
			char* folder;											// stringa contenente il percorso da aprire (verrà creato in seguito)
			folder = collegaSlash(getcwd(NULL, 0), x);
			mkdir(folder, mode);

		}
    }

	
}										


//crea i file ocntenuti nella sezione %FILES%
	void crea_file(FILE *f)												// crea i file estratti dall'archivio
	{
		int contatore = 0;
		char x[8192];
		bool listTrovata = false;
		while (fscanf(f, " %s", x) == 1) {
			if(strcmp(x, "%LIST%")==0 && listTrovata==false)
			{
				listTrovata= true;
				//puts("trovato il primo \n");
				continue;
			}
			else if (strcmp(x, "%LIST%")==0 && listTrovata)
			{
			
				fseek(f, 0, SEEK_END);
				break;
			}
			else if (listTrovata)
			{
				contatore++;
				char* file;							// stringa contenente il percorso da aprire (verrà creato in seguito)
				file = collegaSlash(getcwd(NULL, 0), x);
				creat(file, PERMS);
			
				trovaInizioFine(contatore); //cerco il carattere di inizio e fine del content
			
				scriviFile(file);
				
			}
		}
		
	}

/* trova il carattere di inizio e di fine del file nella sezione %CONTENT% */

	void trovaInizioFine(int cont)											{
		FILE* contenuto;
		char * path;
		char x[8192];
		int i =1;
		path = collegaSlash(getcwd(NULL, 0), nome);
		
		contenuto = fopen(path, "rb");
		while (fscanf(contenuto, "%s", x) == 1) 
		{
			if(strcmp(x, "%CONTENT%")==0 && i == ((cont*2)-1) )
			{
				inizio = ftell(contenuto);
				i++;
			}
			else if(strcmp(x, "%CONTENT%")==0 && i == ((cont*2)) )
			{
				fine = ftell(contenuto);
				daleggere = fine-9;
				break;
				i++;
			}
			else if(strcmp(x, "%CONTENT%")==0)
			{
				i++;	
			}


	
		}
		
	
		fclose(contenuto);
		
	}

	void scriviFile(const char * arrivo)								//scrive i file creati in precedenza
	{
		FILE * partenza;
		FILE * target;
		int c;
		int pos;
		char * path;
		path = collegaSlash(getcwd(NULL, 0), nome);
		partenza = fopen(path, "rb");
		fseek(partenza, inizio+1, SEEK_SET);
		target = fopen(arrivo, "wb");											//apro il file
		if (target) {																//se è aperto
			while ((c = fgetc(partenza)) != EOF && ftell(partenza)<=fine-10) {									//e il carattere preso non eccede la fine del file
				fputc(c, target);
				//fputc(c, stdout);
				pos = ftell(partenza);
				if(pos==fine)
				{
					break;
				}
																				//scrivo lo stesso carattere in out (file in uscita)
			}																	//
	  
			fclose(target);														//chiudo il file
			fclose(partenza);
		} 
		else 
		{
			printf("errore di scrittura del file \n");
			scrivilog("errore di scrittura del file \n");
		}
		
	}


//-------------------------------------------------------INIZIO LIST PER %LIST%-----------------------------------------------------------------------
	//per FTW - lista i nomi dei file
int list(const char *name, const struct stat *status, int type) {
	int index = 0;
	char * copy;
	char * position;
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0 && strcmp(name + lung,"/.DS_Store")!=0)
	{
	 //se è una cartella non fare nulla
	}
	else if(type == FTW_F  && strcmp(name+ lung,"/.DS_Store")!=0) //se è un file archivialo
	{
	
		
		copy = strdup(name);
		
		position = strchr(copy, ' ');
		while(position!=NULL)
		{
			 index = position-copy;
			 copy[index] = '-';
			 position = strchr(copy, ' ');
		 }
	
		fprintf(arch, "%s ", copy + lung);
		printf( "archivio file \t%s \n", copy + lung);
		scrivilog( "archivio file \t%s \n", copy + lung);
	}	
	
	return 0;
}

//-------------------------------------------------------FINE LIST-----------------------------------------------------------------------
//-------------------------------------------------------INIZIO LISTC PER %CONTENT%-----------------------------------------------------------------------

int listC(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp(name + lung,".DS_Store")!=0)
	{
	}
	
	
	if(type == FTW_F && strcmp(name + lung,".DS_Store")!=0) //se è un file scrivilo carattere per carattere
	{
		fprintf(arch, "%s", CONTENT);
		fprintf(arch, "%s", " ");
		writeByChar(name, arch);
		fprintf(arch, "%s", " ");
		fprintf(arch, "%s", CONTENT);
		fprintf(arch, "\n");
		
	}
	
	return 0;
}	// per FTW - lista i contenuti dei file

//-------------------------------------------------------FINE LISTC-----------------------------------------------------------------------
//-------------------------------------------------------INIZIO LISTD PER %DIRS%-----------------------------------------------------------------------

int listD(const char *name, const struct stat *status, int type) {
	int index = 0;
	char * copy;
	char * position;
	
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0 && strcmp(name + lung, ".DS_Store")!=0)
	{
		copy = strdup(name);
		
		position = strchr(copy, ' ');
		while(position!=NULL)
		{
			index = position-copy;
			copy[index] = '-';
			position = strchr(copy, ' ');
		}
	
		
		fprintf(arch, "%s ", copy + lung);
		//puts("fatto");
		//printf( "%s ", name + lung);
	}
	//puts("uscito");
	
	return 0;
}	// per FTW - lista i nomi delle directory

//-------------------------------------------------------FINE LISTD-----------------------------------------------------------------------

void stampa()															// stampa la sezione %LIST% dell'archivio
{
	FILE * archivio;											
	char* daListare;											// stringa contenente il percorso da aprire (verrà creato in seguito)
	daListare = collegaSlash(getcwd(NULL, 0), nome);			// concatena il path attuale (quello dove viene eseguito il programma) con il nome del file passato
    if(access(daListare, F_OK)==-1)
	{
		puts("Il file che si vuole listare non esiste");
		scrivilog("Il file che si vuole listare non esiste");
		exit(1);
	}
	
	archivio = fopen(daListare, "rb");								// apre il file di archivio in modalità solo lettura
	read_words(archivio);										// legge il file parola per parola
	fclose(archivio);
	puts("\nlettura terminata!");
	scrivilog("\nlettura terminata!");
}

void estrai()															// estrae i dati dall'archivio
{
	FILE * archivio;											
	char* daEstrarre;											// stringa contenente il percorso da aprire (verrà creato in seguito)
	daEstrarre = collegaSlash(getcwd(NULL, 0), nome);			// concatena il path attuale (quello dove viene eseguito il programma) con il nome del file passato
    	if(access(daEstrarre, F_OK)==-1)
	{
		puts("Il file da estrarre non esiste");
		scrivilog("Il file da estrarre non esiste");
		exit(1);
	}
	archivio = fopen(daEstrarre, "rb");								// apre il file di archivio in modalità solo lettura
	read_dirs(archivio);											// legge il file parola per parola
	rewind(archivio);
	crea_file(archivio);
	
	fclose(archivio);
	puts("estrazione terminata!");
	scrivilog("estrazione terminata!");
}

void usage()															// come utilizzare il programma
{
	printf("possibili comandi:\n");
	printf("-f			crea/ estrae archivio\n");
	printf("-c			creo archivio\n");
	printf("-x			estrae archivio\n");
	printf("-t			elenco archivio\n");
	printf("è obbligatorio usare f per ogni operazione\n");
}
