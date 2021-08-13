#include "MAT.h"

using namespace std;
static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ? (iminarg1) : (iminarg2))
#define IMAX(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) > (iminarg2) ? (iminarg1) : (iminarg2))

// set is done so that array elemnts has to start from 1
Mat::Mat()
{
	r = 0;
	c = 0;
	val = NULL;
}

Mat::Mat(int row,int col)
{
	allocate(row,col);
	for (int i=0;i<row;i++)
	{
		for (int j=0;j<col;j++)
		val[i][j] = 0;
	}
}

Mat::Mat(const Mat& inp)
{
	allocate(inp.r,inp.c);
  	for (int i=0; i<r; i++)
    	memcpy(val[i],inp.val[i],inp.c*sizeof(double));
}

Mat::~Mat()
{
	release();
}

int Mat::rsize() const {return this->r;}
int Mat::csize() const {return this->c;}

void Mat::size(){cout << r << " X " << c << endl;}

void Mat::show()
{
	if ((r != 0)&&(c != 0))
	{
		for (int i=0;i<r;i++)
		{
			for (int j=0;j<c;j++)
			cout << " " <<  val[i][j];
			cout << "\n";
		}
		return;
	}

	if (WARNING) cout << "warning: Nothing there to show(show())\n";
}

void Mat::reset(int row,int col)
{
	if ((row == 0)||(col == 0))
	{
	        if (WARNING) cout << "warning: data is lost, Careful eh!!(reset(r,c))\n";
		release();
		allocate(0,0);
		return;
	}
	if ((row < r)||(col < c))       
        if (WARNING) cout << "warning: Truncating the matrix, data is lost, Careful eh!!(reset(r,c))\n";

	Mat temp(row,col);
        for (int i=0,imax = IMIN(row,r);i<imax;i++)
        { 
        	for (int j=0,jmax = IMIN(col,c);j<jmax;j++)
                temp.val[i][j] = val[i][j];
        }

	release();
	allocate(row,col);
	for (int i=0; i<r; i++)
        memcpy(val[i],temp.val[i],temp.c*sizeof(double));
	
}

void Mat::set(double value)
{
	if (val != NULL)
	{
		for (int i=0;i<r;i++)
		{
			for (int j=0;j<c;j++)
			val[i][j] = value;
		}
	}
	else
	{
		allocate(1,1);
		set(value);
	}
}

void Mat::set(int pos,double value)
{
	int p = abs(pos);
	if (p == 0)
	{
		if (WARNING) cout << "error: Boss access starts from 1 not 0!!!(set(pos,data))\n";
		return;
	}
	if ((r == 1)||(c == 1))
	{
		if (r == 1)
		{
			if (p > c)
			{
				reset(1,p);
				val[0][p-1] = value;
				return;
			}
			val[0][p-1] = value;
			return; 
		}
		if (c == 1)
		{
			if (p > r)
			{
				reset(p,1);
				val[p-1][0] = value;
				return;
			}
			val[p-1][0] = value;
			return;
		}
	}
	else if (val == 0)
	{
		if (WARNING) cout << "warning: Making a row matrix(set(pos,data))\n";
		reset(1,p);
		val[0][p-1] = value;
		return;
	}
	else
	{
		if (WARNING) cout << "error: Man you tried to update matrix which is not a row/column matrix, so no changes done(set(pos,data))\n";
		return;
	}

}

void Mat::set(int ro,int cl,double value)
{
	int row = abs(ro);
	int col = abs(cl);
	
	if ((row == 0)||(col == 0))
        {
                if (WARNING) cout << "error: Boss access starts from 1 not 0!!!(set(r,c,v))\n";
                return;
        }
	
	if ((row < r)&&(col < c))
	val[row-1][col-1] = value;
	else
	{
		if (WARNING) cout << "warning: changing the matrix size(set(r,c,v))\n";
		reset(IMAX(row,r),IMAX(col,c));
		val[row-1][col-1] = value;
	}

	
}

