#include "Functions.h"


bg *removes(pid_t pid, bg *background)
{
	if(background!=NULL)
	{
		bg*temp=background;
		if(background->pid==pid)
		{
			background=background->next;
			free(temp);
			return background;
		}	
		else
		{
			bg *temp2;
			while(temp!=NULL && temp->pid!=pid)
			{
				temp2=temp;
				temp=temp->next;
			}
			if(temp!=NULL)
			{
				temp2->next=temp->next;
				free(temp);
			}
			return temp2;
		
		}
	}
}
