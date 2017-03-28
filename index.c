#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct _Node
{
	char *word;
	struct _Node left;
	struct _Node right;
	struct _Node head;
}Node;

/*indexDirectory(directoryName, myIndex){
	open directoryName
	while(we havent finished everything in directoryName){
		thing = get something from directoryName
		if(thing is a file){
			indexFile(thing, myIndex);
		}
		else if(thing is a directory)
		{
			indexDirectory(thing, myIndex)
		}
	}
}*/

int main(int argc, char const *argv[])
{
	/*
	--Test if file is a directory or a file--
		if(argv[1]is a file){
			call IndexFile(argv[1], myIndex);
		}
		else if(argv[1] is a directory){
			call IndexDirectory(argv[1], myIndex);
		}


	*/
	struct dirent *something = NULL;
	DIR *directory =opendir("./"); //opening a directory in current working directory

	do
	{
		something=readdir(directory);
		if(something!=NULL)
		{
			if(something->d_type==DT_REG) // regular file
			{
				char *
				//do something
			}	
			else if(something->d_type==DT_DIR)	//	directory
			{
				//do something
			}
		}
	}while(something!=NULL);
	
	




	closedir(directory); //closing the directory
	return 0;
}
