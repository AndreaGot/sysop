/*
 ============================================================================
 Name        : equal.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main( int argc, char *argv[]) {

	bool euguale;
	euguale= true;
	int ind = 0;
	 if ( argc != 3 ) /* argc should be 3 for correct execution */
	    {
	        /* We print argv[0] assuming it is the program name */
	        printf( "usage: %s filename", argv[0] );
	    }else
	    {
	    	// We assume argv[1] is a filename to open
	    	        FILE *first = fopen( argv[1], "r" );
			FILE *second = fopen(argv[2], "r");


	    	        /* fopen returns 0, the NULL pointer, on failure */
	    	        if ( first == 0 )
	    	        {
	    	            printf( "Could not open file %s \n",argv[1]);
	    	        }
	    	        else if (second == 0)
			{
				printf( "Could not open file %s \n",argv[1]);
			}else
	    	        {
	    	            int x;
			    int y;
	    	            /* read one character at a time from file, stopping at EOF, which
	    	               indicates the end of the file.  Note that the idiom of "assign
	    	               to a variable, check the value" used below works because
	    	               the assignment statement evaluates to the value assigned. */
	    	            while  ( (( x = fgetc( first ) ) != EOF ) &&  (( y = fgetc( second ) ) != EOF)&&(euguale==true))
	    	            {	
				if (x != y)
				{	
				   euguale = false;
				}
				ind++;
	    	            }
	    	            fclose( first );
			    fclose( second );
	    		}
	}


	if (euguale==true)
	{
	    printf("%d \n",0);
	}else
	{
	    char* nomifile;
	    nomifile = strcat(argv[1],argv[2]);
	    printf("%d \n",1);
	    printf("I file diversi sono %s \n",nomifile);
	}

	 

	return 0;
}

