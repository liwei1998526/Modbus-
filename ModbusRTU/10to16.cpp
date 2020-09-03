#include<iostream>
#include<cstdlib>
#include<string>
using namespace std;
string DEtoHEX(int n)
{
	string hex[16] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "0" };
	int m;
	string ch;
	while (n != 0)
	{
		m = n % 16;
		if (m == 0)
		{
			m = 16;
			n = n / 16;
		}
		else
		{
			n = n / 16;
		}
		ch = hex[m - 1] + ch;
	}
	/*while (1)
	{
	if (ch.size() != 4)
	{
	ch = "0" + ch;
	continue;
	}
	break;
	}*/
	return ch;
}