void Mat::set(const Mat& inp)
{

	if (this != &inp)
	{
		release();
		allocate(inp.r,inp.c);	
		for (int i=0; i<r; i++)
        	memcpy(val[i],inp.val[i],inp.c*sizeof(double));
	}
}

void Mat::set(int ro,int cl,const Mat& inp)
{
	int row = abs(ro);
	int col = abs(cl);

	if ((row == 0)||(col == 0))
	{
		if (WARNING) cout << "error: start index stats from 1 not zero(set(i,i,i,i,mat))\n";
		return;
	}
	if ((this == &inp)&&(row == 1)&&(col == 1))
	{
		if (WARNING) cout << "warning: nothing is done boss why do you want to copy same matrix into same matrix(set(i,i,i,i,mat))\n";
		return; 
	}
	int imax = row+inp.r;
	int jmax = col+inp.c;

	if ((imax <= r+1)&&(jmax <= c+1))
	{
		for (int i=row;i<imax;i++)
		{
			for (int j=col;j<jmax;j++)
			val[i-1][j-1] = inp.val[i-row][j-col];
		}
		return;
	}

	reset(IMAX(imax-1,r),IMAX(jmax-1,c));
	for (int i=row;i<imax;i++)
        {
        	for (int j=col;j<jmax;j++)
               	val[i-1][j-1] = inp.val[i-row][j-col];
        }

	return;

}

double Mat::get(int ro,int cl)
{
	int row = abs(ro);
	int col = abs(cl);

	if ((row == 0)||(col == 0))
	{
		if (WARNING) cout << "error: index start from 1 not 0 (get(row,col))\n";
		return 0;
	}
	if (val == 0)
	{
		if (WARNING) cout << "error: empty matrix (get(row,col))\n";
		return 0; 
	}
	if ((row > r)||(col > c))
	{
		if (WARNING) cout << "error: accessing outside the matrix limits (get(row,col))\n";
		return 0;
	}

	return val[row-1][col-1];
}

double Mat::get(int pos)
{
	int p = abs(pos);
	if (val == NULL)
	{
                if (WARNING) cout << "error: empty matrix (get(pos))\n";
                return 0;
        }
	if (p == 0)
        {
                if (WARNING) cout << "error: index start from 1 not 0 (get(pos))\n";
                return 0;
        }
	if ((r != 1)&&(c != 1))
	{
		if (WARNING) cout << "error: not a column/row matrix (get(pos))\n";
		return 0;
	}
	if (r == 1)
	{
		if (p > c) 
		{
			if (WARNING) cout << "error: traccessing outside the matrix limits (get(pos))\n";
			return 0;
		}
		else    return val[0][p-1];
	}
	if (c == 1)
        {
                if (p > r) 
		{
			if (WARNING) cout << "error: traccessing outside the matrix limits (get(pos))\n";
                	return 0;
		}
		else    return val[p-1][0];
        }
}


