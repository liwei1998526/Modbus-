#include<iostream>
#include<cstdlib>
#include<string>
using namespace std;
string DEtoHEX(int n)
{
	string hex[16] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "0" };
	int m;
	string ch;
	int k = 0;
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
	return ch;
}
//int main()
//{
//        int n = 58;
//        string ch = DEtoHEX(n);
//        cout << ch << endl;
//        getchar();
//        return 0;
//}