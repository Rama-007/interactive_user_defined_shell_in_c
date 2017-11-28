#include "Functions.h"

bg * killallbg(bg * background)
{
	int flag=0;
	bg *temp = background;
	while(temp!=NULL)
	{
		if(killpg(temp->pid,9)<0)
		{
			flag=1;
			perror("Error Killing Process");
		}
		temp=temp->next;
	}
	if(flag==0)
		fprintf(stderr, "Killed all background processes\n" );
	background=temp;
	return background;
}