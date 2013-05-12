#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	char * perc;
	printf("si apre?");
	perc = malloc(16);
	
	sprintf(perc, "/proc/%d/stat\0", 1);
	printf("%s \n", perc);
}
