#include "QUAT.h"

using namespace std;

Quat::Quat()
{
	Q[0] = 0.0;
	Q[1] = 0.0;
	Q[2] = 0.0;
	Q[3] = 1.0;
	norm();
	
	IR1 = 3;IR2 = 2;IR3 = 1;
	ComputeDcm();
	ComputeEul();
}

Quat::Quat(double q1,double q2,double q3)
{
	Q[0] = q1;
        Q[1] = q2;
        Q[2] = q3;
	double temp = (1 - Q[0]*Q[0] - Q[1]*Q[1] - Q[2]*Q[2]);
	if (temp > 0)
	Q[3] = sqrt(temp);
	else
	{
		cout << " Constructor:: q1^2 + q2^2 + q3^2 in 3 quaternion input > 1,q4 is set to 0\n";
		Q[3] = 0.0;
	} 
	norm();
	IR1 = 3;IR2 = 2;IR3 = 1;
	ComputeDcm();
	ComputeEul();
}

Quat::Quat(double q1,double q2,double q3,double q4)
{
	Q[0] = q1;
	Q[1] = q2;
	Q[2] = q3;
	Q[3] = q4;

	norm();
	IR1 = 3;IR2 = 2;IR3 = 1;
        ComputeDcm();
        ComputeEul();
}

Quat::~Quat()
{
}

Quat& Quat::operator= (const Quat &Qin)
{
	if (this!=&Qin) 
	{
		
		this->Q[0] = Qin.Q[0];
		this->Q[1] = Qin.Q[1];
		this->Q[2] = Qin.Q[2];
		this->Q[3] = Qin.Q[3];
		norm();
		IR1 = 3;IR2 = 2;IR3 = 1;
		ComputeDcm();
		ComputeEul();
	}
    
  return *this;
}

Quat  Quat::operator+ (const Quat &Qin)
{
	Quat local = (*this)*Qin;
	return local;
	
}

Quat  Quat::operator- (const Quat &Qin )
{
	Quat local = ~(*this)*Qin;
	return local;
}

Quat  Quat::operator* (const Quat &Qin)
{
	double q1[4],q2[4],qf[4];
	int i,j;
	for (i=0;i<4;i++)
	{
		q1[i] = this->Q[i];
		q2[i] = Qin.Q[i];
	}
	double Qmat[4][4] =	{{q2[3],  q2[2],   -q2[1],  q2[0]},
                    		{-q2[2] ,q2[3],   q2[0]  , q2[1]},
                    		{q2[1]  ,-q2[0],  q2[3] ,  q2[2]},
                    		{-q2[0] , -q2[1] ,-q2[2]  ,q2[3]}};

	Quat local;
	
 	for (i = 0;i<4;i++)
 	{qf[i] = 0;
  	 for (j = 0;j<=3;++j){qf[i] = qf[i] + Qmat[i][j]*q1[j];}}

	local.setq(qf[0],qf[1],qf[2],qf[3]);
	return local;
}


Quat  Quat::operator~ (void)
{
	Quat local;
	local.setq(-this->Q[0],-this->Q[1],-this->Q[2],this->Q[3]);
	local.norm();
	local.ComputeDcm();
        local.ComputeEul();
	return local;
}

Quat  Quat::operator/ (Quat &Qin)  // q1/q2 = q1 * ~q2
{
	Quat local = (*this)*(~Qin);
	return local;
}


void Quat::norm()
{
	int i;
 	double Qmag,coeff;
 	Qmag = sqrt(Q[0]*Q[0] + Q[1]*Q[1] + Q[2]*Q[2] + Q[3]*Q[3]);
 	if (Q[3] != 0.0) coeff = sign(Q[3])/Qmag;
 	else coeff = 1.0/Qmag;
 	for (i=0;i<4;++i){Q[i] = coeff*Q[i];}
}

double Quat::sign(double value)
{
	if (value > 0.0) return 1.0;
	else if (value == 0.0) return 0.0;
	else return -1.0;
}

