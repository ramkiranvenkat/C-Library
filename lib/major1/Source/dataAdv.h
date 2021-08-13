#ifndef _DATA_ADV_
#define _DATA_ADV_

#include <iostream>
#include <stdio.h>
#include <string>
#include "dreadcpp.h"

using namespace std;

class data
{
        public:
                data();
                data(char*);
		data(string);
		~data();
                void load(char*);
		void load(string);
                void unload();
                int i(unsigned int,unsigned int);
                float f(unsigned int,unsigned int);
                double d(unsigned int,unsigned int);
                long l(unsigned int,unsigned int);
                string s(unsigned int,unsigned int);
                int length();
		int NumOfColInRow(int row);

        private:
                DATA local;
};

#endif
