#include "delay.h"

delay::delay(int inp)
{
	itr = 0;
	if (inp)
	{
		n = fabs(inp);
		array = new Mat(fabs(inp),1);
	}
	else
	{
		n = 1;
		array = new Mat(1,1);
	}
	setBit = false;
}

delay::delay()
{
	itr = 0;
	n = 0;
	array = NULL;
	setBit = false;
}

delay::~delay()
{
	n = 0;
	if (array) delete array;
	setBit = false;
}

void delay::set(int inp)
{
	if (!setBit)
	{
		n = fabs(inp);
		if (array) delete array;
		array = new Mat(fabs(inp),1);
	}
	setBit = true;
}

void delay::show()
{
	setBit = true;
	std::cout << "Delay Size : " << n << std::endl;
	std::cout << "Delay matrix : " << std::endl;
	(~(*array)).show();
}

double delay::IO(double inp)
{
	setBit = true;
	(*array)(itr + 1) = inp;
	itr = (itr + 1)%n;
	return (*array)(itr + 1);
}

double delay::IO(double inp,bool update,int pos)
{
	setBit = true;
	int location;
	if (update)
	{
		location = (itr-pos)%n;
		if (location < 0) location = n+location;
		(*array)(itr + 1) = inp;
		itr = (itr + 1)%n;
		return (*array)(location + 1);
	}
	else
	{
		location = (itr-pos-1)%n;
		if (location < 0) location = n+location;
		return (*array)(location + 1);
	}

}
