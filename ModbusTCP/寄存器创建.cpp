#include<iostream>
#include<vector>
using namespace std;
void WriteRigisterCommand(int &m, int &n, vector<int>&val)
{
	cout << "������Ĵ�����ʼ��ַ��";
	cin >> m;
	while (1)
	{
		cout << "������Ĵ���������";
		cin >> n;
		if (m + n > 10000)
		{
			cout << "�Ĵ���Խ��" << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		int k;
		/*cout << "������Ĵ�����Ӧ��ֵ��0Ϊ�أ�1Ϊ������" << endl;*/
		for (k = 1; k <= n; k++)
		{
			int y = 0;
			while (1)
			{
				/*cout << "�������" << k << "���Ĵ�����ֵ:";
				cin >> y;
				if (y > 65535)
				{
					cout << "����ֵ���淶" << endl;
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