#include <stdio.h>
#include <string.h>


void read_words (FILE *f) {
    char x[1024];
    while (fscanf(f, " %s", x) == 1) {
		if(strcmp(x, "hello")==0)
			printf("ciao");
        puts(x);
    }


 }
 
 int main(int argc, char * argv[]) {
    FILE *ciao;
    ciao = fopen(argv[1], "r");
	 read_words(ciao);
	 return 0;
} 