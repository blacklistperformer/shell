// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// executort.c file, it contains execution files of the comand input bruh
// github repo link https://github.com/blacklistperformer/shell.git

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void my_mkdir(int argc, char *c[])
{

	if(argc==1)
	{
		printf("Enter a name\n");
	}


	if(strcmp(c[1],"-v")==0)
	{
		DIR* dir =opendir(c[2]);
		if(dir)
		{
			printf("Directory already exists\n");
			closedir(dir);
		}

		else
		{
			mkdir((const char *)c[2], S_IRWXU);
			printf("Directory has been created \n" );
		}
	}
	else if(strcmp(c[1],"-m")==0)
	{
		int mode =atoi(c[2]);

		DIR* dir =opendir(c[3]);

		if(dir)
		{
			printf("Directory already exists\n");
			closedir(dir);
		}

		else
		{
			mkdir((const char *)c[3], mode);

			printf("Directory has been created \n" );
		}
	}

	else
	{
		DIR* dir =opendir(c[1]);

		if(dir)
		{
			printf("Directory already exists\n");
			closedir(dir);
		}

		else
		{
			mkdir((const char *)c[1], S_IRWXU);
		}
	}
}
