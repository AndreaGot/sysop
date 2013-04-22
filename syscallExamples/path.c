#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	printf("%s\n", getcwd(NULL, 0));
	printf("%i\n", chdir(argv[0]));
	printf("%s\n", getcwd(NULL, 0));
}