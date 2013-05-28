/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : equal.c
 Description   : Ritorna true se gli oggetti passati come parametro sono uguali, false altrimenti 
 ============================================================================
 */


#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <"../managelogs/managelogs.h">

bool isDIR(char * folder);
int lunghezza;
int lung;s

bool compareFile(char * primo, char* secondo);
void compareArray(char** first,char** second, int fLun,int sLun,char* fHead, char* sHead);
void savedir(char *folder1, int depth,char*** percorsi,int* i,int* n,char* radice);
char** scorripercorso(char* folder,int* n);
int comp(const void *x, const void *y);

void savedir(char *folder1, int depth,char*** percorsi,int* i,int* n,char* radice)
{

    DIR *dir1;

    struct dirent *ent;
    char * new_folder1;
    dir1 = opendir(folder1);

   
	
  
  // la prima parte del file è stata messa in quanto c'è un bug nella readdir
  // se si passa un NULL alla readdir va in segmentation fault
  // quindi sfrutto il fatto che C è pigro e in questo modo in caso
  // dir1 sia NULL non faccio la readdir

   while ( (dir1 != NULL) && (NULL != (ent = readdir(dir1)) ) ){
        
	if ( strcmp(ent->d_name, ".")  == 0){
			continue;
		}
	if ( strcmp(ent->d_name, "..")  == 0){
			continue;
		}

   	


            /* Found a directory, but ignore . and .. */
            if(strcmp(".",ent->d_name) == 0 || strcmp("..",ent->d_name) == 0)
                continue;

	    
	    lunghezza =  strlen(radice);
	    lung = strlen(folder1) + strlen(ent->d_name) + 2; 

	    new_folder1 = malloc(sizeof(char*)*(lung));
	    strcpy(new_folder1, folder1);    
	    strcat(new_folder1, "/");
	    strcat(new_folder1, ent->d_name);        
		

	   
	    (*percorsi)[*i] = malloc(sizeof(char*) * ( (lung) -lunghezza));
	    strcpy((*percorsi)[*i],(new_folder1+lunghezza)) ;

	     (*i)++;


	    if(*i>=( (*n) -1)){
		(*n) = (*n)*2;
		*percorsi =  realloc((*percorsi),sizeof(char**) * (*n));
		}
	    
	    //printf("%*s%s \n",depth,"",(*percorsi)[(*i)-1]);
            /* Recurse at a new indent level */

	    if (isDIR(new_folder1)){
		
            savedir(new_folder1,depth+1,percorsi,i,n,radice);
	   
       		}
		
	  
	
    }
 
    
    closedir(dir1);

}

int main( int argc, char *argv[])
{

  
    char ** percorsi; 
    char ** percorsi2;
  
   if ( argc != 3 ) /* argc should be 3 for correct execution */
	    {
	        /* We print argv[0] assuming it is the program name */
	        printf( "Manca almeno un argomento \n", argv[0] );
			exit(0);
	    }

   if( !(isDIR(argv[1])) && !(isDIR(argv[2])) ){

		//------------- sono 2 file ----------------
	
		if (compareFile(argv[1],argv[2]) )
		{
			printf("true \n");
			exit(0);
		}else{
			printf("false \n");
			exit(0);
		}

	}else{
		
		// ------- il primo è una cartella e il secondo è un file
		 if( (isDIR(argv[1])) && !(isDIR(argv[2])) )
		{
			printf("false \n");
			exit(0);
			// ----------------- il primo è un file e il secondo è una cartella
		}else if( !(isDIR(argv[1])) && (isDIR(argv[2])) )
		{
			printf("false \n");
			exit(0);
		}
	}

    char * first = malloc(sizeof(char*) * (sizeof(argv[1])+3));
    first = argv[1];
   
    char * second = malloc(sizeof(char*) * (sizeof(argv[2])+3));
    second = argv[2];

   
   
   // savedir(inizio,0,&percorsi,temp,num,inizio);
    
    int t = 0;//scorro il percorso e lo salvo nell'array percorsi
    percorsi = scorripercorso(first,&t);

   // printf("\n il primo è finito \n \n");

    int t2 = 0;//scorro il percorso e lo salvo nell'array percorsi2
    percorsi2 = scorripercorso(second,&t2);
	
   

    qsort((char**)percorsi,t, sizeof(char**),comp);
    qsort((char**)percorsi2,t2,sizeof(char**),comp);

     /* qui controllo se c'è il simbolo '/' alla fine del secondo elemento, se c'è
        lo tolgo*/

     char* sndPos = strrchr(second,47);
     int pos1 = second - sndPos;
      
     if ( (strlen(second)+pos1)==1)
	{
  	  
	  second[pos1*(-1)] = '\0';
	}
    /*
		Qui faccio la stessa cosa di sopra...
	*/    
	char* fstPos = strrchr(first,47);
    int pos = first - fstPos;

  
    if ((strlen(first)+pos)==1)
	{
	
	  first[pos*(-1)] = '\0';  
	}

    //qui confronto i 2 array
    compareArray(percorsi,percorsi2,t,t2,first,second);

	
       

    exit(0);
}

char** scorripercorso(char* folder,int* n)
{
    int num [1];
    (*num) = 10;
    char ** percorsi = malloc(sizeof(char**) * (*num));
    int temp [1];
    char * inizio = malloc(sizeof(char*) * sizeof(folder));
    strcpy(inizio,folder);
    *temp = 0;

    //printf("Directory scan %s:\n",inizio);
    savedir(inizio,0,&percorsi,temp,num,inizio);

    (*n) = (*temp);
    return percorsi;

}


