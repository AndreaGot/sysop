#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>



double inizio[10], fine[10], start, end;
int *processi;
double *cpuinizio;
double *cpufine;

int size = 0;


double CPUvalue(double * array)
{
	double result;
	int i = 0;
	FILE * proc;
	proc = fopen("/proc/stat", "rb");
	printf("file opened \n");
	fscanf(proc, "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &array[0], &array[1], &array[2], &array[3], &array[4], &array[5], &array[6], &array[7], &array[8], &array[9]); //legge ogni valore dei vari settori della cpu
	fclose(proc);
	
	while(i<=9) //somma i valori 
	{
		result += array[i];
		i++;
	}
	
	return result;

}	

void trovaProcessi()
{
int i = 0;
DIR *dir;
struct dirent *ent;
bool trovato = false;


if ((dir = opendir ("/proc/")) != NULL) 
	{
		while ((ent = readdir (dir)) != NULL) {
			if(trovato)
			{
    			size++;
    			processi=(int *) realloc(processi,size*sizeof(int));
    			processi[size-1] = atoi(ent->d_name);
    			
    		}
    		if(strcmp(ent->d_name, "self")==0)
    			trovato = true;
  		}
  		closedir (dir);
  		while(i<size)
  		{
  			printf("%d \n", processi[i]);
  			i++;
  		}
  		printf("Il numero di processi è %d \n", size);
	} 
	else 
	{
  /* could not open directory */
  	perror ("");
	}
	

}


void leggiPidStat(int index, double *array)
{
	printf("entrato \n");
	FILE * pid;
	double cpu[2];
	double cpusum;
	char * perc;
	printf("si apre? \n");
	perc = malloc(16);
	
	sprintf(perc, "/proc/%d/stat\0", processi[index]);
	pid = fopen(perc, "r");
	fscanf(pid, "%*d %*s %*s %*d %*d %*d %*d %*d %*f %*f %*f %*d %*d %lf %lf", &cpu[0], &cpu[1]);
	
	cpusum = cpu[0] + cpu[1];
	
	array[index] = cpusum;

}




void q_sort(double *array, int ileft, int iright)
{
  int l_hold = ileft;
  int r_hold = iright;
  double pivot = array[ileft];
  double t;
  int tmp;
  while (ileft < iright)
  {
    while ((array[iright] >= pivot) && (ileft < iright))
      iright--;
    if (ileft != iright)
    {
      t = array[ileft]; 
      array[ileft] = array[iright]; 
      array[iright] = t;
      
      tmp = processi[ileft]; 
      processi[ileft] = processi[iright]; 
      processi[iright] = tmp;
      
      ileft++;
    }
    while ((array[ileft] <= pivot) && (ileft < iright))
      ileft++;
    if (ileft != iright)
    {
      t = array[iright]; 
      array[iright] = array[ileft]; 
      array[ileft] = t;
      
      tmp = processi[iright]; 
      processi[iright] = processi[ileft]; 
      processi[ileft] = tmp;
      
      iright--;
    }
  }
  array[ileft] = pivot;
  pivot = ileft;
  ileft = l_hold;
  iright = r_hold;
  if (ileft < pivot)
    q_sort(array, ileft, pivot-1);
  if (iright > pivot)
    q_sort(array, pivot+1, iright);
}

void quickSort(double *array, int array_size)
{
  q_sort(array, 0, array_size - 1);
}



int main()
{
	int i =0;
	trovaProcessi();
	cpuinizio=(double *) malloc(size*sizeof(double));
	cpufine=(double *) malloc(size*sizeof(double));
	while(i<size)
	{
		printf("entrato con valore di i pari a %d\n", i);
		leggiPidStat(i, cpuinizio);
		i++;
	}
	start = CPUvalue(inizio);
	
	sleep(1);
	
	i=0;
	while(i<size)
	{
		printf("entrato con valore di i pari a %d\n", i);
		leggiPidStat(i, cpufine);
		cpufine[i] -= cpuinizio[i]; //ho la differenza di utilizzo cpu dei processi
		i++;
	}
	end = CPUvalue(fine);
	end-= start; //ho la differenza di utilizzo cpu nel tempo
	
	quickSort(cpufine, size);
	
	i=0;
	
	while(i<11)
	{
		printf("%d \t \t \t %lf \n", processi[size-1-i], (cpufine[size-1-i]/end)*100);
		i++;
	}

	//printf("la cpu utilizzata nell'intervallo di tempo \n è pari a %lf jiffies, \n cioè al %lf per cento \n", end-start, ((end-start)/end)*100); //cpu utilizzata e percentuale sul totale
	
	return 0;
}