void Quat::ComputeDcm()
{

	 DCM[0][0] = Q[0]*Q[0]-Q[1]*Q[1]-Q[2]*Q[2]+Q[3]*Q[3];
	 DCM[0][1] = 2.0*(Q[0]*Q[1]+Q[2]*Q[3]);
	 DCM[0][2] = 2.0*(Q[0]*Q[2]-Q[1]*Q[3]);
	 DCM[1][0] = 2.0*(Q[0]*Q[1]-Q[2]*Q[3]);
	 DCM[1][1] = Q[1]*Q[1]-Q[0]*Q[0]-Q[2]*Q[2]+Q[3]*Q[3];
	 DCM[1][2] = 2.0*(Q[1]*Q[2]+Q[0]*Q[3]);
	 DCM[2][0] = 2.0*(Q[0]*Q[2]+Q[1]*Q[3]);
	 DCM[2][1] = 2.0*(Q[1]*Q[2]-Q[0]*Q[3]);
	 DCM[2][2] = Q[2]*Q[2]-Q[0]*Q[0]-Q[1]*Q[1]+Q[3]*Q[3];

}

void Quat::ComputeEul()
{
      double TEMP,ANG[3];
      ComputeDcm();
      if((IR1==1)&&(IR2==2)&&(IR3==3))
       {
        TEMP = sqrt(DCM[2][1]*DCM[2][1]+DCM[2][2]*DCM[2][2]);
        ANG[0] =-atan2(DCM[2][1],DCM[2][2]) ;
        ANG[1] = atan2(DCM[2][0],TEMP);
        ANG[2] =-atan2(DCM[1][0],DCM[0][0]);
       }
      else if((IR1==1)&&(IR2==3)&&(IR3==2))
       {
        TEMP =sqrt(DCM[0][0]*DCM[0][0]+DCM[2][0]*DCM[2][0]);
        ANG[0] = atan2(DCM[1][2],DCM[1][1]);
        ANG[1] = atan2(DCM[2][0],DCM[0][0]);
        ANG[2] =-atan2(DCM[1][0],TEMP);
       }
      else if((IR1==2)&&(IR2==3)&&(IR3==1))
       {
        TEMP = sqrt(DCM[0][0]*DCM[0][0]+DCM[0][2]*DCM[0][2]);
        ANG[0] =-atan2(DCM[2][1],DCM[1][1]);
        ANG[1] =-atan2(DCM[0][2],DCM[0][0]);
        ANG[2] = atan2(DCM[0][1],TEMP);
       }
      else if((IR1==2)&&(IR2==1)&&(IR3==3))
       {
        TEMP = sqrt(DCM[2][0]*DCM[2][0]+DCM[2][2]*DCM[2][2]);
        ANG[0] =-atan2(DCM[2][1],TEMP);
        ANG[1] = atan2(DCM[2][0],DCM[2][2]);
        ANG[2] = atan2(DCM[0][1],DCM[1][1]);
       }
      else if((IR1==3)&&(IR2==2)&&(IR3==1))
       {
        TEMP = sqrt(DCM[1][2]*DCM[1][2]+DCM[2][2]*DCM[2][2]);
        ANG[0] = atan2(DCM[1][2],DCM[2][2]);
        ANG[1] =-atan2(DCM[0][2],TEMP);
        ANG[2] = atan2(DCM[0][1],DCM[0][0]);
       }
     else if((IR1==3)&&(IR2==1)&&(IR3==2))
       {
        TEMP = sqrt(DCM[0][2]*DCM[0][2]+DCM[2][2]*DCM[2][2]);
        ANG[0] = atan2(DCM[1][2],TEMP);
        ANG[1] =-atan2(DCM[0][2],DCM[2][2]);
        ANG[2] =-atan2(DCM[1][0],DCM[1][1]);
       }

     EUL[0] = ANG[0];EUL[1] = ANG[1];EUL[2] = ANG[2];	
}

void Quat::SetRotSeq(int i1,int i2,int i3)
{
	if ((IR1>3)||(IR1<1)||(IR2>3)||(IR2<1)||(IR3>3)||(IR3<1)) 
	{
		cout << "yaar rotation sequence out size 1 and 3 check for input greater than 3 or less than 1\n";
		return;
	}
	IR1 = i1;IR2 = i2;IR3 = i3;
}

double Quat::eul(int n)
{
	if ((n > 3)||(n < 1))
	{
		cout << "Abey enter number between 1 & 3 for euler angles!!!\n";
		return INT_MAX;
	}

	return EUL[n-1];	
}

double Quat::dcm(int n,int m)
{
	if ((n > 3)||(n < 1)||(m > 3)||(m < 1))
	{
		cout << "kya yaar DCM indeces, 1 & 3 ke beech me hota hain !!! ye bhi nehi pata\n";
		return INT_MAX;
	}
	
	return DCM[n-1][m-1];

}

Mat Quat::dcm()
{
	Mat ret(3,3);
	for (int i=1;i<=3;i++)
	{
		for (int j=1;j<=3;j++)
		ret(i,j) = DCM[i-1][j-1];
	}
	return ret;
}

