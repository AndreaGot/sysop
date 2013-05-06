
#include <stdio.h>
#include <string.h>
	
	int main (int argc, char* argv[])
	{
		char str[] = argv[1];
		char * pch;
		pch=strrchr(str,'/');
		printf ("Last occurence of '/' found at %ld \n",pch-str+1);
		printf("The substring from last occurrence to the end is %s", strndup(pch+1, strlen(str)-pch));
		return 0;
	}