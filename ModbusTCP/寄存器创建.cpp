#include<iostream>
#include<vector>
using namespace std;
void WriteRigisterCommand(int &m, int &n, vector<int>&val)
{
	cout << "请输入寄存器起始地址：";
	cin >> m;
	while (1)
	{
		cout << "请输入寄存器数量：";
		cin >> n;
		if (m + n > 10000)
		{
			cout << "寄存器越界" << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		int k;
		/*cout << "请输入寄存器对应的值（0为关，1为开）：" << endl;*/
		for (k = 1; k <= n; k++)
		{
			int y = 0;
			while (1)
			{
				/*cout << "请输入第" << k << "个寄存器的值:";
				cin >> y;
				if (y > 65535)
				{
					cout << "输入值不规范" << endl;
					continue;
				}*/
				break;
			}

			val.push_back(y);
		}
		break;
	}
	/*int j = 0;*/
	/*for (int i = m ; i < m + n; i++)
	{
		cout << "register" << " " << i << ":" << val[j] << endl;
		j++;
	}*/
	getchar();
}