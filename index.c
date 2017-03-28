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

// typedef struct _Node
// {
// 	char *word;
// 	struct* _Node left;
// 	struct* _Node right;
// 	struct* _Node head;
// }Node;

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

int recursive(DIR * d, struct dirent * s);
int recursive(DIR * d, struct dirent * s)
{
	do
	{
		if(s!=NULL)
		{
			if(s->d_type==DT_REG) // regular file
			{
				printf("%s\n", "found a file!");
				printf("%s\n",s->d_name);
				return 0;
			}	
			else if(s->d_type==DT_DIR)	//	directory
			{
				printf("%s\n", "found a directory!");
				printf("%s\n",s->d_name);
				s=readdir(d);
				// d=opendir(s->d_name);
				recursive(d,s);
				closedir(d);
				return 0;
			}
			else
			{
				printf("%s\n", "Found something that is not a directory or a file. Exiting indexer.");
			}


			// nameLen = strlen(d_name);
			// result = write(fd, something->d_name, nameLen);
		}
	}while(s!=NULL);
	
	closedir(d); //closing the directory
	return 0;
}
int main(int argc, char const *argv[])
{
	struct dirent *something = NULL;
	int status;
    struct stat st_buf;
	status = stat (argv[1], &st_buf);

    if (status != 0) {
        printf ("Error, errno = %d\n", errno);
        return 1;
    }

    if (S_ISREG (st_buf.st_mode)) {
    	//Perform indexFile function
        printf ("%s is a regular file.\n", argv[1]);
    }
    if (S_ISDIR (st_buf.st_mode)) {
        printf ("%s is a directory.\n", argv[1]);
        DIR *directory =opendir(argv[1]); //opening a directory in current working directory
			if(directory==NULL)
			{
				printf("%s\n", "Argument is not a directory. Exiting indexer.");

			}
		something = readdir(directory);
		recursive(directory, something);
		return 0;
    }

	// int result = 0;
	// int nameLen = 0;
	// char *invertedIndexFileName = argv[1]; //get the file name for the inverted index file
	// const char *startingDirectory = argv[1];
	// int fd = open("./invertedIndexFileName",O_RDWR|O_CREAT,S_IWUSR|S_IRUSR);
}
