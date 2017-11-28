#include "Functions.h"

void jobs(bg *background)
{
	int i=1;
	while(background!=NULL)
	{
		fprintf(stdout, "[%d] %s [%d]\n",i++,background->name,background->pid );
		background=background->next;
	}
	return;
}