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




int lunghezza;// questa variabile la utilizzo nella savedir per salvarmi la lunghezza della radice
int lung; //questa variabile la utilizzo nella savedir per allocare la giusta quantità di memoria per il nuovo elemento trovato

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

		// nelle operazioni qui sotto mi salvo il nuovo elemento trovato in new_folder1
	    free(new_folder1);
	    new_folder1 = malloc(sizeof(char*)*(lung)); 
	    strcpy(new_folder1, folder1);    
	    strcat(new_folder1, "/");
	    strcat(new_folder1, ent->d_name);        
		

	   //qui inserisco nella posizione i dell'array il nuovo elemento (senza la radice)
	    (*percorsi)[*i] = malloc(sizeof(char*) * ( (lung) -lunghezza));
	    strcpy((*percorsi)[*i],(new_folder1+lunghezza)) ;

	     (*i)++;

	//qui controllo se non ho più spazio nel vettore, se si lo ingrandisco
	    if(*i>=( (*n) -1)){
		(*n) = (*n)*2;
		*percorsi =  realloc((*percorsi),sizeof(char**) * (*n));
		}
	    
	   //printf("%s \n",(*percorsi)[(*i)-1]);
            /* Recurse at a new indent level */

	    if (isDIR(new_folder1)){
		
            savedir(new_folder1,percorsi,i,n,radice);
	   
       		}
		
	  
	
    }
 
    free(new_folder1);
    closedir(dir1);

}

//qui inizia il programma
int main( int argc, char *argv[])
{
  //  int differenzaFraFile = 0;
    bool uguali = true;
    char ** percorsi; 
    char ** percorsi2;

    crealog(argv[0]);
    scrivilog("Inizia il programma, gli argomenti ricevuti sono %s e %s \n",argv[1],argv[2]);
     atexit(chiudiprogramma);
  
   if ( argc != 3 ) /* argc should be 3 for correct execution */
	    {
	        scrivilog("Mancano i nomi dei percorsi da verificare \n");
	        printf( "Uso corretto: ./equal path1 path2 \n");
		exit(0);
	    }

   scrivilog("Controllo se i percorsi esistono \n");
   //controllo se i percorsi esistono
   int esiste1 = access(argv[1],F_OK);
   int esiste2 = access(argv[2],F_OK);

   if (( esiste1 ==-1 ) && ( esiste2==-1))
	{
	printf("Entrambi i percorsi non esistono, verifica i percorsi inseriti \n");
	scrivilog("Entrambi i percorsi sono sbagliati \n");
	exit(0);

	}else if(esiste1==-1)
	{

	printf("Il percorso %s non esiste, inseriscine un'altro\n",argv[1]);
	scrivilog("Il primo percorso è sbagliato \n");
	exit(0);

	}else if(esiste2==-1){

	printf("Il percorso %s non esiste, inseriscine un'altro\n", argv[2]);
	scrivilog("Il secondo percorso è sbagliato \n");
	exit(0);

	}

	scrivilog("Esistono \n");

   int sonocartella1 = isDIR(argv[1]);
   int sonocartella2 = isDIR(argv[2]);
   //entrambi file
   if( !(sonocartella1) && !(sonocartella2) ){

		//------------- sono 2 file ----------------
		scrivilog("Sono 2 file \n"); 
		uguali = compareFile(argv[1],argv[2],&uguali);
		if (uguali){
			printf("0 \n");
			scrivilog("Sono uguali \n 0");
			}		
		exit(0);

	}
		
	// ------- il primo è una cartella e il secondo è un file
	
	scrivilog("Controllo se gli elementi sono dello stesso tipo (cartella o file) \n");
	if( (sonocartella1) && !(sonocartella2) )
	{
			printf("1 \n");
			scrivilog("1 \nGli elementi sono di tipo diverso: %s è una cartella e %s è un file \n",argv[1],argv[2]);
			exit(0);
		
	// ----------------- il primo è un file e il secondo è una cartella
	}else if( !(sonocartella1) && (sonocartella2) )
		{
			printf("1 \n");
			scrivilog("1 \nGli elementi sono di tipo diverso: %s è una file e %s è una cartella \n",argv[1],argv[2]);
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
    scrivilog("Riempio l'array dei percorsi associato a %s \n",folder);
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
È la funzione di comparazione che poi
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
	scrivilog("Inizio a confrontare i due array dei percorsi \n"); 
	int i = 0;
	int j = 0;
	bool sonouguali = true;
	bool primavolta = true;
	int res = 0;
	//int difference = 0;

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
         		
			sonouguali = sonouguali & compareFile(primo,secondo,&primavolta);

			
		   }else
			scrivilog("In entrambi i percorsi c'è la cartella %s \n",first[i]);

		   i++;
		   j++;			
	    }else if (res < 0)
		{
			//l'elemento %s non è contenuto nel secondo array
			if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("1 \nHo trovato il primo elemento che differisce \n");
			printf("1 \n");
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
			scrivilog("1 \n Ho trovato il primo elemento che differisce \n");	
			printf("1 \n");
			}
		
		
		sonouguali = false; //setto la variabile a false così alla fine so che le due cartelle non sono uguali

	
		j++;
	

		//prima di mettere a video un'elemento che differisce,
 		//controllo che non sia la sottocartella di un elemento che abbiamo già stampato sul dispay
		//così evito di scrivere tutti i sottopercorsi diversi
		verificaLast(&ultimodiverso,second[j-1]," >>>>");
		}

	}
	
	// nel caso la prima cartella contenga più elementi della seconda
	if ( i<fLun){

		if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("1 \n Ho trovato il primo elemento che differisce \n");
			printf("1 \n");
			}

		sonouguali = false;

		while(i<fLun){	

		i++;
		verificaLast(&ultimodiverso,first[i-1],"<<");


		}
	}

	

	//nel caso la seconda cartella contenga più elementi della prima
	if (j <sLun){

		if(primavolta) // se è la prima volta che trovo un elemento diverso scrivo false
			{
			primavolta = false;
			scrivilog("1 \n Ho trovato il primo elemento che differisce \n");
			printf("1 \n");
			}

		sonouguali = false;
		while(j<sLun){	

		j++;
		verificaLast(&ultimodiverso,second[j-1]," >>>>");
		}
	}


	if(sonouguali){   //se non ho trovato neanche un elemento che differisce scrivo true
		printf("0 \n"); 
		scrivilog("0 \n");
		}else{
		scrivilog("1 \n");
		}
}



