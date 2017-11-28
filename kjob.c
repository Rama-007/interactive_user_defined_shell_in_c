#include "Functions.h"

bg * kjob(bg *background, char * args[1024] , int argc)
{
	int pid;
	bg * temp = background;
	int n=atoi(args[1]);
	n--;
	while(temp!=NULL && n--)
			temp=temp->next;
	if(temp!=NULL)
		pid=temp->pid;
	else
		pid=-1;
	if(argc==3)
	{
		if(pid>=0)
		{
			if(killpg(pid,atoi(args[2]))>=0)
			{
				printf("killed [%d] + %s +[%d]\n",atoi(args[1]),temp->name, pid);
				background=removes(pid , background);
				
			}
			else
			{
				perror("Signal not sent!!");
			}
		}
		else
			fprintf(stderr,"No such job number\n");
	}
	else
		fprintf(stderr,"Takes Two Arguments!!\n");
	return background;
}