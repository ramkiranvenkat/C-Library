#include "tf.h"

using namespace std;
static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ? (iminarg1) : (iminarg2))
#define IMAX(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) > (iminarg2) ? (iminarg1) : (iminarg2))
#define arraysize(array) sizeof(array)/sizeof(array[0])

TFn::TFn()
{
	num = NULL;
	den = NULL;
	method = 0;		
	
	bufx = NULL;
	bufy = NULL;
	dt = 1;
}

TFn::TFn(Mat n,Mat d)
{
	if ((n.rsize() != 1)&&(n.csize() != 1)) {cout << "Numerator should be a Vector\n";exit(0);}
	if ((d.rsize() != 1)&&(d.csize() != 1)) {cout << "Denominator should be a Vector\n";exit(0);}

	if (!n.get(1)) {cout << "numerator should start with 'non-zero' value\n";exit(0);}
	if (!d.get(1)) {cout << "denominator should start with 'non-zero' value\n";exit(0);}
	num = new Mat(n);
	den = new Mat(d);

	*num = *num/den->get(1);
	*den = *den/den->get(1);

	method = 0;

	bufx = NULL;
	bufy = NULL;
	Bufs();
	dt = 1;
}

TFn::~TFn()
{
	delete num;
	delete den;
	delete bufx;
	delete bufy;
}
TFn::TFn(const TFn& inp)
{
	if (inp.num) {num = new Mat(inp.getNum());}
	else num = NULL;
	if (inp.den) {den = new Mat(inp.getDen());}
	else den = NULL;
	method = 0;

	bufx = NULL;
	bufy = NULL;
	Bufs();
	dt = inp.getDt();
}
TFn& TFn::operator= (const TFn& inp)
{
	
	if (this != &inp)
	{	
		if (inp.num)
		{
			if (!num) num = new Mat(inp.getNum());
			else {delete num;num = new Mat(inp.getNum());}
		}
		else num = NULL;
		if (inp.den)
		{
			if (!den) den = new Mat(inp.getDen());
			else {delete den;den = new Mat(inp.getDen());}
		}
		else den = NULL;
		method = 0;

		bufx = NULL;
		bufy = NULL;
		Bufs();

		dt = inp.getDt();
	}
	return *this;
	
}

void TFn::info()
{
	cout << "Numerator :\n";
	cout << num << endl;
	if (num) num->show();
	cout << "Denominator :\n";
	cout << den << endl;
	if (den) den->show();

	cout << "Method : " << method << endl;

	cout << "BufferX :\n";
	cout << bufx << endl;
	if (bufx) bufx->show();
	cout << "BufferY :\n";
	cout << bufy << endl;
	if (bufy) bufy->show();

	cout << "dt : " << dt << endl;
}

void TFn::setNum(Mat& n)
{
	if ((n.rsize() != 1)&&(n.csize() != 1)) {cout << "Numerator should be a Vector\n";exit(0);}
	if (!n.get(1)) {cout << "numerator should start with 'non-zero' value\n";exit(0);}
	
	if (!num) {num = new Mat(n);}
	else {num->set(n);}
	if (!den) {den = new Mat(1,1);den->set(1);}

	Bufs();
}
void TFn::setDen(Mat& d)
{
	if ((d.rsize() != 1)&&(d.csize() != 1)) {cout << "Denominator should be a Vector\n";exit(0);}
	if (!d.get(1)) {cout << "denominator should start with 'non-zero' value\n";exit(0);}

	if (!den) {den = new Mat(d);}
	else {den->set(d);}
	if (!num) {num = new Mat(1,1);num->set(1);}

	*num = *num/den->get(1);
	*den = *den/den->get(1);

	Bufs();
}
Mat TFn::getNum() const {return *num;}
Mat TFn::getDen() const {return *den;}
void TFn::setMethod(int i){method = 1;}	
int  TFn::getMethod()const {return method;}
void TFn::setDt(double inp){dt = inp;}
double TFn::getDt() const{return dt;}
Mat TFn::getBufx() const {return *bufx;}
Mat TFn::getBufy() const {return *bufy;}

void TFn::Bufs()
{
	/*
	int sn,sd;
	if ((!num)||(!den)) {return;} 
	if (num->rsize() == 1) sn = num->csize();
	else sn = num->rsize();
	if (den->rsize() == 1) sd = den->csize();
	else sd = den->rsize();

	int diff = sd-sn,quon;
	if (diff > 0)
	{
		rem = new Mat(getDen());
		rem->set(0);
		for (int i=1;i<=sn;i++) rem->set(i+diff,num->get(i));
		quo = new Mat(1,1);
		quon = 0;
	}
	else
	{	
		int order = -diff,j=0,k;
		Mat trem(*num);
		quo = new Mat(1,1);
		rem = new Mat(1,1);
		while (order >= 0)
		{	quo->set(j+1,trem.get(j+1)/den->get(1));
			for (k=1;k<=sd;k++) trem.set(k+j,trem.get(k+j) - quo->get(j+1)*den->get(k));
			order--;j++;
		}
		
		trem.show();
		for (k=1;k<sd;k++) rem->set(k+1,trem.get(k+j));
		quon = -diff+1;
	}

	unsigned int bufn = IMAX(quon,sd-1);
	buf = new Mat(1,bufn);
	*/

	if ((!num)||(!den)) {return;} 
	if (num->rsize() == 1) sx = num->csize();
	else sx = num->rsize();
	if (den->rsize() == 1) sy = den->csize();
	else sy = den->rsize();

	if (num) {bufx = new Mat(sx,1);}
	if (den) {bufy = new Mat(sy-1,1);} // what if sy = 1
	mx = 0;
	my = 0;
}

