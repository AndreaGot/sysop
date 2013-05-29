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
#include "../managelogs/managelogs.h"
#include "equal.h"


int lunghezza;
int lung;

/*
   Questa funzione scorre la cartella e tutte le sotto cartelle di una certa cartella @folder1.
   Man mano che attraversa il percorso lo salva in un array @percorsi.
   @radice serve per togliere sempre la parte iniziale che altrimenti sarebbe uguale per ogni elemento
   @n invece serve per ingrandire l'array quando è pieno attraverso l'uso della realloc
   @i serve prima di tutto per progredire nell'array, inoltre alla fine conterrà la dimensione dell'array

*/
void savedir(char *folder1, char*** percorsi,int* i,int* n,char* radice)
{

    DIR *dir1;

    struct dirent *ent;
    char * new_folder1;
    dir1 = opendir(folder1);
    new_folder1 = malloc (sizeof(char**) *5);
   
	
  

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

	    free(new_folder1);
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
		
            savedir(new_folder1,percorsi,i,n,radice);
	   
       		}
		
	  
	
    }
 
    
    closedir(dir1);

}

//qui inizia il programma
int main( int argc, char *argv[])
{
    int differenzaFraFile = 0;
  
    char ** percorsi; 
    char ** percorsi2;

    crealog(argv[0]);
  
   if ( argc != 3 ) /* argc should be 3 for correct execution */
	    {
	        scrivilog("Mancano i nomi dei percorsi da verificare \n");
	        printf( "Manca almeno un argomento \n");
		exit(0);
	    }

   scrivilog("controllo se i percorsi esistono");
   //controllo se i percorsi esistono
   int esiste1 = access(argv[1],F_OK);
   int esiste2 = access(argv[2],F_OK);

   if (( esiste1 ==-1 ) && ( esiste2==-1))
	{
	printf("Entrambi i percorsi non esistono, verifica ed inserisci quelli corretti \n");
	scrivilog("Entrambi i percorsi sono sbagliati \n");
	exit(0);

	}else if(esiste1==-1)
	{

	printf("Il primo percorso inserito non esiste, inseriscine un'altro\n");
	scrivilog("Il primo percorso è sbagliato \n");
	exit(0);

	}else if(esiste2==-1){

	printf("Il secondo percorso inserito non esiste, inseriscine un'altro\n");
	scrivilog("Il secondo percorso è sbagliato \n");
	exit(0);

	}

   int sonocartella1 = isDIR(argv[1]);
   int sonocartella2 = isDIR(argv[2]);
   //entrambi file
   if( !(sonocartella1) && !(sonocartella2) ){

		//------------- sono 2 file ----------------

		differenzaFraFile = compareFile(argv[1],argv[2]);
		if (differenzaFraFile==0 )
		{
			scrivilog("Sono 2 file uguali \n");
			printf("true \n");
			exit(0);
		}else{
			
		
			scrivilog("Sono 2 file diversi \n");
			printf("false \n sono diversi al byte %d \n",differenzaFraFile);
			exit(0);
		}

	}
		
	// ------- il primo è una cartella e il secondo è un file
	
	scrivilog("controllo se gli elementi sono dello stesso tipo (cartella o file) \n");
	if( (sonocartella1) && !(sonocartella2) )
	{
			printf("false \n");
			scrivilog("gli elementi sono di tipo diverso \n");
			exit(0);
		
	// ----------------- il primo è un file e il secondo è una cartella
	}else if( !(sonocartella1) && (sonocartella2) )
		{
			printf("false \n");
			scrivilog("gli elementi sono di tipo diverso \n");
			exit(0);
		}
	
    scrivilog("Sono due cartelle \n");
	
    char * first = malloc(sizeof(char*) * (sizeof(argv[1])+3));
    first = argv[1];
   
    char * second = malloc(sizeof(char*) * (sizeof(argv[2])+3));
    second = argv[2];

   
   
   
    //scorro il percorso e lo salvo nell'array percorsi e in t salvo la grandezza dell'array
    int t = 0;	
    percorsi = scorripercorso(first,&t);

  
 	//scorro il percorso e lo salvo nell'array percorsi2 e in t2 salvo la grandezza dell'array
    int t2 = 0;
    percorsi2 = scorripercorso(second,&t2);
	
   
    scrivilog("Ordino entrambi gli array in modo che sia più facile confrontare gli elementi dei 2 percorsi \n");
	
    //ordino gli array in modo che poi la verifica se le cartelle sono uguali o diverse è più veloce
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


    /*qui controllo se c'è il simbolo '/' alla fine del secondo elemento, se c'è
        lo tolgo*/    
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


/*
  Questa è la funzione che chiama savedir()
  sostanzialmente inizializza le variabili e i parametri da passare alla savedir
  inoltre restituisce l'array riempito dalla savedir().
  @folder, contiene la cartella da scorrere
  @n, qui verrà salvata la lunghezza dell'array, infatti in input và dato l'indirizzo della variabile che lo conterrà

*/
char** scorripercorso(char* folder,int* n)
{
    scrivilog("Sono nella funzione scorripercorso e inizio a riempire l'array dei percorsi \n");
    int num [1];
    (*num) = 10;
    char ** percorsi = malloc(sizeof(char**) * (*num));
    int temp [1];
    char * inizio = malloc(sizeof(char*) * sizeof(folder));
    strcpy(inizio,folder);
    *temp = 0;

    
    savedir(inizio,&percorsi,temp,num,inizio);

    (*n) = (*temp);
    return percorsi;

}

/*
   Questa funzione dice se un determinato elemento è una cartella o meno
   @folder, l'elemento che viene verificato
*/
bool isDIR(char * folder){
	
	struct stat statbuf;
	stat(folder, &statbuf);
	return S_ISDIR(statbuf.st_mode);

}


/*
Questa funzione viene utilizzata per comparare le stringhe.
E' particolarmente importante perchè è la funzione di comparazione che poi
viene data in input alla funzione q_sort
*/
int comp(const void *x, const void *y)
{
    return(strcmp(*((char**)x),*((char**)y)));      
}

/*
Questa funzione riceve in input
@first, è l'array di elementi(cartelle o file) associato alla prima cartella
@second, è l'array di elementi(cartelle o file) associato alla seconda cartella
@fLun, è la lunghezza dell'array first
@sLun, è la lunghezza dell'array second
@fHead, è l'intestazione che andrà aggiunta agli elementi del primo array per verificare, in caso ci siano file, se sono uguali
@sHead, è l'intestazione che andrà aggiunta agli elementi del secondo array per verificare, in caso ci siano file, se sono uguali
*/
void compareArray(char** first, char** second,int fLun,int sLun,char* fHead, char* sHead)
{
	scrivilog("entro nella funzione compareArray, inizio a confrontare i due array \n"); 
	int i = 0;
	int j = 0;
	bool sonouguali = true;
	bool primavolta = true;
	int res = 0;
	int difference = 0;

	char* ultimodiverso = malloc ( sizeof(char*) * 200);
	strcpy(ultimodiverso," ");
	
	while(i<fLun && j<sLun)
	{
            res = strcmp(first[i],second[j]); // qui confronto i due elementi dell'array
	    if(res == 0){ 		      
			
		  //creo 1 stringa dalla lunghezza dell'elemento dell'array e l'header, cioè il nome iniziale del percorso
		  // in questo modo potrò verificare se sono cartelle/file e confrontarli
		  char * primo = malloc( sizeof(char**) * (strlen(first[i]) + strlen(fHead) +2) ) ; 

		  //ripeto l'operazione sopra
		  char * secondo = malloc( sizeof(char**) * (strlen(second[j]) + strlen(sHead) +2) ) ;
		  
		  strcpy(primo, fHead); //qui copio dentro la nuova stringa l'intestazione
          	  strcat(primo, first[i]); //qui aggiungo l'elemento dell'array che sto visitando
		  
		  strcpy(secondo, sHead); //qui copio dentro la nuova stringa l'intestazione
		  strcat(secondo, second[j]); //qui aggiungo l'elemento dell'array che sto visitando

		
		 
		  if ( !isDIR(primo) && !isDIR(secondo) ){ //se sono entrambi cartelle sono uguali
		    					   //se sono file devo verificare se sono uguali e dove differiscono 
         		difference = compareFile(primo,secondo);     
	
			if ( difference == 0 ){
		    		scrivilog("I file sono uguali \n");
			}else
			{
				
				if(primavolta){ // se è la prima volta che trovo un elemento diverso scrivo false
				primavolta = false;
				scrivilog("Ho trovato il primo elemento che differisce \n");
				printf("false \n");
				}

				sonouguali = false; //setto la variabile a false così alla fine so che le due cartelle non sono uguali

				printf("I file %s sono diversi al byte %d \n",first[i],difference);
				scrivilog("I file %s diversi al byte %d \n",first[i],difference);
			}
		   }else
			scrivilog("In entrambi i percorsi c'è la stessa cartella \n");

		   i++;
		   j++;			
	    }else if (res < 0)
		{
			//l'elemento %s non è contenuto nel secondo array
			if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("Ho trovato il primo elemento che differisce \n");
			printf("false \n");
			}
		

		sonouguali = false; //setto la variabile a false così alla fine so che le due cartelle non sono uguali
		
		i++;
		/* questo controllo serve ad evitare di scrivere tutti i sottopercorsi
		 in caso ci siano cartelle diverse*/
		
		verificaLast(&ultimodiverso,first[i-1],"<<");

		
		}
		else
		{
		//l'elemento %s non è contenuto nel primo array

		if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("Ho trovato il primo elemento che differisce \n");	
			printf("false \n");
			}
		
		sonouguali = false; //setto la variabile a false così alla fine so che le due cartelle non sono uguali

	
		j++;
	

		//prima di mettere a video un'elemento che differisce,
 		//controllo che non sia la sottocartella di un elemento che sappiamo già che differisce
		//così evito di scrivere tutti i sottopercorsi
		verificaLast(&ultimodiverso,second[j-1]," >>>>");
		}

	}
	
	// in questo punto controllo se ci sono elementi che non ho guardato perchè il ciclo si è concluso prima
	if ( i<fLun){

		if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("Ho trovato il primo elemento che differisce \n");
			printf("false \n");
			}

		sonouguali = false;

		while(i<fLun){	
		i++;
		
		verificaLast(&ultimodiverso,first[i-1],"<<");


		}
	}

	

	// in questo punto controllo se ci sono elementi che non ho guardato perchè il ciclo si è concluso prima
	if (j <sLun){

		if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("Ho trovato il primo elemento che differisce \n");
			printf("false \n");
			}

		sonouguali = false;
		while(j<sLun){	
		j++;
	
		
		verificaLast(&ultimodiverso,second[j-1]," >>>>");
		}
	}


	if(sonouguali){
		printf("true \n"); //se non ho trovato neanche un elemento che differisce scrivo true
		scrivilog("Sono uguali \n");
		}
}


