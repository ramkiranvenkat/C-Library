/*

CLeared work includes

2. Single time call for numok function call. --------------------------- closed
		Decided against it!!!
		Since a function call is invoked only once it is called and no one will call the function for both int & double
		types.Any future suggestion should be considered seriously and the software design is should encourage changes as per
		the input of the current generation.

3. Error checking on input file segments. ------------------------------ closed
		__CheckAvailability(row,column) is written to check whether a given row or column is avilable in the given file.

4. Close of read file -- memory deallocation of Map. ------------------- closed - on old LOAD
		memory check is done using valgring succesfully

6. SEEK_CUR access. ---------------------------------------------------- postponed
		Considering seek as an atomic operation.

8. Multiple opening of files and its attributes --------------------------- implemented !!!
---------------------------------pending-----------------------------------------------------------------------------------------

1. MAP cache development for faster access and limited size value read.
3.1 Error checking on memory allocation.----------------------------------------safety measures, isnum.c still pending
5. Stringent Data declaration like DATAD,DATAI.
7. Static declaration of functions. --------------------------------------- current
9. Freeing memory and file id management ---------------------------------- implemented
10. line read of data and data read in format syntax for reading an array

-- unknown in future --

/**/
#include "dread.h"

static NODE* root;
static LIST* globalHead = NULL;
char __CheckAvailability(DATA,unsigned int,unsigned int);
static unsigned int semaphore;
static bool init = true;	
DATA LOAD(__STRING istr)
{
   	DATA dlocal;
	int j;
	if (init)
	{
		root = (NODE* )malloc(sizeof(NODE));
		root->end  = false;
		root->info  = NULL;
		root->head = NULL;
		root->next = (NODE** )malloc(128*sizeof(NODE*));
		for (j = 0;j<128;j++) root->next[j] = NULL;
		init = false;
		semaphore = 0;
	}
	dlocal = Log(istr);
	if (!dlocal->fid)
	{
		dlocal->fp = fopen(istr,"r");
		MAP(dlocal);
	}
	semaphore++;
	return dlocal;

}

char 		__CheckAvailability(DATA var,unsigned int r,unsigned int c)
{
	unsigned int c_lim;

	if ((r<0)||(c<0)) 	return 0;
	if (r>(var->__cow).row)	return 0;
	if (r==1)
	c_lim = (var->__map)._seek_newline[r-1];
	else
	c_lim = (var->__map)._seek_newline[r-1] - (var->__map)._seek_newline[r-2];

	if (c>c_lim)		return 0;

	return 1;
}

int 		RINT(DATA var,unsigned int r,unsigned int c)
{	
	unsigned int __SEEK_START_LOCAL,__SEEK_SIZE_LOCAL,__SEEK_NEWLINE_LOCAL;
	FILE* flocal = var->fp;

	if (!__CheckAvailability(var,r,c))
	{
		printf("Can't access data point in %s at (%d,%d)!!!\n",var->fname,r,c);
		return INT_MAX;
	}

	if (r == 1) __SEEK_NEWLINE_LOCAL = 0;
	else __SEEK_NEWLINE_LOCAL = (var->__map)._seek_newline[r-2];


	__SEEK_START_LOCAL = (var->__map)._seek_start[__SEEK_NEWLINE_LOCAL + c - 1];
	__SEEK_SIZE_LOCAL  = (var->__map)._seek_size[__SEEK_NEWLINE_LOCAL + c - 1];	


	__STRING temp = malloc((__SEEK_SIZE_LOCAL+1)*sizeof(char));
	if (temp == NULL)
		return INT_MAX;

	fseek(flocal,(__SEEK_START_LOCAL-1)*sizeof(char),SEEK_SET);
	
	fread(temp,(__SEEK_SIZE_LOCAL)*sizeof(char),1,flocal);
	
	temp[__SEEK_SIZE_LOCAL] = '\0';
	NUM tempNUM = numok(temp);
		
	fseek(flocal,0,SEEK_SET);
	free(temp);temp = NULL;
	return tempNUM.i;
}

