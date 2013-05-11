#include <stdio.h>
#include <ftw.h>

double inizio[10], fine[10], start, end;
int *array;

void CPUvalue()
{
	FILE * proc;
	proc = fopen("/proc/stat", "rb");
	printf("file opened \n");
	fscanf(proc, "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &inizio[0], &inizio[1], &inizio[2], &inizio[3], &inizio[4], &inizio[5], &inizio[6], &inizio[7], &inizio[8], &inizio[9]); //legge ogni valore dei vari settori della cpu
	fclose(proc);
	
	sleep(10); //aspetta
	
	proc = fopen("/proc/stat", "rb");
	fscanf(proc, "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &fine[0], &fine[1], &fine[2], &fine[3], &fine[4], &fine[5], &fine[6], &fine[7], &fine[8], &fine[9]);
	fclose(proc); //legge ogni valore dei vari settori della cpu
	
	int i =0;
	while(i<=9) //somma i valori 
	{
		start += inizio[i];
		end += fine[i];
		
		printf(" %lf \t \t \t  %lf \n", inizio[i], fine[i]);
		i++;
	}
	
	printf("la cpu utilizzata nell'intervallo di tempo \n è pari a %lf jiffies, \n cioè al %lf per cento \n", end-start, ((end-start)/end)*100); //cpu utilizzata e percentuale sul totale

}	

int list(const char *name, const struct stat *status, int type) {
	int i = 0;
	if(type == FTW_NS)
		return 0;
	
	if(type == FTW_D && strcmp(".", name) != 0 && strcmp("..", name) != 0)
	{
		//salva il nome della cartella in un array solo se è un numero
	}
	else if(type == FTW_F  && strcmp(name+ lung,"/.DS_Store")!=0)
	{
		
	}	
	
	return 0;
}	

int main()
{
	ftw(param[i-1], list, 1); //forse è meglio la opendir
	
	//prendi l'array delle cartelle
	//scorrilo aprendole tutte
	//leggi il file /proc/pid/stat
	//salva in un array il valore relativo alla cpu
	//chiudi il file
	
	//trova valore cpu totale
	
	
	//aspetta x secondi
	
	
	//riapri tutti i file
	//leggi il valore della cpu e sottrai il valore precedentemente salvato
	
	//trova valore cpu totale
	//crea variabile CPUdiff =  cputotalefine - cputotaleinizio
	
	// crea un array con PIDdiff/CPUdiff
	//ordina l'array mantenendo anche i nomi dei PID
	
	
	
	//stampa i primi 10 PID

}

