#include<iostream>
#include<vector>
using namespace std;
void WriteRigisterCommand(int &m, int &n, vector<int>&val)
{
	while (1)
	{
		cout << "������Ĵ�����ʼ��ַ��";
		cin >> m;
		if (m > 247)
		{
			cout << "�Ĵ���Խ��" << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		cout << "������Ĵ���������";
		cin >> n;
		if (m + n > 247)
		{
			cout << "�Ĵ���Խ��" << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		int x, k;
		cout << "������Ĵ�����Ӧ��ֵ��0Ϊ�أ�1Ϊ������" << endl;
		for (k = 1; k <= n; k++)
		{
			int y = 0;
			while (1)
			{
				cout << "�������" << k << "���Ĵ�����ֵ:";
				cin >> y;
				if (y > 255)
				{
					cout << "����ֵ���淶" << endl;
					continue;
				}
				break;
			}

			val.push_back(y);
		}
		break;
	}
	int j = 0;
	for (int i = m - 1; i < m + n - 1; i++)
	{
		cout << "register" << " " << i << ":" << val[j] << endl;
		j++;
	}
	getchar();
}