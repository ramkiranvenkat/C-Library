#ifndef DREAD_H
#define DREAD_H 
#include "isnum.h"
#define TRIE_LEVEL 2

typedef char*    __STRING;

struct __Map
{
	int* _seek_start;
	int* _seek_size;
	int* _seek_newline;
};
typedef struct __Map DMAP;

struct __dCOW
{
	unsigned int bow;
	unsigned int sow;
	unsigned int row;
};
typedef struct __dCOW __COW;

struct CriticalData
{
	FILE *fp;
	DMAP __map;
	__COW __cow;
	int fid;
	char* fname;
	void* LLptr;
	void* LLptrPrev;
	void* Lnodeptr;
};
typedef struct CriticalData __CD;
typedef struct CriticalData* DATA;

DATA LOAD(char* );

void MAP(__CD*);
unsigned int AddSeekStart(unsigned int,unsigned int,DMAP*);
unsigned int AddSeekSize(unsigned int,unsigned int,DMAP*);
unsigned int AddNewlineWord(unsigned int,unsigned int,DMAP*);
void ReleseMemory(DMAP*);

struct LL
{
	char* value;
	struct CriticalData *info;
	struct LL *next;
};
typedef struct LL LIST;

struct node
{
	bool end;
	LIST* head;
	struct CriticalData *info;
	struct node **next; 
};
typedef struct node NODE;

__CD* Log(char* );
__CD* SEARCH(const char* ,LIST* );

int RINT(DATA var,unsigned int r,unsigned int c);
float RFLOAT(DATA var,unsigned int r,unsigned int c);
double RDOUBLE(DATA var,unsigned int r,unsigned int c);
long RLONG(DATA var,unsigned int r,unsigned int c);
int RSTRING(DATA var,char* inpstr,unsigned int r,unsigned int c);
int WordLength(DATA var, unsigned int r, unsigned int c);

bool UNLOAD(DATA);
bool ReleaseRoot(NODE* );
int datasize(DATA a);
int NumOfCol(DATA a,int row);

#endif
