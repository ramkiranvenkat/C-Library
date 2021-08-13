#include <stdio.h>
#include <stdlib.h>
#include "sensq.h"
#define MIN(x,y) ((x)<(y)?(x):(y))

static tsensorqueue sensq[etotsens];
static double curtime = 0.0;

void getcurtime(double t)
{
	curtime = t;
}

void rsensqpoi()
{
  int i;
  for(i = 0; i < etotsens; i++)
    {
      sensq[i].tsz = SENS_Q_SIZE;
      sensq[i].sz = 0;
      sensq[i].cur = 0;
      sensq[i].end = 0;
    }
}

static int fsenspush(tsensname sens, tsdata sd)
{
  int ret;
  int *cur, *sz;
  ret = 0;
  cur = &sensq[sens].cur;
  sz = &sensq[sens].sz;
  if(*sz < sensq[sens].tsz)
    {
      ret = 1;
      sensq[sens].sq[*cur] = sd;
      *cur += 1;
      *sz += 1;
      if(*cur >= SENS_Q_SIZE)
	{
	  *cur = 0;
	}
      if(*sz > SENS_Q_SIZE)
	{
	  *sz = SENS_Q_SIZE;
	}
    }
  return ret;
}

static int fsenspop(tsensname sens, tsdata *sd)
{
  int *end, *sz;
  int ret;
  
  ret = 0;
  end = &sensq[sens].end;
  sz = &sensq[sens].sz;
  
  if(*sz > 0)
    {
      ret = 1;
      *sd = sensq[sens].sq[*end];
      *end += 1;
      *sz -= 1;
      if(*end >= SENS_Q_SIZE)
	{
	  *end = 0;
	} 
    }
  return ret;
}

static int fsenspeek(tsensname sens, tsdata *sd)
{
  int end, sz;
  int ret;
  
  ret = 0;
  end = sensq[sens].end;
  sz = sensq[sens].sz;
  
  if(sz > 0)
    {
      ret = 1;
      *sd = sensq[sens].sq[end];
    }
  return ret;
}
void rsyncnotify(tsensname sens, double t)
{
  tsdata sd;
  sd.issync = 1;
  sd.data[0] = 1.234;
  sd.data[1] = 1.234;
  sd.t = t;
  fsenspush(sens, sd);
}

void rdatanotify(tsensname sens, double data[2], double t)
{
  tsdata sd;
  sd.issync = 0;
  sd.data[0] = data[0];
  sd.data[1] = data[1];
  sd.t = t;
  fsenspush(sens, sd);
}

int fgetinorderdata(tsensname *sens, double data[2], double *t)
{
  int j;
  int ret, shut;
  tsdata sd, msd;
  tsensname minsens = etotsens;
  ret = 0;
  for(j = 0; j < etotsens; j++)
    {
		shut = fsenspeek(j, &sd);
      if(shut == 1)
		{
     	 while(((sd.issync == 1) && (sensq[j].sz > 1)) || ((sd.t + 11.0) < curtime))
		{	
	 	 fsenspop(j, &sd);
	 	 shut = fsenspeek(j, &sd);
			if(shut == 0) break;
		}		
		}
    }

  msd.t = 1.0E30;
  for(j = 0; j < etotsens; j++)
    {
      if(fsenspeek(j, &sd) == 1)
	{
	  ret = 1;
	  if(sd.t < msd.t)
	    {
	      msd = sd;
	      minsens = j;
	    }
	}
    }

  if(ret == 1)
    {
      if(msd.issync == 1)
	{
	  ret = 0;
	}
      else
	{
	  fsenspop(minsens, &sd);
	  *sens = minsens;
	  data[0] = msd.data[0];
	  data[1] = msd.data[1];
	  *t = msd.t;
	}
    }
  return ret;   
}
