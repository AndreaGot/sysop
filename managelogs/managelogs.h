/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : managelogs.h
 Description   : File di header di managelogs.c
 ============================================================================
 */

#define PATH "/var/log/utility/"

// crea il file di log (o lo apre nel caso esistesse già);

int crealog(char * );

// scrive il file di log

int scrivilog(char * , ...);

//chiude il file di log

int chiudilog();


