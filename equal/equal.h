/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : equal.h
 Description   : File di header di equal.c
 ============================================================================
 */
bool isDIR(char * folder);
void compareArray(char** first,char** second, int fLun,int sLun,char* fHead, char* sHead);
void savedir(char *folder1, char*** percorsi,int* i,int* n,char* radice);
char** scorripercorso(char* folder,int* n);
int comp(const void *x, const void *y);
void verificaLast(char** x, char* y, char* add);
bool compareFile(char* primo, char* secondo, bool* primavolta);
void stampaBit(char a,char b,char s,int ind);
void chiudiprogramma();