float 		RFLOAT(DATA var,unsigned int r,unsigned int c)
{	
	unsigned int __SEEK_START_LOCAL,__SEEK_SIZE_LOCAL,__SEEK_NEWLINE_LOCAL;
	FILE* flocal = var->fp;

	if (!__CheckAvailability(var,r,c))
	{
		printf("Can't access data point in %s at (%d,%d)!!!\n",var->fname,r,c);
		return INT_MAX;
	}

	if (r == 1) __SEEK_NEWLINE_LOCAL = 0;
	else __SEEK_NEWLINE_LOCAL = (var->__map)._seek_newline[r-2];


	__SEEK_START_LOCAL = (var->__map)._seek_start[__SEEK_NEWLINE_LOCAL + c - 1];
	__SEEK_SIZE_LOCAL  = (var->__map)._seek_size[__SEEK_NEWLINE_LOCAL + c - 1];	


	__STRING temp = malloc((__SEEK_SIZE_LOCAL+1)*sizeof(char));
	if (temp == NULL)
		return INT_MAX;

	fseek(flocal,(__SEEK_START_LOCAL-1)*sizeof(char),SEEK_SET);
	
	fread(temp,(__SEEK_SIZE_LOCAL)*sizeof(char),1,flocal);
	
	temp[__SEEK_SIZE_LOCAL] = '\0';
	NUM tempNUM = numok(temp);
		
	fseek(flocal,0,SEEK_SET);
	free(temp);temp = NULL;
	return tempNUM.f;
}

double 		RDOUBLE(DATA var,unsigned int r,unsigned int c)
{	
	unsigned int __SEEK_START_LOCAL,__SEEK_SIZE_LOCAL,__SEEK_NEWLINE_LOCAL;
	FILE* flocal = var->fp;

	if (!__CheckAvailability(var,r,c))
	{
		printf("Can't access data point in %s at (%d,%d)!!!\n",var->fname,r,c);
		return INT_MAX;
	}

	if (r == 1) __SEEK_NEWLINE_LOCAL = 0;
	else __SEEK_NEWLINE_LOCAL = (var->__map)._seek_newline[r-2];


	__SEEK_START_LOCAL = (var->__map)._seek_start[__SEEK_NEWLINE_LOCAL + c - 1];
	__SEEK_SIZE_LOCAL  = (var->__map)._seek_size[__SEEK_NEWLINE_LOCAL + c - 1];	


	__STRING temp = malloc((__SEEK_SIZE_LOCAL+1)*sizeof(char));
	if (temp == NULL)
		return INT_MAX;

	fseek(flocal,(__SEEK_START_LOCAL-1)*sizeof(char),SEEK_SET);
	
	fread(temp,(__SEEK_SIZE_LOCAL)*sizeof(char),1,flocal);
	
	temp[__SEEK_SIZE_LOCAL] = '\0';
	NUM tempNUM = numok(temp);
		
	fseek(flocal,0,SEEK_SET);
	free(temp);temp = NULL;
	return tempNUM.d;
}

