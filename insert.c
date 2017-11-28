#include "Functions.h"

bg  *insert(char name[1024],pid_t pid ,bg *background)
{
	bg *new=(bg*)malloc(sizeof(bg));
	strcpy(new->name,name);
	new->pid=new->pgid=pid;
	new->next=NULL;
	if(background==NULL)
		return new;
	else
	{
		bg *temp=background;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new;
		return temp;
	}
}