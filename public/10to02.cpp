#include<iostream>
#include<cstdlib>
#include<string>
using namespace std;
string DEtoBIN(int n)
{
	string hex[16] = { "1", "0" };
	int m;
	string ch;
	while (n != 0)
	{
		m = n % 2;
		if (m == 0)
		{
			m = 2;
			n = n / 2;
		}
		else
		{
			n = n / 2;
		}
		ch = hex[m - 1] + ch;
	}

	return ch;
}