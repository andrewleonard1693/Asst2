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
#define MIN(a,b) (((a)<(b))?(a):(b))


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
	struct _wNode *left;
	struct _wNode *right;
	struct _fNode *head;
}wordNode;

void update(char *token,wordNode* tree,char* fileName)
{
	wordNode* ptr = NULL;
	fileNode* fptr = NULL;
	ptr = tree;

	/*CASE: 1st word inputed into the tree*/
	if(tree->word == NULL){
		tree->word = token;
		tree->head = (fileNode*)malloc(sizeof(fileNode));
		fptr=tree->head;
		fptr->next = NULL;
		fptr->fileName = fileName;
		fptr->numberOfOccurrences++;
		return;
	}
	/*cycle through BST*/
	while(ptr != NULL){
		if(strncmp(ptr->word,token,MIN(strlen(ptr->word),strlen(token))) == 0){
			if(strlen(ptr->word)>strlen(token))
			{
				if(ptr->left == NULL){
				//make new WordNode
				ptr->left = (wordNode*)malloc(sizeof(wordNode));
				ptr = ptr->left;
				ptr->word = token;
				ptr->right = NULL;
				ptr->left = NULL;
				//make new fileNode
				ptr->head = (fileNode*)malloc(sizeof(fileNode));
				fptr = ptr->head;
				fptr->next = NULL;
				fptr->fileName = fileName;
				fptr->numberOfOccurrences++;
				fptr = NULL;
				return;
			}
				ptr=ptr->left;
			}else if(strlen(ptr->word)<strlen(token))
			{
				if(ptr->right == NULL){
				//make new wordNode
				ptr->right = (wordNode*)malloc(sizeof(wordNode));
				ptr = ptr->right;
				ptr->word = token;
				ptr->right = NULL;
				ptr->left = NULL;
				//make new fileNode
				ptr->head = (fileNode*)malloc(sizeof(fileNode));
				fptr = ptr->head;
				fptr->next = NULL;
				fptr->fileName = fileName;
				fptr->numberOfOccurrences++;
				fptr = NULL;
				return;

				}
				ptr=ptr->right;
			}
			//update occ
			fptr = ptr->head;
			while(fptr != NULL){
				if(fptr->fileName == fileName){
					fptr->numberOfOccurrences++;
					return;
				}
				fptr = fptr->next;
			}
			//new fileName
			fileNode* temp = ptr->head;
			ptr->head = (fileNode*)malloc(sizeof(fileNode));
			fptr = ptr->head;
			fptr->next = temp;
			fptr->fileName = fileName;
			fptr->numberOfOccurrences++;
			return;
		}else if(strncmp(ptr->word,token,MIN(strlen(ptr->word),strlen(token)))>0){
			//move left
			if(ptr->left == NULL){
				//make new WordNode
				ptr->left = (wordNode*)malloc(sizeof(wordNode));
				ptr = ptr->left;
				ptr->word = token;
				ptr->right = NULL;
				ptr->left = NULL;
				//make new fileNode
				ptr->head = (fileNode*)malloc(sizeof(fileNode));
				fptr = ptr->head;
				fptr->next = NULL;
				fptr->fileName = fileName;
				fptr->numberOfOccurrences++;
				fptr = NULL;
				return;
			}
			ptr = ptr->left;
		}else{
			//move right
			if(ptr->right == NULL){
				//make new wordNode
				ptr->right = (wordNode*)malloc(sizeof(wordNode));
				ptr = ptr->right;
				ptr->word = token;
				ptr->right = NULL;
				ptr->left = NULL;
				//make new fileNode
				ptr->head = (fileNode*)malloc(sizeof(fileNode));
				fptr = ptr->head;
				fptr->next = NULL;
				fptr->fileName = fileName;
				fptr->numberOfOccurrences++;
				fptr = NULL;
				return;
			}
			ptr = ptr->right;
		}
	}
	return;
}
void swap(fileNode *a, fileNode *b);
void swap(fileNode *a, fileNode *b)
{
    int temp = a->numberOfOccurrences;
    a->numberOfOccurrences = b->numberOfOccurrences;
    b->numberOfOccurrences = temp;

    int len = strlen(a->fileName);
    char *tempStr;
    tempStr= a->fileName;
    a->fileName = b->fileName;
    b->fileName= tempStr;
}
void bubbleSort(fileNode *start);
void bubbleSort(fileNode *start)
{
    int swapped, i;
    fileNode *ptr1;
    fileNode *lptr = NULL;
 
    /* Checking for empty list */
    if (ptr1 == NULL)
        return;
 
    do
    {
        swapped = 0;
        ptr1 = start;
 
        while (ptr1->next != lptr)
        {
            if (ptr1->numberOfOccurrences < ptr1->next->numberOfOccurrences)
            { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}


void printBST(wordNode* tree, int fd, int res)
{
	if(tree == NULL){
		return;
	}
	printBST(tree->left, fd, res);
	// res = write(fd,"\n",1);
	res = write(fd,"\t",1);
	char *string = (char*)calloc(15+strlen(tree->word),sizeof(char));
	sprintf(string,"<word text=\"%s\">",tree->word);
	res = write(fd,string,strlen(string));
	res = write(fd,"\n",1);
	res = write(fd,"\t",1);
	res = write(fd,"\t",1);
	char *wordCloseTag = "</word>";

	fileNode *ptr = tree->head;
	bubbleSort(tree->head);
	char buffer[10];
	while(ptr!=NULL)
	{
		int num = ptr->numberOfOccurrences;
		snprintf(buffer, 10, "%d", num);
		char* fileLine = (char*)calloc(21+1+strlen(ptr->fileName)+strlen(buffer),sizeof(char));
		sprintf(fileLine,"<file name=\"%s\">%s</file>",ptr->fileName,buffer);
		res = write(fd,fileLine,strlen(fileLine));
		if(ptr->next==NULL)
		{
			res = write(fd,"\n",1);
			res = write(fd,"\t",1);
		}else
		{
			res = write(fd,"\n",1);
			res = write(fd,"\t",1);
			res = write(fd,"\t",1);
		}
	// printf("file: %s, OCC: %d ",ptr->fileName,ptr->numberOfOccurrences);

	ptr=ptr->next;
	}
	res = write(fd,wordCloseTag,strlen(wordCloseTag));
	res = write(fd,"\n",1);
	printBST(tree->right, fd, res);
}

/*Function to get return a copy of the text within the file*/
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
/*Function to take our start and end pointers that are on the copied string
and create a new string to be returned.*/
char* stringCopier(char* start, char* end, int count);
char* stringCopier(char* start, char* end, int count)
{
	char* returnChar = (char*)calloc((count+1),sizeof(char));
	int i = 0;
	while(start!=end){
				returnChar[i]=tolower(*start);
				start++;
				i++;
			}
			return returnChar;	
}

/*Function ignores the . and .. when we are going through directories*/
int is_dot_or_dot_dot(char const* name);
int is_dot_or_dot_dot(char const* name)
{
   return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 );
}

/*Recursive function that goes through the directories to get to every file*/
void listdir(char const* dirname,wordNode* root);
void listdir(char const* dirname,wordNode* root)
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
						update(temp,root,curr_ent->d_name);
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
         listdir(subdir, root);
         // Free the allocated memory.
         free(subdir);
      }
   }
   // Close the directory
   closedir(dirp);
}

