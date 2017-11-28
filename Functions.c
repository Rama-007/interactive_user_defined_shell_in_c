

#include "Functions.h"



void display_required(char *directory)
{
	char *username=getenv("LOGNAME");
	char name[1024];
	char *dir=getcwd(NULL,0);
	gethostname(name,1024);
	int len=strlen(directory);
	if(strstr(dir,directory))
		strcpy(dir,dir+len);
	fprintf(stderr, "<%s@%s:~%s>",username,name,dir);
	return;
}