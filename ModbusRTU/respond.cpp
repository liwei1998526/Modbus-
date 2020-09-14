#include"main.h"
void respond_massage(string read_str,string send_buf)
{
	string send_code = send_buf.substr(2, 2);
	int send_code_int = stoi(send_code, 0, 16);
	string respond_code = read_str.substr(2, 2);
	int respond_code_int = stoi(respond_code, 0, 16);
	string send_math = send_buf.substr(8, 4);
	int send_digit = stoi(send_math, 0, 16);
	string recv_id = read_str.substr(0, 2);
	string send_id = send_buf.substr(0, 2);
	if (recv_id != send_id)
	{
		cout << "设备号出错" << endl;
		return;
	}
	if ((respond_code_int != send_code_int) && (respond_code_int != send_code_int + 128))
	{
		cout << "功能码错误" << endl;
		return;
	}
	if (respond_code_int >= 128)
	{
		if (read_str.size() != 10)
		{
			cout << "数据出错" << endl;
			return;
		}
		string Exception_code = read_str.substr(4, 2);
		if (Exception_code == "01")
		{
			cout << "功能码异常，从站不接受所发功能码" << endl;
		}
		else if ((Exception_code == "02"))
		{
			cout << "寄存器超出从站范围" << endl;
		}
		else if (Exception_code == "03")
		{
			cout << "寄存器数据超限，或寄存器受限" << endl;
		}
		else if (Exception_code == "04")
		{
			cout << "从站设备故障" << endl;
		}
	}
	else
	{
		if (respond_code_int == 1)
		{
			//检查数据长度是否符合
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
				cout << "数据出错" << endl;
				return;
			}
			respond_01(read_str, send_buf);
		}
		else if (respond_code_int == 3)
		{//检查数据长度是否符合
			if (read_str.size() != (send_digit * 4 + 10))
			{
				cout << "数据出错" << endl;
				return;
			}
			respond_03(read_str, send_buf);
		}
		else if (respond_code_int == 15 || respond_code_int == 16)
		{//检查数据长度是否符合
			if (read_str.size() != 16)
			{
				cout << "数据出错" << endl;
				return;
			}
			cout << "修改完成" << endl;
		}
	}
}