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
				char * wexec;
				int wparent;
				FILE * wproc;

//questa funzione modifica le proprietà del terminale in modo da fargli accettare i caratteri inseriti. 
//Alla fine della routine controlal se i caratteri corrispondono a un dato pattern, e ripristina il settaggio del terminale
int kbhit(void) 
{		
	//serve per non inserire in un buffer l'input
	struct termios oldt, newt;
	int ch;
	int oldf;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); 
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); //cambia immediatamente la configurazione
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
	ch = getchar();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch > 48 && ch < 58) //se il carattere inserito e'compreso tra 0 e 9
	{
		dormi = ch-48;
		scrivilog("E' stato premuto un numero, setto la frequenza di aggiornamento a %d secondi \n", ch-48);
	}
	else if(ch == 81 || ch==113) // se il carattere inserito e' q o Q
	{
		scrivilog("E' stato premuto il tasto 'q' o 'Q'. Esco dal programma \n");
		exit(1);
	}
	
	
	return 0;
}



//funzione che calcola l'utilizzo della cpu inun dato momento
double CPUvalue(double * array) 					//restituisce il numero di jiffies utilizzati dalla cpu in un certo momento
{
	double result;
	int i = 0;
	FILE * proc;
	proc = fopen("/proc/stat", "rb");				//apro il file /proc/stat, che contiene i parametri generali della CPU (10 valori corrispondenti a 10 diversi utilizzi)
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
// prendo la lista dei processi attivi nel momento della chiamata	
void trovaProcessi()								
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
    		if(strcmp(ent->d_name, "self")==0)//Il file "self" è l'ultimo prima dei processi, nel modo in cui li legge la readdir(), quindi da "self"+1 in poi sono tutti processi
    			trovato = true;
  		}
  		closedir (dir);
	} 
	else 
	{
		/* could not open directory */
		perror ("");
	}
	

}

//legge il file /proc/pid/stat e legge i dati relativi all'utilizzo della CPU
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
		fscanf(pid, "%*d %*s %*s %*d %*d %*d %*d %*d %*f %*f %*f %*d %*d %lf %lf", &cpu[0], &cpu[1]); //prendo solo il 14mo e il 15mo campo (user cpu e kernel cpu, children esclusi)
	
		cpusum = cpu[0] + cpu[1];
		fclose(pid);
	}						
	
	array[index] = cpusum;							//scrive nell'array passato, il valore della cpu (sempre in jiffies)
	
}



//quick sort per ordinare le differenze di utilizzo cpu. Ordina anche i nomi
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


//dato un pid, restituisce il suo nome esteso
char * trovaNome(int pid)
{
	char * perc;
	char * nome;
	perc = malloc(16);								//alloca 16 caratteri ("/proc" + pid + "/stat")
	FILE * proc;
	
	sprintf(perc, "/proc/%d/stat\0", pid);
	
	if(access(perc, F_OK)==-1)
	{
		nome = NULL;
	}
	else
	{
		proc = fopen(perc, "rb");
		fscanf(proc, "%*d %s", nome); 
		fclose(proc);
	
	return nome;
	}			
	
	return NULL;			
	
}


//dato un pid, restituisce il pid del padre
int trovaPadre(int pid)
{
	char * perc;
	int padre = 0;
	perc = malloc(16);							//alloca 16 caratteri ("/proc" + pid + "/stat")
	FILE * proc;
	
	sprintf(perc, "/proc/%d/stat\0", pid);
	
	if(access(perc, F_OK)==-1)
	{
	padre = 0;
	}
	else
	{
	proc = fopen(perc, "r");
	fscanf(proc, "%*d %*s %*s %d", &padre); 
	fclose(proc);
	
	return padre;
	}						
	return 123;
}




int main(int argc, char* argv[])
{

	crealog(argv[0]);
	scrivilog("Inizia l'esecuzione del programma \n");
	int i =0;
	int j = 0;
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
    i=0;


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
			j=0;
			while(j<31)
			{		
				printf("\n");
				j++;
			}
			printf("PID \t padre \t %CPU \t nome \n \n")
			while(i<numeroproc)
			{
				wparent = trovaPadre(processi[size-1-i]);
				wexec = trovaNome(processi[size-1-i]);
				
				
				printf("%d \t %d \t %lf \t %s \n", processi[size-1-i], wparent,(cpufine[size-1-i]/end)*100, wexec);
				
				i++;
			}

			free(cpuinizio);
			free(cpufine);
		}
	//}
	return 0;
}

