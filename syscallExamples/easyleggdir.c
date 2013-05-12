#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


DIR *dir;
struct dirent *ent;
bool trovato = false;

int main() {
if ((dir = opendir ("/proc/")) != NULL) 
	{
		while ((ent = readdir (dir)) != NULL) {
			if(trovato)
    			printf ("%s\n", ent->d_name);
    		if(strcmp(ent->d_name, "self")==0)
    			trovato = true;
  		}
  		closedir (dir);
	} 
	else 
	{
  /* could not open directory */
  	perror ("");
  	return 1;
	}
	
	return 0;
}