void Quat::sete(double e1,double e2,double e3)
{
	Quat Q1,Q2,Q3,Qf;
  	int i; 

      	Q1.setq(sin(0.5*e1),0,0,cos(0.5*e1));
	Q2.setq(0,sin(0.5*e2),0,cos(0.5*e2));
	Q3.setq(0,0,sin(0.5*e3),cos(0.5*e3));   
      
    	if((IR1==1)&&(IR2==2)&&(IR3==3)) Qf = Q1*Q2*Q3;
    	else if ((IR1==1)&&(IR2==3)&&(IR3==2)) Qf = Q1*Q3*Q2;
    	else if ((IR1==2)&&(IR2==3)&&(IR3==1)) Qf = Q2*Q3*Q1;
    	else if((IR1==2)&&(IR2==1)&&(IR3==3)) Qf = Q2*Q1*Q3;
    	else if((IR1==3)&&(IR2==2)&&(IR3==1)) Qf = Q3*Q2*Q1;
    	else if((IR1==3)&(IR2==1)&(IR3==2)) Qf = Q3*Q1*Q2; 

	this->setq(Qf.q(1),Qf.q(2),Qf.q(3),Qf.q(4));

}

void Quat::sete(double ANG[3])
{
	Quat Q1,Q2,Q3,Qf;
  	int i; 

      	Q1.setq(sin(0.5*ANG[0]),0,0,cos(0.5*ANG[0]));
	Q2.setq(0,sin(0.5*ANG[1]),0,cos(0.5*ANG[1]));
	Q3.setq(0,0,sin(0.5*ANG[2]),cos(0.5*ANG[2]));   
      
    	if((IR1==1)&&(IR2==2)&&(IR3==3)) Qf = Q1*Q2*Q3;
    	else if ((IR1==1)&&(IR2==3)&&(IR3==2)) Qf = Q1*Q3*Q2;
    	else if ((IR1==2)&&(IR2==3)&&(IR3==1)) Qf = Q2*Q3*Q1;
    	else if((IR1==2)&&(IR2==1)&&(IR3==3)) Qf = Q2*Q1*Q3;
    	else if((IR1==3)&&(IR2==2)&&(IR3==1)) Qf = Q3*Q2*Q1;
    	else if((IR1==3)&(IR2==1)&(IR3==2)) Qf = Q3*Q1*Q2; 

	this->setq(Qf.q(1),Qf.q(2),Qf.q(3),Qf.q(4));
}

void Quat::setd(double dcm[3][3])
{
	double tr,pp[4],maxp,p[4];
 	tr = dcm[0][0]+dcm[1][1]+dcm[2][2];
 	pp[0]=1+tr;
 	pp[1]=1+2*dcm[0][0]-tr;
 	pp[2]=1+2*dcm[1][1]-tr;
 	pp[3]=1+2*dcm[2][2]-tr;

 	maxp=maximum(pp,4);

 	if (maxp == pp[0]) 
   	{
		p[0]=sqrt(pp[0]);
    	 	p[1]=(dcm[1][2]-dcm[2][1])/p[0];
    	 	p[2]=(dcm[2][0]-dcm[0][2])/p[0];
    	 	p[3]=(dcm[0][1]-dcm[1][0])/p[0];
	}
 	else if (maxp == pp[1])
   	{
		p[1]=sqrt(pp[1]);
    		p[0]=(dcm[1][2]-dcm[2][1])/p[1];
    		p[2]=(dcm[1][0]+dcm[0][1])/p[1];
    		p[3]=(dcm[0][2]+dcm[2][0])/p[1];
	} 
 	else if (maxp == pp[2])
   	{
		p[2]=sqrt(pp[2]);
    		p[0]=(dcm[2][0]-dcm[0][2])/p[2];
    		p[1]=(dcm[1][0]+dcm[0][1])/p[2];
    		p[3]=(dcm[2][1]+dcm[1][2])/p[2];
	}
 	else if (maxp == pp[3])
   	{
		p[3]=sqrt(pp[3]);
    		p[0]=(dcm[0][1]-dcm[1][0])/p[3];
    		p[1]=(dcm[0][2]+dcm[2][0])/p[3];
    		p[2]=(dcm[2][1]+dcm[1][2])/p[3];
	}

	if  (p[0]<0.0) this->setq(-0.5*p[1],-0.5*p[2],-0.5*p[3],-0.5*p[0]);
	else this->setq(0.5*p[1],0.5*p[2],0.5*p[3],0.5*p[0]);
}

