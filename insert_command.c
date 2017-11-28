
 
#include "Functions.h"

list * insert_command(int i,char *args[1024],char *inputfile, char *outputfile)
{
	int j;
	list *new=(list*)malloc(sizeof(list));
	for(int j=0;j<i;j++)
	{
		new->name[j]=(char*)malloc(1024);
		strcpy(new->name[j],args[j]);
	}
	new->name[j]=NULL;
	if(inputfile==NULL)
	{
		new->inputfile=NULL;
	}
	else
	{
		new->inputfile=(char*)malloc(1024);
		strcpy(new->inputfile,inputfile);
	}
	if(outputfile==NULL)
	{
		new->outputfile=NULL;
	}
	else
	{
		new->outputfile=(char*)malloc(1024);
		strcpy(new->outputfile,outputfile);
	}
	new->argc=i;
	new->next=NULL;
	return new;
}