double TFn::IO(double val,double delt)
{
	if ((!num)||(!den)||(!bufx)||(!bufy)) {cout << "No Input Allocated !!!" << endl;exit(0);}
	
	dt = delt;
	bufx->set(mx+1,val);
	x = 0;
	for (int i=0;i<sx;i++)
	{
		int itr = (mx-i)%sx;
		double bary[sx-i-1];
		double coeff = num->get(i+1);
		binomial_neg(bary,sx-i-1);
		for (int j=0;j<sx-i;j++) 
		{
			// cout << "coeff :  " << coeff << " bary: " << bary[j] << " indx :  ";
			// cout << (mx-j)%sx + 1 << " value: " << bufx->get((mx-j)%sx + 1) << endl;
			x = x + coeff*bary[j]*bufx->get((mx-j)%sx + 1)/pow(dt,sx-i-1);
		}
		// cout << "OUT : " << x << endl; 
	}	
	
	mx = (mx+1)%sx;

	if (sy == 1) return x;
	else
	{
		if (method == 0)
		{
			Mat u(sy-1,1);
			u.set(sy-1,x); 
			bufy->set(rk4_tf(*bufy,u,dt));
		}
		if (method == 1)
		{
			Mat u(sy-1,1);
			u.set(sy-1,x); 
			bufy->set(dgl_tf(*bufy,u,dt));
		}
	}
	return bufy->get(1);
}

void TFn::binomial_neg(double outarray[],int n)
{
 if (n == 0)
  outarray[0] = 1;
 if (n == 1)
 {outarray[0] = 1;outarray[1] = -1;}
 if (n >= 2)
 {
  double initial[] = {1,-1};
  outarray[0] = 1;outarray[1] = -1;
  int i;
  for(i=0;i<n-1;++i)
    conv(outarray,outarray,initial,i+2,arraysize(initial));
 }
}

void TFn::binomial_pos(double outarray[],int n)
{
 if (n == 0)
  outarray[0] = 1;
 if (n == 1)
 {outarray[0] = 1;outarray[1] = 1;}
 if (n >= 2)
 {
  double initial[] = {1,1};
  outarray[0] = 1;outarray[1] = 1;
  int i;
  for(i=0;i<n-1;++i)
    conv(outarray,outarray,initial,i+2,arraysize(initial));
 }
}

void TFn::conv(double out[],double in1[],double in2[],int n1,int n2)
{
 int n,i,j,k;
 n = n1 + n2 - 1;
 double temp[n1][n];
 for(i=0;i<n1;++i)
   {for(j=0;j<n;++j)
      temp[i][j] = 0.0;
   }	  
 for(i=0;i<n1;++i)
   {for(j=0;j<n2;++j)
      {
	   temp[i][i+j] = in2[j]*in1[i];
	  }
   }
  for(i=0;i<n;++i)
    out[i] = 0.0;
  for(i=0;i<n;++i)
    {for(j=0;j<n1;++j)
	   out[i] = out[i] + temp[j][i];
    }
	
}

Mat TFn::tfdyn(Mat inp)
{
	Mat A(sy-1,sy-1);
	if (sy == 2)
	{
		A.set(1,1,-den->get(2));
	}
	else 
	{
		A.set(1,2,eye(sy-2));
		for (int i=2;i<=sy;i++) A.set(sy-1,sy-i+1,-den->get(i));
	}
	if (bufy->csize() == 1)	return A*inp;
	else return ~(A*(~inp));
}

Mat TFn::rk4_tf(Mat x,Mat u,double ts)
{int i;
 // x should be column matrix
        Mat k1,k2,k3,k4,kd,xtemp;
	Mat u1;
	if (x.csize()!=u.csize()) u1 = ~u;
	else u1 = u;

        xtemp = x;
        kd = tfdyn(xtemp);
        k1 = (kd+u1)*ts;

        xtemp = x + k1/2.0;
        kd = tfdyn(xtemp);
        k2 = (kd+u1)*ts;

        xtemp = x + k2/2.0;
        kd = tfdyn(xtemp);
        k3 = (kd+u1)*ts;

        xtemp = x + k3;
        kd = tfdyn(xtemp);
        k4 = (kd+u1)*ts;

        return x + (k1 + 2.0*k2 + 2.0*k3 + k4)/6.0;
}

Mat TFn::dgl_tf(Mat x,Mat u,double ts)
{int i;
 double s2 = sqrt(2.0);	
 // x should be column matrix
        Mat k1,k2,k3,k4,kd,xtemp;
	Mat u1;
	if (x.csize()!=u.csize()) u1 = ~u;
	else u1 = u;
	
        xtemp = x;
        kd = tfdyn(xtemp);
        k1 = (kd+u1)*ts;

        xtemp = x + k1/2.0;
        kd = tfdyn(xtemp);
        k2 = (kd+u1)*ts;

        xtemp = x + (-1.0 + s2)*k1/2.0 + (1.0 - s2/2.0)*k2;
        kd = tfdyn(xtemp);
        k3 = (kd+u1)*ts;

        xtemp = x -s2*k2/2.0 + (1.0 + s2/2.0)*k3;
        kd = tfdyn(xtemp);
        k4 = (kd+u1)*ts;

        return x + (k1 + (2.0 - s2)*k2 + (2.0 + s2)*k3 + k4)/6.0;

}
