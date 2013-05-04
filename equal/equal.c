/*
 ============================================================================
 Name        : equal.c
 Author      : Giuliano Tortoreto and Andrea Gottardi
 Version     : 0.1
 Copyright   : Your copyright notice
 Description : 
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dirent.h>

struct dirent *readdir(DIR *dirp);

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);


int main( int argc, char *argv[]) {

	bool euguale;
	euguale= true;
	int ind = 0;
	char* nomeFirst;
	char* nomeSecond;
	char* percorsoFirst;
	char* percorsoSecond;

	 if ( argc != 3 ) /* argc should be 3 for correct execution */
	    {
	        /* We print argv[0] assuming it is the program name */
	        printf( "usage: %s filename", argv[0] );
	    }else
	    {
	    	// We assume argv[1] is a filename to open
	    	       // FILE *first = fopen( argv[1], "O_RDONLY");
			//FILE *second = fopen(argv[2], "O_RDONLY");
			//percorsoFirst = argv[1];
			//percorsoSecond = argv[2];

			if(ISDIR(argv[1])){
				readdir(argv[1]);
				printf("quest è cartell %s \n",argv[1]);

				}


	 

	return 0;
}