int main(int argc, char const *argv[])
{
	if(argc!=3)
	{
		printf("%s\n", "Incorrect number of arguments. Exiting now.");
		return 0;
	}
	//create word BST root
	wordNode* root=(wordNode*)malloc(1*sizeof(wordNode));
	//set the word to null to start with
	root->word=NULL;
	root->left=NULL;
	root->right=NULL;

	struct dirent *something = NULL;
	int status;
    struct stat st_buf;

    const char *invertedIndexFileName = argv[1]; //get the file name for the inverted index file
    int fd = open(invertedIndexFileName,O_RDWR|O_CREAT,S_IWUSR|S_IRUSR);
    int result = 0;
    char *xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    static char *fileIndexOpenTag = "<fileIndex>";
    static char *fileIndexCloseTag = "</fileIndex>";
    static char *wordCloseTag = "</word>";
    result = write(fd,xml,strlen(xml));
    result = write(fd,"\n",1);
    result = write(fd,fileIndexOpenTag,strlen(fileIndexOpenTag));
    result = write(fd,"\n",1);

	status = stat (argv[2], &st_buf);


    if (status != 0) {
        printf ("Error, errno = %d\n", errno);
        return 1;
    }
    //Case where passed in parameter is a file and not a directory
    if (S_ISREG (st_buf.st_mode)) 
    {
    	//Perform indexFile function here
    	int inputLen = strlen(argv[2]);
		/*Allocate memory for the size of the argument string*/
		char *strC = (char*)calloc((inputLen+1),sizeof(char));
		/*Test to see if the allocation of memory failed*/
			if (strC==NULL){
				printf("Could not allocate memory on line 17\n");
				return -1;
			}
		/*Make copy of argument string*/
		strcpy(strC, argv[2]);
    	FILE * textFile =fopen(argv[2],"r");
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
						//Call BST function here
						update(temp,root,strC);
					}
					if(!*end)
					{
						break;
					}
			end++;
			start=end;
   		}
    	printBST(root, fd, result);
    	result = write(fd,fileIndexCloseTag,strlen(fileIndexCloseTag));
    	return 0;
    }
    else if (S_ISDIR (st_buf.st_mode)) 
    {
        //enter the recursive function
		listdir(argv[2], root);
		printBST(root, fd, result);
		result = write(fd,fileIndexCloseTag,strlen(fileIndexCloseTag));
		return 0;
    }
    else
    {
    	printf("%s\n", "Argument is neither a file or directory. Exiting now.");
    	return 0;
    }


	// int result = 0;
	// int nameLen = 0;
	
}
