#include "string.h"
#include "stdlib.h"
#include "sort.h"

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif


void merge(char** stringhe,char** ausil, int left, int center, int right)
{
	int i = left;
	int k = center +1 ;
	int j = 0;

	while( (i<=center) && (k<=right))
	{
		if ( qualeminore(stringhe[i],stringhe[k]) == 0 )
		{
			ausil[j] = stringhe[i];
			i++;

		}else
		{
			ausil[j] = stringhe[k];
			k++;
		}
		j++;
	}

	while(i<=center)
	{
		ausil[j] = stringhe[i];
		i++;
		j++;
	}


	while(k<=right)
	{
		ausil[j] = stringhe[k];
		k++;
		j++;
	}

	k=left;
	while(k<=right)
	{
		stringhe[k] = ausil[k - left] ;
		k++;
	}
}

void mergesort(char** stringhe, char** ausil, int left, int right)
{
	int center = 0;
	if (left<right)
	{
		center = (left+right)/2;
		mergesort(stringhe,ausil,left,center);
		mergesort(stringhe,ausil,center+ 1,right);
		merge(stringhe,ausil,left,center + 1,right);

	}
}

void sort(char** vettore,int lunghezza)
{
	char ** temp = (char**) malloc(sizeof(char**)*lunghezza);
	mergesort(vettore,temp,0,lunghezza-1);
	
	free(temp);
	

}


//ritorna 0 se la prima è minore della seconda e 1 se la seconda è minore della prima
//in caso siano uguali mette prima la più corta

int qualeminore(char* stringOne, char* stringTwo)
{
	int i = 0;
	int res = -1;

	char ch1;
	char ch2;
	while(i< min(strlen(stringOne),strlen(stringTwo)) && res == -1)
	{
	
		ch1 = stringOne[i];
            	ch2 = stringTwo[i];
		if(ch1== ch2)
			i++;
		else if (ch1<ch2)
			res = 0;
		else
			res = 1;
	}
	
	if( (res==-1))
	{	
		if (strlen(stringOne)<strlen(stringTwo) )
			res=0;
		else
			res = 1;
	}
	else

	return res;
}



