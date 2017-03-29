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

/*This is the node that will hold the fileNames and 
number of occurrences in those files for each word*/
typedef struct _fNode{
	char *fileName;
	int numberOfOccurrences;
	struct _fNode *next;
}fileNode;

/*Word node contans the word string, left and right pointers, 
and a head pointer to the first node in the linked list of file names that
the word appears in.*/
typedef struct _wNode
{
	char *word;
	struct _Node *left;
	struct _Node *right;
	struct _fNode *head;
}wordNode;

char* copyFileInput(FILE *d_file);
char* copyFileInput(FILE *d_file)
{
    fseek(d_file, 0, SEEK_END);
    long int size = ftell(d_file);
    rewind(d_file);
    char *string = calloc((size+1), sizeof(char));
    fread(string,1,size,d_file);
    return string;
}
char* stringCopier(char* start, char* end, int count);
char* stringCopier(char* start, char* end, int count){
	char* returnChar = (char*)calloc((count+1),sizeof(char));
	int i = 0;
	while(start!=end){
				returnChar[i]=*start;
				start++;
				i++;
			}
			return returnChar;	
}

int is_dot_or_dot_dot(char const* name);
int is_dot_or_dot_dot(char const* name)
{
   return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 );
}

void listdir(char const* dirname);
void listdir(char const* dirname)
{
   char* subdir;
   DIR* dirp = opendir(dirname);
   struct dirent *curr_ent;

   if ( dirp == NULL )
   {
      return;
   }

   while ((curr_ent = readdir(dirp)) != NULL )
   { 
   		// Allocate memory for the subdirectory.
        // 1 additional for the '/' and the second additional for '\0'.
         subdir = malloc(strlen(dirname) + strlen(curr_ent->d_name) + 2);
         // Flesh out the subdirectory name.
         strcpy(subdir, dirname);
         strcat(subdir, "/");
         strcat(subdir, curr_ent->d_name);
   	if(curr_ent->d_type==DT_REG)
   	{
   		//open file
   		FILE * textFile =fopen(subdir,"r");
   		if(textFile==NULL)
   		{
   			printf("Error: %d (%s)\n", errno, strerror(errno));
   		}
   		//get string of characters from file
   		char *copiedString=copyFileInput(textFile);
   		//parse words from file
   		char *start = copiedString;
   		char *end = copiedString;
   		while(*end!='\0')
   		{
   			int count = 0;
   			while(isalpha(*end))
				{
				count++;
				end++;
				}
					
					if(count>0&&(!isalpha(*end)))
					{
						char* temp = stringCopier(start,end,count);
						printf("%s is in file: %s\n",temp,curr_ent->d_name);
						//Call BST function here
					}
					if(!*end)
					{
						break;
					}
			end++;
			start=end;
   		}
   	}

      // Traverse sub-directories excluding ignoring . and ..
      if (curr_ent->d_type == DT_DIR && !(is_dot_or_dot_dot(curr_ent->d_name)) )
      {
         // List the contents of the subdirectory.
         listdir(subdir);
         // Free the allocated memory.
         free(subdir);
      }
   }
   // Close the directory
   closedir(dirp);
}

int main(int argc, char const *argv[])
{
	//create word BST root
	wordNode* root=(wordNode*)malloc(1*sizeof(wordNode));
	//set the word to null to start with
	root->word=NULL;

	struct dirent *something = NULL;
	int status;
    struct stat st_buf;
	status = stat (argv[1], &st_buf);

    if (status != 0) {
        printf ("Error, errno = %d\n", errno);
        return 1;
    }
    //Case where passed in parameter is a file and not a directory
    if (S_ISREG (st_buf.st_mode)) {
    	//Perform indexFile function here
        printf ("%s is a regular file.\n", argv[1]);
    }
    if (S_ISDIR (st_buf.st_mode)) {
        printf ("%s is a directory.\n", argv[1]);
        //enter the recursive function
		listdir(argv[1]);
		return 0;
    }

	// int result = 0;
	// int nameLen = 0;
	// char *invertedIndexFileName = argv[1]; //get the file name for the inverted index file
	// const char *startingDirectory = argv[1];
	// int fd = open("./invertedIndexFileName",O_RDWR|O_CREAT,S_IWUSR|S_IRUSR);
}
