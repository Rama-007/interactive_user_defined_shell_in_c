#include "Functions.h"

bg * fg(bg *background, char * args[1024] , int argc,pid_t shell_pgid ,int shell)
{
	if(argc==2)
	{
		pid_t pid , pgid;
		int status;
		bg * temp =background;
		int n=atoi(args[1]);
		n--;
		while(temp!=NULL && n--)
			temp=temp->next;
		if(temp!=NULL)
			pid=temp->pid;
		else
			pid=-1;
		if(pid>=0)
		{
			fprintf(stderr, "%s\n", temp->name);
			pgid=getpgid(pid);
			tcsetpgrp(shell,pgid);
			if(killpg(pgid,SIGCONT)<0)
					perror("Cannot Continue");
			waitpid(pid,&status,WUNTRACED);
			if(WIFSTOPPED(status))
			{
				fprintf(stderr, "\n[%d]+ stopped %s\n",pgid,temp->name );
			}
			else
			{
				background=removes(pid,background);
			}
			tcsetpgrp(shell,shell_pgid);
		}
		else
			{
				fprintf(stderr, "No such job\n" );
			}
		}
		else
			fprintf(stderr, "Takes only one argument\n" );

		return background;
}