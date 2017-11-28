#include "Functions.h"


void changedir(int argc,char *directory,char *args[1024])
{
	char path[1024];
	if(argc<2)
	{
		if(chdir(directory)<0)
		{
			perror("Error going home");
			return;
		}
	}
	else
	{
		if(args[1][0]=='~')
		{
			strcpy(path,directory);
			strcat(path,args[1]+1);
			if(chdir(path)<0)
			{
				perror("No such path exists");
				return;
			}
		}
	
		else if(args[1][0]=='/')
		{
			getcwd(path,1024);
			strcat(path,'/');
			strcat(path,args[1]);
			if(chdir(path)<0)
			{
				perror("No such path exists");
				return;
			}
		}
		else
		{
			if(chdir(args[1])<0)
			{
				perror("No such path exists");
				return;
			}
		}
		return;
	}
}