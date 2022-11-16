// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// executort.c file, it contains execution files of the comand input bruh
// github repo link https://github.com/blacklistperformer/shell.git

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

void ls(int argc, char *c[])
{

	DIR *folders;
	struct dirent *file_names;

	// printf("never mind\n");
	folders = opendir(".");

	if (argc == 1)
	{
		// printf("inside folders\n");
		if (folders != NULL)
		{
			// printf("inside folders.....\n");
			while ((file_names = readdir(folders)) != NULL)
			{
				if (!(strcmp(file_names->d_name, ".") == 0 || strcmp(file_names->d_name, "..") == 0))
					printf("%s\n", file_names->d_name);
			}
		}

		closedir(folders);
	}

	if (strcmp(c[1], "-m") == 0)
	{
		printf("\n");

		if (folders != NULL)
		{
			while ((file_names = readdir(folders)) != NULL)
			{
				if (!(strcmp(file_names->d_name, ".") == 0 || strcmp(file_names->d_name, "..") == 0))
					printf("%s,\t", file_names->d_name);
			}

			printf("\n");
		}

		closedir(folders);
	}

	else if (strcmp(c[1], "-1") == 0)
	{
		// printf("inside folders\n");
		if (folders != NULL)
		{
			// printf("inside folders.....\n");
			while ((file_names = readdir(folders)) != NULL)
			{
				if (!(strcmp(file_names->d_name, ".") == 0 || strcmp(file_names->d_name, "..") == 0))
					printf("%s\n", file_names->d_name);
			}
		}
		closedir(folders);
	}
}
