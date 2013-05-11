/*
 ============================================================================
 Name        : equal.c
 Author      : Giuliano Tortoreto and Andrea Gottardi
 Version     : 0.1
 Copyright   : Your copyright notice
 Description : 
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ftw.h>
#include <dirent.h>

#include <sort.h>


struct dirent *readdir(DIR *dirp);

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

int conta(const char *name, const struct stat *status, int type);
int salvapercSND(const char *name, const struct stat *status, int type);
int salvapercFST(const char *name, const struct stat *status, int type);

bool isDIRECTORY(char * elemento);
bool compareFile(char * primo, char* secondo);
bool confronta(char** primo, char ** secondo);

int lung = 0;
int globalsum;
char** percorsiFST;
char** percorsiSND;
int FSTlen = 0;
int SNDlen = 0;

int main( int argc, char *argv[]) {

	bool firstIsFile = true;
	bool secondIsFile = true;
	bool result;	

	char* percfirst = argv[1];
	
	char* percsecond = argv[2];
	

	 if ( argc != 3 ) /* argc should be 3 for correct execution */
	    {
	        /* We print argv[0] assuming it is the program name */
	        printf( "usage: %s filename", argv[0] );
	    }else
	    {
	    	if (isDIRECTORY(argv[1]))
		{
			firstIsFile = false;
			lung = strlen(percfirst)-1;
			globalsum = 0;

			//conto quanti elementi a percfirst
	 		ftw(percfirst, conta, 1);	
			printf("\n il numero di elementi è %d \n",globalsum);
			FSTlen = globalsum;

			// inizializzo l'array alla lunghezza percfirst
			percorsiFST = malloc (sizeof(char*)*FSTlen);
			globalsum = 0;
			//riempio l'array
			ftw(percfirst,salvapercFST,1);
		}

		
		if (isDIRECTORY(argv[2]))
		{
			secondIsFile = false;
			lung = strlen(percsecond)-1;
			globalsum = 0;

			//conto quanti elementi a percsecond
			ftw(percsecond, conta, 1);	
			printf("\n il numero di elementi è %d \n",globalsum);
			SNDlen = globalsum;

			// inizializzo l'array alla lunghezza percSND
			percorsiSND = malloc (sizeof(char*)*SNDlen);
			globalsum = 0;
			//riempio l'array
			ftw(percsecond,salvapercSND,1);
		}

		if (secondIsFile && firstIsFile)
			{
				result = compareFile(percfirst,percsecond);//finisce il programma
				if (result)
					printf("true \n");
				else
					printf("false: %s and %s are different \n",percfirst,percsecond);

			}else if (secondIsFile)
			{
				char** percorso;
				*percorso = percsecond;
				//result = confronta(percorso,percorsiFST);

			}else if (firstIsFile)
			{
				char** percorso;
				*percorso = percfirst;
				//result = confronta(percorso,percorsiSND);
			
			}else //sono tutte e due cartelle
			{
				//result = confronta(percorsiFST,percorsiSND);
			}

	}

	
	return 0;
}

/* 
	Questa funzione conta quanti elementi ci saranno nell'array di percorsi
*/
int conta(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0)
	{
		printf( "%s \n", name + lung);
		globalsum++;
	}
	else if(type == FTW_F)
	{
		
		globalsum++;
		printf( "%s \n", name + lung);
	}	
	
	return 0;
}



/*
Questa funzione salva tutti i percorsi nell'array percorsifst
*/
int salvapercFST(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0 && globalsum != FSTlen)
	{
		
		char * stringa = (char*)(name+lung);
		percorsiFST[globalsum] = stringa ;
		printf( "%s \n", percorsiFST[globalsum] );
		
		globalsum++;
	}
	else if(type == FTW_F && globalsum != FSTlen)
	{
		
		char * stringa = (char*)(name+lung);
		percorsiFST[globalsum] = stringa;
		printf( "%s \n", percorsiFST[globalsum] );
		globalsum++;
	}	
	
	return 0;
}

bool isDIRECTORY(char* elemento)
{
	struct stat statbuf;
	stat(elemento, &statbuf);
	return S_ISDIR(statbuf.st_mode);

}

/*
Questa funzione salva tutti i percorsi nell'array percorsiSND
*/
int salvapercSND(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0 && globalsum != SNDlen)
	{
		
		char * stringa = (char*)(name+lung);
		percorsiSND[globalsum] = stringa ;
		printf( "%s \n", percorsiSND[globalsum] );
		
		globalsum++;
	}
	else if(type == FTW_F && globalsum != FSTlen)
	{
		
		char * stringa = (char*)(name+lung);
		percorsiSND[globalsum] = stringa;
		printf( "%s \n", percorsiSND[globalsum] );
		globalsum++;
	}	
	
	return 0;
}

bool compareFile(char * primo, char* secondo)
{
	FILE *fileFST;
	FILE *fileSND;
	fileFST = fopen(primo, "rb");
	fileSND = fopen(secondo, "rb");
	bool uguali = true;

	if ( fileFST == NULL )
	       {
	       printf("Cannot open %s for reading ", primo );
	       return false;
	       }
	else if (fileSND == NULL)
	       {
	       	printf("Cannot open %s for reading ", secondo );
		return false;
		}
   	else
       	{
       		char ch1  =  getc( fileFST ) ;
       		char ch2  =  getc( fileSND ) ;

       while( (ch1!=EOF) && (ch2!=EOF) && (uguali==true))
        {
            ch1 = getc(fileFST);
            ch2 = getc(fileSND) ;
	    if(ch1!=ch2)
		uguali = false;
        }

        if (uguali)
            return true;
        else if (ch1 !=  ch2)
            return false;

        fclose ( fileFST );
        fclose ( fileSND );
       }
	
}


bool confronta(char** primo, char ** secondo)
{


	return true;
}
