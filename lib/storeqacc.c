#include <math.h>
#include "storeqacc.h"

#define L(v, x) for(v=0;v<x;v++)

#define ACCQSIZE (2500) // 20 sec for 8ms
#define QQSIZE (2500)
#define SVQSIZE (2500)

static double strgacc[ACCQSIZE][3];
static int accsize = 0;
static int acccur = 0;

static double strgsv[SVQSIZE][6];
static int svsize = 0;
static int svcur = 0;
static double svstamp = -1000.0;

static double strgq[QQSIZE][4];
static int qsize = 0;
static int qcur = 0;
static double accstamp = -1000.0;
static double qstamp = -1000.0;

static double strgqlb[QQSIZE][4];
static int qlbsize = 0;
static int qlbcur = 0;
static double qlbstamp = -1000.0;

void storeacc(double acc[3], double t)
{
  int i;
  static double accmul[3] = {0.0, 0.0, 0.0};
  accstamp = t;
  L(i, 3) accmul[i] += acc[i];
  L(i, 3) strgacc[acccur][i] = accmul[i];
  acccur++;
  if(accsize < ACCQSIZE)
    {
      accsize++;
    }
  if(acccur >= ACCQSIZE)
    {
      acccur = 0;
    }
}

void storesv(double sv[6], double t)
{
  int i;

  svstamp = t;

  L(i, 6) strgsv[svcur][i] = sv[i];
  svcur++;
  if(svsize < SVQSIZE)
    {
      svsize++;
    }
  if(svcur >= SVQSIZE)
    {
      svcur = 0;
    }
}

void storeq(double q[4], double t)
{
  int i;
  qstamp = t;
  L(i, 4) strgq[qcur][i] = q[i];
  qcur++;
  if(qsize < QQSIZE)
    {
      qsize++;
    }
  if(qcur >= QQSIZE)
    {
      qcur = 0;
    }
}

void storeqlb(double q[4], double t)
{
  int i;
  qlbstamp = t;
  L(i, 4) strgqlb[qlbcur][i] = q[i];
  qlbcur++;
  if(qlbsize < QQSIZE)
    {
      qlbsize++;
    }
  if(qlbcur >= QQSIZE)
    {
      qlbcur = 0;
    }
}

void getaccdt(double acc[3], double t2, double t1)
{
  int i;
  double a1[3], a2[3];

  getacc(a2, t2);
  getacc(a1, t1);

  L(i, 3) acc[i] = (a2[i] - a1[i]) / (t2 - t1);
}

void getacc(double acc[3], double t)
{
  int m, n, i;
  double dt, ni, mi;
  double macc[3], nacc[3];
  dt = accstamp - t;
  m = (int)(dt / 0.008);
  mi = (dt - m * 0.008) / 0.008;
  ni = 1.0 - mi;
  m = acccur - m - 1;
  n = m - 1;
  if(n < 0)
    {
      n = ACCQSIZE - 1;
    }
    
  if(m < 0)
    {
      m = ACCQSIZE - 1;
    }
  
  L(i, 3)
    {
      macc[i] = strgacc[m][i];
      nacc[i] = strgacc[n][i];
      acc[i] = macc[i]*ni + nacc[i]*mi;
    }
  
}

void getsv(double sv[6], double t)
{
  int m, n, i;
  double dt, ni, mi;
  dt = svstamp - t;
  m = (int)(dt / 0.008 + 0.5);
  m = svcur - m - 1;
    
  if(m < 0)
    {
      m = SVQSIZE - 1;
    }
  
  L(i, 6)
    {
      sv[i] = strgsv[m][i];
    }
  
}

