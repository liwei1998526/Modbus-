#include"main.h"
void respond_massage(string read_str,string send_buf)
{
	string respond_high_code = read_str.substr(2, 1);
	int high_code = stoi(respond_high_code, 0, 16);
	string respond_code = read_str.substr(2, 2);
	int respond_code_int = stoi(respond_code, 0, 16);
	if (high_code >= 8)
	{
		string Exception_code = read_str.substr(4, 2);
		if (Exception_code == "01")
		{
			cout << "�������쳣����վ����������������" << endl;
		}
		else if (Exception_code == "02")
		{
			cout << "�Ĵ���������վ��Χ" << endl;
		}
		else if (Exception_code == "03")
		{
			cout << "�Ĵ������ݳ��ޣ���Ĵ�������" << endl;
		}
		else if (Exception_code == "04")
		{
			cout << "��վ�豸����" << endl;
		}
	}
	else
	{
		if (respond_code_int == 1)
		{
			respond_01(read_str, send_buf);
		}
	}
}