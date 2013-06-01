/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : mkbkp.h
 Description   : Header file di mkbkp.c
 ============================================================================
 */

void usage();
char * collega(char * a, char * b);
char * collegaSlash(char * a, char * b);
void read_words (FILE *f);
void read_dirs (FILE *f);
void writeByChar(const char *filename, FILE *out);
void crea_file(FILE *f);

void creabkp(int numpar, char * param[], int ind);
void stampa();
void estrai();
void trovaInizioFine( int cont);
void scriviFile(const char * arrivo);
int list(const char *name, const struct stat *status, int type);
int listC(const char *name, const struct stat *status, int type);
int listD(const char *name, const struct stat *status, int type);

