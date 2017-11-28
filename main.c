

#include "Functions.h"

 int bgflag=0,shell;

pid_t shell_pgid;
pid_t child_pid;

char *directory;
char *inputfile , *outputfile;

char *args[100000024];



list * commands;


 bg * background;



void shell_initialization()
{
	bgflag=0;
	shell=STDIN_FILENO;
	if(isatty(STDIN_FILENO))
	{
		while(tcgetpgrp(STDIN_FILENO)!=(shell_pgid=getpgrp()))
			kill( shell_pgid, SIGTTIN);
		signal (SIGINT, SIG_IGN);
      	signal (SIGQUIT, SIG_IGN);
      	signal (SIGTSTP, SIG_IGN);
      	signal (SIGTTIN, SIG_IGN);
      	signal (SIGTTOU, SIG_IGN);
      	signal (SIGCHLD, SIG_IGN);
      	if(setpgid(shell_pgid,shell_pgid)<0)
      	{
      		perror("Can't put a shell as a member of it's own process group");
      		exit(1);
      	}
      	tcsetpgrp(STDIN_FILENO,shell_pgid);
	}
}



void execute(int argc ,int flag) 	
{
	int p=0;
	if(strcmp("echo",args[0])==0)
	{
		p=echos(argc,args);
	}
	else if(strcmp("quit",args[0])==0)
	{
		exit(0);
	}
	else if(strcmp("cd",args[0])==0)
	{
		changedir(argc,directory,args);
	}
	else if(strcmp("pwd",args[0])==0)
		pwds();
	else if(strcmp("xjobs",args[0])==0)
	{
		jobs(background);
		fflush(stdout);
	}
	else if(strcmp("fg",args[0])==0)
	{
		background=fg(background,args,argc,shell_pgid,shell);
	}
	else if(strcmp("kjob",args[0])==0)
	{
		background=kjob(background,args,argc);
	}
	else if(strcmp("killallbg",args[0])==0)
	{
		background=killallbg(background);
	}
	else if(strcmp("pinfo",args[0])==0)
	{
		FILE *fp;
		pid_t pid;
		char buffer1[1024],buffer2[1024];
		long long unsigned size;
		char name[1024],state;
		if(argc==1)
		{
			sprintf(buffer1,"/proc/%d/stat",getpid());
			sprintf(buffer2,"/proc/%d/exe",getpid());

		}
		else
		{
			sprintf(buffer1,"/proc/%d/stat",args[1]);
			sprintf(buffer2,"/proc/%d/exe",args[1]);
		}	
		if((fp=fopen(buffer1,"r"))!=NULL)
		{
			fscanf(fp,"%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %llu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d",&pid,&state,&size);												
			printf("pid -- %d\nprocess status -- %c\nmemory -- %llu\n",pid,state,size);
			readlink(buffer2,name,1024);
			fclose(fp);
			char temp[4]="~";
			if(strstr(name,directory))
			{
				strcat(temp,name+strlen(directory));
				printf("Executable Path -- %s\n", temp);
			}
			else
			{
				printf("Executable Path -- %s\n", name);
			}
		}
		else
			perror("No Such Process Exists");	
	}	
	else if(p!=1)
	{
		pid_t pid;
		int in,out;
		pid=fork();
		if(pid<0)
			perror("Child not created");

		else if(pid==0)
		{
			int runcommand;
			prctl(PR_SET_PDEATHSIG,SIGHUP);
			setpgid(getpid(),getpid());
			if(outputfile!=NULL)
			{
				if(flag!=1)
					out=open(outputfile,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				else
					out=open(outputfile,O_APPEND | O_WRONLY  | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				dup2(out,1);
				close(out);
			}
			if(inputfile!=NULL)
			{
				in=open(inputfile,O_RDONLY);
				dup2(in,0);
				close(in);
			}
			if(bgflag==0)
			{
				tcsetpgrp(shell,getpid());
			}
			signal (SIGINT, SIG_DFL);
			signal (SIGQUIT, SIG_DFL);
			signal (SIGTSTP, SIG_DFL);
			signal (SIGTTIN, SIG_DFL);
			signal (SIGTTOU, SIG_DFL);
			signal (SIGCHLD, SIG_DFL);
			runcommand=execvp(args[0],args);
			if(runcommand<0)
			{
				perror("Error Executing command");
				exit(-1);         
			}
			exit(0);
		}
		if(bgflag==0)
		{
			tcsetpgrp(shell,pid);
			background=insert(args[0],pid,background);
			child_pid=pid;
			int status;
			waitpid(pid,&status,WUNTRACED);
			if(WIFSTOPPED(status))
			{
				fprintf(stderr, "\n[%d]+stopped %s\n",child_pid,args[0]);
			}
			else
			{
				background=removes(pid,background);
			}
			tcsetpgrp(shell,shell_pgid);
		}
		else
		{
			background=insert(args[0],pid,background);
		}
	}
	fflush(stdout);
	return;
}


void piped_execute(int num,char *input,int flag)
{
	int i,j=num-1,k,pgid,com=0;
	k=2*(num-1);
	int pipes[k];
	for(i=0;j--;i=i+2)
	{
		if(pipe(pipes+i)<0)
		{
			perror("Piping Error");
			return;
		}
	}
	list *temp=commands;
	while(temp!=NULL)
	{
		int pid=fork();
		int in,out;
		if(temp->next==NULL)
			background= insert(input,pid,background);
		if(com==0 && pid!=0)
			pgid=pid;
		if(pid!=0)
			setpgid(pid,pgid);
		if(pid==0)
		{
			signal (SIGINT, SIG_DFL);
			signal (SIGQUIT, SIG_DFL);
			signal (SIGTSTP, SIG_DFL);
			signal (SIGTTIN, SIG_DFL);
			signal (SIGTTOU, SIG_DFL);
			signal (SIGCHLD, SIG_DFL);
			if(temp->outputfile!=NULL)
			{
				if(flag!=1)
					out=open(outputfile,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				else
					out=open(outputfile,O_APPEND | O_WRONLY  | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				dup2(out,1);
				close(out);
			}
			else if(temp->next!=NULL)
			{
				if((dup2(pipes[2*com +1],1))<0)
					perror("Error Occured");
			}
			if(temp->inputfile!=NULL)
			{
				in=open(inputfile,O_RDONLY);
				dup2(in,0);
				close(in);
			}
			else if(com!=0)
			{
				if((dup2(pipes[2*(com - 1)],0))<0)
					perror("Error Occured");
			}
			for(i=0;i<k;i++)
			{
				close(pipes[i]);
			}
			if((execvp(temp->name[0],temp->name))<0)
			{
				perror("Cannot execute");
				exit(-1);
			}
		}
		else if(pid<0)
		{
			perror("Couldnot fork Child");
			return;
		}
		temp=temp->next;
		if(temp!=NULL)
			com++;
	}
	for(i=0;i<k;i++)
		close(pipes[i]);
	if(bgflag==0)
	{
		tcsetpgrp(shell,pgid);
		for(i=0;i<num;i++)
		{
			int pid1,status;
			pid1=waitpid(-pgid,&status,WUNTRACED);
			if(WIFSTOPPED(status))
				killpg(pgid,SIGSTOP);
			else
				background=removes(pid1,background);

		}
		tcsetpgrp(shell,shell_pgid);
	}
}

void insert_commands(int i)
{
	int j;
	list *new=(list*)malloc(sizeof(list));
	for(j=0;j<i;j++)
	{
		new->name[j]=(char*)malloc(10024);
		strcpy(new->name[j],args[j]);
	}
	new->name[j]=NULL;
	if(inputfile==NULL)
	{
		new->inputfile=NULL;
	}
	else
	{
		new->inputfile=(char*)malloc(10024);
		strcpy(new->inputfile,inputfile);
	}
	if(outputfile==NULL)
	{
		new->outputfile=NULL;
	}
	else
	{
		new->outputfile=(char*)malloc(10024);
		strcpy(new->outputfile,outputfile);
		
	}
	new->argc=i;
	new->next=NULL;
	if(commands!=NULL)
	{
		list *temp=commands;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new;
		commands=temp;
	}
	else
	{
		commands=new;
	}
}

void parser(char input[1000000])
{
	int i,j,flag,k;
	char *a,*b,*c;
	char *str1,*str2, *str3;
	for(str2=input;;str2=NULL)
	{
		char *tokencopy;
		int m;
		char *token=strtok_r(str2,";",&a);
		if(token==NULL)
			break;
		tokencopy=malloc((strlen(token))*sizeof(char));
		for(m=0;m<strlen(token);m++)
			tokencopy[m]=token[m];
		char *temp = token;
		for(str3=temp,j=0;;str3=NULL,j++)
		{
			char *token2;
			token2=strtok_r(str3,"|",&c);
			if(token2==NULL)
				break;
			char *temp2=token2;
			flag=0;
			for(str1=temp2,i=0;;str1=NULL,i++)
			{
				args[i]=strtok_r(str1," \t",&b);
				if(args[i]!=NULL)
					if(strcmp("&",args[i])==0)
					{
						bgflag=1;
						args[i]=NULL;
					}
				if(args[i]==NULL)
				{
					break;
				}
				if(strcmp(args[i],"<")==0)
				{
					if((inputfile =strtok_r(NULL," \t",&b))==NULL)
					{
						perror("No input file given!");
						return;
					}
					args[i--]=NULL;
				}
				else if(strcmp(args[i],">")==0)
				{
					if((outputfile=strtok_r(NULL," \t",&b))==NULL)
					{
						perror("No output file given!");
						return;
					}
					args[i--]=NULL;
				}
				else if(strcmp(args[i],">>")==0)
				{
					if((outputfile=strtok_r(NULL," \t",&b))==NULL)
					{
						perror("No output file given!");
						return;
					}
					else
						flag=1;
					args[i--]=NULL;
				}
			}
			if(strcmp(args[i-1],"")==0)
			{
				args[i-1]=NULL;
				i--;
			}
			for(k=1;k<i;k++)
			{
				int p=0,q;
				if(args[k]!=NULL)
				{
					p=strlen(args[k]);
				}
				if(args[k][0]=='*')
				{
					for(q=1;q<p;q++)
					{
						args[k][q-1]=args[k][q];
					}
					args[k][q-1]=NULL;
				}
				else if((args[k][0]=='\"' && args[k][p-1]=='\"')||(args[k][0]=='\'' && args[k][p-1]=='\''))
				{
					for(q=1;q<p-1;q++)
					{
						args[k][q-1]=args[k][q];
					}
					args[k][q-1]=NULL;	
					args[k][q]=NULL;

				}
			}
			insert_commands(i);
		}
		if(j==1)
			execute(commands->argc,flag);
		else
			piped_execute(j,tokencopy,flag);
		bgflag=0;
		inputfile=outputfile=NULL;
	}
	return;
}



void sig(int signal)
{
	if(signal==SIGINT)
	{
		fprintf(stderr, "\n");
		display_required(directory);
	}
	else if(signal == SIGCHLD)
	{
		int status;
		pid_t processid;
		while((processid=waitpid(-1,&status,WNOHANG))>0)
		{
			if(processid!=-1 && processid!=0)
			{
				bg *temp=background;
				while(temp!=NULL && temp->pid!=processid)
					temp=temp->next;
				if(WIFEXITED(status))
				{
					if(temp!=NULL)
					{
						fprintf(stderr,"%s with pid %d exited normally\n",temp->name,processid);
						background=removes(processid,background);	
					}
				}
			}
		}
	}
}


int main()
{
	char input[1000000];
	shell_initialization();
	directory=getcwd(NULL,0);
	commands=NULL;
	while(1)
	{
		if((signal(SIGINT,sig)==SIG_ERR)||(signal(SIGCHLD,sig)==SIG_ERR))
			perror("No signal!!");
		display_required(directory);
		char c=getc(stdin);
		while(1)
		{
			if(!isspace(c))
				break;
			if(c=='\n')
				break;
			c=getc(stdin);
		}	
		if(c=='\n')
		{
			fflush(stdout);
			continue;
		}
		if(c==EOF)
		{
			fprintf(stderr, "\n");
			break;
		}
		ungetc(c,stdin);
		fgets(input,1000000,stdin);
		int length=strlen(input);
		while(isspace(input[length-1]))
			input[length-1]=NULL;
		length=strlen(input);
		if(input[length-1]=='&')
		{
			bgflag=1;
		}
		length=strlen(input);
		while(isspace(input[length-1]))
			input[length-1]=NULL;

		parser(input);
		bgflag=0;
		commands=NULL;
	}
	return 0;
}
