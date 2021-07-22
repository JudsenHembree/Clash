#include <string.h>
#include <stdio.h>

int main () {
   char str[80] = "This is an example - string\n";
   const char s[3] = " \n";
   char *token;
   
   /* get the first token */
   token = strtok(str, s);
   
   /* walk through other tokens */
   while( token != NULL ) {
      printf( " %s\n", token );
      token = strtok(NULL, s);
   }
   
   return(0);
}

