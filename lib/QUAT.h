#ifndef _QUAT_H_
#define _QUAT_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include "MAT.h"

class Quat
{
public :
	Quat();
	Quat(double,double,double);
	Quat(double,double,double,double);
	//Quat(const Mat);
	//Quat(const Quat&);
	~Quat();

	double q(int);
	void setq(int,double);
	void setq(double,double,double);
	void setq(double,double,double,double);
	//void setq(const Mat);
	
	void SetRotSeq(int,int,int);
	double eul(int);
	double dcm(int,int);
	Mat dcm();

	void sete(double,double,double);
	void sete(double inp[3]);
	//void sete(const Mat);
	void setd(double inp[3][3]);
	void setd(const Mat);

	Quat& operator= (const Quat &Q);
	Quat  operator+ (const Quat &Q);
	Quat  operator- (const Quat &Q);  // q1-q2 = ~q1 * q2
	Quat  operator* (const Quat &Q);
	Quat  operator~ (void 	      );
	Quat  operator/ (      Quat &Q);  // q1/q2 = q1 * ~q2

	void PrintQuat();
	void PrintEul();
	void PrintDCM();
	void PrintRotSeq();
	
private:
	double Q[4];
	double EUL[3];
	double IR1,IR2,IR3;
	double DCM[3][3];
	void norm();
	double sign(double);
	void ComputeDcm();
	void ComputeEul();
	double maximum(double ary[],int size);
};

#endif
