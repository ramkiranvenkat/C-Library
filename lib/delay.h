#ifndef delayFunc
#define delayFunc

#include <iostream>
#include <string>
#include <fstream>
#include <stdbool.h>
#include <iomanip>
#include <math.h>

#include "MAT.h"
#include <stdbool.h>
using namespace std;

class delay
{
	public:
		delay();
		delay(int );
		~delay();
		void set(int );
		void show();
		double IO(double );
		double IO(double ,bool ,int );
	private:
		int n,itr;
		Mat* array;
		bool setBit;
};

#endif
