 /*
    ============================================================================
    Name        : copyfile.c
    Author      : Andrea Gottardi (151955)
    Version     :
    Description : Dati in input due file, copa il primo file nel secondo.
    ============================================================================
    */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <errno.h>
 #define PERMS 0666

int main( int argc, char *argv[]) 
{
	
	// SE IL NUMERO DI ARGOMENTI È DIVERSO DA 3 ALLORA ESCE DAL PROGRAMMA
	 
	if ( argc != 3 ) //argc should be 3 for correct execution 
	{
			perror( "Usage: ./a.out source target" );
			exit(1);
	}
	
			
	int first;
	int second;
	int n;
	char buf[BUFSIZ];
	
	
	// CONTROLLO SE IL PERCORSO 1 PORTA A UN FILE ESISTENTE (CRITICAL)
			
	if( access(argv[1], F_OK ) != -1 )						//se riesce ad accedere al file source..
	{														//.. il file esiste!
		printf("il file esiste!");							//
		first = open( argv[1], O_RDONLY );					// Quindi lo apro
	} 
	else													// altrimenti...
	{														//
		perror("il file non esiste!");						// errore!
		exit(1);
	}
	
	
	// CONTROLLO SE IL PERCORSO 2 PORTA A UN FILE ESISTENTE
	
	
	if( access(argv[2], F_OK ) != -1 )						//se riesce ad accedere al file source..
	{														//.. il file esiste!
		printf("il file esiste! lo cancello e lo ricreo");	//
		unlink(argv[2]);									// Lo cancello (altrimenti sovrascriverebbe il file, lasciando le parti "vecchie")
	}
	else													// altrimenti...
	{														//
		printf("il file non esiste!, lo creo");				// lo creo semplicemente
	}
	
	second = creat(argv[2], PERMS);							// qui la creazione del file (si fa comunque, non ci fosse il file uscirebbe prima)
	
	
	// COPIA IL CONTENUTO DEL FILE SOURCE IN TARGET
	
			while ((n = read(first, buf, BUFSIZ)) > 0)
			{
				write(second, buf, n);
				puts("entro");
			}
return 0;
}