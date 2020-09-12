#include"main.h"
void respond_massage(string read_str,string send_buf)
{
	string respond_high_code = read_str.substr(2, 1);
	int high_code = stoi(respond_high_code, 0, 16);
	string respond_code = read_str.substr(2, 2);
	int respond_code_int = stoi(respond_code, 0, 16);
	string send_math = send_buf.substr(8, 4);
	int send_digit = stoi(send_math, 0, 16);
	if (high_code >= 8)
	{
		if (read_str.size() != 10)
		{
			cout << "���ݳ���" << endl;
			return;
		}
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
			int v;
			if (send_digit > 8 && send_digit % 8 != 0)
			{
				v = send_digit / 8 + 1;
			}
			else if (send_digit % 8 == 0)
			{
				v = send_digit / 8;
			}
			else if (send_digit < 8)
			{
				v = 1;
			}
			if (read_str.size() != (v * 2 + 10))
			{
				cout << "���ݳ���" << endl;
				return;
			}
			respond_01(read_str, send_buf);
		}
		else if (respond_code_int == 3)
		{
			if (read_str.size() != (send_digit * 4 + 10))
			{
				cout << "���ݳ���" << endl;
				return;
			}
			respond_03(read_str, send_buf);
		}
		else if (respond_code_int == 15 || respond_code_int == 16)
		{
			if (read_str.size() != 16)
			{
				cout << "���ݳ���" << endl;
				return;
			}
			cout << "�޸����" << endl;
		}
	}
}