long 		RLONG(DATA var,unsigned int r,unsigned int c)
{	
	unsigned int __SEEK_START_LOCAL,__SEEK_SIZE_LOCAL,__SEEK_NEWLINE_LOCAL;
	FILE* flocal = var->fp;

	if (!__CheckAvailability(var,r,c))
	{
		printf("Can't access data point in %s at (%d,%d)!!!\n",var->fname,r,c);
		return INT_MAX;
	}

	if (r == 1) __SEEK_NEWLINE_LOCAL = 0;
	else __SEEK_NEWLINE_LOCAL = (var->__map)._seek_newline[r-2];


	__SEEK_START_LOCAL = (var->__map)._seek_start[__SEEK_NEWLINE_LOCAL + c - 1];
	__SEEK_SIZE_LOCAL  = (var->__map)._seek_size[__SEEK_NEWLINE_LOCAL + c - 1];	


	__STRING temp = malloc((__SEEK_SIZE_LOCAL+1)*sizeof(char));
	if (temp == NULL)
		return INT_MAX;

	fseek(flocal,(__SEEK_START_LOCAL-1)*sizeof(char),SEEK_SET);
	
	fread(temp,(__SEEK_SIZE_LOCAL)*sizeof(char),1,flocal);
	
	temp[__SEEK_SIZE_LOCAL] = '\0';
	NUM tempNUM = numok(temp);
		
	fseek(flocal,0,SEEK_SET);
	free(temp);temp = NULL;
	return tempNUM.l;
}
int		RSTRING(DATA var,char* inpstr,unsigned int r,unsigned int c)
{
	unsigned int __SEEK_START_LOCAL,__SEEK_SIZE_LOCAL,__SEEK_NEWLINE_LOCAL;	
	FILE* flocal = var->fp;

	if (!__CheckAvailability(var,r,c))
	{
		printf("Can't access data point in %s at (%d,%d)!!!\n",var->fname,r,c);
		return INT_MAX;
	}
	if (r == 1) __SEEK_NEWLINE_LOCAL = 0;
	else __SEEK_NEWLINE_LOCAL = (var->__map)._seek_newline[r-2];

	__SEEK_START_LOCAL = (var->__map)._seek_start[__SEEK_NEWLINE_LOCAL + c - 1];
	__SEEK_SIZE_LOCAL  = (var->__map)._seek_size[__SEEK_NEWLINE_LOCAL + c - 1];		
	char temp[(__SEEK_SIZE_LOCAL+1)];
	if (temp == NULL)
	{	printf("string memory allocation problem\n");
		return INT_MAX;
	}

	fseek(flocal,(__SEEK_START_LOCAL-1)*sizeof(char),SEEK_SET);
	
	fread(temp,(__SEEK_SIZE_LOCAL)*sizeof(char),1,flocal);
	
	temp[__SEEK_SIZE_LOCAL] = '\0';
	strcpy(inpstr,temp);
		
	fseek(flocal,0,SEEK_SET);
	return 0;

}