void Quat::setd(const Mat dcm)
{
	if ((dcm.rsize() != 3)||(dcm.csize() != 3))
	{
		cout << "matrix dimension incorrect\n";
		exit(0);
	}
	double tr,pp[4],maxp,p[4];
 	tr = dcm.val[0][0]+dcm.val[1][1]+dcm.val[2][2];
 	pp[0]=1+tr;
 	pp[1]=1+2*dcm.val[0][0]-tr;
 	pp[2]=1+2*dcm.val[1][1]-tr;
 	pp[3]=1+2*dcm.val[2][2]-tr;

 	maxp=maximum(pp,4);

 	if (maxp == pp[0]) 
   	{
		p[0]=sqrt(pp[0]);
    	 	p[1]=(dcm.val[1][2]-dcm.val[2][1])/p[0];
    	 	p[2]=(dcm.val[2][0]-dcm.val[0][2])/p[0];
    	 	p[3]=(dcm.val[0][1]-dcm.val[1][0])/p[0];
	}
 	else if (maxp == pp[1])
   	{
		p[1]=sqrt(pp[1]);
    		p[0]=(dcm.val[1][2]-dcm.val[2][1])/p[1];
    		p[2]=(dcm.val[1][0]+dcm.val[0][1])/p[1];
    		p[3]=(dcm.val[0][2]+dcm.val[2][0])/p[1];
	} 
 	else if (maxp == pp[2])
   	{
		p[2]=sqrt(pp[2]);
    		p[0]=(dcm.val[2][0]-dcm.val[0][2])/p[2];
    		p[1]=(dcm.val[1][0]+dcm.val[0][1])/p[2];
    		p[3]=(dcm.val[2][1]+dcm.val[1][2])/p[2];
	}
 	else if (maxp == pp[3])
   	{
		p[3]=sqrt(pp[3]);
    		p[0]=(dcm.val[0][1]-dcm.val[1][0])/p[3];
    		p[1]=(dcm.val[0][2]+dcm.val[2][0])/p[3];
    		p[2]=(dcm.val[2][1]+dcm.val[1][2])/p[3];
	}

	if  (p[0]<0.0) this->setq(-0.5*p[1],-0.5*p[2],-0.5*p[3],-0.5*p[0]);
	else this->setq(0.5*p[1],0.5*p[2],0.5*p[3],0.5*p[0]);
}
double Quat::q(int n)
{
	if ((n < 1)||(n > 4))
	{
		cout << "quaternions 1,2,3,4 hi hota hain dekhke input enter karo !!\n";
		return INT_MAX;
	}
	return Q[n-1];
		
}

void Quat::setq(int n,double val)
{
	if ((n < 1)||(n > 4))
        {
                cout << "quaternions 1,2,3,4 hi hota hain dekhke input enter karo !!\n";
                return;
        }

	Q[n-1] = val;
	norm();
}

void Quat::setq(double q1,double q2,double q3)
{
	Q[0] = q1;
        Q[1] = q2;
        Q[2] = q3;
	double temp = (1 - Q[0]*Q[0] - Q[1]*Q[1] - Q[2]*Q[2]);
	if (temp > 0)
	Q[3] = sqrt(temp);
	else
	{
		cout << " setq:: q1^2 + q2^2 + q3^2 in 3 quaternion input > 1,q4 is set to 0\n";
		Q[3] = 0.0;
	} 
	norm();
	ComputeDcm();
	ComputeEul();
}

void Quat::setq(double q1,double q2,double q3,double q4)
{
	Q[0] = q1;
	Q[1] = q2;
	Q[2] = q3;
	Q[3] = q4;

	norm();
	ComputeDcm();
	ComputeEul();
}
void Quat::PrintQuat()
{
	cout << Q[0] << " " << Q[1] << " " << Q[2] << " " << Q[3] << "\n";  
}
void Quat::PrintEul()
{
	cout << EUL[0] << " " << EUL[1] << " " << EUL[2] << "\n";
}
void Quat::PrintDCM()
{
	int i,j;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		cout << DCM[i][j] << " ";
		cout << "\n";
	}
}

void Quat::PrintRotSeq()
{
	cout << IR1 << "-" << IR2 << "-" << IR3 <<"\n";
}

double Quat::maximum(double ary[],int size)
{
 int i;
 double val;
 val = ary[0];
 for (i=0;i<size;++i)
    {
	 if (val < ary[i])
	     val = ary[i];
    }	
	return val;
}
