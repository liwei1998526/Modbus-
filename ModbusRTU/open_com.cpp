#include"main.h"
bool OPEN_COM(HANDLE &H_Com)
{
	string COMM;
	string COM;
	int Bund_rate = 0;
	int bund;//ѡ������
	int check;//ѡ��У��λ
	HANDLE test_com;
	string com = "com";
	while (1)
	{    //��ӡ���п�ʹ�õĴ��ڡ�
		int COM_open = 0;
		cout << "��ʹ�ô��ڣ�" << endl;
		for (int port = 1; port < 100; port++)
		{
			COM = com + to_string(port);
			test_com = InitCOM((LPCTSTR)COM.c_str(), 9600, 8, 0, 1);
			if (test_com == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			else
			{
				cout << COM << endl;
				COM_open++;
				CloseHandle(test_com);
				continue;
			}
		}
		if (COM_open == 0)
		{
			cout << "�޿��ô��ڣ��Ƿ��ٴμ�⣨0�������̣�1���¼�⣬2�������У���";
			int yes;
			cin >> yes;
			if (yes == 1)
			{
				continue;
			}
			else if (yes == 0)
			{
				return false;
			}
		}
		//��ʼ�򿪴���
		cout << "����˿ں�Ϊ��";
		string com = "com";
		string how;
		cin >> how;
		COMM = com + how;
		cout << "1--9600" << " " << "2--14400" << " " << "3--19200" << " " << "4--115200" << endl;
		cout << "��ѡ�����ʣ�";
		cin >> bund;
		if (bund == 1)
		{
			Bund_rate = 9600;
		}
		else if (bund == 2)
		{
			Bund_rate = 14400;
		}
		else if (bund == 3)
		{
			Bund_rate = 19200;
		}
		else if (bund == 4)
		{
			Bund_rate = 115200;
		}
		cout << "0--��У��" << " " << "1--��У��" << " " << "2--żУ��" << endl;
		cout << "����У��λ��";
		cin >> check;
		H_Com = InitCOM((LPCTSTR)COMM.c_str(), Bund_rate, 8, 0, check);
		if (H_Com == INVALID_HANDLE_VALUE)
		{
			int or;
			cout << "���ڴ�ʧ�ܣ��Ƿ��������(0������1����)��";
			while (1)
			{
				cin >> or;
				if (or == 0 || or == 1)
				{
					break;
				}
				cout << "����ֵ���淶�����������루0������1��������";
				continue;
			}
			if (or == 1)
			{
				continue;
			}
			else if (or == 0)
			{
				return false;
			}
		}
		else
		{
			cout << "���ڴ򿪳ɹ�" << endl;
			return true;
		}
		break;
	}
	return true;
}
