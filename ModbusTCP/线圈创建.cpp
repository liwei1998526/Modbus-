#include<iostream>
#include<vector>
using namespace std;
void WriteCoilCommand(int &m, int &n, vector<int>&val)
{
	while (1)
	{
		cout << "请输入寄存器起始地址：";
		cin >> m;
		if (m > 2000)
		{
			cout << "寄存器越界" << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		cout << "请输入寄存器数量：";
		cin >> n;
		if (m + n > 9999)
		{
			cout << "寄存器越界" << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		int x, k;
		cout << "请输入寄存器对应的值（0为关，1为开）：" << endl;
		for (k = 1; k <= n; k++)
		{
			int y = 0;
			/*while (1)
			{
				cout << "请输入第" << k << "个线圈的值:";
				cin >> y;
				if (y > 1)
				{
					cout << "输入值不规范" << endl;
					continue;
				}
				break;
			}*/

			val.push_back(y);
		}
		break;
	}
	int j = 0;
	for (int i = m; i < m + n; i++)
	{
		cout << "register" << " " << i << ":" << val[j] << endl;
		j++;
	}
	getchar();
}
//int main()
//{
//	char val[255];
//	memset(val, 0, sizeof(val));
//	cin >> val;
//	int m = 10;
//	int n = 10;
//	WriteCoilCommand(m, n, val);
//	getchar();
//	return 0;
//}