bool isDIR(char * folder){
	
	struct stat statbuf;
	stat(folder, &statbuf);
	return S_ISDIR(statbuf.st_mode);

}

int comp(const void *x, const void *y)
{
    return(strcmp(*((char**)x),*((char**)y)));      
}

void compareArray(char** first, char** second,int fLun,int sLun,char* fHead, char* sHead)
{
	int i = 0;
	int j = 0;
	bool sonouguali = true;
	bool primavolta = true;
	int res = 0;

	char* ultimodiverso = malloc ( sizeof(char*) * 200);
	strcpy(ultimodiverso," ");
	
	while(i<fLun && j<sLun)
	{
            res = strcmp(first[i],second[j]);
	    if(res == 0){
		  char * primo = malloc( sizeof(char**) * (strlen(first[i]) + strlen(fHead) +2) ) ;
		  char * secondo = malloc( sizeof(char**) * (strlen(second[j]) + strlen(sHead) +2) ) ;
		  
		  strcpy(primo, fHead);
          	strcat(primo, first[i]);
		  
		  strcpy(secondo, sHead);
		  strcat(secondo, second[j]);

		//  printf(" %s %s \n",primo,secondo); 

		  if ( isDIR(primo) && isDIR(secondo) ){
		
		  }else{
		    
                 

			if ( compareFile(primo,secondo) ){
		    		//printf("%s = %s \n",first[i],second[j]);
			}else
			{
				if(primavolta){
				primavolta = false;
				printf("false \n");
				}
				sonouguali = false;
				printf("I file %s sono diversi \n",first[i]);
			}
		   }

		   i++;
		   j++;			
	    }else if (res < 0)
		{
			if(primavolta)
			{
			primavolta = false;
			printf("false \n");
			}
		

		sonouguali = false;
		
		i++;
		// questo controllo serve ad evitare di scrivere tutti i sottopercorsi
		// in caso ci siano cartelle 

		if (strcmp(ultimodiverso," ")== 0){
			printf("<< %s \n",first[i-1]);
			strcpy(ultimodiverso,first[i-1]);
		    	
			}
		else
 		   {
			if ( strncmp(first[i-1],ultimodiverso, strlen(ultimodiverso)) != 0)
				{
				strcpy(ultimodiverso,first[i-1]);
			    	printf("<< %s \n",first[i-1]);

				}
			}

		
		}
		else
		{
		if(primavolta)
			{
			primavolta = false;
			printf("false \n");
			}
		
		sonouguali = false;

	
		j++;
		// questo controllo serve ad evitare di scrivere tutti i sottopercorsi
		// in caso ci siano cartelle 
			if (strcmp(ultimodiverso," ")== 0){
				printf(">> %s \n",second[j-1]);
				strcpy(ultimodiverso,second[j-1]);
		    
				}
			else
 		   	{
				if ( strncmp(second[j-1],ultimodiverso, strlen(ultimodiverso)) != 0)
					{
					strcpy(ultimodiverso,second[j-1]);
			   		printf(">> %s \n",second[j-1]);

					}	
			}

		}

	}
	
	// in questo punto controllo se ci sono elementi che non ho guardato perchè il ciclo si è concluso prima
	if ( i<fLun){
		sonouguali = false;

		while(i<fLun){	
		i++;
		   if (strcmp(ultimodiverso," ")== 0){
				printf("<< %s \n",first[i-1]);
				strcpy(ultimodiverso,first[i-1]);
		    
				}
			else
 		   	{
				if ( strncmp(first[i-1],ultimodiverso, strlen(ultimodiverso)) == 0)
					continue;
				else{
					strcpy(ultimodiverso,first[i-1]);
			   		printf("<< %s \n",first[i-1]);

					}	
			}


		}
	}

	

	// in questo punto controllo se ci sono elementi che non ho guardato perchè il ciclo si è concluso prima
	if (j <sLun){
		sonouguali = false;
		while(j<sLun){	
		j++;
	
		if (strcmp(ultimodiverso," ")== 0){
				printf(">> %s \n",second[j-1]);
				strcpy(ultimodiverso,second[j-1]);
				}
		else
 		   	{
				if ( strncmp(second[j-1],ultimodiverso, strlen(ultimodiverso)) == 0)
					continue;
				else{
					strcpy(ultimodiverso,second[j-1]);
			   		printf(">> %s \n",second[j-1]);

					}	
			}
		}
	}


	if(sonouguali)
		printf("true \n");
}

bool compareFile(char * primo, char* secondo)
{
	FILE *fileFST;
	FILE *fileSND;
	if (strcmp(primo,secondo) == 0)
		return true;


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
		
		if (ch1!=ch2)
			return false;

       int i = 0;
       while( (ch1!=EOF) && (ch2!=EOF) && (uguali==true))
        {
            ch1 = getc(fileFST);
            ch2 = getc(fileSND) ;
	    if(ch1!=ch2)
	     {
		uguali = false;
		i++;
             }
        }

        if (uguali)
            return true;
        else if (ch1 !=  ch2)
            return false;

        fclose ( fileFST );
        fclose ( fileSND );
       }
	
	return true;
	
}


