#include<iostream>
#include<vector>
using namespace std;
void WriteCoilCommand(int &m, int &n, vector<int>&val)
{
	while (1)
	{
		cout << "������Ĵ�����ʼ��ַ��";
		cin >> m;
		if (m > 2000)
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
		if (m + n > 9999)
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
			/*while (1)
			{
				cout << "�������" << k << "����Ȧ��ֵ:";
				cin >> y;
				if (y > 1)
				{
					cout << "����ֵ���淶" << endl;
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