void getq(double q[4], double t)
{
  int m, i, n;
  double dt, mi, ni;
  double q2[4], q1[4], dq[4], qsq;
  
  dt = qstamp - t;
  m = (int)(dt / 0.008);
  mi = (dt - m * 0.008) / 0.008;
  ni = 1.0 - mi;
  m = qcur - m - 1;
  n = m - 1;
  if(n < 0)
    {
      n = QQSIZE - 1;
    }
  if(m < 0)
    {
      m = QQSIZE - 1;
    }
  L(i, 4)
    {
      q2[i] = strgq[m][i];
      q1[i] = strgq[n][i];
    }
  
  dq[0] = q1[3]*q2[0] - q1[0]*q2[3] - q1[1]*q2[2] + q1[2]*q2[1];
  dq[1] = q1[3]*q2[1] + q1[0]*q2[2] - q1[1]*q2[3] - q1[2]*q2[0];
  dq[2] = q1[3]*q2[2] - q1[0]*q2[1] + q1[1]*q2[0] - q1[2]*q2[3];
  dq[3] = q1[3]*q2[3] + q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2];
  
  L(i, 3) dq[i] *= ni;
  dq[3] = sqrt(1.0 - dq[0]*dq[0] + dq[1]*dq[1] + dq[2]*dq[2]);
    
  /* Normalize this kid */
  qsq = 0.0;
  L(i, 4) qsq +=  dq[i]*dq[i];
  qsq = sqrt(qsq);
  L(i, 4) dq[i] /= qsq;
  
  q[0] = dq[3] * q1[0] + dq[2] * q1[1] - dq[1] * q1[2] + dq[0] * q1[3];
  q[1] = -dq[2] * q1[0] + dq[3] * q1[1] + dq[0] * q1[2] + dq[1] * q1[3];
  q[2] = dq[1] * q1[0] - dq[0] * q1[1] + dq[3] * q1[2] + dq[2] * q1[3];
  q[3] = -dq[0] * q1[0] - dq[1] * q1[1] - dq[2] * q1[2] + dq[3] * q1[3];

  /* Normalize this kid */
  qsq = 0.0;
  L(i, 4) qsq +=  q[i]*q[i];
  qsq = sqrt(qsq);

  L(i, 4) q[i] /= qsq;
}

void getqlb(double q[4], double t)
{
  int m, i, n;
  double dt, mi, ni;
  double q2[4], q1[4], dq[4], qsq;
  
  dt = qlbstamp - t;
  m = (int)(dt / 0.008);
  mi = (dt - m * 0.008) / 0.008;
  ni = 1.0 - mi;
  m = qlbcur - m - 1;
  n = m - 1;
  if(n < 0)
    {
      n = QQSIZE - 1;
    }
  if(m < 0)
    {
      m = QQSIZE - 1;
    }
  L(i, 4)
    {
      q2[i] = strgqlb[m][i];
      q1[i] = strgqlb[n][i];
    }
  
  dq[0] = q1[3]*q2[0] - q1[0]*q2[3] - q1[1]*q2[2] + q1[2]*q2[1];
  dq[1] = q1[3]*q2[1] + q1[0]*q2[2] - q1[1]*q2[3] - q1[2]*q2[0];
  dq[2] = q1[3]*q2[2] - q1[0]*q2[1] + q1[1]*q2[0] - q1[2]*q2[3];
  dq[3] = q1[3]*q2[3] + q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2];
  
  L(i, 3) dq[i] *= ni;
  dq[3] = sqrt(1.0 - dq[0]*dq[0] + dq[1]*dq[1] + dq[2]*dq[2]);
    
  /* Normalize this kid */
  qsq = 0.0;
  L(i, 4) qsq +=  dq[i]*dq[i];
  qsq = sqrt(qsq);
  L(i, 4) dq[i] /= qsq;
  
  q[0] = dq[3] * q1[0] + dq[2] * q1[1] - dq[1] * q1[2] + dq[0] * q1[3];
  q[1] = -dq[2] * q1[0] + dq[3] * q1[1] + dq[0] * q1[2] + dq[1] * q1[3];
  q[2] = dq[1] * q1[0] - dq[0] * q1[1] + dq[3] * q1[2] + dq[2] * q1[3];
  q[3] = -dq[0] * q1[0] - dq[1] * q1[1] - dq[2] * q1[2] + dq[3] * q1[3];

  /* Normalize this kid */
  qsq = 0.0;
  L(i, 4) qsq +=  q[i]*q[i];
  qsq = sqrt(qsq);

  L(i, 4) q[i] /= qsq;
}

#undef L
#undef ACCQSIZE
#undef QQSIZE