Mat Mat::row(int ro)
{
	int roo = abs(ro);
	Mat local;
	if (roo == 0)
	{
		if (WARNING) cout << "row index starts from 0 (row(rows))\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	} 
	if (roo > r)
	{
		if (WARNING) cout << "row out of matrix bound (row(rows))\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(1,c);
	for (int i=0;i<c;i++) local.val[0][i] = val[roo-1][i];
	return local;
}

Mat Mat::col(int cl)
{
        int clm = abs(cl);
        Mat local;
        if (clm == 0)
        {
                if (WARNING) cout << "col index starts from 0 (col(cols))\n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        } 
        if (clm > c)
        {
                if (WARNING) cout << "col out of matrix bound (col(cols))\n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        local.reset(r,1);
        for (int i=0;i<r;i++) local.val[i][0] = val[i][clm-1];
        return local;
}

Mat Mat::submat(int ro_st,int cl_st,int ro_sp,int cl_sp)
{
	int row_st = abs(ro_st);
	int col_st = abs(cl_st);
	int row_sp = abs(ro_sp);
	int col_sp = abs(cl_sp);

	Mat local;

	if ((row_st == 0)||(col_st == 0)||(row_sp == 0)||(col_sp == 0))
	{
		if (WARNING) cout << "you entered 0 in indeces - indeces start from 1(submat)\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}

	if ((row_st > row_sp)||(col_st > col_sp)||(row_sp > r)||(col_sp > c))
	{
		if (WARNING) cout << "index out of bounds (submat)\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}

	int imax = row_sp-row_st+1;
	int jmax = col_sp-col_st+1;
	local.reset(imax,jmax);

	for (int i=0;i<imax;i++)
	{
		for (int j=0;j<jmax;j++)
		local.val[i][j] = val[i+row_st-1][j+col_st-1];
	}
	
	return local;

}

Mat Mat::zero()
{
	Mat local(r,c);
	return local;
}

Mat Mat::Const(double value)
{
	Mat local(r,c);
	local.set(value);
	return local;
}

Mat Mat::eye()
{
	Mat local;
	if (r != c)
	{
		if (WARNING) cout << "matrix is a rectagular matrix\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(r,r);
	for (int i=0;i<r;i++)
	{
		for (int j=0;j<r;j++)
		{
			if (i == j)local.val[i][j] = 1;
			else local.val[i][j] = 0;
		}
	}
	return local;
}

Mat Mat::eyec()
{
	Mat local(c,c);
	for (int i=0;i<c;i++)
        {
                for (int j=0;j<c;j++)
                {
                        if (i == j)local.val[i][j] = 1;
                        else local.val[i][j] = 0;
                }
        }
        return local;
}

Mat Mat::eyer()
{
        Mat local(r,r);
        for (int i=0;i<r;i++)
        {
                for (int j=0;j<r;j++)
                {
                        if (i == j)local.val[i][j] = 1;
                        else local.val[i][j] = 0;
                }
        }
        return local;
}

Mat Mat::diag()
{
	Mat local;
        if (r != c)
        {
                if (WARNING) cout << "matrix is a rectagular matrix\n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        local.reset(1,r);
        for (int i=0;i<r;i++)
        {
                for (int j=0;j<r;j++)
                {
                        if (i == j)local.val[0][j] = val[i][i];
                }
        }
        return local;
}



void Mat::close()
{
	release();
	r=0;
	c=0;
}

Mat Mat::inv()
{
	Mat local(*this);
	if (local.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty (inv)\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	if (local.r != local.c)
	{
		if (WARNING) cout << "Matrix is not a square Matrix (inv)\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	int pivot;	
	double factor;
	local.set(1,local.c+1,this->eye());
	for (int i=0,imax=local.r;i<imax;i++)
	{
		pivot = -1;
		for (int j=i,jmax=local.r;j<jmax;j++)
		{
			if (fabs(local.val[j][i]) > 0)
			{
				pivot = j;
				break;
			}
		}

		if (pivot == -1)
		{
			if (WARNING) cout << "Matrix is singular (inv)\n";	
			cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
		}
		if (pivot != i)
		{
			double temp;
			for (int j=0,jmax=local.c;j<jmax;j++)
			{
				temp = local.val[i][j];
				local.val[i][j] = local.val[pivot][j];
				local.val[pivot][j] = temp;
			}
		}

		// remove lower triangular matrix
		
		for (int j=i+1,jmax=local.r;j<jmax;j++)
		{
			factor = local.val[j][i]/local.val[i][i];
			for (int k=0,kmax=local.c;k<kmax;k++)
			local.val[j][k] = local.val[j][k] - factor*local.val[i][k];
		}
	}

	// remove upper triangular matrix
	for (int i=local.r-1;i>-1;--i)
   	{
    		for (int j=i-1;j>-1;--j)
     		{
			factor = local.val[j][i]/local.val[i][i];
             		for (int k=0;k<local.c;++k)
		        local.val[j][k] = local.val[j][k] - local.val[i][k]*factor;
		}
      	}

	// diagonal elements to 1
	
	for (int i=0;i<local.r;++i)
   	{
		factor = 1/local.val[i][i];
    		for (int j=0;j<local.c;++j)
     		local.val[i][j] = local.val[i][j]*factor;
	}

	return local.submat(1,local.r+1,local.r,local.c);
}

Mat& Mat::operator= (const Mat& inp) 
{
  	if (this!=&inp) 
	{
		this->release();
		this->allocate(inp.r,inp.c);
		for (int i=0; i<r; i++)
                memcpy(this->val[i],inp.val[i],inp.c*sizeof(double));
	}	
  	return *this;
}

double& Mat::operator()(unsigned int row,unsigned int col)
{
	if ((row == 0)||(col == 0))
        {
                if (WARNING) cout << "error: index start from 1 not 0\n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        if (val == 0)
        {
                if (WARNING) cout << "error: empty matrix\n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        if ((row > r)||(col > c))
        {
                if (WARNING) cout << "error: accessing outside the matrix limits\n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }

        return val[row-1][col-1];
}

double& Mat::operator()(unsigned int p)
{
	if (val == NULL)
        {
                if (WARNING) cout << "error: empty matrix \n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        if (p == 0)
        {
                if (WARNING) cout << "error: index start from 1 not 0 \n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        if ((r != 1)&&(c != 1))
        {
                if (WARNING) cout << "error: not a column/row matrix \n";
                cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
        }
        if (r == 1)
        {
                if (p > c)
                {
                        if (WARNING) cout << "error: traccessing outside the matrix limits \n";
                        cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
                }
                else    return val[0][p-1];
        }
        if (c == 1)
        {
                if (p > r)
                {
                        if (WARNING) cout << "error: traccessing outside the matrix limits \n";
                        cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
                }
                else    return val[p-1][0];
        }
}

void Mat::allocate(int row,int col)
{
	r = abs(row);c = abs(col);
	if ((r == 0)||(c == 0))
	{
		if (WARNING) cout << "warning : no memory allocated\n";
		val = NULL;
		return;
	}

	val    = (double**)malloc(r*sizeof(double*));
  	val[0] = (double*)calloc(r*c,sizeof(double));
  	for(int i=1; i<r; i++)
    	val[i] = val[i-1]+c;
}

void Mat::release() 
{
  if (val!=0) {
    free(val[0]);
    free(val);
  }
}

Mat operator+(const Mat lhs,double rhs)
{
	Mat local;
	if (lhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty +\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(lhs.rsize(),lhs.csize());
	for (int i=0,imax=lhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=lhs.csize();j<jmax;j++)
		local.val[i][j] = lhs.val[i][j] + rhs;
	}
	return local;
}

Mat operator+(double lhs,const Mat rhs)
{
	Mat local;
	if (rhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty +\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(rhs.rsize(),rhs.csize());
	for (int i=0,imax=rhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=rhs.csize();j<jmax;j++)
		local.val[i][j] = lhs + rhs.val[i][j];
	}
	return local;
}

Mat operator+(const Mat lhs,const Mat rhs)
{
	Mat local;
	
	if ((lhs.val == NULL)||(rhs.val == NULL))
	{
		if (WARNING) cout << "Matrix is empty +\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	if ((lhs.rsize() != rhs.rsize())||(lhs.csize() != rhs.csize()))
	{
		if (WARNING) cout << "matrices are of different order +\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(rhs.rsize(),rhs.csize());
	for (int i=0,imax=rhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=rhs.csize();j<jmax;j++)
		local.val[i][j] = lhs.val[i][j] + rhs.val[i][j];
	}
	return local;
}

Mat operator-(const Mat lhs,double rhs)
{
	Mat local;
	if (lhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty -\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(lhs.rsize(),lhs.csize());
	for (int i=0,imax=lhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=lhs.csize();j<jmax;j++)
		local.val[i][j] = lhs.val[i][j] - rhs;
	}
	return local;
}

Mat operator-(double lhs,const Mat rhs)
{
	Mat local;
	if (rhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty -\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(rhs.rsize(),rhs.csize());
	for (int i=0,imax=rhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=rhs.csize();j<jmax;j++)
		local.val[i][j] = lhs - rhs.val[i][j];
	}
	return local;
}

Mat operator-(const Mat lhs,const Mat rhs)
{
	Mat local;
	
	if ((lhs.val == NULL)||(rhs.val == NULL))
	{
		if (WARNING) cout << "Matrix is empty -\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	if ((lhs.rsize() != rhs.rsize())||(lhs.csize() != rhs.csize()))
	{
		if (WARNING) cout << "matrices are of different order -\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(rhs.rsize(),rhs.csize());
	for (int i=0,imax=rhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=rhs.csize();j<jmax;j++)
		local.val[i][j] = lhs.val[i][j] - rhs.val[i][j];
	}
	return local;
}

Mat operator-(const Mat inp)
{
	Mat local;
	if (inp.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty ~";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(inp.rsize(),inp.csize());

	for (int i=0,imax=inp.rsize();i<imax;i++)
	{
		for (int j=0,jmax=inp.csize();j<jmax;j++)
		local.val[i][j] = -inp.val[i][j];
	}
	return local;
}
Mat operator*(const Mat lhs,double rhs)
{
	Mat local;
	if (lhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty *\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(lhs.rsize(),lhs.csize());
	for (int i=0,imax=lhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=lhs.csize();j<jmax;j++)
		local.val[i][j] = lhs.val[i][j]*rhs;
	}
	return local;
}

Mat operator*(double lhs,const Mat rhs)
{
	Mat local;
	if (rhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty *\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(rhs.rsize(),rhs.csize());
	for (int i=0,imax=rhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=rhs.csize();j<jmax;j++)
		local.val[i][j] = lhs*rhs.val[i][j];
	}
	return local;
}

Mat operator*(const Mat lhs,const Mat rhs)
{
	Mat local;
	
	if ((lhs.val == NULL)||(rhs.val == NULL))
	{
		if (WARNING) cout << "Matrix is empty *\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	if ((lhs.csize() != rhs.rsize()))
	{
		if (WARNING) cout << "matrices cannot be multiplied *\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(lhs.rsize(),rhs.csize());
	for (int i=0,imax=lhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=rhs.csize();j<jmax;j++)
		{
			local.val[i][j] = 0;
			for (int k=0,kmax=lhs.csize();k<kmax;k++)
			local.val[i][j] = local.val[i][j] + lhs.val[i][k]*rhs.val[k][j];
		}
	}
	return local;
}

Mat operator/(const Mat lhs,double rhs)
{
	Mat local;
	if (lhs.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty /\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	if (rhs == 0) {if (WARNING) cout << "Divison by zero\n";}
	local.reset(lhs.rsize(),lhs.csize());
	for (int i=0,imax=lhs.rsize();i<imax;i++)
	{
		for (int j=0,jmax=lhs.csize();j<jmax;j++)
		local.val[i][j] = lhs.val[i][j]/rhs;
	}
	return local;
}

Mat operator~(const Mat inp)
{
	Mat local;
	if (inp.val == NULL)
	{
		if (WARNING) cout << "Matrix is empty ~";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	local.reset(inp.csize(),inp.rsize());
	for (int i=0,imax=inp.rsize();i<imax;i++)
	{
		for (int j=0,jmax=inp.csize();j<jmax;j++)
		local.val[j][i] = inp.val[i][j];
	}
	return local;
}

Mat eye(int n)
{
	Mat local(n,n);
	if (n == 0)
	{
		if (WARNING) cout << "No identity marix with zero elements (eye)\n";
		cout << "Exiting in : " << __FILE__ << " @  " << __LINE__ << " in func : " << __builtin_FUNCTION() << " " << __FUNCTION__ << endl;exit(0);
	}
	return local.eye();
}
Mat fnorm(const Mat inp)
{
	Mat local(inp);
	double sum = 0;
	for (int i=0;i<local.rsize();i++)
	{
		for (int j=0;j<local.csize();j++)
		sum = sum + local.val[i][j]*local.val[i][j];
	}
	sum = sqrt(sum);
	return local/sum;
}

void MATsize(const Mat inp){cout << inp.rsize() << " X " << inp.csize() << endl;}
