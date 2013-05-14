/*
 ============================================================================
 Academic Year : 2012-2013
 Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
 Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
 Name          : plive.c
 Description   : Visualizza gli N processi più "costosi" in termini di utilizzo di tempo CPU
 ============================================================================
 */





#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include "../managelogs/managelogs.h"



double inizio[10], fine[10], start, end;
int *processi;
double *cpuinizio;
double *cpufine;
int dormi = 1;
int size = 0;
				char * wpath;
				char * wexec;
				int wparent;
				FILE * wproc;
int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
	ch = getchar();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch > 48 && ch < 58)
	{
		dormi = ch-48;
	}
	else if(ch == 81 || ch==113)
	{
		exit(1);
	}
	
	
	return 0;
}




double CPUvalue(double * array) 					//restituisce il numero di jiffies utilizzati dalla cpu in un certo momento
{
	double result;
	int i = 0;
	FILE * proc;
	proc = fopen("/proc/stat", "rb");				//apro il file /proc/stat, che contiene i parametri generali della CPU (10 valori corrispondenti a 10 diversi utilizzi)
	printf("file opened \n");
													//legge ogni valore dei vari settori della cpu
	fscanf(proc, "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &array[0], &array[1], &array[2], &array[3], &array[4], &array[5], &array[6], &array[7], &array[8], &array[9]); 
	fclose(proc);
	
	while(i<=9) 									//somma i valori 
	{
		result += array[i];
		i++;
	}
	
	return result;								

}	

void trovaProcessi()								// prendo la lista dei processi attivi nel momento della chiamata	
{
int i = 0;
DIR *dir;
struct dirent *ent;
bool trovato = false;

size = 0;
if ((dir = opendir ("/proc/")) != NULL) 			//apro la cartella /proc/
	{
		while ((ent = readdir (dir)) != NULL) {		//e leggo il suo contenuto
			if(trovato)
			{
    			size++;
    			processi=(int *) realloc(processi,size*sizeof(int));
    			processi[size-1] = atoi(ent->d_name);
    			
    		}
    		if(strcmp(ent->d_name, "self")==0)		//Il file "self" è l'ultimo prima dei processi, nel modo in cui li legge la readdir(), quindi da "self"+1 in poi sono tutti processi
    			trovato = true;
  		}
  		closedir (dir);
  		while(i<size)
  		{
  			//printf("%d \n", processi[i]);			//stampa i processi trovati (inutile, si può togliere)
  			i++;
  		}
  		//printf("Il numero di processi è %d \n", size);
	} 
	else 
	{
  /* could not open directory */
  	perror ("");
	}
	

}


void leggiPidStat(int index, double *array)
{
	FILE * pid;
	double cpu[2];
	double cpusum;
	char * perc;
	perc = malloc(16);								//alloca 16 caratteri ("/proc" + pid + "/stat")
	
	sprintf(perc, "/proc/%d/stat", processi[index]);	//scrive su path il percorso del file (usata altrimenti non riuscivo a convertire un int e.g. "3457" in stringa
	
	if(access(perc, F_OK)==-1)
	{
	cpu[0] = 0; cpu[1] = 0;
	}
	else
	{
	pid = fopen(perc, "r");
	fscanf(pid, "%*d %*s %*s %*d %*d %*d %*d %*d %*f %*f %*f %*d %*d %lf %lf", &cpu[0], &cpu[1]); //prendo solo il 16mo e il 17mo campo (user cpu e kernel cpu, children compresi)
	
	cpusum = cpu[0] + cpu[1];
	fclose(pid);
	}						
	
	array[index] = cpusum;							//scrive nell'array passato, il valore della cpu (sempre in jiffies)
	
}




void q_sort(double *array, int ileft, int iright) //quicksort
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

void quickSort(double *array, int array_size) //chiamata a quicksort
{
  q_sort(array, 0, array_size - 1);
}



int main(int argc, char* argv[])
{

	crealog(argv[0]);
	int i =0;
	int numeroproc;	
	numeroproc = 10;

	while ( (i = getopt(argc, argv, "n")) != -1)
	{
		switch (i) 
		{
			case 'n':
				numeroproc = atoi(argv[2]);
				break;
			default:
				numeroproc = 10;
		}
	}	
	printf("%d \n", numeroproc);
i=0;


	//for(;;)
	//{
		while(!kbhit())
		{
			trovaProcessi();
			cpuinizio=(double *) malloc(size*sizeof(double));	//siccome ho già trovato il numero dei processi, posso allocare 
			cpufine=(double *) malloc(size*sizeof(double));	

			i=0;
																//la memoria degli array che conterranno i cpu jiffies
			while(i<size)
			{
				leggiPidStat(i, cpuinizio);						//legge il file /proc/pid/stat, cercando i valori di cpu
				i++;
			}
			start = CPUvalue(inizio);							//legge il file /proc/stat, estraendo i valori di cpu
			
			
			sleep(dormi);
			
			i=0;
			while(i<size)
			{
				//printf("entrato con valore di i pari a %d\n", i);
				leggiPidStat(i, cpufine);
				cpufine[i] -= cpuinizio[i]; //ho la differenza di utilizzo cpu dei processi (in jiffies
				i++;
			}
			end = CPUvalue(fine);
			end-= start; //ho la differenza di utilizzo cpu nel tempo (in jiffies)
			
			quickSort(cpufine, size);
			
			i=0;
			
			while(i<numeroproc)
			{
				wpath = malloc(17);
				sprintf(wpath, "/proc/%d/stat\0", processi[size-1-i]);
				wproc = fopen(wpath, "r");
				fscanf(wproc, "%*d (%s) %*s %d", wexec, &wparent);
				fclose(wproc);
				free(wpath);
				
				printf("%d \t %s \t %d \t %lf \n", processi[size-1-i],wexec, wparent,(cpufine[size-1-i]/end)*100);
				
				i++;
			}

			free(cpuinizio);
			free(cpufine);
		}
	//}
	return 0;
}