int WordLength(DATA var, unsigned int r, unsigned int c)
{
	unsigned int __SEEK_START_LOCAL,__SEEK_SIZE_LOCAL,__SEEK_NEWLINE_LOCAL;
	FILE* flocal = var->fp;

	if (!__CheckAvailability(var,r,c))
	{
		printf("Can't access data point in %s at (%d,%d)!!!\n",var->fname,r,c);
		return -1;
	}

	if (r == 1) __SEEK_NEWLINE_LOCAL = 0;
	else __SEEK_NEWLINE_LOCAL = (var->__map)._seek_newline[r-2];

	__SEEK_SIZE_LOCAL  = (var->__map)._seek_size[__SEEK_NEWLINE_LOCAL + c - 1];
	return __SEEK_SIZE_LOCAL;	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAP(__CD* dlocal) //
{
	if (dlocal->fp == NULL)
	{
		printf("problem opening file %s !!!\n",dlocal->fname);
	}
	
	char readc;
	unsigned int itr=1,flag,flagp=0,bow=0,sow=0,row=0,i; //bow - seek for beginning of word,sow - size of a word

	while(fread(&readc,sizeof(char),1,dlocal->fp))	
	{	
		flag = 0;
		if (!((readc == ' ')||(readc == '\t')||(readc == '\n')))
		flag = 1;
		
		if ((flag - flagp) == +1) bow = AddSeekStart(bow,itr,&(dlocal->__map)); //seek start
		if ((flag - flagp) == -1) sow = AddSeekSize(sow,itr,&(dlocal->__map)); //seek size

		if (readc == '\n') row = AddNewlineWord(row,bow,&(dlocal->__map));		

		flagp = flag;
		itr++;
	}
	
	__COW temp;
	temp.bow = bow;
	temp.sow = sow;
	temp.row = row;
	dlocal->__cow = temp;

	fseek(dlocal->fp,0,SEEK_SET);
	
}

unsigned int AddSeekStart(unsigned int count,unsigned int itr,DMAP *__map)
{
if (!count){
	(*__map)._seek_start = malloc(sizeof(int));
	if ((*__map)._seek_start == NULL)
		return -1;
	(*__map)._seek_start[count] = itr;
	count++;
}else      {
	(*__map)._seek_start = realloc((*__map)._seek_start,(count+1)*sizeof(int));
	if ((*__map)._seek_start == NULL)
		return -1;
	(*__map)._seek_start[count] = itr;
	count++;
}
return count;
}

unsigned int AddSeekSize(unsigned int count,unsigned int itr,DMAP *__map)
{
if (!count){
	(*__map)._seek_size = malloc(sizeof(int));
	if ((*__map)._seek_size == NULL)
		return -1;
	(*__map)._seek_size[count] = itr - (*__map)._seek_start[count];
	count++;
}else      {
	(*__map)._seek_size = realloc((*__map)._seek_size,(count+1)*sizeof(int));
	if ((*__map)._seek_size == NULL)
		return -1;
	(*__map)._seek_size[count] = itr - (*__map)._seek_start[count];
	count++;
}
return count;
}
unsigned int AddNewlineWord(unsigned int count,unsigned int itr,DMAP *__map)
{
if (!count){
	(*__map)._seek_newline = malloc(sizeof(int));
	if ((*__map)._seek_newline == NULL)
		return -1;
	(*__map)._seek_newline[count] = itr;
	count++;
}else      {
	(*__map)._seek_newline = realloc((*__map)._seek_newline,(count+1)*sizeof(int));
	if ((*__map)._seek_newline == NULL)
		return -1;
	(*__map)._seek_newline[count] = itr;
	count++;
}
return count;
}

void ReleseMemory(DMAP *__map)
{
	free((*__map)._seek_newline);(*__map)._seek_newline = NULL;
	free((*__map)._seek_size);(*__map)._seek_size = NULL;
	free((*__map)._seek_start);(*__map)._seek_start = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////

__CD* Log(char* buf)
{
	int strsize = (int)(strlen(buf));
	strsize = strsize+1;
	char inp[strsize];
	strcpy(inp,buf);
	inp[strsize-1] = '\0';
	
	int j,i = 0;
    	NODE* Lnode = root;
    	
    	while ((inp[i] != '\0')&&(i<TRIE_LEVEL))
    	{
        	int indx = (int)inp[i];
		    
        	if (Lnode->next[indx] == NULL)
        	{
			NODE *temp = (NODE* )malloc(sizeof(NODE));
			temp->end  = false;
			temp->head = NULL;
			temp->info = NULL;
			temp->next = NULL;
			if (i<(TRIE_LEVEL-1))
       			{
				temp->next = (NODE** )malloc(128*sizeof(NODE*));
				for (j = 0;j<128;j++) temp->next[j] = NULL;
			}
       			Lnode->next[indx] = temp;
       			
        	}	
		Lnode = Lnode->next[indx];
		i++;      
    	}
	if (inp[i] == '\0')
	{
		if (Lnode->end == false)
		{		
			Lnode->end = true;
			Lnode->info = malloc(sizeof(__CD));
			(Lnode->info)->fid = 0;
			(Lnode->info)->fname = malloc(strsize*sizeof(char));
			strcpy((Lnode->info)->fname,inp);
			(Lnode->info)->LLptr = NULL;
			(Lnode->info)->LLptrPrev = NULL;
			(Lnode->info)->Lnodeptr = Lnode;
			return Lnode->info;
			
		}
		else 
		{
			((Lnode->info)->fid)++;
			return Lnode->info;
		}
	}
	else
	{
		Lnode->head = globalHead;
		LIST *temp = malloc(sizeof(LIST));
		temp->value = malloc((strsize-TRIE_LEVEL)*sizeof(char));
		temp->info = NULL;
		temp->next = NULL;
		for (i=TRIE_LEVEL;i<strsize;++i) temp->value[i-TRIE_LEVEL] = inp[i];
		__CD *dataV = SEARCH(temp->value,Lnode->head);

		if (dataV->fid != 0) 
		{	
			free(temp->value);temp->value = NULL;
			return dataV;
		}		
	
		dataV->LLptr = temp;
		dataV->fname = malloc(strsize*sizeof(char));
		strcpy(dataV->fname,inp);
		temp->info = dataV;

		temp->next = Lnode->head;
		Lnode->head = temp;
		globalHead = Lnode->head;

		if (temp->next != NULL)	((temp->next)->info)->LLptrPrev = temp;
		return temp->info;
	}

	
}

__CD* SEARCH(const char* str,LIST* head)
{
	LIST* LLL;
	LLL = head;
	while(LLL != NULL)
	{
		if (!strcmp(str,LLL->value))	
		{	
			((LLL->info)->fid)++;
			return LLL->info;
		}
		LLL = LLL->next;
	}
	
	__CD *temp = malloc(sizeof(__CD));
	temp->fid = 0;
	temp->LLptr = NULL;
	temp->LLptrPrev = NULL;
	temp->Lnodeptr = NULL;

	return temp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool UNLOAD(DATA var)
{
	if (!semaphore) 
	{
		printf("Err: Closed all files!\n");
		return false;
	}
	semaphore--;
	if (var->fid < 0)
	{
		printf("Err: Trying for multiple closing of same file");
		return false;
	}	
	if (!var->fid)
	{
		fclose(var->fp);
		ReleseMemory(&(var->__map));
		free(var->fname);var->fname = NULL;

		if (var->LLptr != NULL)
		{	
			if (var->LLptrPrev != NULL) ((LIST* )(var->LLptrPrev))->next = ((LIST* )(var->LLptr))->next;
			if (((LIST* )(var->LLptr))->next != NULL) ((((LIST* )(var->LLptr))->next)->info)->LLptrPrev = var->LLptrPrev;
			if (globalHead == ((LIST* )(var->LLptr))) globalHead = (((LIST* )(var->LLptr))->next);

			free(((LIST* )(var->LLptr))->value);((LIST* )(var->LLptr))->value = (char* )NULL;
			free((LIST* )(var->LLptr));(var->LLptr) = (void* )NULL;
			free(var);var = (__CD* )NULL;
			
			
		}
		else if (var->Lnodeptr != NULL)
		{
			((NODE* )(var->Lnodeptr))->end = false;
			free(((NODE* )(var->Lnodeptr))->info);((NODE* )(var->Lnodeptr))->info = (__CD* )NULL;
		}
		if (!semaphore) 
		{
			init = true;
			ReleaseRoot(root);
		}
		//var->fid--;
		return true;
	}
	var->fid--;
	return true;
}

bool ReleaseRoot(NODE* var)
{
	int i;
	if (var->next != NULL)
	{
		for(i=0;i<128;++i)
		{
			if (var->next[i] != NULL)  ReleaseRoot(var->next[i]);
		}
	}
	free(var->next);var->next = NULL;
	free(var);var = NULL;

	return true;	
}

int datasize(DATA inp)
{
	if (inp->fp == NULL)
	{
		printf("file not there\n");
		return 0;
	}
	
	return (inp->__cow).row;
}

int NumOfCol(DATA inp,int rin)
{
	if (inp->fp == NULL)
	{
		printf("file not there (NumOfCol)\n");
		return 0;
	}
	int ccnt = 1; // column counter
	while (__CheckAvailability(inp,rin,ccnt)) ccnt++;
	
	return ccnt-1;
}
