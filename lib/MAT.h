#ifndef _MAT_H_
#define _MAT_H_

#define WARNING 0
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <string.h>

class Mat
{
public:
	Mat();
	Mat(int ,int ); //row column
	Mat(const Mat& ); //deep copy;
	~Mat();

	int rsize() const;
	int csize() const;

	void size();
	
	void show();	

	Mat& operator= (const Mat& );	
	double& operator()(unsigned int ,unsigned int );
	double& operator()(unsigned int );

	// set value in matrix
	void reset(int row,int col); // copies the old matrix into first rXc matrix and alongates the matrix to row and column
				     // and fills with zeros	
	void set(int,double); // only row or column matrix - print warning if matrix in not row/column matrix

	void set(double); // sets entire matrix by double value

	void set(int,int,double); // row,col,value - print warning if input row and column is greater than current row/column
				  // and increases the matrix size
	void set(const Mat& ); // constructor like deep copy -- prints warning if both matrices are of different size and replaces the 
			// current matrix by input matrix (in size)	
	void set(int,int,const Mat& ); //sets a mat from position int row,int column

	
	// get value in matrix
	double get(int,int);
	double get(int);
	
	Mat  row(int);
	Mat  col(int);
	
	Mat submat(int,int,int,int);

	Mat zero();
	
	Mat Const(double );

	Mat eye();

	Mat eyec();
	
	Mat eyer();
	
	Mat diag();
	
	void close();

	Mat inv();
	/*
	Mat eig();
	Mat pinv();
	Mat eigmat();
	void svd(Mat &U,Mat &W,Mat &V);
	Mat lu(Mat &L,Mat &U);
	double det();
	int rank();
	double trace();
	*/
	double **val;

private:
	int r,c;
	void allocate(int,int);
	void release();
};

/* append or remove
	
	Mat AppHor(Mat,Mat);
	Mat AppVer(Mat,Mat);
	Mat RemHor(Mat,int,int);
	Mat RemVer(Mat,int,int);
*/
Mat operator+(Mat,double);
Mat operator+(double,const Mat);
Mat operator+(const Mat,const Mat);
Mat operator-(const Mat,double);
Mat operator-(double,const Mat);
Mat operator-(const Mat,const Mat);
Mat operator-(const Mat);
Mat operator*(const Mat,double);
Mat operator*(double,const Mat);
Mat operator*(const Mat,const Mat);
Mat operator/(const Mat,double);
Mat operator~(const Mat);

Mat eye(int);
Mat fnorm(const Mat);
void MATsize(const Mat );
#endif
