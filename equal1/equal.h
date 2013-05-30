bool isDIR(char * folder);
void compareArray(char** first,char** second, int fLun,int sLun,char* fHead, char* sHead);
void savedir(char *folder1, char*** percorsi,int* i,int* n,char* radice);
char** scorripercorso(char* folder,int* n);
int comp(const void *x, const void *y);
void verificaLast(char** x, char* y, char* add);
bool compareFile(char* primo, char* secondo, bool* primavolta);
