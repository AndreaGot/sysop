
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

struct dirent *readdir(DIR *dirp);

main(int argc, char * argv[]) {
	DIR *cartella;										// puntatore alla cartella (che verrà aperta usando il path al lancio del programma)
	struct dirent *elemento;							// puntatore all'elemento della cartella (struct composta da vari valori tra cui: IDelemento, offset, LungElemento, nome)
	
	if ((cartella = opendir(argv[1])) == NULL)			// se non c'è il parametro
		perror("opendir() error");						// perror() gestisce l'errore stampando la frase e la motivazione dell'errore
	else {
		printf("contents of root: \n");
		while ((elemento = readdir(cartella)) != NULL)	// se si arriva qui la cartella ESISTE, per cui si può ciclare tra i suoi elementi
			printf("  %s\n", elemento->d_name);			// stampa attributo nome dell'elemento nella struct dirent (== elemento in tabella)
		closedir(cartella);								// chiude la connessione alla cartella
	}
}