/*
Questa funzione verifica qual'è l'ultimo elemento diverso che ho stampato a video e quello che sto per stampare.
SE quello che sto per stampare ai primi strlen(ultimo elemento diverso) caratteri uguali, vuol dire che 
quello che sto per stampare è una sottocartella dell'ultimo elemento diverso.
Ma sarebbe ripetitivo, inoltre è ovvio che se una cartella non appartiene ad entrambi neanche le sue 
sottocartelle e i sottofile apparterrano ad entrambi.

*/
void verificaLast(char** x, char* y,char* add)
{
	if (strcmp( (*x) ," ")== 0){
		printf("%s %s \n",add,y);

		if (strcmp(add,"<<")== 0)
			scrivilog("L'elemento %s è contenuto solo nella prima cartella \n",y);
		else
			scrivilog("L'elemento %s è contenuto solo nella seconda cartella \n",y);

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

			if (strcmp(add,"<<")== 0)
				scrivilog("l'elemento %s è contenuto solo nella prima cartella \n",y);
			else
				scrivilog("l'elemento %s è contenuto solo nella seconda cartella \n",y);

			printf("%s %s \n",add,y);

		}	
	}
		
}

/*
   Se sono uguali restituisce true, altrimenti restituisce false
   Inoltre stampa a video la prima differenza, mentre nei log scrive tutte le posizioni dei byte in cui differisce
*/
bool compareFile(char* primo, char* secondo, bool* primavolta){
	
	scrivilog("Confronto il file %s e %s \n",primo,secondo);
	FILE *fileFST;
	FILE *fileSND;

	int numval = 0;//questa variabile serve solo per tenere ordinato il file di log


	bool printedfirst = false;//questa booleana la uso in modo da stampare solo il primo bit che differisce
				//tutto il resto viene scritto nel file di log


	//qui controllo semplicemente se i file che sto controllando sono esattamente gl stessi
	//in questo caso neanche vado avanti
	if (strcmp(primo,secondo) == 0){		
		return true;
		}
	fileFST = fopen(primo, "rb");
	fileSND = fopen(secondo, "rb");
	bool uguali = true;


	/*
	Prima verifico se i 2 file esistono e si possono aprire
	*/
	if ( fileFST == NULL )
	       {
	       printf("1 \n Il file %s non esiste ", primo );
	       return false;
	       }
	else if (fileSND == NULL)
	       {
	       	printf("1 \n Il file %s non esiste ", secondo );
		return false;
	

	// i file esistono
	}else{


	//ora inizio a confrontare i 2 file
       	char ch1  =  getc( fileFST ) ; //uso i char che equivalgono ad un byte
        char ch2  =  getc( fileSND ) ; //uso i char che equivalgono ad un byte
		
	if (ch1!=ch2){
		uguali = false;

		if(*primavolta){ // se è la prima volta che trovo un elemento diverso scrivo false
			*primavolta = false;
			scrivilog("1 \n Ho trovato il primo elemento che differisce \n");
			numval++;
			printf("1 \n");
			printedfirst = true;
			uguali = false;
			}
			
		printf("%s Differisce al byte: 1",primo);
		scrivilog("%s Differisce al byte: 1",primo);
		stampaBit(ch1,ch2,'s',numval);
		stampaBit(ch1,ch2,'l',numval);
		
	}	

	  int i = 1;//inizio a scorrere entrambi i file
       while( (ch1!=EOF) && (ch2!=EOF))
        {
            ch1 = getc(fileFST); 
            ch2 = getc(fileSND) ;

	    i++;
	    if(ch1!=ch2)
	     {
		uguali = false;
			

		if(*primavolta){ // se è la prima volta che trovo un elemento diverso scrivo false
			*primavolta = false;
			scrivilog("1 \nHo trovato i primi elementi che differiscono: %s \n",primo);
			scrivilog("Differiscono al byte:\n ",i); 
			printf("1 \n %s differiscono al byte: %d \n",primo,i);
			printedfirst = true;
			numval++;

			stampaBit(ch1,ch2,'s',numval);
		}else{

			//questo if mi serve affinchè mostri a display solo il primo byte diverso
			//invece nel file di log li scrivo tutti, in modo che lì sarà visibile tutte le differenze fra i 2 file
			//ma a schermo mostro solo le maggiori differenze
			if (printedfirst==false)
			{			
			scrivilog("I file %s differiscono al byte:\n ",primo);	
			numval++;
			printf("%s differiscono al byte: ",primo);
			printf("%d",i);
			printedfirst = true;

			

			stampaBit(ch1,ch2,'s',numval);
			}

		}

		scrivilog(" %d",i);
		numval++;
		
		stampaBit(ch1,ch2,'l',numval);
	
             }

		
        }

	//nel caso un file sia più piccolo dell'altro continuo a scrivere le differenze
	int j= i;
	while( (ch1!=EOF) )
	{
		ch1 = getc(fileFST);
		j++;
		uguali = false;

		if(*primavolta){ // se è la prima volta che trovo un elemento diverso scrivo false
		*primavolta = false;
		scrivilog("1 \nHo trovato il primo elemento che differisce \n");
		printf("1 \n %s differiscono al byte: %d \n",primo,j);
		printedfirst = true;
		}else{
			//questo if mi serve affinchè mostri a display solo il primo byte diverso
			//invece nel file di log li scrivo tutto, in modo che lì sarà visibile tutte le differenze fra i 2 file
			//ma a schermo mostro solo le maggiori differenze
			if (printedfirst==false)
			{	
			scrivilog("I file %s differiscono al byte:\n ",primo);
			printf("I file %s differiscono al byte: %d(bit: all) \n",primo,j);
			printedfirst = true;
			}

			//questa parte serve solo a tenere il file di log ordinato
			scrivilog(" %d(bit: all)",j);//stampo all perchè l'intero byte non è presente nell'altro elemento
			numval++;
			if(numval%4==0)
				scrivilog("\n");
			else
				scrivilog("\t");
		}

	}
	
	//nel caso un file sia più piccolo dell'altro continuo a scrivere le differenze
	while( (ch2!=EOF) )
	{
		ch2 = getc(fileSND);
		i++;
		
		uguali = false;

		if(*primavolta){ // se è la prima volta che trovo un elemento diverso scrivo false
		*primavolta = false;
		scrivilog("1 \nHo trovato il primo elemento che differisce \n");
		printf("1 \n %s differiscono al byte: %d \n",primo,i);
		printedfirst = true;
		}else{
				
			//questo if mi serve affinchè mostri a display solo il primo byte diverso
			//invece nel file di log li scrivo tutto, in modo che lì sarà visibile tutte le differenze fra i 2 file
			//ma a schermo mostro solo le maggiori differenze
			if (printedfirst==false)
			{	
			scrivilog("I file %s differiscono al byte:\n ",primo);
			printf("%s differiscono al byte: %d(bit: all) \n",primo,i);
			printedfirst = true;
			}

			//questa parte serve solo per tenere il file di log ordinato
			scrivilog(" %d(bit: all)",i);//stampo all perchè l'intero byte non è presente
			numval++;
			if(numval%4==0)
				scrivilog("\n");
			else
				scrivilog("\t");
			//qui finisce 
		}

	}


        if (uguali){
	    scrivilog("I file sono uguali \n\n");
            return true;
	}else{
		scrivilog("Ho finito di scorrere i 2 file %s, sono diversi \n\n");
		return false;
	}

        fclose ( fileFST );
        fclose ( fileSND );
       }




}


/*
	Stampa a video o sul file di log le differenze fra due byte. Cioè le posizioni dei bit in cui differisce.
	Le stampa a video se @s == 's', altrimenti viene stampato sul file di log

*/
void stampaBit(char a,char b,char s,int ind)
{
//questa parte serve a stampare sul file di log i bit in cui è diverso ogni byte
		int ibit = 0;
		bool retval;

		if (s == 's')
			printf("(al bit:");
		else
			scrivilog("(al bit:");

		while(ibit<8){
			retval=!( (a & 1) ^ ( b & 1) );
			
			if ( retval == false)	
			{	
				if(s == 's')
					printf(" %d",ibit);
				else
					scrivilog(" %d",ibit);
			}

		
			a=a>>1;
			b=b>>1;
			ibit++;
		}
		//qui finisce

		//questa parte la utilizzo per tenere ordinato il file di log

		if(s == 's')	{	
			printf(") \n");
		}else{
				
			if(ind%4==0)
				scrivilog(") \n");
			else
				scrivilog(")\t");
		}	
}

/* questa funzione chiude il log */
void chiudiprogramma(){
	scrivilog("%s","Sono dentro alla funzione che chiude questo log\n");
	chiudilog();
}

