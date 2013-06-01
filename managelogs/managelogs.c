/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : managelogs.c
 Description   : Codice di gestione dei log di sistema
 ============================================================================
 */


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <libgen.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <dirent.h>
#include <fcntl.h>
#include "managelogs.h"

static FILE * log;

// questa funzione si occupa di creare il log se non esiste, altrimenti lo apre
int crealog(char * progname){


	progname = basename(strdup(progname));
	  if(log != NULL){
       return 1;					//se il log esiste aprilo
    }
	int size = strlen(progname);
	char * file_to_open = malloc(strlen(PATH) + size + 1);
	strcpy(file_to_open, PATH); 			//qui viene aggiunta l'intestazione /var/log/utility
	strcat(file_to_open, progname);			//qui viene aggiunto il nome progname

    FILE * temp = fopen(file_to_open, "r");
    if(temp == NULL)
	{
        fprintf(stderr,"Il file di log %s non esiste. Eseguo la creazione \n",file_to_open);
    }
	else
	{
        fclose(temp);
    }

	log = fopen(file_to_open, "a");// viene creato e aperto in modalità append()
				// i privilegi sono di sudoer			
	if (log == NULL)
	{
		return 1;
	}

    free(file_to_open);
	return 0;
}


//si occupa di scrivere i log
int scrivilog(char * format, ...){
    va_list arg_list;
    va_start(arg_list, format);
    if(log == NULL)
	{
        return 1;
    }
   
    vfprintf(log, format, arg_list);
    va_end(arg_list);
return 0;
}

int chiudilog(){
    if(log == NULL)
	{
        return 1;
    }

	
	int out = fclose(log);
	if (out != 0)
	{
		return 1;
	}

	return 0;
}

