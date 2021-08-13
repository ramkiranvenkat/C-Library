#include "dataAdv.h"

data::data(){local = NULL;}
data::data(char* inp)
{local = LOAD(inp);}

data::data(string inpS)
{
	int sl = inpS.length();
	char inp[sl+1];
	inp[sl] = '\0';
	for (int j=0;j<sl;j++) inp[j] = inpS[j];
	local = LOAD(inp);
}
data::~data(){if(local != NULL)UNLOAD(local);local=NULL;}
void data::load(char* inp)
{
if (local == NULL)local = LOAD(inp);
else
{
	UNLOAD(local);
	local = LOAD(inp);
}
}
void data::load(string inpS)
{
int sl = inpS.length();
char inp[sl+1];
inp[sl] = '\0';
for (int j=0;j<sl;j++) inp[j] = inpS[j];

if (local == NULL)local = LOAD(inp);
else
{
	UNLOAD(local);
	local = LOAD(inp);
}
}
void data::unload(){if(local != NULL)UNLOAD(local);local=NULL;}
int data::i(unsigned int r,unsigned int c){return RINT(local,r,c);}
float data::f(unsigned int r,unsigned int c){return RFLOAT(local,r,c);}
double data::d(unsigned int r,unsigned int c){return RDOUBLE(local,r,c);}
long data::l(unsigned int r,unsigned int c){return RLONG(local,r,c);}
string data::s(unsigned int r,unsigned int c)
{
	unsigned int WL = WordLength(local,r,c);
	if (WL > 0)
	{        
		char read[WL+1];
        	RSTRING(local,read,r,c);
        	string out(read);
        	return out;
	}
	else
	{
		cout << "Be careful what u wish for::String does not have data" << endl;
		string out;
        	return out;
	}
}
int data::length(){return datasize(local);}
int data::NumOfColInRow(int row){return NumOfCol(local,row);}
