#ifndef IsNum_H
#define IsNum_H
#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<math.h>
#include <limits.h>
#include<string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>


struct RetStr{
int c;
int s;
int v;
};
typedef struct RetStr ret;

struct Out{
int i;
long int l;
float f;
double d;
char t;
};
typedef struct Out NUM;

typedef char* STRING;

ret checkint(STRING); 
ret checksgn(STRING);

NUM isnum(STRING );
NUM numok(STRING );
#endif
