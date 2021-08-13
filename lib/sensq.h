#ifndef __SENSQ_H__
#define __SENSQ_H__

#define SENS_Q_SIZE (128)
typedef enum
  {
    elasa,
    ekara_alt,
    ekara_vel,
    einr_px,
    einr_py,
    einr_pz,
    einr_vx,
    einr_vy,
    einr_vz,
    elpdc,
    elhvc,
    einr,
    etotsens
  } tsensname;

typedef struct
{
  int issync;
  double data[2];
  double t;
} tsdata;

typedef struct
{
  tsdata sq[SENS_Q_SIZE];
  int tsz;
  int sz;
  int cur;
  int end;
} tsensorqueue;

void getcurtime(double t);
void rsensqpoi(); // initialize
void rsyncnotify(tsensname sens, double t); // while sync
void rdatanotify(tsensname sens, double data[2], double t); // while data ready
int fgetinorderdata(tsensname *sens, double data[2], double *t); // fetch to abs nav algo

#endif
