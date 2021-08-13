#ifndef __STOREQACC_H__
#define __STOREQACC_H__

void storeacc(double acc[3], double t);
void storesv(double sv[6], double t);
void storeq(double q[4], double t);
void storeqlb(double q[4], double t);
void getaccdt(double acc[3], double t2, double t1);
void getacc(double acc[3], double t);
void getsv(double acc[6], double t);
void getq(double q[4], double t);
void getqlb(double q[4], double t);
#endif
