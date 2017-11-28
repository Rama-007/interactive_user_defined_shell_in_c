

#include "Functions.h"



int echos(int argc,char *args[1024])
{
	int i,j;
	char *name;
	if(argc <=1)
	{
		printf("\n");
	}
	else
	{
		for(i=1;i<argc;i++)
		{
			name=args[i];
			int l=strlen(name),j,k;
			for(j=0;j<l;j++)
			{
				if(name[j]!='\'' && name[j]!='\"')
					printf("%c", name[j]);
			}
			printf(" ");
		}
		printf("\n");
	}
	return 0;
}