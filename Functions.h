#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include<unistd.h>
#include<termios.h>
#include<signal.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include <sys/prctl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>


typedef struct bg
{
	char name[1024];
	pid_t pid,pgid;
	struct bg *next;
}bg;



typedef struct list
{
	char *name[1024];
	char *inputfile , *outputfile; 
	struct list *next;
	int argc;
}list;

void display_required(char *directory);

list *insert_command(int i,char *args[1024],char *inputfile, char *outputfile);

void pwds();

bg  *insert(char name[1024],pid_t pid ,bg *background);


void changedir(int argc,char *directory,char *args[1024]);


bg *removes(pid_t pid, bg *background);

void jobs(bg *background);
bg * fg(bg *background, char * args[1024] , int argc,pid_t shell_pgid ,int shell);

bg * kjob(bg *background, char * args[1024] , int argc);

bg * killallbg(bg * background);


#endif