/*
Questa funzione, dati 2 file, verifica se sono uguali
ritorna 0 se sono uguali e 1 se sono diversi
-1 in caso uno dei due elementi non esista
Il controllo viene fatto byte per byte
*/
int compareFile(char * primo, char* secondo)
{
	scrivilog("confronto il file %s e %s se sono uguali ritorno 0 altrimenti un numero pari al primo byte diverso \n",primo,secondo);
	FILE *fileFST;
	FILE *fileSND;

	//qui controllo semplicemente se i file che sto controllando sono esattamente gl stessi
	if (strcmp(primo,secondo) == 0)
		return 0;


	fileFST = fopen(primo, "rb");
	fileSND = fopen(secondo, "rb");
	bool uguali = true;
	
	/*
	Prima verifico se i 2 file esistono e si possono aprire
	*/
	if ( fileFST == NULL )
	       {
	       printf("false \n Il file %s non esiste ", primo );
	       return -1;
	       }
	else if (fileSND == NULL)
	       {
	       	printf("false \n Il file %s non esiste ", secondo );
		return -1;
		}
   	else
       	{
		//ora inizio a confrontare i 2 file
       		char ch1  =  getc( fileFST ) ;
       		char ch2  =  getc( fileSND ) ;
		
		if (ch1!=ch2)
			return 1;

       int i = 1;
       while( (ch1!=EOF) && (ch2!=EOF) && (uguali==true))
        {
            ch1 = getc(fileFST);
            ch2 = getc(fileSND) ;

	    i++;
	    if(ch1!=ch2)
	     {
		uguali = false;
		return i;
             }
        }

        if (uguali)
            return 0;
       

        fclose ( fileFST );
        fclose ( fileSND );
       }
	
	return true;
	
}
/*
Questa funzione verifica quale elemento ho scritto per ultimo e se la prima parte del nuovo
file è del tutto uguale al precedente ultimo scritto, Vuol dire che sono diversi perchè, il nuovo elemento che differisce, è un file 
o una sottocartella che appartiene alla cartella che era diversa prima.
Quindi evito di scrivere anche i file e le sottocartelle diverse, dato che se una cartella non è presente in uno dei due percorsi
neanche tutti i suoi sottofile e sottocartelle lo saranno.
*/
void verificaLast(char** x, char* y,char* add)
{
	if (strcmp( (*x) ," ")== 0){
		printf("%s %s \n",add,y);

		scrivilog("l'elemento %s è contenuto in uno solo dei due percorsi",y);

		free(*x);
		(*x) = malloc( sizeof(char*) * sizeof(y) );
		strcpy( (*x) ,y);
	}
	else
 	{
		if ( strncmp(y,(*x) , strlen( (*x) ) ) != 0){

			free(*x);
			(*x) = malloc(sizeof(char*) * sizeof(y));
			strcpy( (*x) ,y);

			scrivilog("l'elemento %s è contenuto in uno solo dei due percorsi",y);

			printf("%s %s \n",add,y);

		}	
	}
		
}


