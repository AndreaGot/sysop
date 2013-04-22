#include <unistd.h>
#include <stdio.h>

main()
{
	char cwd[1024];
	
	if (chdir("/tmp") != 0)
		perror("chdir() error()");
	else
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("getcwd() error");
		else
		{
			printf("current working directory is: %s\n", cwd);
			
	}
}
