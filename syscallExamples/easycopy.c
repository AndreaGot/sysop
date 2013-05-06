#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char * argv[])
{
   char ch, source_file[20], target_file[20];
   FILE *source, *target;
 
   source = fopen(argv[1], "r");
 
   if( source == NULL )
   {
      printf("Press any key to exit...\n");
      exit(EXIT_FAILURE);
   }
 
 
   target = fopen(argv[2], "a");
 
   if( target == NULL )
   {
      fclose(source);
      printf("Press any key to exit...\n");
      exit(EXIT_FAILURE);
   }
 
   while( ( ch = fgetc(source) ) != EOF )
      fputc(ch, target);
 
   printf("File copied successfully.\n");
 
   fclose(source);
   fclose(target);
 
   return 0;
}