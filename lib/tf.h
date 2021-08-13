#ifndef _TFn_H_
#define _TFn_H_

#include "MAT.h"

class TFn
{
public:
	TFn();
	
	TFn(Mat n,Mat d);
	~TFn();
	TFn(const TFn& );
	TFn& operator= (const TFn& );

	void info();	
	
	void setNum(Mat& );
	void setDen(Mat& );
	Mat getNum() const;
	Mat getDen() const;

	void setMethod(int );	
	int  getMethod() const;
	
	void setDt(double);
	double getDt() const;

	// double IO(double val);
	double IO(double val,double dt);
private:
	Mat* num;
	Mat* den;
	int method;
	Mat* bufx;unsigned int mx,sx;
	Mat* bufy;unsigned int my,sy; 
	Mat getBufx() const;
	Mat getBufy() const;
	void Bufs();
	void binomial_neg(double outarray[],int n);
	void binomial_pos(double outarray[],int n);
	void conv(double out[],double in1[],double in2[],int n1,int n2);
	Mat tfdyn(Mat );
	Mat rk4_tf(Mat x,Mat u,double ts);
	Mat dgl_tf(Mat x,Mat u,double ts);

	double dt,x;
};

/*
TFn operator+(TFn,double);
TFn operator+(double,const TFn);
TFn operator+(const TFn,const TFn);
TFn operator-(const TFn,double);
TFn operator-(double,const TFn);
TFn operator-(const TFn,const TFn);
TFn operator-(const TFn);
TFn operator*(const TFn,double);
TFn operator*(double,const TFn);
TFn operator*(const TFn,const TFn);
TFn operator/(const TFn,double);
TFn operator/(double,const TFn);
TFn operator/(const TFn,const TFn);
*/
#endif
