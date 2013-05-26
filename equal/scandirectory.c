#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

bool isDIR(char * folder);
int lunghezza;
int lung;

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
	    
	    printf("%*s%s \n",depth,"",(*percorsi)[(*i)-1]);
            /* Recurse at a new indent level */

	    if (isDIR(new_folder1)){
		
            savedir(new_folder1,depth+1,percorsi,i,n,radice);
	   
       		}
		
	  
	
    }
   if(isDIR(new_folder1)){
    closedir(dir1);
	}
}

int main( int argc, char *argv[])
{

  
    char ** percorsi; 
    char ** percorsi2;
  
   
    char * first = malloc(sizeof(char*) * (sizeof(argv[1])+3));
    first = argv[1];
   
    char * second = malloc(sizeof(char*) * (sizeof(argv[2])+3));
    second = argv[2];

   
   
   // savedir(inizio,0,&percorsi,temp,num,inizio);
    
    int t = 0;
    percorsi = scorripercorso(first,&t);

    printf("\n il primo è finito \n \n");

    int t2 = 0;
    percorsi2 = scorripercorso(second,&t2);
	
    printf("done.\n");
    printf(" Ci sono %d elementi \n",t);
	
    

    qsort((char**)percorsi,t, sizeof(char**),comp);
    qsort((char**)percorsi2,t2,sizeof(char**),comp);
    
    //char* prova ;
   // int i = 0;
   /* while(i<t2)
	{
	prova  = malloc(sizeof(char*) * 150);
	strcpy(prova,first);
	prova = strcat(prova,percorsi[i]);
	printf("percoso[%d]=%s \n",i,prova);
	free(prova);
	i++;
    	}*/
    

     // questa parte toglie questo simbolo '/' dalla fine del file se c'è
     // prima verifico se ha '/' e poi tolgo

   // printf(" %s \n",second);


     char* sndPos = strrchr(second,47);
     int pos1 = second - sndPos;
      
	
     if ( (strlen(second)+pos1)==1)
	{
  	  printf("%s \n", second);
	  second[pos1*(-1)] = '\0';
	}
    

    char* fstPos = strrchr(first,47);
    int pos = first - fstPos;

  
    if ((strlen(first)+pos)==1)
	{
	  printf("%s \n", first);
	  first[pos*(-1)] = '\0';  
	}

     printf(" il primo è %p e %s \n",&first,first);
     printf("il secondo è %p e %s \n",&second,second);
   
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

    printf("Directory scan %s:\n",inizio);
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

	
	while(i<fLun && j<sLun)
	{
	    if(strcmp(first[i],second[i])==0){
		 

	    }
	j++;
	i++;
	}
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
	
	return true;
	
}


