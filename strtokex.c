/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="<tag attr1=\"value1\" attr2=\"value2\" attr3=\"value3\" >";
  char * pch;
  char str2[256] = "";
  char line[256];
  strcpy(line, str);
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," =\"");
  while (pch != NULL)
  {
    strcpy(str2,pch);
    strcat(str2,"=");
    if (strstr(line,str2)) {
      printf ("name=%s\n",pch);
    } else {
      strcpy(str2,"\"");
      strcat(str2,pch);
      if (strstr(line,str2)) {
        printf ("value=%s\n",pch);
      } 
    }
    pch = strtok(NULL, " =\"");
  }
  return 0;
}
